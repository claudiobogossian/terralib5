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
  \file terralib/dataaccess/datasource/DataTypeCapabilities.cpp

  \brief A class that represents the supported data types of a specific data source.
*/

// TerraLib
#include "DataTypeCapabilities.h"

te::da::DataTypeCapabilities::DataTypeCapabilities()
{
  m_types[te::dt::BIT_TYPE]        = false;
  m_types[te::dt::CHAR_TYPE]       = false;
  m_types[te::dt::UCHAR_TYPE]      = false;
  m_types[te::dt::INT16_TYPE]      = false;
  m_types[te::dt::UINT16_TYPE]     = false;
  m_types[te::dt::INT32_TYPE]      = false;
  m_types[te::dt::UINT32_TYPE]     = false;
  m_types[te::dt::INT64_TYPE]      = false;
  m_types[te::dt::UINT64_TYPE]     = false;
  m_types[te::dt::BOOLEAN_TYPE]    = false;
  m_types[te::dt::FLOAT_TYPE]      = false;
  m_types[te::dt::DOUBLE_TYPE]     = false;
  m_types[te::dt::NUMERIC_TYPE]    = false;
  m_types[te::dt::STRING_TYPE]     = false;
  m_types[te::dt::BYTE_ARRAY_TYPE] = false;
  m_types[te::dt::GEOMETRY_TYPE]   = false;
  m_types[te::dt::DATETIME_TYPE]   = false;
  m_types[te::dt::ARRAY_TYPE]      = false;
  m_types[te::dt::COMPOSITE_TYPE]  = false;
  m_types[te::dt::DATASET_TYPE]    = false;
  m_types[te::dt::RASTER_TYPE]     = false;
  m_types[te::dt::XML_TYPE]        = false;

  // No listed on TerraLib Wiki
  m_types[te::dt::UNKNOWN]          = false;
  m_types[te::dt::VOID_TYPE]        = false;
  m_types[te::dt::CINT16_TYPE]      = false;
  m_types[te::dt::CINT32_TYPE]      = false;
  m_types[te::dt::CFLOAT_TYPE]      = false;
  m_types[te::dt::CDOUBLE_TYPE]     = false;
  m_types[te::dt::DATASETITEM_TYPE] = false;
  m_types[te::dt::POLYMORPHIC_TYPE] = false;
}

te::da::DataTypeCapabilities::~DataTypeCapabilities()
{
}

bool te::da::DataTypeCapabilities::supportsBit() const
{
  return hasSupport(te::dt::BIT_TYPE);
}

