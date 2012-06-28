/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mongodb/FwDataSet.cpp

  \brief A forward-only dataset implementation for MongoDB.
*/

// TerraLib
#include "../common/Translator.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "FwDataSet.h"

// MongoDB
#include <mongo/client/dbclient.h>

te::mongodb::FwDataSet::FwDataSet(mongo::DBClientCursor* cursor)
  : m_cursor(cursor)
{
}

te::mongodb::FwDataSet::~FwDataSet()
{
  delete m_cursor;
}

te::common::TraverseType te::mongodb::FwDataSet::getTraverseType() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::common::AccessPolicy te::mongodb::FwDataSet::getAccessPolicy() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSetType* te::mongodb::FwDataSet::getType()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

const te::da::DataSetType* te::mongodb::FwDataSet::getType() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::mongodb::FwDataSet::getTransactor() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::loadTypeInfo()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSet* te::mongodb::FwDataSet::getParent() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::gm::Envelope* te::mongodb::FwDataSet::getExtent(const te::dt::Property* p)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setFilter(te::dt::Property* p,
                                       const te::gm::Geometry* g,
                                       te::gm::SpatialRelation r)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}
 
void te::mongodb::FwDataSet::setFilter(te::dt::Property* p,
                                       const te::gm::Envelope* e,
                                       te::gm::SpatialRelation r)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSetItem* te::mongodb::FwDataSet::getItem() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::add(te::da::DataSetItem* item)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::isEmpty() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

std::size_t te::mongodb::FwDataSet::size() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::moveNext()
{
  return m_cursor->more();
}

bool te::mongodb::FwDataSet::movePrevious()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::moveFirst()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::moveBeforeFirst()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::moveLast()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::moveAfterLast()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::move(std::size_t i)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::isAtBegin() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::isBeforeBegin() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::isAtEnd() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::isAfterEnd() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

char te::mongodb::FwDataSet::getChar(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setChar(int i, char value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

unsigned char te::mongodb::FwDataSet::getUChar(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setUChar(int i, unsigned char value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

boost::int16_t te::mongodb::FwDataSet::getInt16(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setInt16(int i, boost::int16_t value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

boost::int32_t te::mongodb::FwDataSet::getInt32(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setInt32(int i, boost::int32_t value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

boost::int64_t te::mongodb::FwDataSet::getInt64(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setInt64(int i, boost::int64_t value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::getBool(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setBool(int i, bool value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

float te::mongodb::FwDataSet::getFloat(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setFloat(int i, float value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

double te::mongodb::FwDataSet::getDouble(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setDouble(int i, double value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

std::string te::mongodb::FwDataSet::getNumeric(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setNumeric(int i, const std::string& value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

std::string te::mongodb::FwDataSet::getString(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setString(int i, const std::string& value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::dt::ByteArray* te::mongodb::FwDataSet::getByteArray(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setByteArray(int i, const te::dt::ByteArray& value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::gm::Geometry* te::mongodb::FwDataSet::getGeometry(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setGeometry(int i, const te::gm::Geometry& value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::rst::Raster* te::mongodb::FwDataSet::getRaster(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setRaster(int i, const te::rst::Raster& value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::dt::DateTime* te::mongodb::FwDataSet::getDateTime(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setDateTime(int i, const te::dt::DateTime& value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::getArray(int i, std::vector<boost::int16_t>& a) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

const unsigned char* te::mongodb::FwDataSet::getWKB(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSet* te::mongodb::FwDataSet::getDataSet(int i)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setDataSet(int i, const te::da::DataSet& value)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::FwDataSet::setValue(int i, te::dt::AbstractData* ad)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::FwDataSet::isNull(int i) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

