/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.
 
 This file is part of the TerraLib - a Framework for building GIS enabled applications.
 
 TerraLib is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License,
 or (at your option) any later version.
 
 TerraLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with TerraLib. See COPYING. If not, write to
 TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
 \file VectorToVectorMemory.h
 
 \brief Vector to Vector processing.
 
 \ingroup attributefill
 */

//Terralib
#include "../common/Translator.h"
#include "../common/StringUtils.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/StringProperty.h"
#include "../geometry.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/Enums.h"

#include "Config.h"
#include "VectorToVectorMemory.h"

// STL
#include <memory>

// Boost
#include <boost/lexical_cast.hpp>

te::attributefill::VectorToVectorMemory::VectorToVectorMemory()
{

}

te::attributefill::VectorToVectorMemory::~VectorToVectorMemory()
{

}

bool te::attributefill::VectorToVectorMemory::run()
{
  te::gm::Envelope fromEnv = m_fromLayer->getExtent();
  std::auto_ptr<te::da::DataSet> fromDs = m_fromLayer->getData();
  std::size_t fromSpatialPos = te::da::GetFirstSpatialPropertyPos(fromDs.get());
  int fromSrid = m_fromLayer->getSRID();
  std::auto_ptr<te::da::DataSetType> fromSchema = m_fromLayer->getSchema();

  te::gm::Envelope toEnv = m_toLayer->getExtent();
  std::auto_ptr<te::da::DataSet> toDs = m_toLayer->getData();
  std::size_t toSpatialPos = te::da::GetFirstSpatialPropertyPos(toDs.get());
  std::auto_ptr<te::da::DataSetType> toSchema = m_toLayer->getSchema();
  int toSrid = m_toLayer->getSRID();

  if(!fromEnv.intersects(toEnv))
  {
    throw te::common::Exception(TE_TR("The two layers do not intersect."));
  }

  std::auto_ptr<te::da::DataSetType> outDst(getOutputDataSetType());

  std::auto_ptr<te::mem::DataSet> outDs(new te::mem::DataSet(outDst.get()));

  te::sam::rtree::Index<size_t, 8>* rtree = getRtree(fromDs.get());

  toDs->moveBeforeFirst();

  while(toDs->moveNext())
  {
    te::mem::DataSetItem* item = new te::mem::DataSetItem(outDs.get());

    for(std::size_t i = 0; i < toSchema->size(); ++i)
    {
      item->setValue(i, toDs->getValue(i).release());
    }

    std::vector<std::size_t> intersections = getIntersections(toDs.get(), fromDs.get(), rtree);

    if(intersections.empty())
    {
      outDs->add(item);
      continue;
    }

    std::map<te::dt::Property*, std::vector<std::string> >::iterator it = m_options.begin();

    while(it != m_options.end())
    {
      te::stat::NumericStatisticalSummary ssNum;
      te::stat::StringStatisticalSummary ssStr;

      std::vector<double> numValues;
      std::vector<std::string> strValues;
      std::vector<te::dt::AbstractData*> dataValues;

      dataValues = getDataValues(fromDs.get(), intersections, it->first->getName());

      if(it->first->getType() == te::dt::STRING_TYPE)
      {
        strValues = getStrValues(dataValues);
        te::stat::GetStringStatisticalSummary(strValues, ssStr, "");
      }
      else
      {
        numValues = getNumValues(dataValues);
        te::stat::GetNumericStatisticalSummary(numValues, ssNum);
      }

      std::vector<std::string> funcs = it->second;

      for(std::size_t i = 0; i < funcs.size(); ++i)
      {
        std::string outPropName = getPropertyName(it->first, funcs[i]);

        if(funcs[i] == "Value")
        {
          if(it->first->getType() == te::dt::STRING_TYPE)
            item->setString(outPropName, ssStr.m_minVal);
          else
            item->setDouble(outPropName, ssNum.m_minVal);
        }
        else if(funcs[i] == "Major Class")
        {
          te::dt::AbstractData* value = getMajorClass(toDs.get(), toSrid, fromDs.get(), fromSrid, intersections, it->first->getName());
          item->setValue(outPropName, value);
        }
        else if(it->first->getType() == te::dt::STRING_TYPE)
        {
          std::string value = getValue(ssStr, funcs[i]);
          item->setString(outPropName, value);
        }
        else
        {
          if(funcs[i] == "Mode")
          {
            std::string value = getModeValue(ssNum);
            item->setString(outPropName, value);
          }
          else
          {
            double value = getValue(ssNum, funcs[i]);
            item->setDouble(outPropName, value);
          }
        }
      }

      ++it;
    }

    outDs->add(item);
  }

  save(outDs, outDst);

  return true;
}

