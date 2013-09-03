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
                                                te::stat::StringStatisticalSummary& ss)
{
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());
  
  if(dsLayer == 0)
  {
    std::auto_ptr<te::da::DataSet> dsLayer((te::da::DataSet*)layer->getData().get());
    std::string propName = prop->getName();

    std::vector<std::string> stringVector = te::stat::GetStringData(dsLayer.get(), propName);

    te::stat::GetStringStatisticalSummary(stringVector, ss);
  }
  else
  {
    te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

    const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();

    if(!dsCapabilities.supportsPreparedQueryAPI())
    {
      std::auto_ptr<te::da::DataSet> ds((te::da::DataSet*)layer->getData().get());
      std::string propName = prop->getName();

      std::vector<std::string> stringVector = te::stat::GetStringData(ds.get(), propName);

      te::stat::GetStringStatisticalSummary(stringVector, ss);
    }
    else
    {


    }
  }
}

void te::stat::GetNumericStatisticalSummaryQuery( const te::map::AbstractLayerPtr& layer,
                                                  const te::dt::Property* prop,
                                                  te::stat::NumericStatisticalSummary& ss)
{
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());
  
  if(dsLayer == 0)
  {
    std::auto_ptr<te::da::DataSet> ds((te::da::DataSet*)layer->getData().get());

    std::string propName = prop->getName();

    std::vector<double> numericVector = te::stat::GetNumericData(ds.get(), propName);

    te::stat::GetNumericStatisticalSummary(numericVector, ss);
  }
  else
  {
    te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

    const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();

    if(!dsCapabilities.supportsPreparedQueryAPI())
    {
      std::auto_ptr<te::da::DataSet> ds((te::da::DataSet*)layer->getData().get());

      std::string propName = prop->getName();

      std::vector<double> numericVector = te::stat::GetNumericData(ds.get(), propName);

      te::stat::GetNumericStatisticalSummary(numericVector, ss);
    }
    else
    {
      te::da::DataSetType* dsType = (te::da::DataSetType*)dsLayer->getSchema().get();
      te::da::PropertyName* p_name = new te::da::PropertyName(prop->getName());

      te::da::Expression* e_min = new te::da::Min(p_name);
      te::da::Field* f_min = new te::da::Field(*e_min, "MIN");

      te::da::Expression* e_max = new te::da::Max(p_name);
      te::da::Field* f_max = new te::da::Field(*e_max, "MAX");

      te::da::Expression* e_count = new te::da::Count(p_name);
      te::da::Field* f_count = new te::da::Field(*e_count, "COUNT");

      te::da::Expression* e_sum = new te::da::Sum(p_name);
      te::da::Field* f_sum = new te::da::Field(*e_sum, "SUM");

      te::da::Expression* e_mean = new te::da::Avg(p_name);
      te::da::Field* f_mean = new te::da::Field(*e_mean, "MEAN");

      te::da::Expression* e_stddev = new te::da::StdDev(p_name);
      te::da::Field* f_stddev = new te::da::Field(*e_stddev, "STD_DEV");

      te::da::Expression* e_variance = new te::da::Variance(p_name);
      te::da::Field* f_variance = new te::da::Field(*e_variance, "VARIANCE");

      te::da::Expression* e_amplitude = new te::da::Sub(*e_max, *e_min);
      te::da::Field* f_amplitude = new te::da::Field(*e_amplitude, "AMPLITUDE");

      te::da::Fields* fields = new te::da::Fields;

      fields->push_back(f_min);
      fields->push_back(f_max);
      fields->push_back(f_count);
      fields->push_back(f_sum);
      fields->push_back(f_mean);
      fields->push_back(f_stddev);
      fields->push_back(f_variance);
      fields->push_back(f_amplitude);

      te::da::FromItem* fromItem = new te::da::DataSetName(dsType->getName());
      te::da::From* from = new te::da::From;
      from->push_back(fromItem);
      
      te::da::Select select(fields, from);

      std::auto_ptr<te::da::DataSourceTransactor> dsTransactor(dataSource->getTransactor());
      std::auto_ptr<te::da::DataSet> dsQuery = dsTransactor->query(select);
      dsQuery->moveFirst();

      ss.m_minVal = boost::lexical_cast<double>(dsQuery->getAsString(0));
      ss.m_maxVal = boost::lexical_cast<double>(dsQuery->getAsString(1));
      ss.m_count = boost::lexical_cast<int>(dsQuery->getAsString(2));
      ss.m_sum = boost::lexical_cast<double>(dsQuery->getAsString(3));
      ss.m_mean = boost::lexical_cast<double>(dsQuery->getAsString(4));
      ss.m_stdDeviation = boost::lexical_cast<double>(dsQuery->getAsString(5));
      ss.m_variance = boost::lexical_cast<double>(dsQuery->getAsString(6));
      ss.m_amplitude = boost::lexical_cast<double>(dsQuery->getAsString(7));
    }
  }
}