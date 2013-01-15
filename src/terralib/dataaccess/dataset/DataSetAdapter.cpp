/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/DataSetAdapter.cpp

  \brief An adapter for DataSet.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../datatype/ByteArray.h"
#include "../../datatype/DataConverterManager.h"
#include "../../datatype/DateTime.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry/Geometry.h"
#include "../../raster/Raster.h"
#include "../datasource/DataSourceCapabilities.h"
#include "../Exception.h"
#include "DataSetAdapter.h"
#include "DataSetType.h"

// STL
#include <cassert>
#include <memory>

te::da::DataSetAdapter::DataSetAdapter(DataSet* dataset, bool isOwner)
  : te::common::Holder<DataSet>(dataset, isOwner),
    m_ds(dataset),
    m_outDataSetType(0)
{
  assert(m_ds);

  // Stores the DataSetType of the given dataset
  m_inDataSetType = m_ds->getType();
  assert(m_inDataSetType);

  // Creates the empty Adapter DataSetType
  m_outDataSetType = new DataSetType(m_inDataSetType->getName(), m_inDataSetType->getId());
}

te::da::DataSetAdapter::DataSetAdapter(DataSet* dataset, const DataSourceCapabilities& capabilities, bool isOwner)
  : te::common::Holder<DataSet>(dataset, isOwner),
    m_ds(dataset),
    m_outDataSetType(0)
{
  assert(m_ds);

  // Stores the DataSetType of the given dataset
  m_inDataSetType = m_ds->getType();
  assert(m_inDataSetType);

  // Creates the empty Adapter DataSetType
  m_outDataSetType = new DataSetType(m_inDataSetType->getName(), m_inDataSetType->getId());

  // Gets the DataTypeCapabilities
  const DataTypeCapabilities& dtCapabilities = capabilities.getDataTypeCapabilities();

  // Try automatic build the adapter
  const std::vector<te::dt::Property*> properties = m_inDataSetType->getProperties();
  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    te::dt::Property* p = properties[i];
    assert(p);

    if(dtCapabilities.supports(p->getType()))
      adapt(i, p->clone());
    else
    {
      // Try create the property from data source capabilities hint
      const te::dt::Property* hintProperty = dtCapabilities.getHint(p->getType());
      if(hintProperty)
        adapt(i, hintProperty->clone());
    }
  }
}

te::da::DataSetAdapter::~DataSetAdapter()
{
  delete m_outDataSetType;
}

te::common::TraverseType te::da::DataSetAdapter::getTraverseType() const
{
  return m_ds->getTraverseType();
}

te::common::AccessPolicy te::da::DataSetAdapter::getAccessPolicy() const
{
  return m_ds->getAccessPolicy();
}

te::da::DataSetType* te::da::DataSetAdapter::getType()
{
  return m_outDataSetType;
}

const te::da::DataSetType* te::da::DataSetAdapter::getType() const
{
  return m_outDataSetType;
}

te::da::DataSourceTransactor* te::da::DataSetAdapter::getTransactor() const
{
  return 0;  // TO DO!
}

void te::da::DataSetAdapter::loadTypeInfo()
{
  getType();
}

te::da::DataSet* te::da::DataSetAdapter::getParent() const
{
  return m_ds->getParent();
}

te::gm::Envelope* te::da::DataSetAdapter::getExtent(const te::dt::Property* /*p*/)
{
  return 0;  // TO DO!
}

void te::da::DataSetAdapter::setFilter(te::dt::Property* /*p*/, const te::gm::Geometry* /*g*/, te::gm::SpatialRelation /*r*/)
{
  // TO DO!
}

void te::da::DataSetAdapter::setFilter(te::dt::Property* /*p*/, const te::gm::Envelope* /*e*/, te::gm::SpatialRelation /*r*/)
{
  // TO DO!
}

te::da::DataSetItem* te::da::DataSetAdapter::getItem() const
{
  return m_ds->getItem();
}

void te::da::DataSetAdapter::add(te::da::DataSetItem* /*item*/)
{
  // TO DO!
}

bool te::da::DataSetAdapter::isEmpty() const
{
  return m_ds->isEmpty();
}

std::size_t te::da::DataSetAdapter::size() const
{
  return m_ds->size();
}

bool te::da::DataSetAdapter::moveNext()
{
  return m_ds->moveNext();
}

bool te::da::DataSetAdapter::movePrevious()
{
  return m_ds->movePrevious();
}

bool te::da::DataSetAdapter::moveFirst()
{
  return m_ds->moveFirst();
}

bool te::da::DataSetAdapter::moveBeforeFirst()
{
  return m_ds->moveBeforeFirst();
}

bool te::da::DataSetAdapter::moveLast()
{
  return m_ds->moveLast();
}

bool te::da::DataSetAdapter::moveAfterLast()
{
  return m_ds->moveAfterLast();
}

bool te::da::DataSetAdapter::move(std::size_t i)
{
  return m_ds->move(i);
}

bool te::da::DataSetAdapter::isAtBegin() const
{
  return m_ds->isAtBegin();
}