void te::da::DataTypeCapabilities::setSupportBit(const bool& support)
{
  setSupport(te::dt::BIT_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsChar() const
{
  return hasSupport(te::dt::CHAR_TYPE);
}

void te::da::DataTypeCapabilities::setSupportChar(const bool& support)
{
  setSupport(te::dt::CHAR_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsUChar() const
{
  return hasSupport(te::dt::UCHAR_TYPE);
}

void te::da::DataTypeCapabilities::setSupportUChar(const bool& support)
{
  setSupport(te::dt::UCHAR_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsInt16() const
{
  return hasSupport(te::dt::INT16_TYPE);
}

void te::da::DataTypeCapabilities::setSupportInt16(const bool& support)
{
  setSupport(te::dt::INT16_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsUInt16() const
{
  return hasSupport(te::dt::UINT16_TYPE);
}

void te::da::DataTypeCapabilities::setSupportUInt16(const bool& support)
{
  setSupport(te::dt::UINT16_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsInt32() const
{
  return hasSupport(te::dt::INT32_TYPE);
}

void te::da::DataTypeCapabilities::setSupportInt32(const bool& support)
{
  setSupport(te::dt::INT32_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsUInt32() const
{
  return hasSupport(te::dt::UINT32_TYPE);
}

void te::da::DataTypeCapabilities::setSupportUInt32(const bool& support)
{
  setSupport(te::dt::UINT32_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsInt64() const
{
  return hasSupport(te::dt::INT64_TYPE);
}

void te::da::DataTypeCapabilities::setSupportInt64(const bool& support)
{
  setSupport(te::dt::INT64_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsUInt64() const
{
  return hasSupport(te::dt::UINT64_TYPE);
}

void te::da::DataTypeCapabilities::setSupportUInt64(const bool& support)
{
  setSupport(te::dt::UINT64_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsBoolean() const
{
  return hasSupport(te::dt::BOOLEAN_TYPE);
}

void te::da::DataTypeCapabilities::setSupportBoolean(const bool& support)
{
  setSupport(te::dt::BOOLEAN_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsFloat() const
{
  return hasSupport(te::dt::FLOAT_TYPE);
}

void te::da::DataTypeCapabilities::setSupportFloat(const bool& support)
{
  setSupport(te::dt::FLOAT_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsDouble() const
{
  return hasSupport(te::dt::DOUBLE_TYPE);
}

void te::da::DataTypeCapabilities::setSupportDouble(const bool& support)
{
  setSupport(te::dt::DOUBLE_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsNumeric() const
{
  return hasSupport(te::dt::NUMERIC_TYPE);
}

void te::da::DataTypeCapabilities::setSupportNumeric(const bool& support)
{
  setSupport(te::dt::NUMERIC_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsString() const
{
  return hasSupport(te::dt::STRING_TYPE);
}

void te::da::DataTypeCapabilities::setSupportString(const bool& support)
{
  setSupport(te::dt::STRING_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsByteArray() const
{
  return hasSupport(te::dt::BYTE_ARRAY_TYPE);
}

void te::da::DataTypeCapabilities::setSupportByteArray(const bool& support)
{
  setSupport(te::dt::BYTE_ARRAY_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsGeometry() const
{
  return hasSupport(te::dt::GEOMETRY_TYPE);
}

void te::da::DataTypeCapabilities::setSupportGeometry(const bool& support)
{
  setSupport(te::dt::GEOMETRY_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsDateTime() const
{
  return hasSupport(te::dt::DATETIME_TYPE);
}

void te::da::DataTypeCapabilities::setSupportDateTime(const bool& support)
{
  setSupport(te::dt::DATETIME_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsArray() const
{
  return hasSupport(te::dt::ARRAY_TYPE);
}

void te::da::DataTypeCapabilities::setSupportArray(const bool& support)
{
  setSupport(te::dt::ARRAY_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsComposite() const
{
  return hasSupport(te::dt::COMPOSITE_TYPE);
}

void te::da::DataTypeCapabilities::setSupportComposite(const bool& support)
{
  setSupport(te::dt::COMPOSITE_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsRaster() const
{
  return hasSupport(te::dt::RASTER_TYPE);
}

void te::da::DataTypeCapabilities::setSupportRaster(const bool& support)
{
  setSupport(te::dt::RASTER_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsDataset() const
{
  return hasSupport(te::dt::DATASET_TYPE);
}

void te::da::DataTypeCapabilities::setSupportDataset(const bool& support)
{
  setSupport(te::dt::DATASET_TYPE, support);
}

bool te::da::DataTypeCapabilities::supportsXML() const
{
  return hasSupport(te::dt::XML_TYPE);
}

void te::da::DataTypeCapabilities::setSupportXML(const bool& support)
{
  setSupport(te::dt::XML_TYPE, support);
}

bool te::da::DataTypeCapabilities::hasSupport(const int& type) const
{
  std::map<int, bool>::const_iterator it = m_types.find(type);
  if(it != m_types.end())
    return it->second;

  return false;
}

void te::da::DataTypeCapabilities::setSupport(const int& type, const bool& support)
{
  m_types[type] = support;
}

void te::da::DataTypeCapabilities::setSupportAll()
{
  std::map<int, bool>::iterator it;
  for(it = m_types.begin(); it != m_types.end(); ++it)
    it->second = true;
}
