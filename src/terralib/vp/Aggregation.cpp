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
  \file Aggregation.h

  \brief Aggregation Vector Processing functions.
*/

//Terralib
#include "../common/Translator.h"
#include "../common/progress/TaskProgress.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/query_h.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/Utils.h"
#include "Aggregation.h"
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


// -- auxiliary functions
bool AggregationQuery(const std::string& inDataset,
                      te::da::DataSource* inDatasource,
                      const std::vector<te::dt::Property*>& groupingProperties,
                      const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                      te::mem::DataSet* outputDataSet,
                      te::gm::GeomType outGeoType);

bool AggregationMemory(const std::string& inDataset,
                       te::da::DataSource* inDatasource,
                       const std::vector<te::dt::Property*>& groupingProperties,
                       const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                       te::mem::DataSet* outputDataSet,
                       te::gm::GeomType outGeoType);

te::da::DataSetType* BuildOutputDataSetType(const std::string& name,
                                            const std::vector<te::dt::Property*>& properties,
                                            const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary);

void SetOutputDatasetQuery( const std::vector<te::dt::Property*>& groupingProperties,
                           te::da::DataSet* dsQuery,
                           te::mem::DataSet* outputDataSet, te::gm::GeomType outGeoType);

std::map<std::string, std::vector<te::mem::DataSetItem*> > GetGroups(te::da::DataSet* inputDataSet,
                                                                     const std::vector<te::dt::Property*>& groupingProperties);

std::map<std::string, std::string> CalculateStringGroupingFunctions(const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                                                    const std::vector<te::mem::DataSetItem*>& items);

std::map<std::string, double> CalculateDoubleGroupingFunctions(const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                                               const std::vector<te::mem::DataSetItem*>& items);

// ---

bool te::vp::Aggregation(const std::string& inDataset,
                         te::da::DataSource* inDatasource,
                         const std::vector<te::dt::Property*>& groupingProperties,
                         const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                         const std::string& outDataset,
                         te::da::DataSource* outDatasource)
{
  assert(inDatasource);
  assert(outDatasource);
  
  // define the schema of the output dataset based on the aggregation parameters for the non-spatial attributes
  std::auto_ptr<te::da::DataSetType> outputDataSetType(BuildOutputDataSetType(outDataset, groupingProperties, statisticalSummary));
  
  // define the resulting spatial property
  std::auto_ptr<te::da::DataSetType> inSchema = inDatasource->getDataSetType(inDataset);
  te::gm::GeometryProperty* p = static_cast<te::gm::GeometryProperty*>(inSchema->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));
  
  // creates the output geometry property
  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  te::gm::GeomType outGeoType = te::vp::GeomOpResultType(p->getGeometryType());
  geometry->setGeometryType(outGeoType);
  geometry->setSRID(p->getSRID());
  outputDataSetType->add(geometry);
  
  // create the output dataset in memory
  std::auto_ptr<te::mem::DataSet> memDataset(new te::mem::DataSet(outputDataSetType.get()));
  
  // select a strategy based on the capabilities of the input datasource
  const te::da::DataSourceCapabilities dsCapabilities = inDatasource->getCapabilities();
  
  // execute the strategy
  bool res;
  if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
  {
    res = AggregationQuery(inDataset, inDatasource, groupingProperties, statisticalSummary, memDataset.get(),outGeoType);
  }
  else
  {
    res = AggregationMemory(inDataset, inDatasource, groupingProperties, statisticalSummary, memDataset.get(),outGeoType);
  }
  
  if (!res)
    return false;

  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outputDataSetType.get(), outDatasource->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(memDataset.get(), converter));

  std::map<std::string, std::string> options;
  // create the dataset
  outDatasource->createDataSet(dsTypeResult, options);

  // copy from memory to output datasource
  memDataset->moveBeforeFirst();
  outDatasource->add(dsTypeResult->getName(),memDataset.get(), options);

  // create the primary key if it is possible
  if (outDatasource->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  {
    std::string pk_name = dsTypeResult->getName() + "_pkey";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
    pk->add(dsTypeResult->getProperty(0));
    outDatasource->addPrimaryKey(outDataset,pk);
  }
  return true;
}


