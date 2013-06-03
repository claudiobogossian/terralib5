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
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetPersistence.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/DataSetTypePersistence.h"
#include "../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleProperty.h"
#include "../../datatype/StringProperty.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/AbstractLayer.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "Aggregation.h"
#include "AggregationDialog.h"
#include "Config.h"
#include "Enums.h"
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
                         const std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >& groupingFunctionsType,
                         const te::vp::MemoryUse memoryUse,
                         const std::string& outputLayerName,
                         const te::da::DataSourceInfoPtr& dsInfo)
{
  std::auto_ptr<te::mem::DataSet> inputDataSet((te::mem::DataSet*)inputLayer->getData());
  std::auto_ptr<te::mem::DataSetItem> dataSetItem(new te::mem::DataSetItem(inputDataSet.get()));

  std::map<std::string, std::vector<te::mem::DataSetItem*> > groupValues = GetGroups(inputDataSet.get(), groupingProperties);
  std::map<std::string, std::vector<te::mem::DataSetItem*> >::const_iterator itGroupValues = groupValues.begin();

  te::da::DataSetType* outputDataSetType = GetDataSetType(outputLayerName, groupingProperties, groupingFunctionsType);
  te::mem::DataSet* outputDataSet = new te::mem::DataSet(outputDataSetType);


  while(itGroupValues != groupValues.end())
  {
    std::string value = itGroupValues->first.c_str();
    int aggregationCount = itGroupValues->second.size();

    std::map<std::string, std::string> functionResultStringMap = CalculateStringGroupingFunctions(groupingFunctionsType, itGroupValues->second);
    std::map<std::string, double> functionResultDoubleMap = CalculateDoubleGroupingFunctions(groupingFunctionsType, itGroupValues->second);

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

  Persistence(outputDataSetType, outputDataSet, dsInfo); 

 }

te::da::DataSetType* te::vp::GetDataSetType(const std::string& outputLayerName, 
                                            const std::vector<te::dt::Property*>& properties, 
                                            const std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >& groupingFunctionsType)
{
  te::da::DataSetType* dataSetType = new te::da::DataSetType(outputLayerName);
  std::string propertyResult;
  std::string functionResult;
  std::vector<te::vp::GroupingFunctionsType> vectorResult;

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    propertyResult += "_" + properties[i]->getName();
  }

  propertyResult.erase(propertyResult.begin());
  te::dt::StringProperty* stringProperty = new te::dt::StringProperty(propertyResult);
  dataSetType->add(stringProperty);

  te::dt::SimpleProperty* aggregationProperty = new te::dt::SimpleProperty("Aggregation_Count", te::dt::INT32_TYPE);
  dataSetType->add(aggregationProperty);

  std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >::const_iterator it = groupingFunctionsType.begin();

  while(it != groupingFunctionsType.end())
  {
    propertyResult = "";
    propertyResult = it->first->getName();
    propertyResult += "_";
    
    vectorResult = it->second;

    for(std::size_t i = 0; i < vectorResult.size(); ++i)
    {
      functionResult = propertyResult;
      functionResult += GetGroupingFunctionsTypeMap(vectorResult[i]);
      
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
  dataSetType->setDefaultGeomProperty(geometry);

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

std::string te::vp::GetGroupingFunctionsTypeMap(const int& type)
{
  switch(type)
  {
    case MIN_VALUE:
      return TR_VP("MIN_VALUE");
      
    case MAX_VALUE:
      return TR_VP("MAX_VALUE");
    
    case MEAN:
      return TR_VP("MEAN");
    
    case SUM:
      return TR_VP("SUM");
    
    case COUNT:
      return TR_VP("COUNT");
    
    case VALID_COUNT:
      return TR_VP("VALID_COUNT");
    
    case STANDARD_DEVIATION:
      return TR_VP("STANDARD_DEVIATION");
    
    case KERNEL:
      return TR_VP("KERNEL");
    
    case VARIANCE:
      return TR_VP("VARIANCE");
    
    case SKEWNESS:
      return TR_VP("SKEWNESS");
    
    case KURTOSIS:
      return TR_VP("KURTOSIS");
    
    case AMPLITUDE:
      return TR_VP("AMPLITUDE");
    
    case MEDIAN:
      return TR_VP("MEDIAN");
    
    case VAR_COEFF:
      return TR_VP("VAR_COEFF");
    
    case MODE:
      return TR_VP("MODE");

    default:
      return ("");
  }
}

te::gm::Geometry* te::vp::GetUnionGeometry(const std::vector<te::mem::DataSetItem*>& items)
{
  te::gm::Geometry* resultGeometry; 
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
        //Gerar log do processamento e ao final avisar o usu�rio.
      }*/
    }
  }
  else
  {
    resultGeometry = geomVector[0];
  }

  return resultGeometry;
}

