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
  \file terralib/terralib4/RasterDataSet.cpp

  \brief Implementation of a dataset for the TerraLib 4 driver.
*/

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Globals.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/Array.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/SimpleData.h"
#include "../datatype/TimeInstant.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/WKBReader.h"
#include "../raster/Raster.h"
#include "RasterDataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "GeomReader.h"
#include "Utils.h"

// Terralib 4.x
#include <terralib/kernel/TeAttribute.h>
#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeLayer.h>

#include <terralib/kernel/TeQuerierParams.h>

// STL
#include <memory>

// Boost
#include <boost/dynamic_bitset.hpp>
#include <boost/lexical_cast.hpp>

terralib4::RasterDataSet::RasterDataSet(TeRaster* raster)
  : m_raster(raster)
{
}

terralib4::RasterDataSet::~RasterDataSet()
{
}

te::common::TraverseType terralib4::RasterDataSet::getTraverseType() const
{
  throw;
}

te::common::AccessPolicy terralib4::RasterDataSet::getAccessPolicy() const
{
  throw;
}

std::auto_ptr<te::gm::Envelope> terralib4::RasterDataSet::getExtent(std::size_t i)
{
  return std::auto_ptr<te::gm::Envelope>(0);
}

std::size_t terralib4::RasterDataSet::getNumProperties() const
{
  return 0;
}

int terralib4::RasterDataSet::getPropertyDataType(std::size_t i) const
{
  return te::dt::RASTER_TYPE;
}

std::string terralib4::RasterDataSet::getPropertyName(std::size_t i) const
{
  return "";
}

std::string terralib4::RasterDataSet::getDatasetNameOfProperty(std::size_t i) const
{
  return "";
}

bool terralib4::RasterDataSet::isEmpty() const
{
  return false;
}

bool terralib4::RasterDataSet::isConnected() const
{
  return true;
}

std::size_t terralib4::RasterDataSet::size() const
{
  return 0;
}

bool terralib4::RasterDataSet::moveNext()
{
  return false;
}

bool terralib4::RasterDataSet::movePrevious()
{
  return false;
}

bool terralib4::RasterDataSet::moveBeforeFirst()
{
  return false;
}

bool terralib4::RasterDataSet::moveFirst()
{
  return false;
}

bool terralib4::RasterDataSet::moveLast()
{
  return false;
}

bool terralib4::RasterDataSet::move(std::size_t /*i*/)
{
  return false;
}

bool terralib4::RasterDataSet::isAtBegin() const
{
  return false;
}

bool terralib4::RasterDataSet::isBeforeBegin() const
{
  return false;
}

bool terralib4::RasterDataSet::isAtEnd() const
{
  return false;
}

bool terralib4::RasterDataSet::isAfterEnd() const
{
  return false;
}

char terralib4::RasterDataSet::getChar(std::size_t i) const
{
  return '\0';
}

unsigned char terralib4::RasterDataSet::getUChar(std::size_t i) const
{
  return '\0';
}

boost::int16_t terralib4::RasterDataSet::getInt16(std::size_t i) const
{
  return 0;
}

boost::int32_t terralib4::RasterDataSet::getInt32(std::size_t i) const
{
  return 0;
}

boost::int64_t terralib4::RasterDataSet::getInt64(std::size_t i) const
{
  return 0;
}

bool terralib4::RasterDataSet::getBool(std::size_t i) const
{
  return 0;
}

float terralib4::RasterDataSet::getFloat(std::size_t i) const
{
  return 0;
}

double terralib4::RasterDataSet::getDouble(std::size_t i) const
{
  return 0;
}

std::string terralib4::RasterDataSet::getNumeric(std::size_t i) const
{
  return "";
}

std::string terralib4::RasterDataSet::getString(std::size_t i) const
{
  return "";
}

std::auto_ptr<te::dt::ByteArray> terralib4::RasterDataSet::getByteArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::ByteArray>(0);
}

std::auto_ptr<te::gm::Geometry> terralib4::RasterDataSet::getGeometry(std::size_t i) const
{
  return std::auto_ptr<te::gm::Geometry>(0);
}

std::auto_ptr<te::rst::Raster> terralib4::RasterDataSet::getRaster(std::size_t i) const
{
  return std::auto_ptr<te::rst::Raster>(0);
}

std::auto_ptr<te::dt::DateTime> terralib4::RasterDataSet::getDateTime(std::size_t i) const
{
  return std::auto_ptr<te::dt::DateTime>(0);
}

std::auto_ptr<te::dt::Array> terralib4::RasterDataSet::getArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::Array>(0);
}

bool terralib4::RasterDataSet::isNull(std::size_t /*i*/) const
{
  return false;
}