/*  Copyright (C) 2009-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/FwDataSet.cpp

  \brief Implementation of a forward-only dataset for the TerraLib SQLite Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "FwDataSet.h"

te::sqlite::FwDataSet::FwDataSet()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

te::sqlite::FwDataSet::~FwDataSet()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

te::common::TraverseType te::sqlite::FwDataSet::getTraverseType() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

te::common::AccessPolicy te::sqlite::FwDataSet::getAccessPolicy() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::size_t te::sqlite::FwDataSet::getNumProperties() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

int te::sqlite::FwDataSet::getPropertyDataType(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::string te::sqlite::FwDataSet::getPropertyName(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::string te::sqlite::FwDataSet::getDatasetNameOfProperty(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isEmpty() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isConnected() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::size_t te::sqlite::FwDataSet::size() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::gm::Envelope> te::sqlite::FwDataSet::getExtent(std::size_t i)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::moveNext()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::movePrevious()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::moveBeforeFirst()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::moveFirst()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::moveLast()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::move(std::size_t i)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isAtBegin() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isBeforeBegin() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isAtEnd() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isAfterEnd() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

char te::sqlite::FwDataSet::getChar(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

unsigned char te::sqlite::FwDataSet::getUChar(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

boost::int16_t te::sqlite::FwDataSet::getInt16(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

boost::int32_t te::sqlite::FwDataSet::getInt32(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

boost::int64_t te::sqlite::FwDataSet::getInt64(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::getBool(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

float te::sqlite::FwDataSet::getFloat(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

double te::sqlite::FwDataSet::getDouble(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::string te::sqlite::FwDataSet::getNumeric(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::string te::sqlite::FwDataSet::getString(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::dt::ByteArray> te::sqlite::FwDataSet::getByteArray(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::gm::Geometry> te::sqlite::FwDataSet::getGeometry(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::rst::Raster> te::sqlite::FwDataSet::getRaster(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::dt::DateTime> te::sqlite::FwDataSet::getDateTime(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::dt::Array> te::sqlite::FwDataSet::getArray(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isNull(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