te::da::DataSetType* te::attributefill::VectorToVectorMemory::getOutputDataSetType()
{
  std::auto_ptr<te::da::DataSetType> toScheme = m_toLayer->getSchema();

  te::da::DataSetType* dst = new te::da::DataSetType(*toScheme.get());
  dst->setName(m_outDset);

  std::map<te::dt::Property*, std::vector<std::string> >::iterator it = m_options.begin();

  while(it != m_options.end())
  {
    std::vector<std::string> funcs = it->second;

    te::dt::Property* currentProperty = it->first;

    for(std::size_t i = 0; i < funcs.size(); ++i)
    {
      te::dt::SimpleProperty* newProp = 0;

      std::string newName = getPropertyName(it->first, funcs[i]);

      if(funcs[i] == "Value")
      {
        if(it->first->getType() == te::dt::STRING_TYPE)
          newProp = new te::dt::StringProperty(newName);
        else
          newProp = new te::dt::SimpleProperty(newName, te::dt::DOUBLE_TYPE);
      }
      else if(funcs[i] == "Major Class")
      {
        newProp = dynamic_cast<te::dt::SimpleProperty*>(currentProperty->clone());
        newProp->setRequired(false);
        newProp->setName(newName);
      }
      else if(funcs[i] == "Percentage per Class")
      {
        
      }
      else if(it->first->getType() == te::dt::STRING_TYPE || funcs[i] == "Mode")
      {
        newProp = new te::dt::StringProperty(newName);
      }
      else
      {
        newProp = new te::dt::SimpleProperty(newName, te::dt::DOUBLE_TYPE);
      }

      dst->add(newProp);
    }

    ++it;
  }

  return dst;
}

te::sam::rtree::Index<size_t, 8>* te::attributefill::VectorToVectorMemory::getRtree(te::da::DataSet* data)
{
  te::sam::rtree::Index<size_t, 8>* rtree = new te::sam::rtree::Index<size_t, 8>;

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(data);

  data->moveBeforeFirst();

  int count = 0;

  while(data->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> geom = data->getGeometry(geomPos);

    rtree->insert(*geom->getMBR(), count);

    ++count;
  }

  return rtree;
}

std::string te::attributefill::VectorToVectorMemory::getPropertyName(te::dt::Property* prop, const std::string& func)
{
  std::string name = te::common::Convert2LCase(prop->getName());

  std::string newName = name + "_";

  if(func == "Value")
    newName += "value";
  else if(func == "Minimum value")
    newName += "min_val";
  else if(func == "Maximum value")
    newName += "max_val";
  else if(func == "Mean")
    newName += "mean";
  else if(func == "Sum of values")
    newName += "sum_values";
  else if(func == "Total number of values")
    newName += "total_values";
  else if(func == "Total not null values")
    newName += "total_notnull_values";
  else if(func == "Standard deviation")
    newName += "stand_dev";
  else if(func == "Variance")
    newName += "variance";
  else if(func == "Skewness")
    newName += "skewness";
  else if(func == "Kurtosis")
    newName += "kurtosis";
  else if(func == "Amplitude")
    newName += "amplitude";
  else if(func == "Median")
    newName += "median";
  else if(func == "Coefficient variation")
    newName += "coeff_variation";
  else if(func == "Mode")
    newName += "mode";
  else if(func == "Major Class")
    newName += "maj_class";
  else if(func == "Percentage per Class")
    newName += "percent_class";
  else if(func == "Minimum Distance")
    newName += "min_distance";

  return newName;
}

