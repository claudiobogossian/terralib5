/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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

  \brief Implementation of a DataSet for OGR data provider.  
 */

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Exception.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/ByteArray.h"
#include "../datatype/Date.h"
#include "../datatype/TimeDuration.h"
#include "../datatype/TimeInstant.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/WKBReader.h"
#include "../memory/DataSetItem.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "DataSet.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

te::ogr::DataSet::DataSet(DataSourceTransactor* trans, OGRLayer* layer, bool isOwner)
  : m_trans(trans),
    m_dt(0),
    m_ogrLayer(layer),
    m_currentFeature(0),
    m_i(-1),
    m_wkbArray(0),
    m_wkbArraySize(0),
    m_isOwner(isOwner)
{
  layer->ResetReading();
}

te::ogr::DataSet::~DataSet()
{
  OGRFeature::DestroyFeature(m_currentFeature);

  delete [] m_wkbArray;

  delete m_dt;

  if(m_isOwner)
    m_trans->getOGRDataSource()->ReleaseResultSet(m_ogrLayer);
}

te::da::DataSetType* te::ogr::DataSet::getType()
{
  if(m_dt == 0)
  {
    m_dt = Convert2TerraLib(m_ogrLayer->GetLayerDefn());
    m_dt->setFullLoaded(true);
  }
  
  return m_dt;
}

const te::da::DataSetType* te::ogr::DataSet::getType() const
{
  if(m_dt == 0)
  {
    m_dt = Convert2TerraLib(m_ogrLayer->GetLayerDefn());
    m_dt->setFullLoaded(true);
  }
  
  return m_dt;
}

te::da::DataSourceTransactor* te::ogr::DataSet::getTransactor() const
{
  return m_trans; 
}

void te::ogr::DataSet::loadTypeInfo()
{
  getType();
}

te::da::DataSet* te::ogr::DataSet::getParent() const
{
  return 0;
}

te::gm::Envelope* te::ogr::DataSet::getExtent(const te::dt::Property* /*p*/)
{
  OGREnvelope psExtent;
  m_ogrLayer->GetExtent(&psExtent);
  return Convert2TerraLib(&psExtent);
}

void te::ogr::DataSet::setFilter(te::dt::Property* /*p*/,
                                 const te::gm::Geometry* g,
                                 te::gm::SpatialRelation /*r*/)
{
  OGRGeometry* ogrg = Convert2OGR(g);
  m_ogrLayer->SetSpatialFilter(ogrg);
  OGRGeometryFactory::destroyGeometry(ogrg);
}

void te::ogr::DataSet::setFilter(te::dt::Property* /*p*/,
                                 const te::gm::Envelope* e,
                                 te::gm::SpatialRelation /*r*/)
{
  m_ogrLayer->SetSpatialFilterRect(e->m_llx, e->m_lly, e->m_urx, e->m_ury);
}

te::da::DataSetItem* te::ogr::DataSet::getItem() const
{
  return new te::mem::DataSetItem(this);
}

void te::ogr::DataSet::add(te::da::DataSetItem* /*item*/)
{
}

bool te::ogr::DataSet::isEmpty() const
{
  return !(size() > 0);
}

std::size_t te::ogr::DataSet::size() const
{
  return m_ogrLayer->GetFeatureCount();
}

bool te::ogr::DataSet::moveNext()
{
  OGRFeature::DestroyFeature(m_currentFeature);
  m_currentFeature = m_ogrLayer->GetNextFeature();
  m_i++;
  return m_currentFeature != 0;
}

bool te::ogr::DataSet::movePrevious()
{
  return move(m_i - 1);
}

bool te::ogr::DataSet::moveFirst()
{
  m_ogrLayer->ResetReading();
  m_i = -1;
  return moveNext();
}

bool te::ogr::DataSet::moveBeforeFirst()
{
  m_ogrLayer->ResetReading();
  m_i = -1;
  return true;
}

bool te::ogr::DataSet::moveLast()
{
  int lastPos = m_ogrLayer->GetFeatureCount() - 1;
  return move(lastPos);
}

bool te::ogr::DataSet::moveAfterLast()
{
  return move(m_ogrLayer->GetFeatureCount() + 1);
}

bool te::ogr::DataSet::move(std::size_t i)
{
  int p = static_cast<int>(i);
  OGRErr error = m_ogrLayer->SetNextByIndex(p);
  m_i = p - 1;
  if(error == OGRERR_NONE)
    return moveNext();
  else
    return false;
}

bool te::ogr::DataSet::isAtBegin() const
{
  return m_i == 0;
}

bool te::ogr::DataSet::isBeforeBegin() const
{
  return m_i < 0;
}

bool te::ogr::DataSet::isAtEnd() const
{
  return m_i >= (int)size();
}

bool te::ogr::DataSet::isAfterEnd() const
{
  return m_i > (int)size();
}

char te::ogr::DataSet::getChar(int /*i*/) const
{
  return '\0'; // Not supported by OGR library
}

char te::ogr::DataSet::getChar(const std::string& /*name*/) const
{
  return '\0'; // Not supported by OGR library
}

