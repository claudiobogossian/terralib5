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
  \file terralib/terralib4/TableDataSet.cpp

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
#include "../datatype/DateTimeInstant.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/SimpleData.h"
#include "../datatype/TimeInstant.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/WKBReader.h"
#include "TableDataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "GeomReader.h"
#include "Utils.h"

// Terralib 4.x
#include <terralib/kernel/TeAttribute.h>
#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeLayer.h>

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/lexical_cast.hpp>

terralib4::TableDataSet::TableDataSet( TeDatabase* db, TeTable table)
  : m_dt(0),
    m_table(table),
    m_i(-1),
    m_size(-1),
    m_nCols(-1),
    m_data(0),
    m_db(db)
{

  m_nCols = m_table.attributeList().size();

  m_dt = new te::da::DataSetType(m_table.name());

  for(int i = 0; i != m_nCols; ++i)
  {
    TeAttributeList attrList = table.attributeList();
    TeAttribute attr = attrList[i];

    std::auto_ptr<te::dt::Property> p(terralib4::Convert2T5(attr.rep_));

    m_dt->add(p.release());
  }

  m_data = new te::mem::DataSet(m_dt);

  TeDatabasePortal* portal = m_db->getPortal();
  portal->query("SELECT * FROM " + m_table.name());

  m_size = portal->numRows();

  while(portal->fetchRow())
  {
    te::mem::DataSetItem* item = new te::mem::DataSetItem(m_data);

    for(int i = 0; i != m_nCols; ++i)
    {
      int pType = m_table.attributeList()[i].rep_.type_;

      switch(pType)
      {
        case TeSTRING:
          item->setString(i, portal->getData(i));
          break;

        case TeREAL:
          item->setNumeric(i, portal->getData(i));
          break;

        case TeINT:
          item->setInt32(i, portal->getInt(i));
          break;

        case TeDATETIME:
        {
          TeTime date = portal->getDate(i);

          int y = date.year();
          int m = date.month();
          int d = date.day();
          int h = date.hour();
          int min = date.minute();
          int s = date.second();

          te::dt::Property* p = m_dt->getProperty(i);
          te::dt::DateTimeType subType = static_cast<te::dt::DateTimeProperty*>(p)->getSubType();
          te::dt::DateTime* dateTime = 0;

          if(subType == te::dt::DATE)
          {
            dateTime = new te::dt::Date((unsigned short)y, (unsigned short)m, (unsigned short)d);
          }
          else if(subType == te::dt::TIME_DURATION)
          {
            dateTime = new te::dt::TimeDuration(h, min, s);
          }
          else if(subType == te::dt::TIME_INSTANT)
          {
            te::dt::Date dt(y, m, d);
            te::dt::TimeDuration td(h, min, s);
            dateTime = new te::dt::TimeInstant(dt, td);
          }

          item->setDateTime(i, dateTime);

        }

        case TeBLOB:
        {
          unsigned char* data;
          long size;
          portal->getBlob(m_dt->getProperty(i)->getName(), data, size);
          item->setByteArray(i, new te::dt::ByteArray((char*)data, (std::size_t)size));
          break;
        }

        case TeCHARACTER:
          item->setChar(i, (char)portal->getData(i));
          break;

        case TeUNSIGNEDINT:
          item->setInt32(i, portal->getInt(i));
          break;

        case TeBOOLEAN:
          item->setBool(i, portal->getBool(i));
          break;

        case TeUNKNOWN:
        case TeOBJECT:
        case TeTEXTTYPE:
        case TeTEXTSETTYPE:
        default:
          throw;
      }
    }

      ((te::mem::DataSet*)m_data)->add(item);
  }
}

terralib4::TableDataSet::~TableDataSet()
{
  delete m_dt;
}

te::common::TraverseType terralib4::TableDataSet::getTraverseType() const
{
  throw;
}

te::common::AccessPolicy terralib4::TableDataSet::getAccessPolicy() const
{
  throw;
}

std::auto_ptr<te::gm::Envelope> terralib4::TableDataSet::getExtent(std::size_t i)
{
  throw;
}