std::vector<std::size_t> te::attributefill::VectorToVectorMemory::getIntersections(te::da::DataSet* toDs,
                                                                             te::da::DataSet* fromDs,
                                                                             te::sam::rtree::Index<size_t, 8>* rtree)
{
  int toSpatialPos = te::da::GetFirstSpatialPropertyPos(toDs);
  int fromSpatialPos = te::da::GetFirstSpatialPropertyPos(fromDs);

  std::map<std::size_t, std::vector<std::size_t> > intersections;

  std::auto_ptr<te::gm::Geometry> geom = toDs->getGeometry(toSpatialPos);
  geom->setSRID(m_toLayer->getSRID());

  std::vector<size_t> report;
  rtree->search(*geom->getMBR(), report);

  std::vector<std::size_t> interVec;
  for(std::size_t i = 0; i < report.size(); ++i)
  {
    fromDs->move(report[i]);

    std::auto_ptr<te::gm::Geometry> g = fromDs->getGeometry(fromSpatialPos);
    g->setSRID(m_fromLayer->getSRID());

    if(geom->intersects(g.get()))
    {
      interVec.push_back(report[i]);
    }
  }
  return interVec;
}

std::vector<double> te::attributefill::VectorToVectorMemory::getNumValues(std::vector<te::dt::AbstractData*> data)
{
  std::vector<double> result;

  for(std::size_t i = 0; i < data.size(); ++i)
  {
    if(data[i]->getTypeCode() == te::dt::INT16_TYPE || 
       data[i]->getTypeCode() == te::dt::UINT16_TYPE || 
       data[i]->getTypeCode() == te::dt::INT32_TYPE || 
       data[i]->getTypeCode() == te::dt::UINT32_TYPE || 
       data[i]->getTypeCode() == te::dt::INT64_TYPE || 
       data[i]->getTypeCode() == te::dt::UINT64_TYPE || 
       data[i]->getTypeCode() == te::dt::FLOAT_TYPE || 
       data[i]->getTypeCode() == te::dt::DOUBLE_TYPE || 
       data[i]->getTypeCode() == te::dt::CINT16_TYPE || 
       data[i]->getTypeCode() == te::dt::CINT32_TYPE || 
       data[i]->getTypeCode() == te::dt::CFLOAT_TYPE || 
       data[i]->getTypeCode() == te::dt::CDOUBLE_TYPE)
    {
      std::string strValue = data[i]->toString();

      result.push_back(boost::lexical_cast<double>(strValue));
    }
  }

  return result;
}

std::vector<std::string> te::attributefill::VectorToVectorMemory::getStrValues(std::vector<te::dt::AbstractData*> data)
{
  std::vector<std::string> result;

  for(std::size_t i = 0; i < data.size(); ++i)
  {
    if(data[i]->getTypeCode() == te::dt::STRING_TYPE)
      result.push_back(data[i]->toString());
  }

  return result;
}

bool te::attributefill::VectorToVectorMemory::isStatistical(const std::string& funcName)
{
  if(funcName == "Minimum value" ||
     funcName == "Maximum value" ||
     funcName == "Mean" ||
     funcName == "Sum of values" ||
     funcName == "Total number of values" ||
     funcName == "Total not null values" ||
     funcName == "Standard deviation" ||
     funcName == "Variance" ||
     funcName == "Skewness" ||
     funcName == "Kurtosis" ||
     funcName == "Amplitude" ||
     funcName == "Median" ||
     funcName == "Coefficient variation" ||
     funcName == "Mode")
  {
    return true;
  }

  return false;
}

std::vector<std::string> te::attributefill::VectorToVectorMemory::getSelectedFunctions()
{
  std::vector<std::string> allSelFuncs;

  std::map<te::dt::Property*, std::vector<std::string> >::iterator it = m_options.begin();

  while(it != m_options.end())
  {
    std::vector<std::string> funcs = it->second;

    for(std::size_t i = 0; i < funcs.size(); ++i)
    {
      if(std::find(allSelFuncs.begin(), allSelFuncs.end(), funcs[i]) == funcs.end())
      {
        allSelFuncs.push_back(funcs[i]);
      }
    }
  }

  return allSelFuncs;
}