bool AggregationQuery(const std::string& inDataset,
                      te::da::DataSource* inDatasource,
                      const std::vector<te::dt::Property*>& groupingProperties,
                      const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                      te::mem::DataSet* outputDataSet,
                      te::gm::GeomType outGeoType)
{
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator itStatSummary = statisticalSummary.begin();
  
  std::auto_ptr<te::da::DataSetType> dsType = inDatasource->getDataSetType(inDataset);
  
  te::da::Fields* fields = new te::da::Fields;
  
  for(std::size_t i = 0; i < groupingProperties.size(); ++i)
  {
    te::da::Field* f_aggName = new te::da::Field(groupingProperties[i]->getName());
    fields->push_back(f_aggName);
  }
  
  te::da::Expression* e_aggCount = new te::da::Count(new te::da::PropertyName(groupingProperties[0]->getName()));
  te::da::Field* f_aggCount = new te::da::Field(*e_aggCount, "Aggregation_Count");
  fields->push_back(f_aggCount);
  
  while(itStatSummary != statisticalSummary.end())
  {
    int propType = itStatSummary->first->getType();
    
    if(propType == te::dt::STRING_TYPE)
    {
      te::da::PropertyName* p_name = new te::da::PropertyName(itStatSummary->first->getName());
      te::da::PropertyName* p_count = new te::da::PropertyName("*");
      
      te::da::Expression* e_min = new te::da::Min(p_name);
      te::da::Field* f_min = new te::da::Field(*e_min, p_name->getName() + "_MIN_VALUE");
      
      te::da::Expression* e_max = new te::da::Max(p_name);
      te::da::Field* f_max = new te::da::Field(*e_max,  p_name->getName() + "_MAX_VALUE");
      
      te::da::Expression* e_count = new te::da::Count(p_count);
      te::da::Field* f_count = new te::da::Field(*e_count,  p_name->getName() + "_COUNT");
      
      te::da::Expression* e_validcount = new te::da::Count(p_name);
      te::da::Field* f_validcount = new te::da::Field(*e_validcount,  p_name->getName() + "_VALID_COUNT");
      
      fields->push_back(f_min);
      fields->push_back(f_max);
      fields->push_back(f_count);
      fields->push_back(f_validcount);
    }
    else
    {
      te::da::PropertyName* p_name = new te::da::PropertyName(itStatSummary->first->getName());
      te::da::PropertyName* p_count = new te::da::PropertyName("*");
      
      te::da::Expression* e_min = new te::da::Min(p_name);
      te::da::Field* f_min = new te::da::Field(*e_min, p_name->getName() + "_MIN_VALUE");
      
      te::da::Expression* e_max = new te::da::Max(p_name);
      te::da::Field* f_max = new te::da::Field(*e_max, p_name->getName() + "_MAX_VALUE");
      
      te::da::Expression* e_count = new te::da::Count(p_count);
      te::da::Field* f_count = new te::da::Field(*e_count, p_name->getName() + "_COUNT");
      
      te::da::Expression* e_validcount = new te::da::Count(p_name);
      te::da::Field* f_validcount = new te::da::Field(*e_validcount, p_name->getName() + "_VALID_COUNT");
      
      te::da::Expression* e_sum = new te::da::Sum(p_name);
      te::da::Field* f_sum = new te::da::Field(*e_sum, p_name->getName() + "_SUM");
      
      te::da::Expression* e_mean = new te::da::Avg(p_name);
      te::da::Field* f_mean = new te::da::Field(*e_mean, p_name->getName() + "_MEAN");
      
      te::da::Expression* e_stddev = new te::da::StdDev(p_name);
      te::da::Field* f_stddev = new te::da::Field(*e_stddev, p_name->getName() + "_STANDARD_DEVIATION");
      
      te::da::Expression* e_variance = new te::da::Variance(p_name);
      te::da::Field* f_variance = new te::da::Field(*e_variance, p_name->getName() + "_VARIANCE");
      
      te::da::Expression* e_amplitude = new te::da::Sub(*e_max, *e_min);
      te::da::Field* f_amplitude = new te::da::Field(*e_amplitude, p_name->getName() + "_AMPLITUDE");
      
      fields->push_back(f_min);
      fields->push_back(f_max);
      fields->push_back(f_count);
      fields->push_back(f_validcount);
      fields->push_back(f_sum);
      fields->push_back(f_mean);
      fields->push_back(f_stddev);
      fields->push_back(f_variance);
      fields->push_back(f_amplitude);
      
    }
    ++itStatSummary;
  }
  
  if(dsType->hasGeom())
  {
    te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(dsType.get());
    
    te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(geom->getName()));
    te::da::Field* f_union = new te::da::Field(*e_union, "geom");
    fields->push_back(f_union);
  }
  
  te::da::FromItem* fromItem = new te::da::DataSetName(dsType->getName());
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);
  
  te::da::Select select(fields, from);
  
  if(!groupingProperties.empty())
  {
    te::da::GroupBy* groupBy = new te::da::GroupBy();
    
    for(std::size_t i = 0; i < groupingProperties.size(); ++i)
    {
      te::da::GroupByItem* e_groupBy = new te::da::GroupByItem(groupingProperties[i]->getName());
      groupBy->push_back(e_groupBy);
    }
    select.setGroupBy(groupBy);
  }
  
  std::auto_ptr<te::da::DataSet> dsQuery = inDatasource->query(select);
  
  SetOutputDatasetQuery(groupingProperties, dsQuery.get(), outputDataSet,outGeoType);
  
  return true;
}

