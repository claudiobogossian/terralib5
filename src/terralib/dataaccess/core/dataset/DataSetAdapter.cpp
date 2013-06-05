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
  \file terralib/dataaccess/core/dataset/DataSetAdapter.cpp

  \brief An adapter for a data set.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../../../datatype/Array.h"
#include "../../../datatype/ByteArray.h"
#include "../../../datatype/DataConverterManager.h"
#include "../../../datatype/DateTime.h"
#include "../../../datatype/Property.h"
#include "../../../datatype/SimpleData.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../raster/Raster.h"
#include "../../datasource/DataSourceCapabilities.h"
#include "../../datasource/DataSourceCatalogLoader.h"
#include "../../datasource/DataSourceTransactor.h"
#include "../utils/Utils.h"
#include "../Exception.h"
#include "DataSetAdapter.h"
#include "DataSetType.h"

// STL
#include <cassert>
#include <memory>

te::da::core::DataSetAdapter::DataSetAdapter(DataSet* dataset, bool isOwner)
  : m_ds(dataset, isOwner)
{
  assert(dataset);
}

te::da::core::DataSetAdapter::~DataSetAdapter()
{
}

te::common::TraverseType te::da::core::DataSetAdapter::getTraverseType() const
{
  return m_ds->getTraverseType();
}

te::common::AccessPolicy te::da::core::DataSetAdapter::getAccessPolicy() const
{
  return m_ds->getAccessPolicy();
}

te::gm::Envelope* te::da::core::DataSetAdapter::getExtent(std::size_t i)
{
  throw Exception(TR_DATAACCESS("Not Implemented Yet!"));
}

std::size_t te::da::core::DataSetAdapter::getNumProperties() const
{
  return m_pnames.size();
}

int te::da::core::DataSetAdapter::getPropertyDataType(std::size_t pos) const
{
  return m_datatypes[pos];
}

std::string te::da::core::DataSetAdapter::getPropertyName(std::size_t pos) const
{
  return m_pnames[pos];
}

std::string te::da::core::DataSetAdapter::getDatasetNameOfProperty(std::size_t pos) const
{
  return "";
}

bool te::da::core::DataSetAdapter::isEmpty() const
{
  return m_ds->isEmpty();
}

std::size_t te::da::core::DataSetAdapter::size() const
{
  return m_ds->size();
}

bool te::da::core::DataSetAdapter::moveNext()
{
  return m_ds->moveNext();
}

bool te::da::core::DataSetAdapter::movePrevious()
{
  return m_ds->movePrevious();
}

bool te::da::core::DataSetAdapter::moveBeforeFirst()
{
  return m_ds->moveBeforeFirst();
}

bool te::da::core::DataSetAdapter::moveFirst()
{
  return m_ds->moveFirst();
}

bool te::da::core::DataSetAdapter::moveLast()
{
  return m_ds->moveLast();
}

bool te::da::core::DataSetAdapter::move(std::size_t i)
{
  return m_ds->move(i);
}

bool te::da::core::DataSetAdapter::isAtBegin() const
{
  return m_ds->isAtBegin();
}

bool te::da::core::DataSetAdapter::isBeforeBegin() const
{
  return m_ds->isBeforeBegin();
}

bool te::da::core::DataSetAdapter::isAtEnd() const
{
  return m_ds->isAtEnd();
}

bool te::da::core::DataSetAdapter::isAfterEnd() const
{
  return m_ds->isAfterEnd();
}

char te::da::core::DataSetAdapter::getChar(std::size_t i) const
{
  std::auto_ptr<te::dt::Char> data(static_cast<te::dt::Char*>(getAdaptedValue(i)));

  return data->getValue();
}

unsigned char te::da::core::DataSetAdapter::getUChar(std::size_t i) const
{
  std::auto_ptr<te::dt::UChar> data(static_cast<te::dt::UChar*>(getAdaptedValue(i)));

  return data->getValue();
}

boost::int16_t te::da::core::DataSetAdapter::getInt16(std::size_t i) const
{
  std::auto_ptr<te::dt::Int16> data(static_cast<te::dt::Int16*>(getAdaptedValue(i)));

  return data->getValue();
}

boost::int32_t te::da::core::DataSetAdapter::getInt32(std::size_t i) const
{
  std::auto_ptr<te::dt::Int32> data(static_cast<te::dt::Int32*>(getAdaptedValue(i)));

  return data->getValue();
}

boost::int64_t te::da::core::DataSetAdapter::getInt64(std::size_t i) const
{
  std::auto_ptr<te::dt::Int64> data(static_cast<te::dt::Int64*>(getAdaptedValue(i)));

  return data->getValue();
}

bool te::da::core::DataSetAdapter::getBool(std::size_t i) const
{
  std::auto_ptr<te::dt::Boolean> data(static_cast<te::dt::Boolean*>(getAdaptedValue(i)));

  return data->getValue();
}

float te::da::core::DataSetAdapter::getFloat(std::size_t i) const
{
  std::auto_ptr<te::dt::Float> data(static_cast<te::dt::Float*>(getAdaptedValue(i)));

  return data->getValue();
}

double te::da::core::DataSetAdapter::getDouble(std::size_t i) const
{
  std::auto_ptr<te::dt::Double> data(static_cast<te::dt::Double*>(getAdaptedValue(i)));

  return data->getValue();
}

std::string te::da::core::DataSetAdapter::getNumeric(std::size_t i) const
{
  std::auto_ptr<te::dt::Numeric> data(static_cast<te::dt::Numeric*>(getAdaptedValue(i)));

  return data->getValue();
}

std::string te::da::core::DataSetAdapter::getString(std::size_t i) const
{
  std::auto_ptr<te::dt::String> data(static_cast<te::dt::String*>(getAdaptedValue(i)));
  return data->getValue();
}

te::dt::ByteArray* te::da::core::DataSetAdapter::getByteArray(std::size_t i) const
{
  return static_cast<te::dt::ByteArray*>(getAdaptedValue(i));
}

te::gm::Geometry* te::da::core::DataSetAdapter::getGeometry(std::size_t i) const
{
  return static_cast<te::gm::Geometry*>(getAdaptedValue(i));
}

te::rst::Raster* te::da::core::DataSetAdapter::getRaster(std::size_t i) const
{
  return static_cast<te::rst::Raster*>(getAdaptedValue(i));
}

te::dt::DateTime* te::da::core::DataSetAdapter::getDateTime(std::size_t i) const
{
  return static_cast<te::dt::DateTime*>(getAdaptedValue(i));
}

te::dt::Array* te::da::core::DataSetAdapter::getArray(std::size_t i) const
{
return static_cast<te::dt::Array*>(getAdaptedValue(i));
}

bool te::da::core::DataSetAdapter::isNull(std::size_t i) const
{
  std::auto_ptr<te::dt::AbstractData> data(getAdaptedValue(i));

  return data.get() != 0;
}

te::da::core::DataSet* te::da::core::DataSetAdapter::getAdaptee() const
{
  return m_ds.get();
}

void te::da::core::DataSetAdapter::add(const std::string& newPropertyName,
                                 int newPropertyType,
                                 const std::vector<std::size_t>& adaptedPropertyPos,
                                 AttributeConverter conv)
{
  m_datatypes.push_back(newPropertyType);
  m_pnames.push_back(newPropertyName);
  m_propertyIndexes.push_back(adaptedPropertyPos);
  m_converters.push_back(conv);
}

te::dt::AbstractData* te::da::core::DataSetAdapter::getAdaptedValue(std::size_t i) const
{
  return m_converters[i](m_ds.get(), m_propertyIndexes[i], m_datatypes[i]);
}
