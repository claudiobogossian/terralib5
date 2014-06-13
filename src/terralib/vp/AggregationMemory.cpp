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
  \file AggregationMemory.h

  \brief Aggregation Vector Processing functions.
*/

//Terralib

#include "../common/progress/TaskProgress.h"
#include "../common/Logger.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/Utils.h"

#include "AggregationMemory.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <map>
#include <math.h>
#include <string>
#include <vector>

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

te::vp::AggregationMemory::AggregationMemory()
{}

te::vp::AggregationMemory::~AggregationMemory()
{}

std::map<std::string, std::string> te::vp::AggregationMemory::calculateStringStats(const std::vector<te::mem::DataSetItem*>& items)
{
  std::map<std::string, std::string> result;
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator it = m_statSum.begin();
  while(it != m_statSum.end())
  {
    if(it->first->getType() == te::dt::STRING_TYPE)
    {
      size_t propPos = m_inDsetType->getPropertyPosition(it->first->getName());
      std::vector<std::string> values;
      for(std::size_t i = 0; i < items.size(); ++i)
      {
        if (!items[i]->isNull(propPos))
          values.push_back(items[i]->getString(propPos));
      }
      
      te::stat::StringStatisticalSummary ss;
      te::stat::GetStringStatisticalSummary(values, ss);
      
      result.insert( std::map<std::string, std::string>::value_type(it->first->getName() + "_MIN_VALUE", ss.m_minVal ));
      result.insert( std::map<std::string, std::string>::value_type(it->first->getName() + "_MAX_VALUE", ss.m_maxVal ));
      result.insert( std::map<std::string, std::string>::value_type(it->first->getName() + "_COUNT", boost::lexical_cast<std::string>(items.size())));
      result.insert( std::map<std::string, std::string>::value_type(it->first->getName()+ "_VALID_COUNT", boost::lexical_cast<std::string>(values.size())));
    }
    ++it;
  }
  return result;
}

std::map<std::string,double> te::vp::AggregationMemory::calculateNumStats(const std::vector<te::mem::DataSetItem*>& items, std::string& modeProp, std::string& modeName)
{
  std::map<std::string, double> result;
  int idProp = 0;
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator it = m_statSum.begin();
  while(it != m_statSum.end())
  {
    if (it->first->getType() != te::dt::STRING_TYPE)
    {
      size_t propPos = m_inDsetType->getPropertyPosition(it->first->getName());
      int propType = it->first->getType();
      
      std::vector<double> values;
      for(std::size_t i = 0; i < items.size(); ++i)
      {
        if (!items[i]->isNull(propPos))
        {
          double numval;
          if (propType == te::dt::INT16_TYPE)
            numval = items[i]->getInt16(propPos);
          else if (propType == te::dt::INT32_TYPE)
            numval = items[i]->getInt32(propPos);
          else if (propType == te::dt::INT64_TYPE)
            numval = (double)items[i]->getInt64(propPos);
          else if (propType == te::dt::FLOAT_TYPE)
            numval = items[i]->getFloat(propPos);
          else if (propType == te::dt::DOUBLE_TYPE)
            numval = items[i]->getDouble(propPos);
          else
            continue;
          
          values.push_back(numval);
        }
      }
      
      std::string propName = it->first->getName();
      
      te::stat::NumericStatisticalSummary ss;
      te::stat::GetNumericStatisticalSummary(values, ss);
      
      result.insert( std::map<std::string, double>::value_type(propName + "_MIN_VALUE", ss.m_minVal));
      result.insert( std::map<std::string, double>::value_type(propName + "_MAX_VALUE", ss.m_maxVal));
      result.insert( std::map<std::string, double>::value_type(propName + "_COUNT", items.size()));
      result.insert( std::map<std::string, double>::value_type(propName + "_VALID_COUNT", values.size()));
      result.insert( std::map<std::string, double>::value_type(propName + "_MEAN", ss.m_mean));
      result.insert( std::map<std::string, double>::value_type(propName + "_SUM", ss.m_sum));
      result.insert( std::map<std::string, double>::value_type(propName + "_STANDARD_DEVIATION", ss.m_stdDeviation));
      result.insert( std::map<std::string, double>::value_type(propName + "_VARIANCE", ss.m_variance));
      result.insert( std::map<std::string, double>::value_type(propName + "_SKEWNESS", ss.m_skewness));
      result.insert( std::map<std::string, double>::value_type(propName + "_KURTOSIS", ss.m_kurtosis));
      result.insert( std::map<std::string, double>::value_type(propName + "_AMPLITUDE", ss.m_amplitude));
      result.insert( std::map<std::string, double>::value_type(propName + "_MEDIAN", ss.m_median));
      result.insert( std::map<std::string, double>::value_type(propName + "_VAR_COEFF", ss.m_varCoeff));
      
      if (!ss.m_mode.empty())
      {
        modeProp = boost::lexical_cast<std::string>(ss.m_mode[0]);
        for(std::size_t i=1; i<ss.m_mode.size(); ++i)
        {
          modeProp += ",";
          modeProp += boost::lexical_cast<std::string>(ss.m_mode[i]);
        }
        modeName = propName + "_MODE";
      }
      else
      {
        modeName = "";
        modeProp = "";
      }
    }
    ++it;
    ++idProp;
  }
  return result;
}

