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
#include "Raster.h"
#include "Utils.h"

// Terralib 4.x
#include <terralib4/kernel/TeAttribute.h>
#include <terralib4/kernel/TeDatabase.h>
#include <terralib4/kernel/TeLayer.h>
#include <terralib4/kernel/TeQuerierParams.h>

// STL
#include <memory>

// Boost
#include <boost/dynamic_bitset.hpp>
#include <boost/lexical_cast.hpp>

terralib4::RasterDataSet::RasterDataSet(TeRaster* raster)
  : m_raster(raster), m_i(-1)
{
}

terralib4::RasterDataSet::~RasterDataSet()
{
}

te::common::TraverseType terralib4::RasterDataSet::getTraverseType() const
{
  return te::common::FORWARDONLY;
}

te::common::AccessPolicy terralib4::RasterDataSet::getAccessPolicy() const
{
  return te::common::RAccess;
}

std::auto_ptr<te::gm::Envelope> terralib4::RasterDataSet::getExtent(std::size_t /*i*/)
{
  return Convert2T5(m_raster->box());
}

std::size_t terralib4::RasterDataSet::getNumProperties() const
{
  return 1;
}

int terralib4::RasterDataSet::getPropertyDataType(std::size_t /*i*/) const
{
  return te::dt::RASTER_TYPE;
}

std::string terralib4::RasterDataSet::getPropertyName(std::size_t /*i*/) const
{
  return "raster";
}

te::common::CharEncoding terralib4::RasterDataSet::getPropertyCharEncoding(std::size_t i) const
{
  return te::common::UNKNOWN_CHAR_ENCODING;
}

std::string terralib4::RasterDataSet::getDatasetNameOfProperty(std::size_t /*i*/) const
{
  return "raster";
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
  return 1;
}

bool terralib4::RasterDataSet::moveNext()
{
  return (++m_i) == 0;
}

bool terralib4::RasterDataSet::movePrevious()
{
  return false;
}

bool terralib4::RasterDataSet::moveBeforeFirst()
{
  m_i = -1;
  return true;
}

bool terralib4::RasterDataSet::moveFirst()
{
  m_i = 0;
  return true;
}

bool terralib4::RasterDataSet::moveLast()
{
  m_i = 0;
  return true;
}

bool terralib4::RasterDataSet::move(std::size_t /*i*/)
{
  return false;
}

bool terralib4::RasterDataSet::isAtBegin() const
{
  return m_i == 0;
}

bool terralib4::RasterDataSet::isBeforeBegin() const
{
  return m_i == -1;
}

bool terralib4::RasterDataSet::isAtEnd() const
{
  return m_i == 0;
}

bool terralib4::RasterDataSet::isAfterEnd() const
{
  return m_i > 0;
}

char terralib4::RasterDataSet::getChar(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

unsigned char terralib4::RasterDataSet::getUChar(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

boost::int16_t terralib4::RasterDataSet::getInt16(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

boost::int32_t terralib4::RasterDataSet::getInt32(std::size_t i) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

boost::int64_t terralib4::RasterDataSet::getInt64(std::size_t i) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

bool terralib4::RasterDataSet::getBool(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

float terralib4::RasterDataSet::getFloat(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

double terralib4::RasterDataSet::getDouble(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

std::string terralib4::RasterDataSet::getNumeric(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

std::string terralib4::RasterDataSet::getString(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

std::auto_ptr<te::dt::ByteArray> terralib4::RasterDataSet::getByteArray(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

std::auto_ptr<te::gm::Geometry> terralib4::RasterDataSet::getGeometry(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

std::auto_ptr<te::rst::Raster> terralib4::RasterDataSet::getRaster(std::size_t i) const
{
  return std::auto_ptr<te::rst::Raster>(new Raster(m_raster));
}

std::auto_ptr<te::dt::DateTime> terralib4::RasterDataSet::getDateTime(std::size_t /*i*/) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

std::auto_ptr<te::dt::Array> terralib4::RasterDataSet::getArray(std::size_t i) const
{
  throw Exception(TE_TR("Not supported by this type of dataset!"));
}

bool terralib4::RasterDataSet::isNull(std::size_t /*i*/) const
{
  return false;
}