double te::attributefill::VectorToVectorMemory::getValue(te::stat::NumericStatisticalSummary ss, const std::string& function)
{
  if(function == "Amplitude")
    return ss.m_amplitude;
  else if(function == "Total number of values")
    return ss.m_count;
  else if(function == "Kurtosis")
    return ss.m_kurtosis;
  else if(function == "Maximum value")
    return ss.m_maxVal;
  else if(function == "Mean")
    return ss.m_mean;
  else if(function == "Median")
    return ss.m_median;
  else if(function == "Minimum value")
    return ss.m_minVal;  
  else if(function == "Skewness")
    return ss.m_skewness;
  else if(function == "Standard deviation")
    return ss.m_stdDeviation;
  else if(function == "Sum of values")
    return ss.m_sum;
  else if(function == "Total not null values")
    return ss.m_validCount;
  else if(function == "Coefficient variation")
    return ss.m_varCoeff;
  else if(function == "Variance")
    return ss.m_variance;
  else
    return -1;
  
}

std::string te::attributefill::VectorToVectorMemory::getValue(te::stat::StringStatisticalSummary ss, const std::string& function)
{
  if(function == "Maximum value")
    return ss.m_maxVal;
  else if(function == "Minimum value")
    return ss.m_minVal; 
  else if(function == "Mode")
    return ss.m_mode;
  else if(function == "Total number of values")
    return boost::lexical_cast<std::string>(ss.m_count);
  else if(function == "Total not null values")
    return boost::lexical_cast<std::string>(ss.m_validCount);
  else
    return "null";
}

std::string te::attributefill::VectorToVectorMemory::getModeValue(te::stat::NumericStatisticalSummary ss)
{
  std::string result = "";
  std::vector<double> values = ss.m_mode;
  for(std::size_t i = 0; i < values.size(); ++i)
  {
    if(i == 0)
      result += boost::lexical_cast<std::string>(values[i]);
    else
      result += ", " + boost::lexical_cast<std::string>(values[i]);
  }

  return result;
}

std::vector<te::dt::AbstractData*> te::attributefill::VectorToVectorMemory::getDataValues(te::da::DataSet* fromDs,
                                                                                    std::vector<std::size_t> dsPos,
                                                                                    const std::string& propertyName)
{
  std::vector<te::dt::AbstractData*> result;

  std::size_t spatialPos = te::da::GetFirstSpatialPropertyPos(fromDs);

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    result.push_back(fromDs->getValue(propertyName).release());
  }

  return result;
}