std::auto_ptr<te::da::DataSetType> te::vp::AggregationMemory::buildOutDataSetType()
{
  std::auto_ptr<te::da::DataSetType> dataSetType(new te::da::DataSetType(m_outDset));
  
  // the property values used to execute the aggregation
  te::dt::StringProperty* stringProperty = new te::dt::StringProperty("AGG_PROP");
  dataSetType->add(stringProperty);
  
  // the number of objects aggregated
  te::dt::SimpleProperty* aggregationProperty = new te::dt::SimpleProperty("NUM_OBJ", te::dt::INT32_TYPE);
  dataSetType->add(aggregationProperty);
  
  // properties generated from the statistics requested
  std::string functionResult;
  std::vector<te::stat::StatisticalSummary> vectorResult;
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator it = m_statSum.begin();
  std::string propResult, funcResult;
  while(it != m_statSum.end())
  {
    propResult = "";
    propResult = it->first->getName();
    propResult += "_";
    
    vectorResult = it->second;
    
    for(std::size_t i = 0; i < vectorResult.size(); ++i)
    {
      funcResult = propResult;
      funcResult += te::stat::GetStatSummaryShortName(vectorResult[i]);
      
      if(it->first->getType() == te::dt::STRING_TYPE || vectorResult[i] == te::stat::MODE)
      {
        te::dt::StringProperty* funcProp = new te::dt::StringProperty(funcResult);
        dataSetType->add(funcProp);
      }
      else
      {
        te::dt::SimpleProperty* funcProp = new te::dt::SimpleProperty(funcResult, te::dt::DOUBLE_TYPE);
        dataSetType->add(funcProp);
      }
    }
    ++it;
  }
  
  // define the resulting spatial property
  te::gm::GeometryProperty* p = static_cast<te::gm::GeometryProperty*>(m_inDsetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));
  
  // creates the output geometry property
  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  geometry->setGeometryType(this->getGeomResultType(p->getGeometryType()));
  geometry->setSRID(p->getSRID());
  dataSetType->add(geometry);
  
  return dataSetType;
}