std::size_t terralib4::TableDataSet::getNumProperties() const
{
  return m_nCols;
}

int terralib4::TableDataSet::getPropertyDataType(std::size_t i) const
{
  return m_dt->getProperty(i)->getType();
}

std::string terralib4::TableDataSet::getPropertyName(std::size_t i) const
{
  //return m_dt->getProperty(i)->getName();
  throw;
}

te::common::CharEncoding terralib4::TableDataSet::getPropertyCharEncoding(std::size_t i) const
{
  return te::common::UNKNOWN_CHAR_ENCODING; // TODO
}

std::string terralib4::TableDataSet::getDatasetNameOfProperty(std::size_t i) const
{
  throw;
}

bool terralib4::TableDataSet::isEmpty() const
{
  return !(size() > 0);
}

bool terralib4::TableDataSet::isConnected() const
{
  throw;
}

std::size_t terralib4::TableDataSet::size() const
{
  return m_size;
}

bool terralib4::TableDataSet::moveNext()
{
  return m_data->moveNext();
}

bool terralib4::TableDataSet::movePrevious()
{
  return false;
}

bool terralib4::TableDataSet::moveBeforeFirst()
{
  return false;
}

bool terralib4::TableDataSet::moveFirst()
{
  return false;
}

bool terralib4::TableDataSet::moveLast()
{
  return false;
}

bool terralib4::TableDataSet::move(std::size_t /*i*/)
{
  return false;
}

bool terralib4::TableDataSet::isAtBegin() const
{
  return false;
}

bool terralib4::TableDataSet::isBeforeBegin() const
{
  return false;
}

bool terralib4::TableDataSet::isAtEnd() const
{
  return false;
}

bool terralib4::TableDataSet::isAfterEnd() const
{
  return false;
}

char terralib4::TableDataSet::getChar(std::size_t i) const
{
  throw;
}

unsigned char terralib4::TableDataSet::getUChar(std::size_t i) const
{
  throw;
}

boost::int16_t terralib4::TableDataSet::getInt16(std::size_t i) const
{
  return m_data->getInt16(i);
}

boost::int32_t terralib4::TableDataSet::getInt32(std::size_t i) const
{
  return m_data->getInt32(i);
}

boost::int64_t terralib4::TableDataSet::getInt64(std::size_t i) const
{
  return m_data->getInt64(i);
}

bool terralib4::TableDataSet::getBool(std::size_t i) const
{
  return m_portal->getBool(i);
}

float terralib4::TableDataSet::getFloat(std::size_t i) const
{
  return m_data->getFloat(i);
}

double terralib4::TableDataSet::getDouble(std::size_t i) const
{
  return m_data->getDouble(i);
}

std::string terralib4::TableDataSet::getNumeric(std::size_t i) const
{
  return m_data->getNumeric(i);
}

std::string terralib4::TableDataSet::getString(std::size_t i) const
{
  return m_data->getString(i);
}

std::auto_ptr<te::dt::ByteArray> terralib4::TableDataSet::getByteArray(std::size_t i) const
{
  /*unsigned char * data;
  long size;
  m_result->getBlob(m_result->getAttribute(i).rep_.name_, data, size);

  return std::auto_ptr<te::dt::ByteArray>(new te::dt::ByteArray((char*)data, size));*/
  throw;
}

std::auto_ptr<te::gm::Geometry> terralib4::TableDataSet::getGeometry(std::size_t i) const
{
  throw;
}

std::auto_ptr<te::rst::Raster> terralib4::TableDataSet::getRaster(std::size_t i) const
{
  throw;
}

std::auto_ptr<te::dt::DateTime> terralib4::TableDataSet::getDateTime(std::size_t i) const
{
  //TeTime time = m_result->getDate(i);
  //time.
  throw;
}

std::auto_ptr<te::dt::Array> terralib4::TableDataSet::getArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::Array>(0);
}

bool terralib4::TableDataSet::isNull(std::size_t i) const
{

  return false;
  std::string val = m_portal->getData(i);

  if(val.empty())
    return true;

  return false;
}