bool te::da::DataSetAdapter::isBeforeBegin() const
{
  return m_ds->isBeforeBegin();
}

bool te::da::DataSetAdapter::isAtEnd() const
{
  return m_ds->isAtEnd();
}

bool te::da::DataSetAdapter::isAfterEnd() const
{
  return m_ds->isAfterEnd();
}

char te::da::DataSetAdapter::getChar(int i) const
{
  std::auto_ptr<te::dt::Char> data(static_cast<te::dt::Char*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setChar(int i, char value) 
{
  setAdaptedValue(i, new te::dt::Char(value));
}

unsigned char te::da::DataSetAdapter::getUChar(int i) const
{
  std::auto_ptr<te::dt::UChar> data(static_cast<te::dt::UChar*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setUChar(int i, unsigned char value) 
{
  setAdaptedValue(i, new te::dt::UChar(value));
}

boost::int16_t te::da::DataSetAdapter::getInt16(int i) const
{
  std::auto_ptr<te::dt::Int16> data(static_cast<te::dt::Int16*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setInt16(int i, boost::int16_t value) 
{
  setAdaptedValue(i, new te::dt::Int16(value));
}

boost::int32_t te::da::DataSetAdapter::getInt32(int i) const
{
  std::auto_ptr<te::dt::Int32> data(static_cast<te::dt::Int32*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setInt32(int i, boost::int32_t value) 
{
  setAdaptedValue(i, new te::dt::Int32(value));
}

boost::int64_t te::da::DataSetAdapter::getInt64(int i) const
{
  std::auto_ptr<te::dt::Int64> data(static_cast<te::dt::Int64*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setInt64(int i, boost::int64_t value) 
{
  setAdaptedValue(i, new te::dt::Int64(value));
}

bool te::da::DataSetAdapter::getBool(int i) const
{
  std::auto_ptr<te::dt::Boolean> data(static_cast<te::dt::Boolean*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setBool(int i, bool value) 
{
  setAdaptedValue(i, new te::dt::Boolean(value));
}

float te::da::DataSetAdapter::getFloat(int i) const
{
  std::auto_ptr<te::dt::Float> data(static_cast<te::dt::Float*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setFloat(int i, float value) 
{
  setAdaptedValue(i, new te::dt::Float(value));
}

double te::da::DataSetAdapter::getDouble(int i) const
{
  std::auto_ptr<te::dt::Double> data(static_cast<te::dt::Double*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setDouble(int i, double value) 
{
  setAdaptedValue(i, new te::dt::Double(value));
}

std::string te::da::DataSetAdapter::getNumeric(int i) const
{
  std::auto_ptr<te::dt::Numeric> data(static_cast<te::dt::Numeric*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setNumeric(int i, const std::string& value) 
{
  setAdaptedValue(i, new te::dt::Numeric(value));
}

std::string te::da::DataSetAdapter::getString(int i) const
{
  std::auto_ptr<te::dt::String> data(static_cast<te::dt::String*>(getAdaptedValue(i)));
  return data->getValue();
}

void te::da::DataSetAdapter::setString(int i, const std::string& value) 
{
  setAdaptedValue(i, new te::dt::String(value));
}

te::dt::ByteArray* te::da::DataSetAdapter::getByteArray(int i) const
{
  return static_cast<te::dt::ByteArray*>(getAdaptedValue(i));
}

void te::da::DataSetAdapter::setByteArray(int i, const te::dt::ByteArray& value) 
{
  setAdaptedValue(i, (te::dt::AbstractData*)&value);
}

te::gm::Geometry* te::da::DataSetAdapter::getGeometry(int i) const
{
  return static_cast<te::gm::Geometry*>(getAdaptedValue(i));
}

void te::da::DataSetAdapter::setGeometry(int i, const te::gm::Geometry& value) 
{
  setAdaptedValue(i, (te::dt::AbstractData*)&value);
}

te::rst::Raster* te::da::DataSetAdapter::getRaster(int i) const
{
  return static_cast<te::rst::Raster*>(getAdaptedValue(i));
}

void te::da::DataSetAdapter::setRaster(int i, const te::rst::Raster& value) 
{
  setAdaptedValue(i, (te::dt::AbstractData*)&value);
}

te::dt::DateTime* te::da::DataSetAdapter::getDateTime(int i) const
{
  return static_cast<te::dt::DateTime*>(getAdaptedValue(i));
}

void te::da::DataSetAdapter::setDateTime(int i, const te::dt::DateTime& value) 
{
  setAdaptedValue(i, (te::dt::AbstractData*)&value);
}

void te::da::DataSetAdapter::getArray(int i, std::vector<boost::int16_t>& a) const
{
  // TO DO!
}

const unsigned char* te::da::DataSetAdapter::getWKB(int i) const
{
  if(m_outDataSetType->getProperty(i)->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_DATAACCESS("Not a geometry property to get the WKB."));

  std::size_t size = 0;
  unsigned char* wkb = (unsigned char*)(getGeometry(i)->asBinary(size));

  return wkb;
}

te::da::DataSet* te::da::DataSetAdapter::getDataSet(int i)
{
  return static_cast<te::da::DataSet*>(getAdaptedValue(i));
}

void te::da::DataSetAdapter::setDataSet(int i, const te::da::DataSet& value)
{
  setAdaptedValue(i, (te::dt::AbstractData*)&value);
}

void te::da::DataSetAdapter::setValue(int i, te::dt::AbstractData* ad)
{
  setAdaptedValue(i, ad);
}

bool te::da::DataSetAdapter::isNull(int i) const
{
  return false;
}

void te::da::DataSetAdapter::getNonAdaptedProperties(std::vector<std::string>& properties)
{
  for(std::size_t i = 0; i < m_inDataSetType->size(); ++i)
  {
    if(isAdapted(i))
      continue;
    properties.push_back(m_inDataSetType->getProperty(i)->getName());
  }
}

void te::da::DataSetAdapter::getNonAdaptedProperties(std::vector<int>& properties)
{
  for(std::size_t i = 0; i < m_inDataSetType->size(); ++i)
  {
    if(isAdapted(i))
      continue;
    properties.push_back(i);
  }
}

void te::da::DataSetAdapter::adapt(const std::string& propertyName, te::dt::Property* p)
{
  std::size_t pos = m_inDataSetType->getPropertyPosition(propertyName);
  adapt(pos, p);
}

void te::da::DataSetAdapter::adapt(int i, te::dt::Property* p)
{
  adapt(i, p, GenericAttributeConverter);
}

void te::da::DataSetAdapter::adapt(const std::string& propertyName, te::dt::Property* p, AttributeConverter conv)
{
  std::size_t pos = m_inDataSetType->getPropertyPosition(propertyName);
  adapt(pos, p, conv);
}

void te::da::DataSetAdapter::adapt(int i, te::dt::Property* p, AttributeConverter conv)
{
  std::vector<int> indexes;
  indexes.push_back(i);
  adapt(indexes, p, conv);
}

void te::da::DataSetAdapter::adapt(const std::vector<std::string>& propertyNames, te::dt::Property* p, AttributeConverter conv)
{
  std::vector<int> indexes;
  for(std::size_t i = 0; i < propertyNames.size(); ++i)
    indexes.push_back(m_inDataSetType->getPropertyPosition(propertyNames[i]));

  adapt(indexes, p, conv);
}

void te::da::DataSetAdapter::adapt(const std::vector<int>& propertyIndexes, te::dt::Property* p, AttributeConverter conv)
{
  assert(!propertyIndexes.empty());
  assert(p);

  // Adding given property on adapted data set type
  m_outDataSetType->add(p);

  // Storing the adapted properties indexes
  m_propertyIndexes.push_back(propertyIndexes);

  // Indexing the AttributeConverter functions
  m_converters.push_back(conv);

  // Adding on adapted properties list
  for(std::size_t i = 0; i < propertyIndexes.size(); ++i)
    m_adaptedProperties.insert(propertyIndexes[i]);
}

te::da::DataSetAdapter* te::da::DataSetAdapter::adapt(DataSet* dataset, bool isOwner)
{
  return new DataSetAdapter(dataset, isOwner);
}

te::da::DataSetAdapter* te::da::DataSetAdapter::adapt(DataSet* dataset, const DataSourceCapabilities& capabilities, bool isOwner)
{
  return new DataSetAdapter(dataset, capabilities, isOwner);
}

bool te::da::DataSetAdapter::needAdapter(DataSet* dataset, const DataSourceCapabilities& capabilities)
{
  assert(dataset);

  // Gets the DataTypeCapabilities
  const DataTypeCapabilities& dtCapabilities = capabilities.getDataTypeCapabilities();

  const DataSetType* type = dataset->getType();
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

bool te::da::DataSetAdapter::isAdapted(int i) const
{
  std::set<int>::const_iterator it = m_adaptedProperties.find(i);
  if(it != m_adaptedProperties.end())
    return true;

  return false;
}

te::dt::AbstractData* te::da::DataSetAdapter::getAdaptedValue(int i) const
{
  return m_converters[i](m_ds, m_propertyIndexes[i], m_outDataSetType->getProperty(i)->getType());
}

void te::da::DataSetAdapter::setAdaptedValue(int i, te::dt::AbstractData* data)
{
  assert(data);

  const std::vector<int>& indexes = m_propertyIndexes[i];
  if(indexes.size() > 1)
    throw Exception(TR_DATAACCESS("The set operation can not be done considering the current adaptation (n -> 1)."));

  assert(!indexes.empty());

  // Property index on input dataset
  int index = indexes[0];

  // Source and Destination Types
  int srcType = data->getTypeCode();
  int dstType = m_inDataSetType->getProperty(index)->getType();
  if(srcType == dstType) // Need conversion?
  {
    m_ds->setValue(index, data);
    return;
  }

  // Try get a data type converter
  const te::dt::DataTypeConverter& converter = te::dt::DataConverterManager::getInstance().get(srcType, dstType);

  // Converts and sets the given data
  te::dt::AbstractData* convertedData = converter(data);
  assert(convertedData);

  m_ds->setValue(index, convertedData);

  delete data;
}
