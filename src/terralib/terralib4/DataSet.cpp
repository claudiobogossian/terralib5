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

terralib4::DataSet::DataSet(TeLayer* layer)
  : m_querier(0),
    m_i(-1),
    m_size(-1),
    m_nCols(-1),
    m_hasGeometry(false),
    m_layer(layer)
{
  TeQuerierParams params(true, true);
  params.setParams(m_layer);

  m_querier = new TeQuerier(params);

  m_querier->loadInstances();

  m_nCols = m_querier->getAttrList().size();

}

terralib4::DataSet::~DataSet()
{
  delete m_querier;
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
  return terralib4::Convert2T5(m_layer->box());
}

std::size_t terralib4::DataSet::getNumProperties() const
{
  return m_nCols;
}

int terralib4::DataSet::getPropertyDataType(std::size_t i) const
{
  if(i >= m_nCols && (m_layer->hasGeometry(TePOLYGONS) || 
    m_layer->hasGeometry(TeLINES) ||
    m_layer->hasGeometry(TePOINTS) ||
    m_layer->hasGeometry(TeNODES) ||
    m_layer->hasGeometry(TeCELLS)))
  {
    return te::dt::GEOMETRY_TYPE;
  }

  return terralib4::Convert2T5(m_querier->getAttrList()[i].rep_.type_);
}

std::string terralib4::DataSet::getPropertyName(std::size_t i) const
{
  /*TeAttributeList list = m_querier->getAttrList();
  return list[i].rep_.name_();*/
  throw;
}

std::string terralib4::DataSet::getDatasetNameOfProperty(std::size_t i) const
{
  throw;
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
  return m_querier->fetchInstance(m_instance);
}

bool terralib4::DataSet::movePrevious()
{
  return false;
}

bool terralib4::DataSet::moveBeforeFirst()
{
  return false;
}

bool terralib4::DataSet::moveFirst()
{
  return false;
}

bool terralib4::DataSet::moveLast()
{
  return false;
}

bool terralib4::DataSet::move(std::size_t /*i*/)
{
  return false;
}

bool terralib4::DataSet::isAtBegin() const
{
  return false;
}

bool terralib4::DataSet::isBeforeBegin() const
{
  return false;
}

bool terralib4::DataSet::isAtEnd() const
{
  return false;
}

bool terralib4::DataSet::isAfterEnd() const
{
  return false;
}

char terralib4::DataSet::getChar(std::size_t i) const
{
  return '\0';
}

unsigned char terralib4::DataSet::getUChar(std::size_t i) const
{
  return '\0';
}

boost::int16_t terralib4::DataSet::getInt16(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<boost::int16_t>(val);
}

boost::int32_t terralib4::DataSet::getInt32(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<boost::int32_t>(val);
}

boost::int64_t terralib4::DataSet::getInt64(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<boost::int64_t>(val);
}

bool terralib4::DataSet::getBool(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<bool>(val);
}

float terralib4::DataSet::getFloat(std::size_t i) const
{
  return (float)getDouble(i);
}

double terralib4::DataSet::getDouble(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<double>(val);
}

std::string terralib4::DataSet::getNumeric(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);

  m_instance.getPropertyValue(val, ii);

  return val;
}

std::string terralib4::DataSet::getString(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);

  m_instance.getPropertyValue(val, ii);

  return val;
}

std::auto_ptr<te::dt::ByteArray> terralib4::DataSet::getByteArray(std::size_t i) const
{
  /*unsigned char * data;
  long size;
  m_result->getBlob(m_result->getAttribute(i).rep_.name_, data, size);

  return std::auto_ptr<te::dt::ByteArray>(new te::dt::ByteArray((char*)data, size));*/
  throw;
}

std::auto_ptr<te::gm::Geometry> terralib4::DataSet::getGeometry(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);

  std::vector<TeGeometry*> geoms;
  m_instance.getGeometry(geoms);


  return terralib4::GeomReader::getGeometry(*geoms[0]);
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

bool terralib4::DataSet::isNull(std::size_t /*i*/) const
{
  return false;
}