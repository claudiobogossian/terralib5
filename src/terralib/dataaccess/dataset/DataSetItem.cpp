/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/DataSetItem.cpp

  \brief A single element in a data collection represented by a dataset is called DataSetItem.
*/

// TerraLib
#include "../../common/HexUtils.h"
#include "../../common/StringUtils.h"
#include "../../datatype/AbstractData.h"
#include "../../datatype/Array.h"
#include "../../datatype/ByteArray.h"
#include "../../datatype/DateTime.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry/Geometry.h"
#include "../../raster/Raster.h"
#include "../utils/Utils.h"
#include "DataSetItem.h"
#include "DataSetType.h"

// STL
#include <memory>

char te::da::DataSetItem::getChar(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getChar(pos);
}

unsigned char te::da::DataSetItem::getUChar(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getUChar(pos);
}

boost::int16_t te::da::DataSetItem::getInt16(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getInt16(pos);
}

boost::int32_t te::da::DataSetItem::getInt32(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getInt32(pos);
}

boost::int64_t te::da::DataSetItem::getInt64(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getInt64(pos);
}

bool te::da::DataSetItem::getBool(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getBool(pos);
}

float te::da::DataSetItem::getFloat(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getFloat(pos);
}

double te::da::DataSetItem::getDouble(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getDouble(pos);
}

std::string te::da::DataSetItem::getNumeric(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getNumeric(pos);
}

std::string te::da::DataSetItem::getString(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getString(pos);
}

te::dt::ByteArray* te::da::DataSetItem::getByteArray(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getByteArray(pos);
}

te::gm::Geometry* te::da::DataSetItem::getGeometry(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getGeometry(pos);
}

te::rst::Raster* te::da::DataSetItem::getRaster(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getRaster(pos);
}

te::dt::DateTime* te::da::DataSetItem::getDateTime(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getDateTime(pos);
}

te::dt::Array* te::da::DataSetItem::getArray(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getArray(pos);
}

te::dt::AbstractData* te::da::DataSetItem::getValue(std::size_t i) const
{
  switch(getPropertyDataType(i))
  {
    case te::dt::CHAR_TYPE:
      return new te::dt::Char(getChar(i));

    case te::dt::UCHAR_TYPE:
      return new te::dt::UChar(getUChar(i));

    case te::dt::INT16_TYPE:
      return new te::dt::Int16(getInt16(i));

    case te::dt::UINT16_TYPE:
      return new te::dt::UInt16(getInt16(i));

    case te::dt::INT32_TYPE:
      return new te::dt::Int32(getInt32(i));

    case te::dt::UINT32_TYPE:
      return new te::dt::UInt32(getInt32(i));

    case te::dt::INT64_TYPE:
      return new te::dt::Int64(getInt64(i));

    case te::dt::UINT64_TYPE:
      return new te::dt::UInt64(getInt64(i));

    case te::dt::BOOLEAN_TYPE:
      return new te::dt::Boolean(getBool(i));

    case te::dt::FLOAT_TYPE:
      return new te::dt::Float(getFloat(i));

    case te::dt::DOUBLE_TYPE:
      return new te::dt::Double(getDouble(i));

    case te::dt::NUMERIC_TYPE:
      return new te::dt::Numeric(getNumeric(i));

    case te::dt::STRING_TYPE:
      return new te::dt::String(getString(i));

    case te::dt::BYTE_ARRAY_TYPE:
       return getByteArray(i);

    case te::dt::GEOMETRY_TYPE:
      return getGeometry(i);

    case te::dt::DATETIME_TYPE:
      return getDateTime(i);

    case te::dt::RASTER_TYPE:
      return getRaster(i);

    default:
      return 0;
  }
}

te::dt::AbstractData* te::da::DataSetItem::getValue(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getValue(pos);
}

std::string te::da::DataSetItem::getAsString(std::size_t i) const
{
  std::string value;

  switch(getPropertyDataType(i))
  {
    case te::dt::CHAR_TYPE:
      value = getChar(i);
    break;

    case te::dt::UCHAR_TYPE:
      value = getUChar(i);
    break;

    case te::dt::INT16_TYPE:
      value = te::common::Convert2String(getInt16(i));
    break;

    case te::dt::INT32_TYPE:
      value = te::common::Convert2String(getInt32(i));
    break;

    case te::dt::INT64_TYPE:
      value = te::common::Convert2String(getInt64(i));
    break;

    case te::dt::BOOLEAN_TYPE:
      value = getBool(i) ? "t" : "f";
    break;

    case te::dt::FLOAT_TYPE:
      value = te::common::Convert2String(getFloat(i));
    break;

    case te::dt::DOUBLE_TYPE:
      value = te::common::Convert2String(getDouble(i));
    break;

    case te::dt::NUMERIC_TYPE:
      value = getNumeric(i);
    break;

    case te::dt::STRING_TYPE:
      value = getString(i);
    break;

    case te::dt::BYTE_ARRAY_TYPE:
      {
        std::auto_ptr<te::dt::AbstractData> b(getByteArray(i));
        value = b->toString();
      }
    break;

    case te::dt::GEOMETRY_TYPE:
      {
        std::auto_ptr<te::dt::AbstractData> g(getGeometry(i));
        value = g->toString();
      }
    break;

    case te::dt::DATETIME_TYPE:
      {
        std::auto_ptr<te::dt::AbstractData> dTime(getDateTime(i));
        value = dTime->toString();
      }
    break;

    case te::dt::ARRAY_TYPE:
      {
        std::auto_ptr<te::dt::Array> a(getArray(i));
        value = a->toString();
      }
    break;

    case te::dt::RASTER_TYPE:
      {
        std::auto_ptr<te::dt::AbstractData> r(getRaster(i));
        value = r->toString();
      }
    break;

    default:
    break;
  }

  return value;
}

std::string te::da::DataSetItem::getAsString(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return getAsString(pos);
}

bool te::da::DataSetItem::isNull(const std::string& name) const
{
  std::size_t pos = GetPropertyPos(this, name);

  return isNull(pos);
}

