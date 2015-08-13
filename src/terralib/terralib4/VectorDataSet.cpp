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
  \file terralib/terralib4/VectorDataSet.cpp

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
#include "../geometry/GeometryProperty.h"
#include "../geometry/WKBReader.h"
#include "VectorDataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "GeomReader.h"
#include "Utils.h"

// Terralib 4.x
#include <terralib4/kernel/TeAttribute.h>
#include <terralib4/kernel/TeDatabase.h>
#include <terralib4/kernel/TeLayer.h>

#include <terralib4/kernel/TeQuerierParams.h>

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/dynamic_bitset.hpp>
#include <boost/lexical_cast.hpp>

terralib4::VectorDataSet::VectorDataSet(TeLayer* layer)
  : m_dt(0),
    m_querier(0),
    m_i(-1),
    m_size(-1),
    m_nCols(-1),
    m_geomCol(-1),
    m_hasGeometry(false),
    m_layer(layer)
{
  TeQuerierParams params(true, true);
  params.setParams(m_layer);

  m_querier = new TeQuerier(params);

  m_querier->loadInstances();

  m_nCols = (int)m_querier->getAttrList().size();

  m_size = m_querier->numElemInstances();

  m_dt = new te::da::DataSetType(layer->name());

  for(int i = 0; i != m_nCols; ++i)
  {
    std::auto_ptr<te::dt::Property> p(terralib4::Convert2T5(m_querier->getAttrList()[i].rep_));

    m_dt->add(p.release());
  }

  if((m_layer->hasGeometry(TePOLYGONS) || 
      m_layer->hasGeometry(TeLINES) ||
      m_layer->hasGeometry(TePOINTS) ||
      m_layer->hasGeometry(TeNODES) ||
      m_layer->hasGeometry(TeCELLS)))
  {
    te::gm::GeometryProperty* gp = new te::gm::GeometryProperty("spatial_data");

    m_dt->add(gp);

    m_geomCol = m_nCols;

    ++m_nCols;
  }

}

terralib4::VectorDataSet::~VectorDataSet()
{
  delete m_querier;
  delete m_dt;
}

te::common::TraverseType terralib4::VectorDataSet::getTraverseType() const
{
  throw;
}

te::common::AccessPolicy terralib4::VectorDataSet::getAccessPolicy() const
{
  throw;
}

std::auto_ptr<te::gm::Envelope> terralib4::VectorDataSet::getExtent(std::size_t i)
{
  return terralib4::Convert2T5(m_layer->box());
}

std::size_t terralib4::VectorDataSet::getNumProperties() const
{
  return m_nCols;
}

int terralib4::VectorDataSet::getPropertyDataType(std::size_t i) const
{
  return m_dt->getProperty(i)->getType();
}

std::string terralib4::VectorDataSet::getPropertyName(std::size_t i) const
{
  /*TeAttributeList list = m_querier->getAttrList();
  return list[i].rep_.name_();*/
  throw;
}

te::common::CharEncoding terralib4::VectorDataSet::getPropertyCharEncoding(std::size_t i) const
{
  return te::common::UTF8;
}

std::string terralib4::VectorDataSet::getDatasetNameOfProperty(std::size_t i) const
{
  throw;
}

bool terralib4::VectorDataSet::isEmpty() const
{
  return !(size() > 0);
}

bool terralib4::VectorDataSet::isConnected() const
{
  throw;
}

std::size_t terralib4::VectorDataSet::size() const
{
  return m_size;
}

bool terralib4::VectorDataSet::moveNext()
{
  return m_querier->fetchInstance(m_instance);
}

bool terralib4::VectorDataSet::movePrevious()
{
  return false;
}

bool terralib4::VectorDataSet::moveBeforeFirst()
{
  return false;
}

bool terralib4::VectorDataSet::moveFirst()
{
  return false;
}

bool terralib4::VectorDataSet::moveLast()
{
  return false;
}

bool terralib4::VectorDataSet::move(std::size_t /*i*/)
{
  return false;
}

bool terralib4::VectorDataSet::isAtBegin() const
{
  return false;
}

bool terralib4::VectorDataSet::isBeforeBegin() const
{
  return false;
}

bool terralib4::VectorDataSet::isAtEnd() const
{
  return false;
}

bool terralib4::VectorDataSet::isAfterEnd() const
{
  return false;
}

char terralib4::VectorDataSet::getChar(std::size_t i) const
{
  throw;
}

unsigned char terralib4::VectorDataSet::getUChar(std::size_t i) const
{
  throw;
}

boost::int16_t terralib4::VectorDataSet::getInt16(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<boost::int16_t>(val);
}

boost::int32_t terralib4::VectorDataSet::getInt32(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<boost::int32_t>(val);
}

boost::int64_t terralib4::VectorDataSet::getInt64(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<boost::int64_t>(val);
}

bool terralib4::VectorDataSet::getBool(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);

  return boost::lexical_cast<bool>(val);
}

float terralib4::VectorDataSet::getFloat(std::size_t i) const
{
  return (float)getDouble(i);
}

double terralib4::VectorDataSet::getDouble(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);
  m_instance.getPropertyValue(val, ii);
  
  CheckDecimalSeparator(val);
  
  return boost::lexical_cast<double>(val);
}

std::string terralib4::VectorDataSet::getNumeric(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);

  m_instance.getPropertyValue(val, ii);

  CheckDecimalSeparator(val);

  return val;
}

std::string terralib4::VectorDataSet::getString(std::size_t i) const
{
  std::string val;
  int ii = static_cast<int>(i);

  m_instance.getPropertyValue(val, ii);

  return val;
}

std::auto_ptr<te::dt::ByteArray> terralib4::VectorDataSet::getByteArray(std::size_t i) const
{
  /*unsigned char * data;
  long size;
  m_result->getBlob(m_result->getAttribute(i).rep_.name_, data, size);

  return std::auto_ptr<te::dt::ByteArray>(new te::dt::ByteArray((char*)data, size));*/
  throw;
}

std::auto_ptr<te::gm::Geometry> terralib4::VectorDataSet::getGeometry(std::size_t i) const
{
  assert(i == m_geomCol);

  std::vector<TeGeometry*> geoms;
  m_instance.getGeometry(geoms);

  std::auto_ptr<te::gm::Geometry> t5g(terralib4::GeomReader::getGeometry(*geoms[0]));

  int srid = m_layer->projection()->epsgCode();
  if(srid == 4979)
    srid = 4326;

  t5g->setSRID(srid);

  return t5g;
}

std::auto_ptr<te::rst::Raster> terralib4::VectorDataSet::getRaster(std::size_t i) const
{
  throw;
}

std::auto_ptr<te::dt::DateTime> terralib4::VectorDataSet::getDateTime(std::size_t i) const
{
  //TeTime time = m_result->getDate(i);
  //time.
  throw;
}

std::auto_ptr<te::dt::Array> terralib4::VectorDataSet::getArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::Array>(0);
}

bool terralib4::VectorDataSet::isNull(std::size_t i) const
{
  if(i == m_geomCol) 
    return false;

  if(getPropertyDataType(i) == te::dt::STRING_TYPE)
    return false;

  std::string val;

  int ii = static_cast<int>(i);

  m_instance.getPropertyValue(val, ii);

  if(val.empty())
    return true;

  return false;
}
