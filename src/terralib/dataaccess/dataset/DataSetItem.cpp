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
#include "../../datatype/ByteArray.h"
#include "../../datatype/DateTime.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../raster/Raster.h"
#include "DataSetItem.h"
#include "DataSetType.h"

// STL
#include <memory>

char te::da::DataSetItem::getChar(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getChar(pos);
}

unsigned char te::da::DataSetItem::getUChar(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getUChar(pos);
}

boost::int16_t te::da::DataSetItem::getInt16(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getInt16(pos);
}

boost::int32_t te::da::DataSetItem::getInt32(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getInt32(pos);
}

boost::int64_t te::da::DataSetItem::getInt64(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getInt64(pos);
}

bool te::da::DataSetItem::getBool(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getBool(pos);
}

float te::da::DataSetItem::getFloat(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getFloat(pos);
}

double te::da::DataSetItem::getDouble(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getDouble(pos);
}

std::string te::da::DataSetItem::getNumeric(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getNumeric(pos);
}

std::string te::da::DataSetItem::getString(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getString(pos);
}

te::dt::ByteArray* te::da::DataSetItem::getByteArray(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getByteArray(pos);
}

te::gm::Geometry* te::da::DataSetItem::getGeometry(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getGeometry(pos);
}

te::gm::Geometry* te::da::DataSetItem::getGeometry() const
{
  const DataSetType* dt = getType();

  int pos = static_cast<int>(dt->getDefaultGeomPropertyPos());

  return getGeometry(pos);
}

te::rst::Raster* te::da::DataSetItem::getRaster(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getRaster(pos);
}

te::rst::Raster* te::da::DataSetItem::getRaster() const
{
  const DataSetType* dt = getType();

  std::size_t size = dt->size();

  for(std::size_t i = 0; i < size; ++i)
    if(dt->getProperty(i)->getType() == te::dt::RASTER_TYPE)
      return getRaster(static_cast<int>(i));

  return 0;
}

te::dt::DateTime* te::da::DataSetItem::getDateTime(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getDateTime(pos);
}

void te::da::DataSetItem::getArray(const std::string& name, std::vector<boost::int16_t>& a) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getArray(pos, a);
}

const unsigned char* te::da::DataSetItem::getWKB(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getWKB(pos);
}

const unsigned char* te::da::DataSetItem::getWKB() const
{
  const DataSetType* dt = getType();

  int pos = static_cast<int>(dt->getDefaultGeomPropertyPos());

  return getWKB(pos);
}

te::da::DataSet* te::da::DataSetItem::getDataSet(const std::string& name)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getDataSet(pos);
}

std::string te::da::DataSetItem::getAsString(int i) const
{
  const DataSetType* dt = getType();
  std::string value;

  if(dt == 0)
    return value;

  const te::dt::Property* p = dt->getProperty(i);

  switch(p->getType())
  {
    case te::dt::CHAR_TYPE:
      value = getChar(i);
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

    /*case ARRAY:
        value = te::common::Convert2String(getInt(i));
    break;*/

    /*case COMPLEX:
        value = te::common::Convert2String(getInt(i));
    break;*/

    /*case DATASET:
        value = te::common::Convert2String(getInt(i));
    break;*/

    case te::dt::RASTER_TYPE:
      {
        std::auto_ptr<te::dt::AbstractData> g(getRaster(i));
        value = g->toString();
      }
    break;

    default:
    break;
  }

  return value;
}

std::string te::da::DataSetItem::getAsString(const std::string& name) const
{
  const DataSetType* dt = getType();

  if(dt == 0)
    return std::string("");

  const te::dt::Property* p = dt->getProperty(name);
  int i = static_cast<int>(dt->getPropertyPosition(p));

  return getAsString(i);
}

te::dt::AbstractData* te::da::DataSetItem::getValue(int i) const
{
  const DataSetType* dt = getType();

  if(dt == 0)
    return 0;
  
  const te::dt::Property* p = dt->getProperty(i);

  te::dt::AbstractData* data = 0;

  switch(p->getType())
  {
    case te::dt::CHAR_TYPE:
      data = new te::dt::Char(getChar(i));
    break;

    case te::dt::UCHAR_TYPE:
      data = new te::dt::UChar(getUChar(i));
    break;

    case te::dt::INT16_TYPE:
      data = new te::dt::Int16(getInt16(i));
    break;

    case te::dt::UINT16_TYPE:
      data = new te::dt::UInt16(getInt16(i));
    break;

    case te::dt::INT32_TYPE:
      data = new te::dt::Int32(getInt32(i));
    break;

    case te::dt::UINT32_TYPE:
      data = new te::dt::UInt32(getInt32(i));
    break;

    case te::dt::INT64_TYPE:
      data = new te::dt::Int64(getInt64(i));
    break;

    case te::dt::UINT64_TYPE:
      data = new te::dt::UInt64(getInt64(i));
    break;

    case te::dt::BOOLEAN_TYPE:
      data = new te::dt::Boolean(getBool(i));
    break;

    case te::dt::FLOAT_TYPE:
      data = new te::dt::Float(getFloat(i));
    break;

    case te::dt::DOUBLE_TYPE:
      data = new te::dt::Double(getDouble(i));
    break;

    case te::dt::NUMERIC_TYPE:
      data = new te::dt::Numeric(getNumeric(i));
    break;

    case te::dt::STRING_TYPE:
      data = new te::dt::String(getString(i));
    break;

    case te::dt::BYTE_ARRAY_TYPE:
       data = getByteArray(i);
    break;

    case te::dt::GEOMETRY_TYPE:
      data = getGeometry(i);
    break;

    case te::dt::DATETIME_TYPE:
      data = getDateTime(i);
    break;

    case te::dt::RASTER_TYPE:
      data = getRaster(i);
    break;

    default:
    break;
  }

  return data;
}

te::dt::AbstractData* te::da::DataSetItem::getValue(const std::string& name) const
{
  const DataSetType* dt = getType();

  if(dt == 0)
    return 0;

  const te::dt::Property* p = dt->getProperty(name);
  int i = static_cast<int>(dt->getPropertyPosition(p));

  return getValue(i);
}

bool te::da::DataSetItem::isNull(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return isNull(pos);
}

