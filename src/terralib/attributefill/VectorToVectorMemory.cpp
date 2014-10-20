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
#include "../common/Logger.h"
#include "../common/progress/TaskProgress.h"
#include "../common/Translator.h"
#include "../common/StringUtils.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/StringProperty.h"
#include "../datatype/SimpleData.h"
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

  te::common::TaskProgress task("Processing Vector to Vector...");
  task.setTotalSteps(toDs->size());
  task.useTimer(true);

  while(toDs->moveNext())
  {
    try
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
          else if(funcs[i] == "Class with larger intersection area")
          {
            te::dt::AbstractData* value = getClassWithLargerIntersectionArea(toDs.get(), toSrid, fromDs.get(), fromSrid, intersections, it->first->getName());

            item->setValue(outPropName, value);
          }
          else if(funcs[i] == "Class with highest occurrence")
          {
            te::dt::AbstractData* value = getClassWithHighestOccurrence(fromDs.get(), intersections, it->first->getName());
            item->setValue(outPropName, value);
          }
          else if(funcs[i] == "Percentage per Class")
          {
            std::map<std::string, double> result = getPercentagePerClass(fromDs.get(), intersections, it->first->getName());

            std::map<std::string, double>::iterator itAux = result.begin();
            while(itAux != result.end())
            {
              std::string newPropName = it->first->getName() + "_" + itAux->first;
              std::replace(newPropName.begin(), newPropName.end(), ' ', '_');

              item->setDouble(newPropName, itAux->second);

              ++itAux;
            }
          }
          else if(funcs[i] == "Presence")
          {
            if(intersections.size() > 0)
            {
              item->setInt32(outPropName, 1);
            }
            else
            {
              item->setInt32(outPropName, 0);
            }
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

      if (task.isActive() == false)
        throw te::common::Exception(TE_TR("Operation canceled!"));

      task.pulse();
    }
    catch(te::common::Exception& e)
    {
      te::common::Logger::logDebug("attributefill", e.what());
    }
    catch(std::exception& e)
    {
      te::common::Logger::logDebug("attributefill", e.what());
    }
  }

  save(outDs, outDst);

  return true;
}

te::da::DataSetType* te::attributefill::VectorToVectorMemory::getOutputDataSetType()
{
  std::auto_ptr<te::da::DataSet> fromDs = m_fromLayer->getData();

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
      else if(funcs[i] == "Class with highest occurrence" ||
              funcs[i] == "Class with larger intersection area")
      {
        newProp = dynamic_cast<te::dt::SimpleProperty*>(currentProperty->clone());
        newProp->setRequired(false);
        newProp->setName(newName);
      }
      else if(funcs[i] == "Percentage per Class")
      {
        continue;//Sera feito fora do for
      }
      else if(funcs[i] == "Presence")
      {
        newProp = new te::dt::SimpleProperty(newName, te::dt::INT32_TYPE);
      }
      else if(it->first->getType() == te::dt::STRING_TYPE || funcs[i] == "Mode" )
      {
        newProp = new te::dt::StringProperty(newName);
      }
      else
      {
        newProp = new te::dt::SimpleProperty(newName, te::dt::DOUBLE_TYPE);
      }

      dst->add(newProp);
    }

    if(std::find(funcs.begin(), funcs.end(), "Percentage per Class") != funcs.end())
    {
      std::vector<std::string> strClasses = getDistinctClasses(fromDs.get(), it->first->getName());

      for(std::size_t i = 0; i < strClasses.size(); ++i)
      {
        std::string newPropName = it->first->getName() + "_" + strClasses[i];
        std::replace(newPropName.begin(), newPropName.end(), ' ', '_');

        te::dt::SimpleProperty* newProp = new te::dt::SimpleProperty(newPropName, te::dt::DOUBLE_TYPE);

        dst->add(newProp);
      }
    }

    ++it;
  }

  return dst;
}

