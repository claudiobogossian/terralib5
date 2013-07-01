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
  \file SummaryFunctions.cpp

  \brief Statistical summary functions.
*/

//Terralib
#include "../../dataaccess/query_h.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../maptools/DataSetLayer.h"
#include "Config.h"
#include "Exception.h"
#include "SummaryFunctions.h"
#include "Utils.h"

// BOOST
#include <boost/lexical_cast.hpp>

//STL
#include <algorithm>
#include <map>
#include <numeric>
#include <vector>

void te::stat::GetStringStatisticalSummary(std::vector<std::string>& values, te::stat::StringStatisticalSummary& ss)
{
  std::sort(values.begin(), values.end());
  
  ss.m_minVal = *values.begin();
  ss.m_maxVal = values[values.size() - 1];

  ss.m_count = values.size();

  for(std::size_t i = 0; i < values.size(); ++i)
  {
    if(!values[i].empty())
    {
      ++ss.m_validCount;
    }
  }

  ss.m_mode = Mode(values);
}

void te::stat::GetNumericStatisticalSummary(std::vector<double>& values, te::stat::NumericStatisticalSummary& ss)
{
  std::sort(values.begin(), values.end());

  ss.m_minVal = *values.begin();
  ss.m_maxVal = values[values.size() - 1];
  ss.m_count = values.size();
  ss.m_validCount = values.size();
  
  int init = 0;
  ss.m_sum = std::accumulate(values.begin(), values.end(), init);

  ss.m_mean = ss.m_sum/ss.m_count;

  for(int i = 0; i < ss.m_count; ++i)
  {
    double v= values[i];
    ss.m_variance += pow((v-ss.m_mean),2);
    ss.m_skewness += pow((v-ss.m_mean),3);
    ss.m_kurtosis += pow((v-ss.m_mean),4);
  }

  ss.m_variance /= ss.m_count;
  ss.m_stdDeviation = pow(ss.m_variance, 0.5);
  ss.m_skewness /= ss.m_count;
  ss.m_skewness /= pow(ss.m_stdDeviation, 3);
  ss.m_kurtosis /= ss.m_count;
  ss.m_kurtosis /= pow(ss.m_stdDeviation, 4);

  ss.m_varCoeff = (100* ss.m_stdDeviation) / ss.m_mean;
  ss.m_amplitude = ss.m_maxVal - ss.m_minVal;

  if((ss.m_count % 2) == 0)
    ss.m_median = (values[(ss.m_count/2)] + values[(ss.m_count/2-1)])/2;
  else
    ss.m_median = values[(ss.m_count-1)/2];

  ss.m_mode = Mode(values);
}

double te::stat::Mode(const std::vector<double>& values)
{
  bool found;
  double mode = 0.0;
  std::map<double, int> mapMode;

  for(std::size_t i = 0; i < values.size(); ++i)
  {
    found = false;

    if(!mapMode.empty())
    {
      std::map<double, int>::iterator itMode = mapMode.begin();

      while(itMode != mapMode.end())
      {
        if(itMode->first == values[i])
        {
          ++itMode->second;
          found = true;
        }
        
        ++itMode;
      }
      if(found == false)
      {
        mapMode.insert( std::map<double, int>::value_type( values[i] , 1 ) );
      }
    }
    else
      mapMode.insert( std::map<double, int>::value_type( values[i] , 1 ) );
  }

  std::map<double, int>::iterator itMode = mapMode.begin();
  int repeat = 0;

  while(itMode != mapMode.end())
  {
    if(repeat < itMode->second)
    {
      repeat = itMode->second;
      mode = itMode->first;
    }

    ++itMode;
  }

  return mode;
}

std::string te::stat::Mode(const std::vector<std::string>& values)
{
  bool found;
  std::string mode = "";
  std::map<std::string, int> mapMode;

  for(std::size_t i = 0; i < values.size(); ++i)
  {
    found = false;

    if(!mapMode.empty())
    {
      std::map<std::string, int>::iterator itMode = mapMode.begin();

      while(itMode != mapMode.end())
      {
        if(itMode->first == values[i])
        {
          ++itMode->second;
          found = true;
        }
        
        ++itMode;
      }
      if(found == false)
      {
        mapMode.insert( std::map<std::string, int>::value_type( values[i] , 1 ) );
      }
    }
    else
      mapMode.insert( std::map<std::string, int>::value_type( values[i] , 1 ) );
  }

  std::map<std::string, int>::iterator itMode = mapMode.begin();
  int repeat = 0;

  while(itMode != mapMode.end())
  {
    if(repeat < itMode->second)
    {
      repeat = itMode->second;
      mode = itMode->first;
    }

    ++itMode;
  }

  return mode;
}