void te::ogr::DataSet::setChar(int /*i*/, char /*value*/) 
 {}

void te::ogr::DataSet::setChar(const std::string& /*name*/, char /*value*/) 
{}

unsigned char te::ogr::DataSet::getUChar(int /*i*/) const
{
  return '\0'; // Not supported by OGR library
}

unsigned char te::ogr::DataSet::getUChar(const std::string& /*name*/) const
{
  return '\0'; // Not supported by OGR library
}

void te::ogr::DataSet::setUChar(int /*i*/, unsigned char /*value*/) 
{}

void te::ogr::DataSet::setUChar(const std::string& /*name*/, unsigned char /*value*/) 
{}

boost::int16_t te::ogr::DataSet::getInt16(int /*i*/) const
{
  return 0; // Not supported by OGR library
}

boost::int16_t te::ogr::DataSet::getInt16(const std::string& /*name*/) const
{
  return 0; // Not supported by OGR library
}

void te::ogr::DataSet::setInt16(int /*i*/, boost::int16_t /*value*/) 
{}

void te::ogr::DataSet::setInt16(const std::string& /*name*/, boost::int16_t /*value*/) 
{}

boost::int32_t te::ogr::DataSet::getInt32(int i) const
{
  return m_currentFeature->GetFieldAsInteger(i);
}

boost::int32_t te::ogr::DataSet::getInt32(const std::string& name) const
{
  return m_currentFeature->GetFieldAsInteger(name.c_str());
}

void te::ogr::DataSet::setInt32(int /*i*/, boost::int32_t /*value*/) 
{}

void te::ogr::DataSet::setInt32(const std::string& /*name*/, boost::int32_t /*value*/) 
{}

boost::int64_t te::ogr::DataSet::getInt64(int /*i*/) const
{
  return 0; // Not supported by OGR library
}

boost::int64_t te::ogr::DataSet::getInt64(const std::string& /*name*/) const
{
  return 0; // Not supported by OGR library
}

void te::ogr::DataSet::setInt64(int /*i*/, boost::int64_t /*value*/) 
{}

void te::ogr::DataSet::setInt64(const std::string& /*name*/, boost::int64_t /*value*/) 
{}

bool te::ogr::DataSet::getBool(int /*i*/) const
{
  return false; // Not supported by OGR library
}

bool te::ogr::DataSet::getBool(const std::string& /*name*/) const
{
  return false; // Not supported by OGR library
}

void te::ogr::DataSet::setBool(int /*i*/, bool /*value*/) 
{}

void te::ogr::DataSet::setBool(const std::string& /*name*/, bool /*value*/) 
{}

float te::ogr::DataSet::getFloat(int /*i*/) const
{
  return 0.0; //Not supported by OGR library
}

float te::ogr::DataSet::getFloat(const std::string& /*name*/) const
{
  return 0.0; //Not supported by OGR library
}

void te::ogr::DataSet::setFloat(int /*i*/, float /*value*/) 
{}

void te::ogr::DataSet::setFloat(const std::string& /*name*/, float /*value*/) 
{}

double te::ogr::DataSet::getDouble(int i) const
{
  return m_currentFeature->GetFieldAsDouble(i);
}

double te::ogr::DataSet::getDouble(const std::string& name) const
{
  return m_currentFeature->GetFieldAsDouble(name.c_str());
}

void te::ogr::DataSet::setDouble(int /*i*/, double /*value*/) 
{}

void te::ogr::DataSet::setDouble(const std::string& /*name*/, double /*value*/) 
{}

std::string te::ogr::DataSet::getNumeric(int /*i*/) const
{
  return "";
}

std::string te::ogr::DataSet::getNumeric(const std::string& /*name*/) const
{
  return "";
}

void te::ogr::DataSet::setNumeric(int /*i*/, const std::string& /*value*/) 
{}

void te::ogr::DataSet::setNumeric(const std::string& /*name*/, const std::string& /*value*/) 
{}

std::string te::ogr::DataSet::getString(int i) const
{
  std::string value(m_currentFeature->GetFieldAsString(i));
  return value;
}

std::string te::ogr::DataSet::getString(const std::string& name) const
{
  std::string value(m_currentFeature->GetFieldAsString(name.c_str()));
  return value;
}

void te::ogr::DataSet::setString(int /*i*/, const std::string& /*value*/) 
{}

void te::ogr::DataSet::setString(const std::string& /*name*/, const std::string& /*value*/) 
{}

te::dt::ByteArray* te::ogr::DataSet::getByteArray(int i) const
{
  int size = 0;
  GByte* bytes = m_currentFeature->GetFieldAsBinary(i, &size);

  te::dt::ByteArray* byteArray = new te::dt::ByteArray(size);
  byteArray->copy((char*)bytes, size);
  
  return byteArray;
}

te::dt::ByteArray* te::ogr::DataSet::getByteArray(const std::string& name) const
{
  int i = m_currentFeature->GetFieldIndex(name.c_str());
  return getByteArray(i);
}