std::vector<std::string> te::attributefill::VectorToVectorMemory::getDistinctClasses(te::da::DataSet* fromDs,
                                                                                     const std::string& propertyName)
{
  std::vector<std::string> result;

  fromDs->moveBeforeFirst();

  while(fromDs->moveNext())
  {
    std::string strClass = fromDs->getAsString(propertyName, 9);

    if(std::find(result.begin(), result.end(), strClass) == result.end())
    {
      result.push_back(strClass);
    }
  }

  return result;
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
  else if(func == "Class with highest occurrence")
    newName += "class_high_occurrence";
  else if(func == "Class with larger intersection area")
    newName += "class_high_area";
  else if(func == "Minimum Distance")
    newName += "min_distance";
  else if(func == "Presence")
    newName += "presence";

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

te::dt::AbstractData* te::attributefill::VectorToVectorMemory::getClassWithHighestOccurrence(te::da::DataSet* fromDs,
                                                                                             std::vector<std::size_t> dsPos,
                                                                                             const std::string& propertyName)
{
  int propIndex = te::da::GetPropertyIndex(fromDs, propertyName);
  int propType = fromDs->getPropertyDataType(propIndex);

  std::map<std::string, std::size_t> counter;
  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    std::string value = fromDs->getAsString(propIndex);

    if(counter.find(value) == counter.end())
    {
      counter[value] = 1;
    }
    else
    {
      std::size_t aux = counter[value] + 1;
      counter[value] = aux;
    }
  }

  std::map<std::string, std::size_t>::iterator it = counter.begin();
  std::string value;
  std::size_t aux = 0;
  while(it != counter.end())
  {
    if(aux < it->second)
    {
      aux = it->second;
      value = it->first;
    }

    ++it;
  }

  te::dt::AbstractData* data = getDataBasedOnType(value, propType);

  return data;
}

te::dt::AbstractData* te::attributefill::VectorToVectorMemory::getClassWithLargerIntersectionArea(te::da::DataSet* toDs,
                                                                                                  std::size_t toSrid,
                                                                                                  te::da::DataSet* fromDs,
                                                                                                  std::size_t fromSrid,
                                                                                                  std::vector<std::size_t> dsPos,
                                                                                                  const std::string& propertyName)
{
  std::size_t fromGeomPos = te::da::GetFirstSpatialPropertyPos(fromDs);
  std::size_t toGeomPos =   te::da::GetFirstSpatialPropertyPos(toDs);

  int propIndex = te::da::GetPropertyIndex(fromDs, propertyName);
  int propType = fromDs->getPropertyDataType(propIndex);

  std::auto_ptr<te::gm::Geometry> toGeom = toDs->getGeometry(toGeomPos);
  if(toGeom->getSRID() <= 0)
      toGeom->setSRID(toSrid);

  std::map<std::string, double> classAreaMap;
  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    std::auto_ptr<te::gm::Geometry> fromGeom = fromDs->getGeometry(fromGeomPos);
    if(fromGeom->getSRID() <= 0)
      fromGeom->setSRID(fromSrid);

    std::auto_ptr<te::gm::Geometry> interGeom(toGeom->intersection(fromGeom.get()));

    std::string value = fromDs->getAsString(propIndex);

    te::gm::GeomType interGeomType = interGeom->getGeomTypeId();

    double area = getArea(interGeom.get());

    if(classAreaMap.find(value) == classAreaMap.end())
    {
      classAreaMap[value] = area;
    }
    else
    {
      double aux = classAreaMap[value];
      classAreaMap[value] = aux + area;
    }
  }

  std::map<std::string, double>::iterator it = classAreaMap.begin();
  std::string value;
  double aux = 0;
  while(it != classAreaMap.end())
  {
    if(aux < it->second)
    {
      aux = it->second;
      value = it->first;
    }

    ++it;
  }

  te::dt::AbstractData* data = getDataBasedOnType(value, propType);

  return data;
}

