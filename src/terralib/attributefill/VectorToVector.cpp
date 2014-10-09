/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.
 
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
 \file VectorToVector.cpp
 */

#include "../common/StringUtils.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"

#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/AbstractData.h"
#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"

#include "../geometry.h"

#include "../memory/DataSetItem.h"

#include "../raster/RasterProperty.h"
#include "../raster/Utils.h"

#include "../rp/RasterAttributes.h"

#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/Utils.h"

#include "../sam.h"

#include "VectorToVector.h"

// Boost
#include <boost/lexical_cast.hpp>

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
 \file VectorToVector.h
 
 \brief Vector to Vector processing.
 
 \ingroup attributefill
 */

//Terralib
#include "../attributefill/VectorToVector.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"

#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

te::attributefill::VectorToVector::VectorToVector()
  : m_outDset("")
{

}

te::attributefill::VectorToVector::~VectorToVector()
{

}

void te::attributefill::VectorToVector::setInput(te::map::AbstractLayerPtr fromLayer,
                                                 te::map::AbstractLayerPtr toLayer)
{
  m_fromLayer = fromLayer;
  m_toLayer = toLayer;
}

void te::attributefill::VectorToVector::setParams(std::map<te::dt::Property*, std::vector<std::string> >& options)
{
  m_options = options;
}

void te::attributefill::VectorToVector::setOutput(te::da::DataSourcePtr outDsrc, std::string dsName)
{
  m_outDsrc = outDsrc;
  m_outDset = dsName;
}

bool te::attributefill::VectorToVector::paramsAreValid()
{
  return true;
}

bool te::attributefill::VectorToVector::run()
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
      std::vector<std::string> funcs = it->second;

      te::stat::NumericStatisticalSummary ssNum;
      te::stat::StringStatisticalSummary ssStr;

      std::vector<double> numValues;
      std::vector<std::string> strValues;
      std::vector<te::dt::AbstractData*> dataValues;

      if(it->first->getType() == te::dt::STRING_TYPE)
      {
        strValues = getStrValues(fromDs.get(), intersections, it->first->getName());
        te::stat::GetStringStatisticalSummary(strValues, ssStr, "");
      }
      else
      {
        numValues = getNumValues(fromDs.get(), intersections, it->first->getName());
        te::stat::GetNumericStatisticalSummary(numValues, ssNum);
      }

      dataValues = getDataValues(fromDs.get(), intersections, it->first->getName());

      for(std::size_t i = 0; i < funcs.size(); ++i)
      {
        std::string outPropName = getPropertyName(it->first, funcs[i]);

        if(funcs[i] == "Value")
        {
          item->setValue(outPropName, dataValues[0]);
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

bool te::attributefill::VectorToVector::save(std::auto_ptr<te::mem::DataSet> result,
                                             std::auto_ptr<te::da::DataSetType> outDsType)
{
  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDsType.get(), m_outDsrc->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(result.get(), converter));

  // create the primary key if it is possible
  if (m_outDsrc->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  {
    std::string pk_name = dsTypeResult->getName() + "_pkey";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
    pk->add(dsTypeResult->getProperty(0));
    outDsType->setPrimaryKey(pk);
  }

  std::map<std::string, std::string> options;
  // create the dataset
  m_outDsrc->createDataSet(dsTypeResult, options);

  // copy from memory to output datasource
  result->moveBeforeFirst();
  m_outDsrc->add(dsTypeResult->getName(),result.get(), options);

  return true;
}

te::da::DataSetType* te::attributefill::VectorToVector::getOutputDataSetType()
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

      if(funcs[i] == "Value" || funcs[i] == "Major Class")
      {
        newProp = dynamic_cast<te::dt::SimpleProperty*>(currentProperty->clone());
        newProp->setRequired(false);
        newProp->setName(newName);
      }
      else if(it->first->getType() == te::dt::STRING_TYPE || funcs[i] == "Mode")
      {
        newProp = new te::dt::StringProperty(newName);
      }
      else
      {
        std::string newName = getPropertyName(it->first, funcs[i]);
        newProp = new te::dt::SimpleProperty(newName, te::dt::DOUBLE_TYPE);
      }

      dst->add(newProp);
    }

    ++it;
  }

  return dst;
}

te::sam::rtree::Index<size_t, 8>* te::attributefill::VectorToVector::getRtree(te::da::DataSet* data)
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

std::string te::attributefill::VectorToVector::getPropertyName(te::dt::Property* prop, const std::string& func)
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

std::vector<std::size_t> te::attributefill::VectorToVector::getIntersections(te::da::DataSet* toDs,
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

std::vector<double> te::attributefill::VectorToVector::getNumValues(te::da::DataSet* fromDs,
                                                                 std::vector<std::size_t> dsPos,
                                                                 const std::string& propertyName)
{
  std::vector<double> result;

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    result.push_back(fromDs->getDouble(propertyName));
  }

  return result;
}

std::vector<std::string> te::attributefill::VectorToVector::getStrValues(te::da::DataSet* fromDs,
                                                                            std::vector<std::size_t> dsPos,
                                                                            const std::string& propertyName)
{
  std::vector<std::string> result;

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    result.push_back(fromDs->getString(propertyName));
  }

  return result;
}

bool te::attributefill::VectorToVector::isStatistical(const std::string& funcName)
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

std::vector<std::string> te::attributefill::VectorToVector::getSelectedFunctions()
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

double te::attributefill::VectorToVector::getValue(te::stat::NumericStatisticalSummary ss, const std::string& function)
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

std::string te::attributefill::VectorToVector::getValue(te::stat::StringStatisticalSummary ss, const std::string& function)
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

std::string te::attributefill::VectorToVector::getModeValue(te::stat::NumericStatisticalSummary ss)
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

std::vector<te::dt::AbstractData*> te::attributefill::VectorToVector::getDataValues(te::da::DataSet* fromDs,
                                                                                    std::vector<std::size_t> dsPos,
                                                                                    const std::string& propertyName)
{
  std::vector<te::dt::AbstractData*> result;

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    result.push_back(fromDs->getValue(propertyName).release());
  }

  return result;
}