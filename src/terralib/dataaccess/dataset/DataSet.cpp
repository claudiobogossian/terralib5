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
  \file terralib/dataaccess/dataset/DataSet.cpp

  \brief DataSet is the fundamental unit of information transferred between a data source and the data access module.
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
#include "DataSet.h"
#include "DataSetType.h"

// STL
#include <memory>

te::gm::Envelope* te::da::DataSet::getExtent()
{
  const DataSetType* dt = getType();

  te::gm::GeometryProperty* gp = dt->getDefaultGeomProperty();

  return getExtent(gp);
}

void te::da::DataSet::setFilter(const te::gm::Geometry* g, te::gm::SpatialRelation r)
{
  const DataSetType* dt = getType();

  te::gm::GeometryProperty* gp = dt->getDefaultGeomProperty();

  return setFilter(gp, g, r);
}

void te::da::DataSet::setFilter(const te::gm::Envelope* e, te::gm::SpatialRelation r)
{
  const DataSetType* dt = getType();

  te::gm::GeometryProperty* gp = dt->getDefaultGeomProperty();

  return setFilter(gp, e, r);
}

char te::da::DataSet::getChar(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getChar(pos);
}

void te::da::DataSet::setChar(const std::string& name, char value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setChar(pos, value);
}

unsigned char te::da::DataSet::getUChar(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getUChar(pos);
}

void te::da::DataSet::setUChar(const std::string& name, unsigned char value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setUChar(pos, value);
}

boost::int16_t te::da::DataSet::getInt16(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getInt16(pos);
}

void te::da::DataSet::setInt16(const std::string& name, boost::int16_t value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setInt16(pos, value);
}

boost::int32_t te::da::DataSet::getInt32(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getInt32(pos);
}

void te::da::DataSet::setInt32(const std::string& name, boost::int32_t value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setInt32(pos, value);
}

boost::int64_t te::da::DataSet::getInt64(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getInt64(pos);
}

void te::da::DataSet::setInt64(const std::string& name, boost::int64_t value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setInt64(pos, value);
}

bool te::da::DataSet::getBool(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getBool(pos);
}

void te::da::DataSet::setBool(const std::string& name, bool value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setBool(pos, value);
}

float te::da::DataSet::getFloat(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getFloat(pos);
}

void te::da::DataSet::setFloat(const std::string& name, float value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setFloat(pos, value);
}

double te::da::DataSet::getDouble(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getDouble(pos);
}

void te::da::DataSet::setDouble(const std::string& name, double value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setDouble(pos, value);
}

std::string te::da::DataSet::getNumeric(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getNumeric(pos);
}

void te::da::DataSet::setNumeric(const std::string& name, const std::string& value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setNumeric(pos, value);
}

std::string te::da::DataSet::getString(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getString(pos);
}

void te::da::DataSet::setString(const std::string& name, const std::string& value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setString(pos, value);
}

te::dt::ByteArray* te::da::DataSet::getByteArray(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getByteArray(pos);
}

void te::da::DataSet::setByteArray(const std::string& name, const te::dt::ByteArray& value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setByteArray(pos, value);
}

te::gm::Geometry* te::da::DataSet::getGeometry(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getGeometry(pos);
}

te::gm::Geometry* te::da::DataSet::getGeometry() const
{
  const DataSetType* dt = getType();

  int pos = static_cast<int>(dt->getDefaultGeomPropertyPos());

  return getGeometry(pos);
}

void te::da::DataSet::setGeometry(const std::string& name, const te::gm::Geometry& value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setGeometry(pos, value);
}

te::rst::Raster* te::da::DataSet::getRaster(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getRaster(pos);
}

te::rst::Raster* te::da::DataSet::getRaster() const
{
  const DataSetType* dt = getType();

  std::size_t size = dt->size();

  for(std::size_t i = 0; i < size; ++i)
    if(dt->getProperty(i)->getType() == te::dt::RASTER_TYPE)
      return getRaster(static_cast<int>(i));

  return 0;
}

void te::da::DataSet::setRaster(const std::string& name, const te::rst::Raster& value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setRaster(pos, value);
}

te::dt::DateTime* te::da::DataSet::getDateTime(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getDateTime(pos);
}

void te::da::DataSet::setDateTime(const std::string& name, const te::dt::DateTime& value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setDateTime(pos, value);
}

void te::da::DataSet::getArray(const std::string& name, std::vector<boost::int16_t>& a) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getArray(pos, a);
}

const unsigned char* te::da::DataSet::getWKB(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getWKB(pos);
}

const unsigned char* te::da::DataSet::getWKB() const
{
  const DataSetType* dt = getType();

  int pos = static_cast<int>(dt->getDefaultGeomPropertyPos());

  return getWKB(pos);
}

te::da::DataSet* te::da::DataSet::getDataSet(const std::string& name)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return getDataSet(pos);
}

void te::da::DataSet::setDataSet(const std::string& name, const DataSet& value)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setDataSet(pos, value);
}

te::dt::AbstractData* te::da::DataSet::getValue(int i) const
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

te::dt::AbstractData* te::da::DataSet::getValue(const std::string& name) const
{
  const DataSetType* dt = getType();

  if(dt == 0)
    return 0;

  const te::dt::Property* p = dt->getProperty(name);

  int i = static_cast<int>(dt->getPropertyPosition(p));

  return getValue(i);
}

void te::da::DataSet::setValue(const std::string& name, te::dt::AbstractData* ad)
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return setValue(pos, ad);
}

std::string te::da::DataSet::getAsString(int i, int precision) const
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
      value = te::common::Convert2String(getDouble(i), precision);
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

    /*case TE_ARRAY:
        value = te::common::Convert2String(getInt(i));
    break;*/

    /*case COMPLEX:
        value = te::common::Convert2String(getInt(i));
    break;*/

    /*case TE_DATASET:
        value = te::common::Convert2String(getInt(i));
    break;*/

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

std::string te::da::DataSet::getAsString(const std::string& name, int precision) const
{
  const DataSetType* dt = getType();

  if(dt == 0)
    return std::string("");

  const te::dt::Property* p = dt->getProperty(name);

  int i = static_cast<int>(dt->getPropertyPosition(p));

  return getAsString(i, precision);
}

bool te::da::DataSet::isNull(const std::string& name) const
{
  int pos = static_cast<int>(getType()->getPropertyPosition(name));

  return isNull(pos);
}