bool AggregationMemory(const std::string& inDataset,
                       te::da::DataSource* inDatasource,
                       const std::vector<te::dt::Property*>& groupingProperties,
                       const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                       te::mem::DataSet* outputDataSet,
                       te::gm::GeomType outGeoType)
{
  std::auto_ptr<te::da::DataSet> inputDataSet = inDatasource->getDataSet(inDataset);
  std::auto_ptr<te::da::DataSetType> dsType = inDatasource->getDataSetType(inDataset);
  
  std::size_t geomIdx;
  std::string geomName = "";
  
  if(dsType->hasGeom())
  {
    te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(dsType.get());
    geomName = geom->getName();
    geomIdx = boost::lexical_cast<std::size_t>(dsType->getPropertyPosition(geomName));
  }
  
  std::map<std::string, std::vector<te::mem::DataSetItem*> > groupValues = GetGroups(inputDataSet.get(), groupingProperties);
  
  std::map<std::string, std::vector<te::mem::DataSetItem*> >::const_iterator itGroupValues = groupValues.begin();
  
  te::common::TaskProgress task("Processing aggregation...");
  task.setTotalSteps(groupValues.size());
  task.useTimer(true);
  
  std::auto_ptr<te::mem::DataSetItem> dataSetItem(new te::mem::DataSetItem(inputDataSet.get()));
  while(itGroupValues != groupValues.end())
  {
    std::string value = itGroupValues->first.c_str();
    int aggregationCount = itGroupValues->second.size();
    
    std::map<std::string, std::string> functionResultStringMap = CalculateStringGroupingFunctions(statisticalSummary, itGroupValues->second);
    std::map<std::string, double> functionResultDoubleMap = CalculateDoubleGroupingFunctions(statisticalSummary, itGroupValues->second);

    
    te::gm::Geometry* geometry = te::vp::GetGeometryUnion(itGroupValues->second, geomIdx, outGeoType);

    if(geometry)
    {
      te::mem::DataSetItem* outputDataSetItem = new te::mem::DataSetItem(outputDataSet);
    
      outputDataSetItem->setString(0, value);
      outputDataSetItem->setInt32(1, aggregationCount);
    
      if(!functionResultStringMap.empty())
      {
        std::map<std::string, std::string>::iterator itFuncResultString = functionResultStringMap.begin();
      
        while(itFuncResultString != functionResultStringMap.end())
        {
          if(te::da::GetPropertyPos(outputDataSet, itFuncResultString->first.c_str()) < outputDataSet->getNumProperties())
            outputDataSetItem->setString(itFuncResultString->first.c_str(), itFuncResultString->second.c_str());
        
          ++itFuncResultString;
        }
      }
    
      if(!functionResultDoubleMap.empty())
      {
        std::map<std::string, double>::iterator itFuncResultDouble = functionResultDoubleMap.begin();

        std::string propMode;
        std::string auxPropMode;
        std::string modeValue;
        std::string auxValue;
        bool mode = false;

        while(itFuncResultDouble != functionResultDoubleMap.end())
        {
          propMode = itFuncResultDouble->first.c_str();
          
          unsigned pos = propMode.find("_");
          propMode = propMode.substr(pos+1);
          pos = propMode.find("_");
          propMode = propMode.substr(pos+1);

          std::string shortMode = "";

          if(propMode.length() > 3)
            shortMode = propMode.substr(propMode.length()-4, 4);

          if(propMode == auxPropMode || auxPropMode == "")
          {
            if(shortMode == "MODE")
            {
              modeValue += ", "+boost::lexical_cast<std::string>(itFuncResultDouble->second);
              auxPropMode = propMode;
              mode = true;
            }
          }
          else
          {
            if(mode)
            {
              modeValue.erase(0,2);

              if(te::da::GetPropertyPos(outputDataSet, auxPropMode) < outputDataSet->getNumProperties())
                outputDataSetItem->setString(auxPropMode, modeValue);

              mode = false;

              if(shortMode == "MODE")
              {
                modeValue = "";
                modeValue += ", "+boost::lexical_cast<std::string>(itFuncResultDouble->second);
                auxPropMode = propMode;
                mode = true;
              }
            }
          }

          if(te::da::GetPropertyPos(outputDataSet, itFuncResultDouble->first.c_str()) < outputDataSet->getNumProperties())
              outputDataSetItem->setDouble(itFuncResultDouble->first.c_str(), itFuncResultDouble->second);

          ++itFuncResultDouble;
        }
      }
    
      outputDataSetItem->setGeometry("geom", geometry);
    
      outputDataSet->add(outputDataSetItem);
    }

    ++itGroupValues;
    
    if(task.isActive() == false)
    {
      throw te::vp::Exception(TR_VP("Operation canceled!"));
    }
    
    task.pulse();
  }
  
  return true;
}