bool te::vp::AggregationMemory::run()
{  
  te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(m_inDsetType.get());
  std::string geomName = geom->getName();
  std::size_t geomIdx = boost::lexical_cast<std::size_t>(m_inDsetType->getPropertyPosition(geomName));
  
  // calculate the groups
  
  // get the positions of the grouping properties
  std::vector<size_t> groupPropIdxs;
  for(std::size_t i=0; i<m_groupProps.size(); ++i)
    groupPropIdxs.push_back(te::da::GetPropertyPos(m_inDsetType.get(), m_groupProps[i]->getName()));
  
  std::map<std::string, std::vector<te::mem::DataSetItem*> > groups;
  std::map<std::string, std::vector<te::mem::DataSetItem*> >::iterator itg;
  
  std::auto_ptr<te::da::DataSet> inDset;

  if(m_oidSet == 0)
    inDset = m_inDsrc->getDataSet(m_inDsetName);
  else
    inDset = m_inDsrc->getDataSet(m_inDsetName, m_oidSet);

  size_t nprops = inDset->getNumProperties();
  
  inDset->moveBeforeFirst();
  while(inDset->moveNext())
  {      
    // the group key is a combination of the distinct grouping property values as a string
    std::string key = inDset->getAsString(groupPropIdxs[0]);
    for(std::size_t i=1; i<groupPropIdxs.size(); ++i)
      key += "_" + inDset->getAsString(groupPropIdxs[i]);
    
    // copy it to a dataset item in memory (TODO: this should be reviewed to avoid the copy)
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(inDset.get());
    for(std::size_t j=0; j<nprops; ++j)
    {
      if (!inDset->isNull(j))
      {
        std::auto_ptr<te::dt::AbstractData> val = inDset->getValue(j);
        dataSetItem->setValue(j,val.release());
      }
    }
    
    itg = groups.find(key);
    if (itg==groups.end())
    {
      std::vector<te::mem::DataSetItem*> dataSetItemVector;
      dataSetItemVector.push_back(dataSetItem);
      groups.insert(std::pair<std::string, std::vector<te::mem::DataSetItem*> >(key,dataSetItemVector));
    }
    else
      itg->second.push_back(dataSetItem);
  }
  
  
  // tratamento de erro se nao gerou grupos
  
  // define the schema of the output dataset based on the aggregation parameters
  // for the non-spatial attributes
  std::auto_ptr<te::da::DataSetType> outDsType = this->buildOutDataSetType();
  
  // create the output dataset in memory
  std::auto_ptr<te::mem::DataSet> outDataset(new te::mem::DataSet(outDsType.get()));
  
  // now calculate the aggregation of non spatial and spatial attributes and save it to the output dataset
  te::common::TaskProgress task("Processing aggregation...");
  task.setTotalSteps(groups.size());
  task.useTimer(true);
  
  itg = groups.begin();
  while(itg != groups.end())
  {
    // calculate the spatial aggregation
    std::string value = itg->first;
    te::gm::GeomType outGeoType = te::vp::GeomOpResultType(geom->getGeometryType());
    te::gm::Geometry* geometry = te::vp::GetGeometryUnion(itg->second, geomIdx, outGeoType);
    
    // if it returned a valid geometry, include the summarization over non-spatial attributes
    if(geometry)
    {
      // calculate the statistical of text attributes
      std::map<std::string, std::string> resultString = calculateStringStats(itg->second);
      
      // calculate the statistical of numerical attributes
      std::string smodeprop, smodeval;  // special treatment for mode values
      std::map<std::string, double> resultNumeric = calculateNumStats(itg->second,smodeval,smodeprop);
      
      te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDataset.get());
      
      outDSetItem->setString(0, value);   // save the group identification (mandatory)
      outDSetItem->setInt32(1, itg->second.size()); // save the number of objects in the group (mandatory)
      
      // save statistics of text attributes
      std::map<std::string, std::string>::iterator itString = resultString.begin();
      while(itString != resultString.end())
      {
        // esse teste é necessário????
        if (te::da::GetPropertyPos(outDataset.get(), itString->first) < outDataset->getNumProperties())
            outDSetItem->setString(itString->first, itString->second);  
        ++itString;
      }
      
      // save statistics of numerical attributes
      if (!smodeval.empty())
        outDSetItem->setString(smodeprop, smodeval);
      
      std::map<std::string, double>::iterator itNumeric = resultNumeric.begin();
      while(itNumeric != resultNumeric.end())
      {
        if (te::da::GetPropertyPos(outDataset.get(), itNumeric->first) < outDataset->getNumProperties())
          outDSetItem->setDouble(itNumeric->first, itNumeric->second);
        ++itNumeric;
      }
      outDSetItem->setGeometry("geom", geometry);
      outDataset->add(outDSetItem);
    }
    ++itg;
  
    if (task.isActive() == false)
      throw te::vp::Exception(TE_TR("Operation canceled!"));
  
    task.pulse();
  }
  
  // save the result
  return save(outDataset,outDsType);
}