std::map<std::string, std::string> te::vp::CalculateStringGroupingFunctions(const std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >& groupingFunctionsType, 
                                                                            const std::vector<te::mem::DataSetItem*>& items)
{
  std::map<std::string, std::string> result;

  std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >::const_iterator it = groupingFunctionsType.begin();

  while(it != groupingFunctionsType.end())
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

      std::sort(values.begin(), values.end());

      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_MIN_VALUE", *values.begin() ) );
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_MAX_VALUE", values[values.size() - 1] ) );
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_COUNT", boost::lexical_cast<std::string>(values.size())));
      result.insert( std::map<std::string, std::string>::value_type( propertyName + "_VALID_COUNT", boost::lexical_cast<std::string>(values.size())));
    }
    ++it;
  }

  return result;
}

std::map<std::string, double> te::vp::CalculateDoubleGroupingFunctions( const std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >& groupingFunctionsType,
                                                                        const std::vector<te::mem::DataSetItem*>& items)
{
  std::map<std::string, double> result;

  int validCount,count;
  validCount = count = 0;

  double maxValue, minValue, mean, sum, standardDeviation, kernel, variance, skewness, kurtosis, amplitude, median, varCoeff, mode;
  maxValue, minValue, mean = sum = standardDeviation = kernel = variance = skewness = kurtosis = amplitude = median = varCoeff = mode = 0.0;

  std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >::const_iterator it = groupingFunctionsType.begin();

  while(it != groupingFunctionsType.end())
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

      std::sort(values.begin(), values.end());

      //Operations

      minValue = *values.begin();

      maxValue = values[values.size() - 1];

      count = values.size();

      sum = Sum(values);

      mean = sum/count;

      for(int i=0; i<count; i++)
	    {
		    double v= values[i];
		    variance += pow((v-mean),2); 
		    skewness += pow((v-mean),3);   
		    kurtosis += pow((v-mean),4); 
	    }

      //if(!count)
		    //return false;

	    variance /= count; 
	    standardDeviation = pow(variance,0.5); 
	    skewness /= count;
	    skewness /= pow(standardDeviation,3); 
	    kurtosis /= count;
	    kurtosis /= pow(standardDeviation,4); 

	    varCoeff = (100*standardDeviation)/mean;
	    amplitude = maxValue-minValue; 

      if((count%2)==0)
		    median = (values[(count/2)]+values[(count/2-1)])/2;
	    else
		    median = values[(count-1)/2];


      mode = Mode(values);

      result.insert( std::map<std::string, double>::value_type( propertyName + "_MIN_VALUE", minValue ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MAX_VALUE", maxValue ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_COUNT", count ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VALID_COUNT", validCount ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MEAN", mean ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_SUM", sum ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_STANDARD_DEVIATION", standardDeviation ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_KERNEL", kernel ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VARIANCE", variance ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_SKEWNESS", skewness ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_KURTOSIS", kurtosis ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_AMPLITUDE", amplitude ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MEDIAN", median ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_VAR_COEFF", varCoeff ) );
      result.insert( std::map<std::string, double>::value_type( propertyName + "_MODE", mode ) );

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

double te::vp::Sum(const std::vector<double>& values)
{
  double sum = 0.0;

  for(std::size_t i = 0; i < values.size(); ++i)
  {
    sum+= values[i];
  }

  return sum;

}

double te::vp::Mode(const std::vector<double>& values)
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
  int aux = 0;
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

