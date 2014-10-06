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

#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"

#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/utils/Utils.h"

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

void te::attributefill::VectorToVector::setParams(std::map<te::dt::Property*, std::vector<std::string> >& options,
                                                  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >&statSum)
{
  m_statSum = statSum;
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

    std::vector<std::size_t> intersections = getIntersections(toDs.get(), fromDs.get(), rtree);

    for(std::size_t i = 0; i < toSchema->size(); ++i)
    {
      item->setValue(i, toDs->getValue(i).release());
    }

    std::map<te::dt::Property*, std::vector<std::string> >::iterator it = m_options.begin();

    while(it != m_options.end())
    {
      std::vector<std::string> funcs = it->second;

      for(std::size_t i = 0; i < funcs.size(); ++i)
      {
        std::string outPropName = getPropertyName(it->first, funcs[i]);

        std::vector<double> values = getValues(fromDs.get(), intersections, funcs[i]);

        te::stat::NumericStatisticalSummary ss;

        te::stat::GetNumericStatisticalSummary(values, ss);

        double value = getValue(ss, funcs[i]);

        item->setDouble(outPropName, value);
      }

      ++it;
    }

    outDs->add(item);
  }

  save(outDs, outDst);
}

bool te::attributefill::VectorToVector::save(std::auto_ptr<te::mem::DataSet> result,
                                             std::auto_ptr<te::da::DataSetType> outDsType)
{
  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDsType.get(), m_outDsrc->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(result.get(), converter));
  
  std::map<std::string, std::string> options;
  // create the dataset
  m_outDsrc->createDataSet(dsTypeResult, options);
  
  // copy from memory to output datasource
  result->moveBeforeFirst();
  m_outDsrc->add(dsTypeResult->getName(),result.get(), options);
  
  // create the primary key if it is possible
  if (m_outDsrc->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  {
    std::string pk_name = dsTypeResult->getName() + "_pkey";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
    pk->add(dsTypeResult->getProperty(0));
    m_outDsrc->addPrimaryKey(m_outDset,pk);
  }
  
  return true;
}

te::da::DataSetType* te::attributefill::VectorToVector::getOutputDataSetType()
{
  std::auto_ptr<te::da::DataSetType> toScheme = m_toLayer->getSchema();

  te::da::DataSetType* dst = new te::da::DataSetType(*toScheme.get());
  dst->setName(m_outDset);

  std::vector<te::dt::Property*> toProps = toScheme->getProperties();

  std::map<te::dt::Property*, std::vector<std::string> >::iterator it = m_options.begin();

  std::vector<std::string> alreadyAdded;
  while(it != m_options.end())
  {
    std::vector<std::string> funcs = it->second;

    te::dt::Property* currentProperty = it->first;

    for(std::size_t i = 0; i < funcs.size(); ++i)
    {
      te::dt::Property* newProp = 0;

      std::string baseName = it->first->getName() + "_";

      if(funcs[i] == "Value" || funcs[i] == "Major Class")
      {
        std::string newName = getPropertyName(it->first, funcs[i]);

        newProp = currentProperty->clone();
        newProp->setName(newName);
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
  std::string name = "";

  std::string baseName = prop->getName() + "_";

  if(func == "Value" || func == "Major Class")
  {
    name = baseName;
    if(func == "Value")
      name += "value";
    else
      name += "major_class";
  }
  else if(func == "Total number of values" ||
          func == "Total not null values")
  {
    name = func;
    std::replace(name.begin(), name.end(), ' ', '_');
    name = baseName + name;
  }
  else if(func == "Mode")
  {
    name = baseName;
    name += "mode";
  }
  else if(func == "Percentage per Class" || func == "Minimum Distance")
  {
    name = baseName;
    if(func == "Percentage per Class")
      name += "percent_class";
    else
      name += "min_distance";
  }
  else if(func == "Presence")
  {
    name = baseName + "presence";
  }
  else
  {
    name = func;
    std::replace(name.begin(), name.end(), ' ', '_');
    name = baseName + name;
  }

  return name;
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

std::vector<double> te::attributefill::VectorToVector::getValues(te::da::DataSet* fromDs, 
                                                                 std::vector<std::size_t> dsPos,
                                                                 const std::string& function)
{
  std::map<te::dt::Property*, std::vector<std::string> >::iterator it = m_options.begin();

  std::vector<double> values;

  std::vector<std::string> propsNames;
  while(it != m_options.end())
  {
    std::vector<std::string> funcVec = it->second;

    if(std::find(funcVec.begin(), funcVec.end(), function) != funcVec.end())
    {
      propsNames.push_back(it->first->getName());
    }

    ++it;
  }

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    fromDs->move(dsPos[i]);

    for(std::size_t j = 0; j < propsNames.size(); ++j)
    {
      values.push_back(fromDs->getDouble(propsNames[j]));
    }
  }

  return values;
}

bool te::attributefill::VectorToVector::isStatistical(const std::string& funcName)
{
  if(funcName == "Minimum value" ||
     funcName == "Maximum value" ||
     funcName == "Mean" ||
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
     return true;
  else
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
  if(function == "Minimum value")
    return ss.m_minVal;
  else if(function == "Maximum value")
    return ss.m_maxVal;
  else if(function == "Mean")
    return ss.m_mean;
  else if(function == "Total number of values")
    return ss.m_count;
  else if(function == "Total not null values")
    return ss.m_validCount;
  else if(function == "Standard deviation")
    return ss.m_stdDeviation;
  else if(function == "Variance")
    return ss.m_variance;
  else if(function == "Skewness")
    return ss.m_skewness;
  else if(function == "Amplitude")
    return ss.m_amplitude;
  else if(function == "Median")
    return ss.m_median;
  else if(function == "Coefficient variation")
    return ss.m_varCoeff;
  else
    return -1;

  /*
  else if(function == "Mode")
    return ss.m_mode;
    */
  
}