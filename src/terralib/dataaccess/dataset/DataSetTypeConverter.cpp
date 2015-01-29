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
  \file terralib/dataaccess/dataset/DataSetTypeConverter.cpp

  \brief An converter for DataSetType.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/PrimaryKey.h"
#include "../datasource/DataSourceCapabilities.h"
#include "../Exception.h"
#include "AttributeConverterManager.h"
#include "DataSetType.h"
#include "DataSetTypeConverter.h"

// STL
#include <cassert>

te::da::DataSetTypeConverter::DataSetTypeConverter(DataSetType* type)
  : m_inDataSetType(dynamic_cast<DataSetType*>(type->clone()))
{
  assert(type);

  // By default no property is automatically converted
  for(std::size_t i = 0; i != type->size(); ++i)
    m_convertedProperties.push_back(0);

  // Creates the empty converted DataSetType
  m_outDataSetType = new DataSetType(type->getName(), type->getId());
  m_outDataSetType->setTitle(type->getName());
}

te::da::DataSetTypeConverter::DataSetTypeConverter(DataSetType* type, const DataSourceCapabilities& capabilities, const te::common::CharEncoding& ce)
  : m_inDataSetType(dynamic_cast<DataSetType*>(type->clone()))
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
      if(p->getType() == te::dt::STRING_TYPE)
      {
        CharEncodingConverter cec(ce);
        add(i, p->clone(), cec);
      }
      else
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

  te::da::PrimaryKey* pk = type->getPrimaryKey();
  if(pk)
  {
    te::da::PrimaryKey* outPk = new te::da::PrimaryKey(pk->getName(), m_outDataSetType);

    std::vector<te::dt::Property*> pkProps = pk->getProperties();
    std::vector<te::dt::Property*> outPkProps;
    for(std::size_t i = 0; i < pkProps.size(); ++i)
      outPkProps.push_back(m_outDataSetType->getProperty(pkProps[i]->getName()));

    outPk->setProperties(outPkProps);
  }
}

te::da::DataSetTypeConverter::~DataSetTypeConverter()
{
  delete m_outDataSetType;
  delete m_inDataSetType;
}

te::da::DataSetType* te::da::DataSetTypeConverter::getConvertee()
{
  return m_inDataSetType;
}

void te::da::DataSetTypeConverter::getNonConvertedProperties(std::vector<std::string>& propertyNames) const
{
  for(std::size_t i = 0; i < m_inDataSetType->size(); ++i)
  {
    if(!isConverted(i))
      propertyNames.push_back(m_inDataSetType->getProperty(i)->getName());
  }
}

void te::da::DataSetTypeConverter::getNonConvertedProperties(std::vector<std::size_t>& propertyPos) const
{
  for(std::size_t i = 0; i < m_inDataSetType->size(); ++i)
  {
    if(!isConverted(i))
      propertyPos.push_back(i);
  }
}

void te::da::DataSetTypeConverter::getConvertedProperties(const std::string& propertyName, std::vector<std::size_t>& convertedPropertyPos)
{
  std::size_t pos = m_outDataSetType->getPropertyPosition(propertyName);
  assert(pos != std::string::npos);

  return getConvertedProperties(pos, convertedPropertyPos);
}

void te::da::DataSetTypeConverter::getConvertedProperties(std::size_t propertyPos, std::vector<std::size_t>& convertedPropertyPos)
{
  assert(propertyPos >= 0 && propertyPos < m_propertyIndexes.size());

  std::vector<std::size_t> indexes = m_propertyIndexes[propertyPos];

  for(std::size_t i = 0; i < indexes.size(); ++i)
  {
    assert(indexes[i] >= 0 && indexes[i] < m_inDataSetType->size());
    convertedPropertyPos.push_back(indexes[i]);
  }
}

std::string te::da::DataSetTypeConverter::getConverterName(std::size_t propertyPos)
{
  return m_functionsNames[propertyPos];
}

void te::da::DataSetTypeConverter::remove(const std::string& propertyName)
{
  std::size_t pos = m_outDataSetType->getPropertyPosition(propertyName);
  remove(pos);
}

void te::da::DataSetTypeConverter::remove(std::size_t propertyPos)
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

void te::da::DataSetTypeConverter::add(const std::string& propertyName, te::dt::Property* p, const std::string& attributeConverterName)
{
  std::size_t pos = m_inDataSetType->getPropertyPosition(propertyName);
  add(pos, p);
}

void te::da::DataSetTypeConverter::add(std::size_t propertyPos, te::dt::Property* p, const std::string& attributeConverterName)
{
  std::vector<std::size_t> indexes;
  indexes.push_back(propertyPos);
  add(indexes, p, attributeConverterName);
}

void te::da::DataSetTypeConverter::add(std::size_t propertyPos, te::dt::Property* p, AttributeConverter conv)
{
  std::vector<std::size_t> indexes;
  indexes.push_back(propertyPos);
  add(indexes, p, conv);
}

void te::da::DataSetTypeConverter::add(const std::vector<std::string>& propertyNames, te::dt::Property* p, const std::string& attributeConverterName)
{
  std::vector<std::size_t> indexes;

  for(std::size_t i = 0; i < propertyNames.size(); ++i)
    indexes.push_back(m_inDataSetType->getPropertyPosition(propertyNames[i]));

  add(indexes, p, attributeConverterName);
}

void te::da::DataSetTypeConverter::add(const std::vector<std::size_t>& propertyPos, te::dt::Property* p, const std::string& attributeConverterName)
{
  assert(!propertyPos.empty());
  assert(p);

  te::da::AttributeConverter conv = te::da::AttributeConverterManager::getInstance().getConverter(attributeConverterName);

  // Adding given property on out data set type
  m_outDataSetType->add(p);

  // Storing the converted properties indexes
  m_propertyIndexes.push_back(propertyPos);

  // Indexing the AttributeConverter functions
  m_converters.push_back(conv);

  // Counting reference to converted properties
  for(std::size_t i = 0; i < propertyPos.size(); ++i)
    m_convertedProperties[propertyPos[i]]++;

  m_functionsNames.push_back(attributeConverterName);
}

void te::da::DataSetTypeConverter::add(const std::vector<std::size_t>& propertyPos, te::dt::Property* p, AttributeConverter conv)
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

  m_functionsNames.push_back("UnknownAttributeConverter");
}


bool te::da::DataSetTypeConverter::needConverter(DataSetType* type, const DataSourceCapabilities& capabilities)
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

te::da::DataSetType* te::da::DataSetTypeConverter::getResult() const
{
  return m_outDataSetType;
}

const std::vector<std::vector<std::size_t> >& te::da::DataSetTypeConverter::getConvertedPropertyIndexes() const
{
  return m_propertyIndexes;
}

const std::vector<te::da::AttributeConverter>& te::da::DataSetTypeConverter::getConverters() const
{
  return m_converters;
}

bool te::da::DataSetTypeConverter::isConverted(std::size_t i) const
{
  assert(i < m_convertedProperties.size());
  return m_convertedProperties[i] > 0;
}
