/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
#include "../../datatype/Array.h"
#include "../../datatype/ByteArray.h"
#include "../../datatype/DataConverterManager.h"
#include "../../datatype/DateTime.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/Envelope.h"
#include "../../raster/Raster.h"
#include "../../srs/Config.h"
#include "../datasource/DataSourceCapabilities.h"
#include "../utils/Utils.h"
#include "../Exception.h"
#include "DataSetAdapter.h"
#include "DataSetType.h"

// STL
#include <cassert>
#include <memory>

te::da::DataSetAdapter::DataSetAdapter(DataSet* dataset, bool isOwner)
  : m_ds(dataset, isOwner)
{
  assert(dataset);
  m_srid = 0;
}

te::da::DataSetAdapter::~DataSetAdapter()
{
}

te::common::TraverseType te::da::DataSetAdapter::getTraverseType() const
{
  return m_ds->getTraverseType();
}

te::common::AccessPolicy te::da::DataSetAdapter::getAccessPolicy() const
{
  return m_ds->getAccessPolicy();
}

std::auto_ptr<te::gm::Envelope> te::da::DataSetAdapter::getExtent(std::size_t i)
{
  return m_ds->getExtent(i);
}

std::size_t te::da::DataSetAdapter::getNumProperties() const
{
  return m_pnames.size();
}

int te::da::DataSetAdapter::getPropertyDataType(std::size_t pos) const
{
  return m_datatypes[pos];
}

std::string te::da::DataSetAdapter::getPropertyName(std::size_t pos) const
{
  return m_pnames[pos];
}

te::common::CharEncoding te::da::DataSetAdapter::getPropertyCharEncoding(std::size_t i) const
{
  return te::common::UNKNOWN_CHAR_ENCODING; // TODO!
}

std::string te::da::DataSetAdapter::getDatasetNameOfProperty(std::size_t pos) const
{
  return "";
}

bool te::da::DataSetAdapter::isEmpty() const
{
  return m_ds->isEmpty();
}

bool te::da::DataSetAdapter::isConnected() const
{
  return m_ds->isConnected();  
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

bool te::da::DataSetAdapter::moveBeforeFirst()
{
  return m_ds->moveBeforeFirst();
}

bool te::da::DataSetAdapter::moveFirst()
{
  return m_ds->moveFirst();
}

bool te::da::DataSetAdapter::moveLast()
{
  return m_ds->moveLast();
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

char te::da::DataSetAdapter::getChar(std::size_t i) const
{
  std::auto_ptr<te::dt::Char> data(static_cast<te::dt::Char*>(getAdaptedValue(i)));

  return data->getValue();
}

unsigned char te::da::DataSetAdapter::getUChar(std::size_t i) const
{
  std::auto_ptr<te::dt::UChar> data(static_cast<te::dt::UChar*>(getAdaptedValue(i)));

  return data->getValue();
}

boost::int16_t te::da::DataSetAdapter::getInt16(std::size_t i) const
{
  std::auto_ptr<te::dt::Int16> data(static_cast<te::dt::Int16*>(getAdaptedValue(i)));

  return data->getValue();
}

boost::int32_t te::da::DataSetAdapter::getInt32(std::size_t i) const
{
  std::auto_ptr<te::dt::Int32> data(static_cast<te::dt::Int32*>(getAdaptedValue(i)));

  return data->getValue();
}

boost::int64_t te::da::DataSetAdapter::getInt64(std::size_t i) const
{
  std::auto_ptr<te::dt::Int64> data(static_cast<te::dt::Int64*>(getAdaptedValue(i)));

  return data->getValue();
}

bool te::da::DataSetAdapter::getBool(std::size_t i) const
{
  std::auto_ptr<te::dt::Boolean> data(static_cast<te::dt::Boolean*>(getAdaptedValue(i)));

  return data->getValue();
}

float te::da::DataSetAdapter::getFloat(std::size_t i) const
{
  std::auto_ptr<te::dt::Float> data(static_cast<te::dt::Float*>(getAdaptedValue(i)));

  return data->getValue();
}

double te::da::DataSetAdapter::getDouble(std::size_t i) const
{
  std::auto_ptr<te::dt::Double> data(static_cast<te::dt::Double*>(getAdaptedValue(i)));

  return data->getValue();
}

std::string te::da::DataSetAdapter::getNumeric(std::size_t i) const
{
  std::auto_ptr<te::dt::Numeric> data(static_cast<te::dt::Numeric*>(getAdaptedValue(i)));

  return data->getValue();
}

std::string te::da::DataSetAdapter::getString(std::size_t i) const
{
  std::auto_ptr<te::dt::String> data(static_cast<te::dt::String*>(getAdaptedValue(i)));
  return data->getValue();
}

std::auto_ptr<te::dt::ByteArray> te::da::DataSetAdapter::getByteArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::ByteArray>(static_cast<te::dt::ByteArray*>(getAdaptedValue(i)));
}

std::auto_ptr<te::gm::Geometry> te::da::DataSetAdapter::getGeometry(std::size_t i) const
{
  return std::auto_ptr<te::gm::Geometry>(static_cast<te::gm::Geometry*>(getAdaptedValue(i)));
}

std::auto_ptr<te::rst::Raster> te::da::DataSetAdapter::getRaster(std::size_t i) const
{
  return std::auto_ptr<te::rst::Raster>(static_cast<te::rst::Raster*>(getAdaptedValue(i)));
}

std::auto_ptr<te::dt::DateTime> te::da::DataSetAdapter::getDateTime(std::size_t i) const
{
  return std::auto_ptr<te::dt::DateTime>(static_cast<te::dt::DateTime*>(getAdaptedValue(i)));
}

std::auto_ptr<te::dt::Array> te::da::DataSetAdapter::getArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::Array>(static_cast<te::dt::Array*>(getAdaptedValue(i)));
}

bool te::da::DataSetAdapter::isNull(std::size_t i) const
{
  std::auto_ptr<te::dt::AbstractData> data(getAdaptedValue(i));

  return data.get() == 0;
}

void te::da::DataSetAdapter::setSRID(int srid)
{
  m_srid = srid;
}

te::da::DataSet* te::da::DataSetAdapter::getAdaptee() const
{
  return m_ds.get();
}

void te::da::DataSetAdapter::add(const std::string& newPropertyName,
                                 int newPropertyType,
                                 const std::vector<std::size_t>& adaptedPropertyPos,
                                 AttributeConverter conv)
{
  m_datatypes.push_back(newPropertyType);
  m_pnames.push_back(newPropertyName);
  m_propertyIndexes.push_back(adaptedPropertyPos);
  m_converters.push_back(conv);
}

te::dt::AbstractData* te::da::DataSetAdapter::getAdaptedValue(std::size_t i) const
{
  te::dt::AbstractData* data = m_converters[i](m_ds.get(), m_propertyIndexes[i], m_datatypes[i]);

  if(data && data->getTypeCode() == te::dt::GEOMETRY_TYPE)
  {
    te::gm::Geometry* geom = dynamic_cast<te::gm::Geometry*>(data);

    if (geom && (geom->getSRID() != TE_UNKNOWN_SRS) && (m_srid != TE_UNKNOWN_SRS) && (geom->getSRID() != m_srid))
      geom->transform(m_srid);
  }

  return data;
}