void te::stat::GetStringStatisticalSummaryQuery(const te::map::AbstractLayerPtr& layer,
                                                                    const te::dt::Property* prop,
                                                                    std::map<std::string, te::stat::StringStatisticalSummary>& ssMap,
                                                                    const std::vector<te::dt::Property*>& restriction)
{
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());
  
  if(dsLayer == 0)
  {
    std::auto_ptr<te::da::DataSet> dsLayer((te::da::DataSet*)layer->getData());
    std::string propName = prop->getName();

    std::vector<std::string> stringVector = te::stat::GetStringData(dsLayer.get(), propName);
    std::map<std::string, te::stat::StringStatisticalSummary>::iterator itSsMap = ssMap.begin();

    te::stat::GetStringStatisticalSummary(stringVector, itSsMap->second);
  }
  else
  {
    te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

    const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();

    if(!dsCapabilities.supportsPreparedQueryAPI())
    {
      std::auto_ptr<te::da::DataSet> ds((te::da::DataSet*)layer->getData());
      std::string propName = prop->getName();

      std::vector<std::string> stringVector = te::stat::GetStringData(ds.get(), propName);
      std::map<std::string, te::stat::StringStatisticalSummary>::iterator itSsMap = ssMap.begin();

      te::stat::GetStringStatisticalSummary(stringVector, itSsMap->second);
    }
    else
    {


    }
  }
}

void te::stat::GetNumericStatisticalSummaryQuery( const te::map::AbstractLayerPtr& layer,
                                                  const te::dt::Property* prop,
                                                  std::map<std::string, te::stat::NumericStatisticalSummary>& ssMap,
                                                  const std::vector<te::dt::Property*>& restriction)
{
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());
  
  if(dsLayer == 0)
  {
    std::auto_ptr<te::da::DataSet> ds((te::da::DataSet*)layer->getData());

    std::string propName = prop->getName();

    std::vector<double> numericVector = te::stat::GetNumericData(ds.get(), propName);
    std::map<std::string, te::stat::NumericStatisticalSummary>::iterator itSsMap = ssMap.begin();

    te::stat::GetNumericStatisticalSummary(numericVector, itSsMap->second);
  }
  else
  {
    te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

    const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();

    if(!dsCapabilities.supportsPreparedQueryAPI())
    {
      std::auto_ptr<te::da::DataSet> ds((te::da::DataSet*)layer->getData());

      std::string propName = prop->getName();

      std::vector<double> numericVector = te::stat::GetNumericData(ds.get(), propName);
      std::map<std::string, te::stat::NumericStatisticalSummary>::iterator itSsMap = ssMap.begin();

      te::stat::GetNumericStatisticalSummary(numericVector, itSsMap->second);
    }
    else
    {
      te::da::DataSetType* dsType = (te::da::DataSetType*)dsLayer->getSchema();

      te::da::Expression* e_sum = new te::da::Sum(new te::da::PropertyName(prop->getName()));
      te::da::Field* f_sum = new te::da::Field(*e_sum, "SUM");

      te::da::Fields fields;
      fields.push_back(f_sum);

      te::da::FromItem* fromItem = new te::da::DataSetName(dsType->getName());
      te::da::From from;
      from.push_back(fromItem);
      
      te::da::Select select(fields, from);

      if(!restriction.empty())
      {
        te::da::GroupBy* groupBy = new te::da::GroupBy();

        for(std::size_t i = 0; i < restriction.size(); ++i)
        {
          te::da::GroupByItem* e_groupBy = new te::da::GroupByItem(restriction[i]->getName());
          groupBy->push_back(e_groupBy);
        }

        select.setGroupBy(groupBy);
      }

      std::auto_ptr<te::da::DataSourceTransactor> dsTransactor(dataSource->getTransactor());
      te::da::DataSet* dsQuery = dsTransactor->query(select);
      dsQuery->moveFirst();
      delete dsQuery;
//      ss.m_sum = boost::lexical_cast<double>(dsQuery->getAsString(0));
    }
  }
}