std::map<std::string, double> te::attributefill::VectorToVectorMemory::getPercentagePerClass(te::da::DataSet* fromDs,
                                                                                             std::vector<std::size_t> dsPos,
                                                                                             const std::string& propertyName)
{
  std::map<std::string, double> result;

  std::map<std::string, std::size_t> aux;
  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    std::string value = fromDs->getAsString(propertyName);

    if(aux.find(value) == aux.end())
    {
      aux[value] = 1;
    }
    else
    {
      aux[value] += 1;
    }
  }

  std::size_t total = 0;
  std::map<std::string, std::size_t>::iterator it = aux.begin();
  while(it != aux.end())
  {
    total += it->second;

    ++it;
  }

  it = aux.begin();
  while(it != aux.end())
  {
    result[it->first] = ((double)it->second / total);

    ++it;
  }

  return result;
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

  double area = 0;

  switch(geomType)
  {
    case te::gm::PolygonType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      area = g->getArea();
      break;
    }
    case te::gm::MultiPolygonType:
    {
      te::gm::MultiPolygon* g = dynamic_cast<te::gm::MultiPolygon*>(geom);
      area = g->getArea();
      break;
    }
    case te::gm::GeometryCollectionType:
    {
      te::gm::GeometryCollection* col = dynamic_cast<te::gm::GeometryCollection*>(geom);
      for(std::size_t j = 0; j < col->getNumGeometries(); ++j)
      {
        te::gm::Geometry* auxGeom = col->getGeometryN(j);
        if(isPolygon(auxGeom->getGeomTypeId()))
        {
          area += dynamic_cast<te::gm::Polygon*>(auxGeom)->getArea();
        }
        else if(isMultiPolygon(auxGeom->getGeomTypeId()))
        {
          area += dynamic_cast<te::gm::MultiPolygon*>(auxGeom)->getArea();
        }
      }

      break;
    }
    default:
    {
      throw te::common::Exception(TE_TR("Unexpected geometry type!"));
    }
  }

  return area;
}

te::dt::AbstractData* te::attributefill::VectorToVectorMemory::getDataBasedOnType(const std::string& strValue, const int type)
{
  te::dt::AbstractData* data = 0;

  switch(type)
  {
    case te::dt::STRING_TYPE:
    {
      data = new te::dt::SimpleData<std::string, te::dt::STRING_TYPE>(strValue);
      break;
    }
    case te::dt::INT16_TYPE:
    {
      int16_t v = boost::lexical_cast<int16_t>(strValue);
      data = new te::dt::SimpleData<std::string, te::dt::INT16_TYPE>(strValue);
      break;
    }
    case te::dt::INT32_TYPE:
    {
      int32_t v = boost::lexical_cast<int32_t>(strValue);
      data = new te::dt::SimpleData<std::string, te::dt::INT32_TYPE>(strValue);
      break;
    }
    case te::dt::INT64_TYPE:
    {
      int64_t v = boost::lexical_cast<int64_t>(strValue);
      data = new te::dt::SimpleData<std::string, te::dt::INT64_TYPE>(strValue);
      break;
    }
    case te::dt::UINT16_TYPE:
    {
      uint16_t v = boost::lexical_cast<uint16_t>(strValue);
      data = new te::dt::SimpleData<std::string, te::dt::UINT16_TYPE>(strValue);
      break;
    }
    case te::dt::UINT32_TYPE:
    {
      uint32_t v = boost::lexical_cast<uint32_t>(strValue);
      data = new te::dt::SimpleData<std::string, te::dt::UINT32_TYPE>(strValue);
      break;
    }
    case te::dt::UINT64_TYPE:
    {
      uint64_t v = boost::lexical_cast<uint64_t>(strValue);
      data = new te::dt::SimpleData<std::string, te::dt::UINT64_TYPE>(strValue);
      break;
    }
    default:
    {
      throw te::common::Exception(TE_TR("Unexpected data type!"));
    }
  }

  return data;
}
