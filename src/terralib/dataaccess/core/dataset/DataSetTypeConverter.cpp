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
  \file terralib/dataaccess/core/dataset/DataSetTypeConverter.cpp

  \brief An converter for a dataset schema.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../datasource/DataSourceCapabilities.h"
#include "../Exception.h"
#include "DataSetType.h"
#include "DataSetTypeConverter.h"

// STL
#include <cassert>

te::da::core::DataSetTypeConverter::DataSetTypeConverter(DataSetType* type)
  : m_inDataSetType(type)
{
  assert(type);

  // By default no property is automatically converted
  for(std::size_t i = 0; i != type->size(); ++i)
    m_convertedProperties.push_back(0);

  // Creates the empty converted DataSetType
  m_outDataSetType = new DataSetType(type->getName(), type->getId());
}

te::da::core::DataSetTypeConverter::DataSetTypeConverter(DataSetType* type, const DataSourceCapabilities& capabilities)
  : m_inDataSetType(type)
{
  assert(type);

  // By default no property is automatically converted
  for(std::size_t i = 0; i !=  type->size(); ++i)
    m_convertedProperties.push_back(0);

  // Creates the empty converted DataSetType
  m_outDataSetType = new DataSetType(type->getName(), type->getId());

  // Gets the DataTypeCapabilities
  const DataTypeCapabilities& dtCapabilities = capabilities.getDataTypeCapabilities();

  // Try automatic build the conversion
  const std::vector<te::dt::Property*> properties = type->getProperties();
  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    te::dt::Property* p = properties[i];
    assert(p);

    if(dtCapabilities.supports(p->getType()))
    {
      add(i, p->clone());
    }
    else
    {
      // Try create the property from data source capabilities hint
      int hintDataType = dtCapabilities.getHint(p->getType());

      if(hintDataType != te::dt::UNKNOWN_TYPE)
      {
        //add(propertyName, hintDataType, i); TODO: Utility method that returns a te::dt::Property* given a name and a data type.
      }
    }
  }
}

te::da::core::DataSetTypeConverter::~DataSetTypeConverter()
{
  delete m_outDataSetType;
}

te::da::core::DataSetType* te::da::core::DataSetTypeConverter::getConvertee()
{
  return m_inDataSetType;
}

void te::da::core::DataSetTypeConverter::getNonConvertedProperties(std::vector<std::string>& propertyNames) const
{
  for(std::size_t i = 0; i < m_inDataSetType->size(); ++i)
  {
    if(!isConverted(i))
      propertyNames.push_back(m_inDataSetType->getProperty(i)->getName());
  }
}

void te::da::core::DataSetTypeConverter::getNonConvertedProperties(std::vector<std::size_t>& propertyPos) const
{
  for(std::size_t i = 0; i < m_inDataSetType->size(); ++i)
  {
    if(!isConverted(i))
      propertyPos.push_back(i);
  }
}

void te::da::core::DataSetTypeConverter::getConvertedProperties(const std::string& propertyName, std::vector<std::size_t>& convertedPropertyPos)
{
  std::size_t pos = m_outDataSetType->getPropertyPosition(propertyName);
  assert(pos != std::string::npos);

  return getConvertedProperties(pos, convertedPropertyPos);
}

void te::da::core::DataSetTypeConverter::getConvertedProperties(std::size_t propertyPos, std::vector<std::size_t>& convertedPropertyPos)
{
  assert(propertyPos >= 0 && propertyPos < m_propertyIndexes.size());

  std::vector<std::size_t> indexes = m_propertyIndexes[propertyPos];

  for(std::size_t i = 0; i < indexes.size(); ++i)
  {
    assert(indexes[i] >= 0 && indexes[i] < m_inDataSetType->size());
    convertedPropertyPos.push_back(indexes[i]);
  }
}

void te::da::core::DataSetTypeConverter::remove(const std::string& propertyName)
{
  std::size_t pos = m_outDataSetType->getPropertyPosition(propertyName);
  remove(pos);
}

void te::da::core::DataSetTypeConverter::remove(std::size_t propertyPos)
{
  assert(propertyPos >= 0 && propertyPos < m_outDataSetType->size());

  // Property that will be removed
  te::dt::Property* p = m_outDataSetType->getProperty(propertyPos);

  // Remove from Converted DataSetType...
  m_outDataSetType->remove(p);

  // Adjusting internal indexes...
  const std::vector<std::size_t> indexes = m_propertyIndexes[propertyPos];

  for(std::size_t i = 0; i < indexes.size(); ++i)
    m_convertedProperties[indexes[i]]--;

  m_propertyIndexes.erase(m_propertyIndexes.begin() + propertyPos);

  m_converters.erase(m_converters.begin() + propertyPos);
}

void te::da::core::DataSetTypeConverter::add(const std::string& propertyName, te::dt::Property* p, AttributeConverter conv)
{
  std::size_t pos = m_inDataSetType->getPropertyPosition(propertyName);
  add(pos, p);
}

void te::da::core::DataSetTypeConverter::add(std::size_t propertyPos, te::dt::Property* p, AttributeConverter conv)
{
  std::vector<std::size_t> indexes;
  indexes.push_back(propertyPos);
  add(indexes, p, conv);
}

void te::da::core::DataSetTypeConverter::add(const std::vector<std::string>& propertyNames, te::dt::Property* p, AttributeConverter conv)
{
  std::vector<std::size_t> indexes;

  for(std::size_t i = 0; i < propertyNames.size(); ++i)
    indexes.push_back(m_inDataSetType->getPropertyPosition(propertyNames[i]));

  add(indexes, p, conv);
}

void te::da::core::DataSetTypeConverter::add(const std::vector<std::size_t>& propertyPos, te::dt::Property* p, AttributeConverter conv)
{
  assert(!propertyPos.empty());
  assert(p);

  // Adding given property on out data set type
  m_outDataSetType->add(p);

  // Storing the converted properties indexes
  m_propertyIndexes.push_back(propertyPos);

  // Indexing the AttributeConverter functions
  m_converters.push_back(conv);

  // Counting reference to converted properties
  for(std::size_t i = 0; i < propertyPos.size(); ++i)
    m_convertedProperties[propertyPos[i]]++;
}

bool te::da::core::DataSetTypeConverter::needConverter(DataSetType* type, const DataSourceCapabilities& capabilities)
{
  assert(type);

  // Gets the DataTypeCapabilities
  const DataTypeCapabilities& dtCapabilities = capabilities.getDataTypeCapabilities();

  const std::vector<te::dt::Property*> properties = type->getProperties();
  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    te::dt::Property* p = properties[i];
    assert(p);

    if(!dtCapabilities.supports(p->getType()))
      return true;
  }

  return false;
}

te::da::core::DataSetType* te::da::core::DataSetTypeConverter::getResult() const
{
  return m_outDataSetType;
}

const std::vector<std::vector<std::size_t> >& te::da::core::DataSetTypeConverter::getConvertedPropertyIndexes() const
{
  return m_propertyIndexes;
}

const std::vector<te::da::core::AttributeConverter>& te::da::core::DataSetTypeConverter::getConverters() const
{
  return m_converters;
}

bool te::da::core::DataSetTypeConverter::isConverted(std::size_t i) const
{
  assert(i < m_convertedProperties.size());
  return m_convertedProperties[i] > 0;
}
