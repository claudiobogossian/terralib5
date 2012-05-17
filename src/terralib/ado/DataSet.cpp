/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSet.cpp

  \brief Implementation of the DataSet class to ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "DataSet.h"
#include "Exception.h"

te::common::TraverseType te::ado::DataSet::getTraverseType() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::common::AccessPolicy te::ado::DataSet::getAccessPolicy() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSetType* te::ado::DataSet::getType()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

const te::da::DataSetType* te::ado::DataSet::getType() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::ado::DataSet::getTransactor() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::loadTypeInfo()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSet* te::ado::DataSet::getParent() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::gm::Envelope* te::ado::DataSet::getExtent(const te::dt::Property* p)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setFilter(te::dt::Property* p,
                                      const te::gm::Geometry* g,
                                      te::gm::SpatialRelation r)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}
 
void te::ado::DataSet::setFilter(te::dt::Property* p,
                                      const te::gm::Envelope* e,
                                      te::gm::SpatialRelation r)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSetItem* te::ado::DataSet::getItem() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::add(te::da::DataSetItem* item)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::isEmpty() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::size_t te::ado::DataSet::size() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::moveNext()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::movePrevious()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::moveFirst()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::moveBeforeFirst()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::moveLast()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::moveAfterLast()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::move(std::size_t i)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::isAtBegin() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::isBeforeBegin() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::isAtEnd() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::isAfterEnd() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

char te::ado::DataSet::getChar(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setChar(int i, char value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

unsigned char te::ado::DataSet::getUChar(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setUChar(int i, unsigned char value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

boost::int16_t te::ado::DataSet::getInt16(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setInt16(int i, boost::int16_t value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

boost::int32_t te::ado::DataSet::getInt32(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setInt32(int i, boost::int32_t value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

boost::int64_t te::ado::DataSet::getInt64(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setInt64(int i, boost::int64_t value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::getBool(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setBool(int i, bool value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

float te::ado::DataSet::getFloat(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setFloat(int i, float value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

double te::ado::DataSet::getDouble(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setDouble(int i, double value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::string te::ado::DataSet::getNumeric(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setNumeric(int i, const std::string& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::string te::ado::DataSet::getString(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setString(int i, const std::string& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::dt::ByteArray* te::ado::DataSet::getByteArray(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setByteArray(int i, const te::dt::ByteArray& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::gm::Geometry* te::ado::DataSet::getGeometry(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setGeometry(int i, const te::gm::Geometry& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::rst::Raster* te::ado::DataSet::getRaster(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setRaster(int i, const te::rst::Raster& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::dt::DateTime* te::ado::DataSet::getDateTime(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setDateTime(int i, const te::dt::DateTime& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::getArray(int i, std::vector<boost::int16_t>& a) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

const unsigned char* te::ado::DataSet::getWKB(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSet* te::ado::DataSet::getDataSet(int i)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setDataSet(int i, const te::da::DataSet& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setValue(int i, te::dt::AbstractData* ad)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::isNull(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