te::dt::AbstractData* te::attributefill::VectorToVectorMemory::getMajorClass(te::da::DataSet* toDs,
                                                                       std::size_t toSrid,
                                                                       te::da::DataSet* fromDs,
                                                                       std::size_t fromSrid,
                                                                       std::vector<std::size_t> dsPos,
                                                                       const std::string& propertyName)
{
  std::size_t toSpatialPos = te::da::GetFirstSpatialPropertyPos(toDs);
  std::size_t fromSpatialPos = te::da::GetFirstSpatialPropertyPos(fromDs);

  std::auto_ptr<te::gm::Geometry> toGeom = toDs->getGeometry(toSpatialPos);
  if(toGeom->getSRID() <= 0)
    toGeom->setSRID(toSrid);

  std::map<std::size_t, double> areaMap;
  std::map<std::string, std::size_t> pointMap;
  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    std::auto_ptr<te::gm::Geometry> fromGeom = fromDs->getGeometry(fromSpatialPos);
    if(fromGeom->getSRID() <= 0)
      fromGeom->setSRID(fromSrid);

    te::gm::Geometry* intersection = toGeom->intersection(fromGeom.get());

    if(isPolygon(toGeom->getGeomTypeId()) || isMultiPolygon(toGeom->getGeomTypeId()))
    {
      if(isPolygon(fromGeom->getGeomTypeId()) || isMultiPolygon(fromGeom->getGeomTypeId()))
      {
        te::gm::Polygon* pol = dynamic_cast<te::gm::Polygon*>(intersection);
        te::gm::MultiPolygon* multiPol = dynamic_cast<te::gm::MultiPolygon*>(intersection);

        if(pol)
          areaMap[dsPos[i]] = pol->getArea();
        else if(multiPol)
          areaMap[dsPos[i]] = multiPol->getArea();
      }
      else if(isLine(fromGeom->getGeomTypeId()) || isMultiLine(fromGeom->getGeomTypeId()))
      {
        te::gm::LineString* line = dynamic_cast<te::gm::LineString*>(intersection);
        te::gm::MultiLineString* multiLine = dynamic_cast<te::gm::MultiLineString*>(intersection);

        if(line)
          areaMap[dsPos[i]] = line->getLength();
        else if(multiLine)
          areaMap[dsPos[i]] = multiLine->getLength();
      }
      else if(isPoint(fromGeom->getGeomTypeId()) || isMultiPoint(fromGeom->getGeomTypeId()))
      {
        te::gm::Point* point = dynamic_cast<te::gm::Point*>(intersection);
        te::gm::MultiPoint* multiPoint = dynamic_cast<te::gm::MultiPoint*>(intersection);

        std::string v = fromDs->getAsString(propertyName, 9);

        if(point)
          areaMap[dsPos[i]] = point->getNPoints();
        else if(multiPoint)
          areaMap[dsPos[i]] = multiPoint->getNPoints();
      }
    }
  }

  std::map<std::size_t, double>::iterator it = areaMap.begin();

  double areaAux;
  std::size_t posAux;
  while(it != areaMap.end())
  {
    if(it == areaMap.begin())
    {
      posAux = it->first;
      areaAux = it->second;
    }
    else
    {
      if(areaAux < it->second)
      {
        areaAux = it->second;
        posAux = it->first;
      }
    }

    ++it;
  }

  fromDs->move(posAux);

  return fromDs->getValue(propertyName).release();
}

bool te::attributefill::VectorToVectorMemory::isPolygon(te::gm::GeomType type)
{
  if(type == te::gm::PolygonType ||
     type == te::gm::PolygonZType ||
     type == te::gm::PolygonMType ||
     type == te::gm::PolygonZMType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isLine(te::gm::GeomType type)
{
  if(type == te::gm::LineStringType ||
     type == te::gm::LineStringMType ||
     type == te::gm::LineStringZMType ||
     type == te::gm::LineStringZType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isPoint(te::gm::GeomType type)
{
  if(type == te::gm::PointType ||
     type == te::gm::PointZType ||
     type == te::gm::PointMType ||
     type == te::gm::PointZMType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isMultiPolygon(te::gm::GeomType type)
{
  if(type == te::gm::MultiPolygonType ||
     type == te::gm::MultiPolygonZType ||
     type == te::gm::MultiPolygonMType ||
     type == te::gm::MultiPolygonZMType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isMultiLine(te::gm::GeomType type)
{
  if(type == te::gm::MultiLineStringMType ||
     type == te::gm::MultiLineStringType ||
     type == te::gm::MultiLineStringZMType ||
     type == te::gm::MultiLineStringZType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isMultiPoint(te::gm::GeomType type)
{
  if(type == te::gm::MultiPointType ||
     type == te::gm::MultiPointZType ||
     type == te::gm::MultiPointMType ||
     type == te::gm::MultiPointZMType)
     return true;

  return false;
}

double te::attributefill::VectorToVectorMemory::getArea(te::gm::Geometry* geom)
{
  te::gm::GeomType geomType = geom->getGeomTypeId();

  switch(geomType)
  {
    case te::gm::PolygonType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      return g->getArea();
    }
    case te::gm::PolygonZType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      return g->getArea();
    }
    case te::gm::PolygonMType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      return g->getArea();
    }
    case te::gm::PolygonZMType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      return g->getArea();
    }
    case te::gm::MultiPolygonType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      return g->getArea();
    }
    case te::gm::MultiPolygonZType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      return g->getArea();
    }
    case te::gm::MultiPolygonMType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      return g->getArea();
    }
    case te::gm::MultiPolygonZMType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      return g->getArea();
    }
    default:
    {
      return 0;
    }
  }
}