te::da::DataSetType* BuildOutputDataSetType(const std::string& name,
                                            const std::vector<te::dt::Property*>& properties, 
                                            const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary)
{
  te::da::DataSetType* dataSetType = new te::da::DataSetType(name);
  
  // those are the properties used to aggregate the objects
  std::string propertyResult;
  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    propertyResult += "_" + properties[i]->getName();
  }
  propertyResult.erase(propertyResult.begin());
  te::dt::StringProperty* stringProperty = new te::dt::StringProperty(propertyResult);
  dataSetType->add(stringProperty);

  // the number of objects aggregated
  te::dt::SimpleProperty* aggregationProperty = new te::dt::SimpleProperty("NUM_OBJ", te::dt::INT32_TYPE);
  dataSetType->add(aggregationProperty);
  
  // properties that reference the statistics requested
  std::string functionResult;
  std::vector<te::stat::StatisticalSummary> vectorResult;
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator it = statisticalSummary.begin();
  while(it != statisticalSummary.end())
  {
    propertyResult = "";
    propertyResult = it->first->getName();
    propertyResult += "_";
    
    vectorResult = it->second;

    for(std::size_t i = 0; i < vectorResult.size(); ++i)
    {
      functionResult = propertyResult;
      functionResult += te::stat::GetStatSummaryShortName(vectorResult[i]);
      
      if(it->first->getType() == te::dt::STRING_TYPE || vectorResult[i] == te::stat::MODE)
      {
        te::dt::StringProperty* functrionProperty = new te::dt::StringProperty(functionResult);
        dataSetType->add(functrionProperty);
      }
      else
      {
        te::dt::SimpleProperty* functrionProperty = new te::dt::SimpleProperty(functionResult, te::dt::DOUBLE_TYPE);
        dataSetType->add(functrionProperty);
      }
    }

    ++it;
  }
  return dataSetType;
}


