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
  \file terralib/ado2/DataSet.cpp

  \brief Implementation of a dataset for the ADO driver.
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
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Utils.h"

// Terralib 4.x
#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeLayer.h>


// STL
#include <memory>

// Boost
#include <boost/dynamic_bitset.hpp>
#include <boost/lexical_cast.hpp>

terralib4::DataSet::DataSet(TeLayer* layer)
  : m_i(-1),
    m_size(-1),
    m_layer(layer),
    m_result(0)
{
  //m_size = m_result->numRows();
}

terralib4::DataSet::~DataSet()
{
}

te::common::TraverseType terralib4::DataSet::getTraverseType() const
{
  throw;
}

te::common::AccessPolicy terralib4::DataSet::getAccessPolicy() const
{
  throw;
}

std::auto_ptr<te::gm::Envelope> terralib4::DataSet::getExtent(std::size_t i)
{
  TeLayer lay;
  m_result->getLayer(lay);

  return terralib4::Convert2T5(lay.box());
}

std::size_t terralib4::DataSet::getNumProperties() const
{
  return m_result->numFields();
}

int terralib4::DataSet::getPropertyDataType(std::size_t i) const
{
  return terralib4::Convert2T5(m_result->getAttribute(i).rep_.type_);
}

std::string terralib4::DataSet::getPropertyName(std::size_t i) const
{
  return m_result->getAttribute(i).rep_.name_;
}

std::string terralib4::DataSet::getDatasetNameOfProperty(std::size_t i) const
{
  return m_result->getAttribute(i).origin_;
}

bool terralib4::DataSet::isEmpty() const
{
  return !(size() > 0);
}

bool terralib4::DataSet::isConnected() const
{
  throw;
}

std::size_t terralib4::DataSet::size() const
{
  return m_size;
}

bool terralib4::DataSet::moveNext()
{
  if(m_i < 0)
    return true;

  return m_result->fetchRow();
}

bool terralib4::DataSet::movePrevious()
{
  if(m_i == -1)
    return false;

  if(m_i == 0)
    return true;

  return m_result->fetchRow(m_i);
}

bool terralib4::DataSet::moveBeforeFirst()
{
  m_i = -1;
  return m_result->fetchRow(0);
}

bool terralib4::DataSet::moveFirst()
{
  m_i = 0;
  return m_result->fetchRow(0);
}

bool terralib4::DataSet::moveLast()
{
  m_i = m_result->numRows()-1;
  return m_result->fetchRow(m_result->numRows()-1);
}

bool terralib4::DataSet::move(std::size_t i)
{
  m_i = i;
  return m_result->fetchRow(i);
}

bool terralib4::DataSet::isAtBegin() const
{
  return m_i == 0;
}

bool terralib4::DataSet::isBeforeBegin() const
{
  return m_i == -1;
}

bool terralib4::DataSet::isAtEnd() const
{
  return m_i == m_result->numRows()-1;
}

bool terralib4::DataSet::isAfterEnd() const
{
  return m_i == m_result->numRows();
}

char terralib4::DataSet::getChar(std::size_t i) const
{
  return *m_result->getData(i);
}

unsigned char terralib4::DataSet::getUChar(std::size_t i) const
{
  return getChar(i);
}

boost::int16_t terralib4::DataSet::getInt16(std::size_t i) const
{
  return m_result->getInt(i);
}

boost::int32_t terralib4::DataSet::getInt32(std::size_t i) const
{
  return getInt16(i);
}

boost::int64_t terralib4::DataSet::getInt64(std::size_t i) const
{
  return getInt16(i);
}

bool terralib4::DataSet::getBool(std::size_t i) const
{
  return m_result->getBool(i);
}

float terralib4::DataSet::getFloat(std::size_t i) const
{
  return (float)m_result->getDouble(i);
}

double terralib4::DataSet::getDouble(std::size_t i) const
{
  return m_result->getDouble(i);
}

std::string terralib4::DataSet::getNumeric(std::size_t i) const
{
  return m_result->getData(i);
}

std::string terralib4::DataSet::getString(std::size_t i) const
{
  return m_result->getData(i);
}

std::auto_ptr<te::dt::ByteArray> terralib4::DataSet::getByteArray(std::size_t i) const
{
  unsigned char * data;
  long size;
  m_result->getBlob(m_result->getAttribute(i).rep_.name_, data, size);

  return std::auto_ptr<te::dt::ByteArray>(new te::dt::ByteArray((char*)data, size));
}

std::auto_ptr<te::gm::Geometry> terralib4::DataSet::getGeometry(std::size_t i) const
{
  std::auto_ptr<te::dt::ByteArray> ba(getByteArray(i));

  std::auto_ptr<te::gm::Geometry> geom(te::gm::WKBReader::read(ba->getData()));

  std::size_t wkb_size = geom->getWkbSize();

  if(ba->bytesUsed() - wkb_size >= 4)
    geom->setSRID(*((int*)(ba->getData() + wkb_size)));

  return geom;
}

std::auto_ptr<te::rst::Raster> terralib4::DataSet::getRaster(std::size_t i) const
{
  throw;
}

std::auto_ptr<te::dt::DateTime> terralib4::DataSet::getDateTime(std::size_t i) const
{
  //TeTime time = m_result->getDate(i);
  //time.
  throw;
}

std::auto_ptr<te::dt::Array> terralib4::DataSet::getArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::Array>(0);
}

bool terralib4::DataSet::isNull(std::size_t i) const
{
  throw;
}