void te::ogr::DataSet::setByteArray(int /*i*/, const te::dt::ByteArray& /*value*/) 
{}

void te::ogr::DataSet::setByteArray(const std::string& /*name*/, const te::dt::ByteArray& /*value*/) 
{}

te::gm::Geometry* te::ogr::DataSet::getGeometry(int /*i*/) const
{
  return getGeometry();
}

te::gm::Geometry* te::ogr::DataSet::getGeometry(const std::string& /*name*/) const
{
  return getGeometry();
}

te::gm::Geometry* te::ogr::DataSet::getGeometry() const
{
  char* wkb = (char*)getWKB(0);
  return te::gm::WKBReader::read(wkb);
}

void te::ogr::DataSet::setGeometry(int /*i*/, const te::gm::Geometry& /*value*/) 
{}

void te::ogr::DataSet::setGeometry(const std::string& /*name*/, const te::gm::Geometry& /*value*/) 
{}

te::rst::Raster* te::ogr::DataSet::getRaster(int /*i*/) const
{
  return 0;
}

te::rst::Raster* te::ogr::DataSet::getRaster(const std::string& /*name*/) const
{
  return 0;
}

void te::ogr::DataSet::setRaster(int /*i*/, const te::rst::Raster& /*value*/) 
{}

void te::ogr::DataSet::setRaster(const std::string& /*name*/, const te::rst::Raster& /*value*/) 
{}

te::dt::DateTime* te::ogr::DataSet::getDateTime(int i) const
{
  if(m_dt==0)
    return 0;
  
  int pnYear,
      pnMonth,
      pnDay,
      pnHour,
      pnMinute,
      pnSecond,
      pnTZFlag;

  if(m_currentFeature->GetFieldAsDateTime(i, &pnYear, &pnMonth, &pnDay, &pnHour, &pnMinute, &pnSecond, &pnTZFlag) == FALSE)
    return new te::dt::Date;
  
  te::dt::Property* p = m_dt->getProperty(i);
  te::dt::DateTimeType subType = static_cast<te::dt::DateTimeProperty*>(p)->getSubType();
  te::dt::DateTime* dateTime = 0;

  if(subType==te::dt::DATE)
    dateTime = new te::dt::Date((unsigned short)pnYear, (unsigned short)pnMonth, (unsigned short)pnDay);
  else if(subType==te::dt::TIME_DURATION)
    dateTime = new te::dt::TimeDuration(pnHour, pnMinute, pnSecond);
  else if(subType==te::dt::TIME_INSTANT)
  {
      te::dt::Date d(pnYear, pnMonth, pnDay);
      te::dt::TimeDuration td(pnHour, pnMinute, pnSecond);
      dateTime = new te::dt::TimeInstant(d, td);
  }
  
  return dateTime;
}

te::dt::DateTime* te::ogr::DataSet::getDateTime(const std::string& name) const
{
  int i = m_currentFeature->GetFieldIndex(name.c_str());
  return getDateTime(i);
}

void te::ogr::DataSet::setDateTime(int /*i*/, const te::dt::DateTime& /*value*/) 
{}

void te::ogr::DataSet::setDateTime(const std::string& /*name*/, const te::dt::DateTime& /*value*/) 
{}

void te::ogr::DataSet::getArray(int /*i*/, std::vector<boost::int16_t>& /*a*/) const
{
  // Not supported by OGR library;
}

void te::ogr::DataSet::getArray(const std::string& /*name*/, std::vector<boost::int16_t>& /*a*/) const
{
  // Not supported by OGR library;
}

const unsigned char* te::ogr::DataSet::getWKB(int /*i*/) const
{
  // The OGR library supports only one geometry field
  OGRGeometry* geom = m_currentFeature->GetGeometryRef();

  if(geom == 0)
    return 0;

  int wkbSize = geom->WkbSize();
  
  if(wkbSize > m_wkbArraySize)
  {
    m_wkbArraySize = wkbSize;
    delete [] m_wkbArray;
    m_wkbArray = new unsigned char[m_wkbArraySize];
  }

  geom->exportToWkb(wkbNDR, m_wkbArray);

// 2.5D geometries have a special code!
  if(geom->getGeometryType() & 0x80000000)
  {
    unsigned int newcode = 0x0FFFFFFF & geom->getGeometryType();
    newcode += 1000;
    memcpy(m_wkbArray + 1, &newcode, sizeof(unsigned int));
  }

  return (const unsigned char*)m_wkbArray;
}

const unsigned char* te::ogr::DataSet::getWKB(const std::string& /*name*/) const
{
  return getWKB(0);
}

te::da::DataSet* te::ogr::DataSet::getDataSet(int /*i*/)
{
  return 0;
}

void te::ogr::DataSet::setDataSet(int /*i*/, const te::da::DataSet& /*value*/)
{}

void te::ogr::DataSet::setValue(int /*i*/, te::dt::AbstractData* /*ad*/)
{
  return;
}
        
bool te::ogr::DataSet::isNull(int i) const
{
  if(m_currentFeature->IsFieldSet(i)==0)
    return true;
  return false;
}

