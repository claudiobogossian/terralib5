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
  \file terralib/grib/Band.cpp

  \brief Band implemntatin for GRIB.
*/

// TerraLib
#include "../common/Translator.h"
#include "../raster/BandProperty.h"
#include "../datatype/Enums.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"

// STL
#include <cstring>

// Boost
#include <boost/format.hpp>

te::grib::Band::Band(Raster* r, std::size_t idx, grib_handle* handle)
  : te::rst::Band(0, idx),
    m_raster(r),
    m_handle(handle),
    m_data(0)
{
  int datatypecode = te::dt::DOUBLE_TYPE;  // change this for multiple codes!

  m_property = new te::rst::BandProperty(idx, datatypecode);

  std::size_t size = 0;

  int err = grib_get_size(m_handle, "values", &size);

  m_data = new double[size]; // change this if other types are needed!

  err = grib_get_double_array(m_handle, "values", m_data, &size);

  m_property->m_noDataValue = getDouble("missingValue");
  m_property->m_blkh = getLong("numberOfPointsAlongYAxis");
  m_property->m_blkw = getLong("numberOfPointsAlongXAxis");
  m_property->m_nblocksx = 1;
  m_property->m_nblocksy = 1;
}

te::grib::Band::Band(const Band& rhs)
  : te::rst::Band(rhs),
    m_raster(0),
    m_handle(0)
{
  throw Exception(TR_GRIB("Not implemented yet!"));
}

te::grib::Band::~Band()
{
  // flush, close?
  if(m_handle)
  {
    grib_handle_delete(m_handle);
    m_handle = 0;
  }
}

te::rst::Raster* te::grib::Band::getRaster() const
{
  return m_raster;
}

te::grib::Band& te::grib::Band::operator=(const Band& rhs)
{
  throw Exception(TR_GRIB("Not implemented yet!"));
}

void te::grib::Band::getValue(unsigned int c, unsigned int r, double& value) const
{
  unsigned int pos = r * m_property->m_blkw + c;

  value = m_data[pos];
}

void te::grib::Band::setValue(unsigned int c, unsigned int r, const double value)
{
  unsigned int pos = r * m_property->m_blkw + c;

  m_data[pos] = value;
}

void te::grib::Band::getIValue(unsigned int c, unsigned int r, double& value) const
{
  throw Exception(TR_GRIB("Complex data not supported by GRIB format!"));
}

void te::grib::Band::setIValue(unsigned int c, unsigned int r, const double value)
{
  throw Exception(TR_GRIB("Complex data not supported by GRIB format!"));
}

void te::grib::Band::read(int x, int y, void* buffer) const
{
  assert(x == 0 && y == 0);

  memcpy(buffer, m_data, getBlockSize());
}

void* te::grib::Band::read(int x, int y)
{
  assert(x == 0 && y == 0);

  return m_data;
}

void te::grib::Band::write(int x, int y, void* buffer)
{
  assert(x == 0 && y == 0);

  memcpy(m_data, buffer, getBlockSize());
}

grib_handle* te::grib::Band::getHandle() const
{
  return m_handle;
}

long te::grib::Band::getLong(const char* key) const
{
  long val = 0;

  int err = grib_get_long(m_handle, key, &val);

  if(err)
    throw Exception((boost::format(TR_GRIB("Can not get key: %1%, due to: %2%.")) % key % getErrMsg(err)).str());

  return val;
}

double te::grib::Band::getDouble(const char* key) const
{
  double val = 0.0;

  int err = grib_get_double(m_handle, key, &val);

  if(err)
    throw Exception((boost::format(TR_GRIB("Can not get key: %1%, due to: %2%.")) % key % getErrMsg(err)).str());

  return val;
}

std::string te::grib::Band::getString(const char* key) const
{
  char val[1024];

  std::size_t length = 0;

  int err = grib_get_string(m_handle, key, val, &length);

  if(err)
    throw Exception((boost::format(TR_GRIB("Can not get key: %1%, due to: %2%.")) % key % getErrMsg(err)).str());

  return std::string(val);
}

std::string te::grib::Band::getErrMsg(int errCode)
{
  const char* errmsg = grib_get_error_message(errCode);

  assert(errmsg);

  return std::string(errmsg);
}

