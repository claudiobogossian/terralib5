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
  \file Aggregation.h

  \brief Aggregation Vector Processing functions.
*/

//Terralib
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetPersistence.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/DataSetTypePersistence.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../dataaccess/query_h.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleProperty.h"
#include "../../datatype/StringProperty.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/AbstractLayer.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../../statistics/core/SummaryFunctions.h"
#include "../../statistics/core/StringStatisticalSummary.h"
#include "../../statistics/core/NumericStatisticalSummary.h"
#include "../../statistics/core/Utils.h"
#include "Aggregation.h"
#include "AggregationDialog.h"
#include "Config.h"
#include "Exception.h"

// STL
#include <map>
#include <math.h>
#include <string>
#include <vector>

// BOOST
#include <boost/lexical_cast.hpp> 

void te::vp::Aggregation(const te::map::AbstractLayerPtr& inputLayer,
                         const std::vector<te::dt::Property*>& groupingProperties,
                         const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                         const std::string& outputLayerName,
                         const te::da::DataSourceInfoPtr& dsInfo)
{
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(inputLayer.get());
  
  te::da::DataSetType* outputDataSetType = GetDataSetType(outputLayerName, groupingProperties, statisticalSummary);
  te::mem::DataSet* outputDataSet = new te::mem::DataSet(outputDataSetType);

  if(dsLayer != 0)
  {
    te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
    const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();

    if(dsCapabilities.supportsPreparedQueryAPI())
    {
      AggregationQuery(dsLayer, groupingProperties, statisticalSummary, outputDataSet);
    }
    else
    {
      AggregationMemory(inputLayer, groupingProperties, statisticalSummary, outputDataSet);
    }
  }
  else
  {
    AggregationMemory(inputLayer, groupingProperties, statisticalSummary, outputDataSet);
  }

  Persistence(outputDataSetType, outputDataSet, dsInfo);
 }