void SetOutputDatasetQuery( const std::vector<te::dt::Property*>& groupingProperties,
                            te::da::DataSet* dsQuery,
                            te::mem::DataSet* outputDataSet,
                            te::gm::GeomType outGeoType)
{
  std::size_t n_dsProp = dsQuery->getNumProperties();
  std::size_t selectedPropSize = groupingProperties.size();
  
  te::mem::DataSetItem* outputDataSetItem;
  int dsPropType;
  
  dsQuery->moveBeforeFirst();
  while(dsQuery->moveNext())
  {
    outputDataSetItem = new te::mem::DataSetItem(outputDataSet);
    std::string aggregItem = "";

    for(std::size_t i = 0; i < n_dsProp; ++i)
    {
      if(i < selectedPropSize)
      {
        if(aggregItem != "")
           aggregItem += "_";

        aggregItem += dsQuery->getAsString(i);

        outputDataSetItem->setString(0, aggregItem);
      }
      else
      {
        std::string propName = dsQuery->getPropertyName(i);

        if(boost::iequals(propName, "NUM_OBJ"))
        {
          int aggregValue = boost::lexical_cast<int>(dsQuery->getAsString(i));
          outputDataSetItem->setInt32(1, aggregValue);
        }
        else
        {
          dsPropType = dsQuery->getPropertyDataType(i);

          if(dsPropType == te::dt::GEOMETRY_TYPE)
          {
            std::auto_ptr<te::gm::Geometry> geometry(dsQuery->getGeometry(i));
            if (geometry->getGeomTypeId() != outGeoType)
            {
              te::gm::GeometryCollection* gc = new te::gm::GeometryCollection(1,te::vp::GeomOpResultType(geometry->getGeomTypeId()),geometry->getSRID());
              gc->setGeometryN(0, geometry.release());
              outputDataSetItem->setGeometry("geom", gc);
            }
            else
              outputDataSetItem->setGeometry("geom", geometry.release());
          }
          if(dsPropType == te::dt::STRING_TYPE)
          {
            std::string propName = dsQuery->getPropertyName(i);
            std::size_t index = te::da::GetPropertyPos(outputDataSetItem->getParent(), propName);
            
            if(index < outputDataSetItem->getNumProperties())
            {
              std::string value = dsQuery->getAsString(i);
              if(!value.empty())
                outputDataSetItem->setString(index, value);
            }
          }
          if(dsPropType == te::dt::NUMERIC_TYPE)
          {
            std::string propName = dsQuery->getPropertyName(i);
            std::size_t index = te::da::GetPropertyPos(outputDataSetItem->getParent(), propName);
            
            if(index < outputDataSetItem->getNumProperties())
            {
              std::string queryValue = dsQuery->getNumeric(i);
              if(!queryValue.empty())
              {
                double value = boost::lexical_cast<double>(queryValue);
                outputDataSetItem->setDouble(index, value);
              }
            }
          }
          if(dsPropType == te::dt::DOUBLE_TYPE)
          {
            std::string propName = dsQuery->getPropertyName(i);
            std::size_t index = te::da::GetPropertyPos(outputDataSetItem->getParent(), propName);
            
            if(index < outputDataSetItem->getNumProperties())
            {
              double value = dsQuery->getDouble(i);
              outputDataSetItem->setDouble(index, value);
            }
          }
          if(dsPropType == te::dt::INT64_TYPE)
          {
            std::string propName = dsQuery->getPropertyName(i);
            std::size_t index = te::da::GetPropertyPos(outputDataSetItem->getParent(), propName);
            
            if(index < outputDataSetItem->getNumProperties())
            {
              int type = outputDataSetItem->getPropertyDataType(index);
              if(type == te::dt::DOUBLE_TYPE)
              {
                std::string queryValue = dsQuery->getAsString(i);
                if(!queryValue.empty())
                {
                  double value = boost::lexical_cast<double>(queryValue);
                  outputDataSetItem->setDouble(index, value);
                }
              }
              if(type == te::dt::STRING_TYPE)
              {
                std::string value = dsQuery->getAsString(i);
                if(!value.empty())
                  outputDataSetItem->setString(index, value);
              }
            }
          }
          if(dsPropType == te::dt::INT32_TYPE)
          {
            std::string propName = dsQuery->getPropertyName(i);
            std::size_t index = te::da::GetPropertyPos(outputDataSetItem->getParent(), propName);
            
            if(index < outputDataSetItem->getNumProperties())
            {
              int type = outputDataSetItem->getPropertyDataType(index);
              if(type == te::dt::DOUBLE_TYPE)
              {
                std::string queryValue = dsQuery->getAsString(i);
                if(!queryValue.empty())
                {
                  double value = boost::lexical_cast<double>(queryValue);
                  outputDataSetItem->setDouble(index, value);
                }
              }
              if(type == te::dt::STRING_TYPE)
              {
                std::string value = dsQuery->getAsString(i);
                if(!value.empty())
                  outputDataSetItem->setString(index, value);
              }
            }
          }
        }
      }
    }

    outputDataSet->add(outputDataSetItem);
  }
  
  outputDataSet->moveBeforeFirst();
}

std::map<std::string, std::vector<te::mem::DataSetItem*> > GetGroups( te::da::DataSet* inputDataSet,
                                                                              const std::vector<te::dt::Property*>& groupingProperties)
{
  std::map<std::string, std::vector<te::mem::DataSetItem*> > groupValues;

  while(inputDataSet->moveNext())
  {
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(inputDataSet);
    
    for(std::size_t i = 0; i < inputDataSet->getNumProperties(); ++i)
    {
      if (!inputDataSet->isNull(i))
      {
        std::auto_ptr<te::dt::AbstractData> val = inputDataSet->getValue(i);
        dataSetItem->setValue(i,val.release());
      }
    }

    std::size_t propertyIndex = 0;

    bool found = false;
    std::vector<te::mem::DataSetItem*> dataSetItemVector;
    std::map<std::string, std::vector<te::mem::DataSetItem*> >::iterator it;

    std::string propertyName;
    std::string value;

    for(std::size_t i = 0; i < groupingProperties.size(); ++i)
    {
      propertyName += "_" + groupingProperties[i]->getName();

      propertyIndex = te::da::GetPropertyPos(dataSetItem->getParent(), groupingProperties[i]->getName());
      value += "_" + inputDataSet->getAsString(propertyIndex);
    }

    propertyName.erase(propertyName.begin());
    value.erase(value.begin());

    for(it = groupValues.begin(); it != groupValues.end(); ++it)
    {
      if(it->first == value)
      {
        it->second.push_back(dataSetItem);
        found = true;
        break;
      }
    }

    if(found == false)
    {
      dataSetItemVector.push_back(dataSetItem);
      groupValues.insert(std::pair<std::string, std::vector<te::mem::DataSetItem*> >(value, dataSetItemVector));
    }
  }

  return groupValues;
}