void te::vp::AggregationQuery(  const te::map::AbstractLayerPtr& inputLayer,
                                const std::vector<te::dt::Property*>& groupingProperties,
                                const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                te::mem::DataSet* outputDataSet)
{

  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator itStatSummary = statisticalSummary.begin();
  
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(inputLayer.get());
  
  te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

  te::da::DataSetType* dsType = (te::da::DataSetType*)dsLayer->getSchema();
  te::da::Fields* fields = new te::da::Fields;

  while(itStatSummary != statisticalSummary.end())
  {
    int propType = itStatSummary->first->getType();

    if(propType == te::dt::STRING_TYPE)
    {
      te::da::PropertyName* p_name = new te::da::PropertyName(itStatSummary->first->getName());

      te::da::Expression* e_min = new te::da::Min(p_name);
      te::da::Field* f_min = new te::da::Field(*e_min, p_name->getName() + "_MIN");

      te::da::Expression* e_max = new te::da::Max(p_name);
      te::da::Field* f_max = new te::da::Field(*e_max,  p_name->getName() + "_MAX");

      te::da::Expression* e_count = new te::da::Count(p_name);
      te::da::Field* f_count = new te::da::Field(*e_count,  p_name->getName() + "_COUNT");

      fields->push_back(f_min);
      fields->push_back(f_max);
      fields->push_back(f_count);
    }
    else
    {
      te::da::PropertyName* p_name = new te::da::PropertyName(itStatSummary->first->getName());

      te::da::Expression* e_min = new te::da::Min(p_name);
      te::da::Field* f_min = new te::da::Field(*e_min, p_name->getName() + "_MIN");

      te::da::Expression* e_max = new te::da::Max(p_name);
      te::da::Field* f_max = new te::da::Field(*e_max, p_name->getName() + "_MAX");

      te::da::Expression* e_count = new te::da::Count(p_name);
      te::da::Field* f_count = new te::da::Field(*e_count, p_name->getName() + "_COUNT");

      te::da::Expression* e_sum = new te::da::Sum(p_name);
      te::da::Field* f_sum = new te::da::Field(*e_sum, p_name->getName() + "_SUM");

      te::da::Expression* e_mean = new te::da::Avg(p_name);
      te::da::Field* f_mean = new te::da::Field(*e_mean, p_name->getName() + "_MEAN");

      te::da::Expression* e_stddev = new te::da::StdDev(p_name);
      te::da::Field* f_stddev = new te::da::Field(*e_stddev, p_name->getName() + "_STD_DEV");

      te::da::Expression* e_variance = new te::da::Variance(p_name);
      te::da::Field* f_variance = new te::da::Field(*e_variance, p_name->getName() + "_VARIANCE");

      te::da::Expression* e_amplitude = new te::da::Sub(*e_max, *e_min);
      te::da::Field* f_amplitude = new te::da::Field(*e_amplitude, p_name->getName() + "_AMPLITUDE");

      fields->push_back(f_min);
      fields->push_back(f_max);
      fields->push_back(f_count);
      fields->push_back(f_sum);
      fields->push_back(f_mean);
      fields->push_back(f_stddev);
      fields->push_back(f_variance);
      fields->push_back(f_amplitude);

    }
    ++itStatSummary;
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

  std::auto_ptr<te::da::DataSourceTransactor> dsTransactor(dataSource->getTransactor());
  te::da::DataSet* dsQuery = dsTransactor->query(select);
  dsQuery->moveFirst();

  while(dsQuery->moveNext())
  {
    std::string min = dsQuery->getAsString(0);
    std::cout << "Min: " << min <<std::endl;
  }

  delete dsLayer;
  delete dsType;
  delete dsQuery;
}

void te::vp::AggregationMemory( const te::map::AbstractLayerPtr& inputLayer,
                                const std::vector<te::dt::Property*>& groupingProperties,
                                const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                te::mem::DataSet* outputDataSet)
{
  std::auto_ptr<te::mem::DataSet> inputDataSet((te::mem::DataSet*)inputLayer->getData());
  std::auto_ptr<te::mem::DataSetItem> dataSetItem(new te::mem::DataSetItem(inputDataSet.get()));

  std::map<std::string, std::vector<te::mem::DataSetItem*> > groupValues = GetGroups(inputDataSet.get(), groupingProperties);
  std::map<std::string, std::vector<te::mem::DataSetItem*> >::const_iterator itGroupValues = groupValues.begin();

  while(itGroupValues != groupValues.end())
  {
    std::string value = itGroupValues->first.c_str();
    int aggregationCount = itGroupValues->second.size();

    std::map<std::string, std::string> functionResultStringMap = CalculateStringGroupingFunctions(inputLayer, statisticalSummary, itGroupValues->second);
    std::map<std::string, double> functionResultDoubleMap = CalculateDoubleGroupingFunctions(inputLayer, statisticalSummary, itGroupValues->second);

    te::gm::Geometry* geometry = GetUnionGeometry(itGroupValues->second);

    te::mem::DataSetItem* outputDataSetItem = new te::mem::DataSetItem(outputDataSet);

    outputDataSetItem->setString(0, value);
    outputDataSetItem->setInt32(1, aggregationCount);

    if(!functionResultStringMap.empty())
    {
      std::map<std::string, std::string>::iterator itFuncResultString = functionResultStringMap.begin();
      
      while(itFuncResultString != functionResultStringMap.end())
      {
        if(PropertyExists(itFuncResultString->first.c_str(), outputDataSet))
          outputDataSetItem->setString(itFuncResultString->first.c_str(), itFuncResultString->second.c_str());

        ++itFuncResultString;
      }
    }

    if(!functionResultDoubleMap.empty())
    {
      std::map<std::string, double>::iterator itFuncResultDouble = functionResultDoubleMap.begin();
      
      while(itFuncResultDouble != functionResultDoubleMap.end())
      {
        if(PropertyExists(itFuncResultDouble->first.c_str(), outputDataSet))
          outputDataSetItem->setDouble(itFuncResultDouble->first.c_str(), itFuncResultDouble->second);

        ++itFuncResultDouble;
      }
    }

    outputDataSetItem->setGeometry("geom", *geometry);

    outputDataSet->add(outputDataSetItem);

    ++itGroupValues;
  }
}

te::da::DataSetType* te::vp::GetDataSetType(const std::string& outputLayerName, 
                                            const std::vector<te::dt::Property*>& properties, 
                                            const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary)
{
  te::da::DataSetType* dataSetType = new te::da::DataSetType(outputLayerName);
  std::string propertyResult;
  std::string functionResult;
  std::vector<te::stat::StatisticalSummary> vectorResult;

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    propertyResult += "_" + properties[i]->getName();
  }

  propertyResult.erase(propertyResult.begin());
  te::dt::StringProperty* stringProperty = new te::dt::StringProperty(propertyResult);
  dataSetType->add(stringProperty);

  te::dt::SimpleProperty* aggregationProperty = new te::dt::SimpleProperty("Aggregation_Count", te::dt::INT32_TYPE);
  dataSetType->add(aggregationProperty);

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
      
      if(it->first->getType() == te::dt::STRING_TYPE)
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
  propertyResult.erase(propertyResult.end());

  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  geometry->setGeometryType(te::gm::GeometryType);
  dataSetType->add(geometry);

  return dataSetType;
}