std::map<std::string, std::string> CalculateStringGroupingFunctions(const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                                                            const std::vector<te::mem::DataSetItem*>& items)
{
  std::map<std::string, std::string> result;

  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator it = statisticalSummary.begin();

  while(it != statisticalSummary.end())
  {
    if(it->first->getType() == te::dt::STRING_TYPE)
    {
      std::vector<std::string> values;
      std::string propertyName = it->first->getName();
      std::size_t index = te::da::GetPropertyPos(items[0]->getParent(),propertyName);
      
      for(std::size_t i = 0; i < items.size(); ++i)
      {
        if (!items[i]->isNull(index))
          values.push_back(items[i]->getString(index));
      }

      te::stat::StringStatisticalSummary ss; 
      te::stat::GetStringStatisticalSummary(values, ss);

      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_MIN_VALUE", ss.m_minVal ));
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_MAX_VALUE", ss.m_maxVal ));
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_COUNT", boost::lexical_cast<std::string>(items.size())));
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_VALID_COUNT", boost::lexical_cast<std::string>(values.size())));
    }
    ++it;
  }

  return result;
}

std::map<std::string, double> CalculateDoubleGroupingFunctions( const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                                      const std::vector<te::mem::DataSetItem*>& items)
{
  std::map<std::string, double> result;
  int idProp = 0;
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator it = statisticalSummary.begin();

  while(it != statisticalSummary.end())
  {
    if(it->first->getType() != te::dt::STRING_TYPE)
    {
      std::string propertyName = it->first->getName();
      std::size_t index = te::da::GetPropertyPos(items[0]->getParent(), propertyName);
      std::size_t type = items[0]->getParent()->getPropertyDataType(index);
      
      std::vector<double> values;
      for(std::size_t i = 0; i < items.size(); ++i)
      {
        if (!items[i]->isNull(index))
        {
          double numval;
          if (type == te::dt::INT16_TYPE)
           numval = items[i]->getInt16(index); 
          else if (type == te::dt::INT32_TYPE)
            numval = items[i]->getInt32(index);
          else if (type == te::dt::INT64_TYPE)
            numval = (double)items[i]->getInt64(index);
          else if (type == te::dt::FLOAT_TYPE)
            numval = items[i]->getFloat(index);
          else if (type == te::dt::DOUBLE_TYPE)
            numval = items[i]->getDouble(index);
          values.push_back(numval);
        }
      }

      te::stat::NumericStatisticalSummary ss;
      te::stat::GetNumericStatisticalSummary(values, ss);

      result.insert( std::map<std::string, double>::value_type( propertyName + "_MIN_VALUE", ss.m_minVal ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MAX_VALUE", ss.m_maxVal ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_COUNT", items.size()) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VALID_COUNT", values.size()) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MEAN", ss.m_mean ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_SUM", ss.m_sum ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_STANDARD_DEVIATION", ss.m_stdDeviation ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VARIANCE", ss.m_variance ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_SKEWNESS", ss.m_skewness ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_KURTOSIS", ss.m_kurtosis ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_AMPLITUDE", ss.m_amplitude ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MEDIAN", ss.m_median ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VAR_COEFF", ss.m_varCoeff ) );
      
      for(std::size_t i = 0; i < ss.m_mode.size(); ++i)
        result.insert( std::map<std::string, double>::value_type( boost::lexical_cast<std::string>(idProp) + "_" + 
                                                                  boost::lexical_cast<std::string>(i) + "_" + 
                                                                  propertyName + "_MODE", ss.m_mode[i] ) );
    }
    ++it;
    ++idProp;
  }

  return result;
}