std::map<std::string, std::vector<te::mem::DataSetItem*> > te::vp::GetGroups( te::mem::DataSet* inputDataSet,
                                                                              const std::vector<te::dt::Property*>& groupingProperties)
{
  std::map<std::string, std::vector<te::mem::DataSetItem*> > groupValues;

  while(inputDataSet->moveNext())
  {
    te::mem::DataSetItem* dataSetItem = inputDataSet->getItem();

    std::size_t propertyType = 0;
    std::size_t propertyIndex = 0;

    bool found = false;
    std::vector<te::mem::DataSetItem*> dataSetItemVector;
    std::map<std::string, std::vector<te::mem::DataSetItem*> >::iterator it;

    std::string propertyName;
    std::string value;

    for(std::size_t i = 0; i < groupingProperties.size(); ++i)
    {
      propertyName += "_" + groupingProperties[i]->getName();

      propertyIndex = GetPropertyIndex(dataSetItem, groupingProperties[i]->getName());
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

std::size_t te::vp::GetPropertyIndex(const te::mem::DataSetItem* item, const std::string propertyName)
{
  std::size_t index = 0;

  for(std::size_t i = 0; i < item->getNumProperties(); ++i)
  {
    if(propertyName == item->getPropertyName(i))
    {
      index = i;
      return index;
    }
  }
  return -1;
}

te::gm::Geometry* te::vp::GetUnionGeometry(const std::vector<te::mem::DataSetItem*>& items)
{
  te::gm::Geometry* resultGeometry = 0; 
  te::gm::Geometry* seedGeometry;
  std::vector<te::gm::Geometry*> geomVector;

  for(std::size_t i = 0; i < items.size(); ++i)
  {
    std::size_t count = items[i]->getNumProperties();

    for(std::size_t j = 0; j < count; ++j)
    {
      std::size_t type = items[i]->getPropertyDataType(j);
      
      if(type == te::dt::GEOMETRY_TYPE)
      {
        geomVector.push_back(items[i]->getGeometry(j));
      }
    }
  }

  int size = geomVector.size();

  if(size > 1)
  {
    seedGeometry = geomVector[0];

    for(std::size_t i = 1; i < geomVector.size(); ++i)
    {
      if(geomVector[i]->isValid())
      {
        resultGeometry = seedGeometry->Union(geomVector[i]);
        seedGeometry = resultGeometry;
      }
      /*else
      {
        //Gerar log do processamento e ao final avisar o usuário.
      }*/
    }
  }
  else
  {
    resultGeometry = geomVector[0];
  }

  return resultGeometry;
}

std::map<std::string, std::string> te::vp::CalculateStringGroupingFunctions(const te::map::AbstractLayerPtr& inputLayer,
                                                                            const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary, 
                                                                            const std::vector<te::mem::DataSetItem*>& items)
{
  std::map<std::string, std::string> result;

  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator it = statisticalSummary.begin();

  while(it != statisticalSummary.end())
  {
    if(it->first->getType() == te::dt::STRING_TYPE)
    {
      std::string propertyName = it->first->getName();
      std::vector<std::string> values;

      for(std::size_t i = 0; i < items.size(); ++i)
      {
        std::size_t index = GetPropertyIndex(items[i], propertyName);

        if(index != -1)
          values.push_back(items[i]->getString(index));
      }

      te::stat::StringStatisticalSummary ss; 
      te::stat::GetStringStatisticalSummary(values, ss);

      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_MIN_VALUE", ss.m_minVal ) );
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_MAX_VALUE", ss.m_maxVal ) );
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_COUNT", boost::lexical_cast<std::string>(ss.m_count) ) );
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_VALID_COUNT", boost::lexical_cast<std::string>(ss.m_validCount) ) );
    }
    ++it;
  }

  return result;
}

std::map<std::string, double> te::vp::CalculateDoubleGroupingFunctions( const te::map::AbstractLayerPtr& inputLayer,
                                                                        const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                                                        const std::vector<te::mem::DataSetItem*>& items)
{
  std::map<std::string, double> result;

  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator it = statisticalSummary.begin();

  while(it != statisticalSummary.end())
  {
    if(it->first->getType() != te::dt::STRING_TYPE)
    {
      std::string propertyName = it->first->getName();
      std::vector<double> values;

      for(std::size_t i = 0; i < items.size(); ++i)
      {
        std::size_t index = GetPropertyIndex(items[i], propertyName);

        if(index != -1)
          values.push_back(items[i]->getDouble(index));
      }

      te::stat::NumericStatisticalSummary ss;
      te::stat::GetNumericStatisticalSummary(values, ss);

      result.insert( std::map<std::string, double>::value_type( propertyName + "_MIN_VALUE", ss.m_minVal ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MAX_VALUE", ss.m_maxVal ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_COUNT", ss.m_count ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VALID_COUNT", ss.m_validCount ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MEAN", ss.m_mean ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_SUM", ss.m_sum ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_STANDARD_DEVIATION", ss.m_stdDeviation ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VARIANCE", ss.m_variance ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_SKEWNESS", ss.m_skewness ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_KURTOSIS", ss.m_kurtosis ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_AMPLITUDE", ss.m_amplitude ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MEDIAN", ss.m_median ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VAR_COEFF", ss.m_varCoeff ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MODE", ss.m_mode ) );

    }
    ++it;
  }

  return result;
}

bool te::vp::PropertyExists(const std::string& propertyName, const te::mem::DataSet* dataSet)
{
  std::string currentPropName;

  for(size_t i = 0; i < dataSet->getNumProperties(); ++i)
  {
    currentPropName = dataSet->getPropertyName(i);

    if(currentPropName == propertyName)
      return true;
  }
  return false;
}

void te::vp::Persistence( te::da::DataSetType* dataSetType,
                          te::mem::DataSet* dataSet,
                          const te::da::DataSourceInfoPtr& dsInfo,
                          const std::map<std::string, std::string> options)
{
  std::pair<te::da::DataSetType*, te::mem::DataSet*> pair;
  pair.first = dataSetType;
  pair.second = dataSet;

  te::da::DataSourcePtr dataSource = te::da::DataSourceManager::getInstance().get(dsInfo->getId(), dsInfo->getType(), dsInfo->getConnInfo());
  std::auto_ptr<te::da::DataSourceTransactor> t(dataSource->getTransactor());
  pair.second->moveFirst();
  te::da::Create(t.get(), pair.first, pair.second, options);
}
