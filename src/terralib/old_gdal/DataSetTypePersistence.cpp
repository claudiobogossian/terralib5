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
  \file terralib/gdal/DataSetTypePersistence.cpp

  \brief DataSetTypePersistence implementation for GDAL data provider.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/Enums.h"
#include "../geometry/Envelope.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "../raster/Band.h"
#include "DataSource.h"
#include "DataSetTypePersistence.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Utils.h"

// Boost
#include <boost/filesystem.hpp>

// STL
#include <cassert>
#include <limits> 

// GDAL
#include <gdal_priv.h>
#include <ogr_spatialref.h>

te::gdal::DataSetTypePersistence::DataSetTypePersistence(te::gdal::DataSourceTransactor* t)
  : m_t(t)
{
  assert(m_t);
}

te::gdal::DataSetTypePersistence::~DataSetTypePersistence()
{
}

void te::gdal::DataSetTypePersistence::create(te::da::DataSetType* dt, const std::map<std::string, std::string>& /*options*/)
{
  DataSource* ds = m_t->getGDALDataSource();

  if (!ds->isDirectory())
    throw Exception(TR_GDAL("Create operation supported just on directory data sources."));

  te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(dt->getProperty(0));

  std::string accessInfo;

  std::map<std::string, std::string>::const_iterator it = m_t->getGDALDataSource()->getConnectionInfo().find("URI");

  if(it != m_t->getGDALDataSource()->getConnectionInfo().end())
  {
    boost::filesystem::path mpath(it->second);
    mpath /= dt->getName();
    accessInfo = mpath.string();
  }

  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = accessInfo;

  rstp->setInfo(rinfo);

  GDALDataset* gds = te::gdal::CreateRaster(rstp->getGrid(), rstp->getBandProperties(), rinfo);

  if (!gds)
    throw Exception(TR_GDAL("GDAL driver couldn't persist the raster file."));

  GDALClose(gds);

  dt->setId(m_t->getDataSource()->getCatalog()->getNumberOfDataSets());

  te::da::DataSetTypePtr ndt(static_cast<te::da::DataSetType*>(dt->clone()));

  m_t->getDataSource()->getCatalog()->add(ndt);
}

void te::gdal::DataSetTypePersistence::clone(const std::string& /*datasetName*/, const std::string& /*newDatasetName*/, const std::map<std::string, std::string>& /*options*/)
{
  throw te::common::Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSetTypePersistence::drop(const std::string& /*datasetName*/)
{
  throw te::common::Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSetTypePersistence::drop(te::dt::Property* p)
{
  DataSource* ds = m_t->getGDALDataSource();
  
  if (!ds->isDirectory())
    throw Exception(TR_GDAL("Drop operation supported just on directory data sources."));

  te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(p);

  std::string driverName = GetDriverName(rstp->getInfo().at("URI"));

  GDALDriverManager* dMan = GetGDALDriverManager();

  GDALDriver* driver = dMan->GetDriverByName(driverName.c_str());

  if (!driver)
    return;

  driver->Delete(rstp->getInfo().at("URI").c_str());

  if(p->getParent())
  {
    te::da::DataSetType* dt = static_cast<te::da::DataSetType*> (p->getParent());
    dt->remove(p);
  }
  else
    delete p;
}

void te::gdal::DataSetTypePersistence::drop(te::da::DataSetType* dt)
{ 
  drop(dt->getProperty(0));

  if(dt->getCatalog())
  {
    te::da::DataSourceCatalog* catalog = dt->getCatalog();
    catalog->remove(dt, true);
  }
  else
    delete dt;
}

void te::gdal::DataSetTypePersistence::rename(te::da::DataSetType* dt, const std::string& newName)
{
  rename(dt->getProperty(0), newName);

  if(dt->getCatalog())
  {
    te::da::DataSourceCatalog* catalog = dt->getCatalog();

    catalog->rename(dt, newName);
  }
  else
    dt->setName(newName);
}

void te::gdal::DataSetTypePersistence::rename(te::dt::Property* p, const std::string& newName)
{ 
  te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(p);

  std::string driverName = GetDriverName(rstp->getInfo().at("URI"));

  GDALDriverManager* dMan = GetGDALDriverManager();

  GDALDriver* driver = dMan->GetDriverByName(driverName.c_str());

  if (!driver)
    return;

  std::string accessInfo = newName;

  std::map<std::string, std::string>::const_iterator it = m_t->getGDALDataSource()->getConnectionInfo().find("URI");

  if(it != m_t->getGDALDataSource()->getConnectionInfo().end())
  {
    boost::filesystem::path mpath(it->second);
    mpath /= newName;
    accessInfo = mpath.string();
  }

  driver->Rename(accessInfo.c_str(), rstp->getInfo().at("URI").c_str());

  rstp->setName(newName);
}

void te::gdal::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::dt::Property* /*p*/)
{
  throw(te::common::Exception(TR_GDAL("GDAL driver doesn't support the persistence of new properties to data set types.")));
}

void te::gdal::DataSetTypePersistence::update(te::dt::Property* /*oldP*/, te::dt::Property* /*newP*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support this method."));
}

void te::gdal::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::PrimaryKey* /*pk*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of primary key."));
}

void te::gdal::DataSetTypePersistence::drop(te::da::PrimaryKey* /*pk*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of primary key."));
}

void te::gdal::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::UniqueKey* /*uk*/) 
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of unique key."));
}

void te::gdal::DataSetTypePersistence::drop(te::da::UniqueKey* /*uk*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of unique key."));
}

void te::gdal::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::Index* /*index*/, const std::map<std::string, std::string>& /*options*/) 
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of index."));
}

void te::gdal::DataSetTypePersistence::drop(te::da::Index* /*index*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of index."));
}

void te::gdal::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::ForeignKey* /*fk*/) 
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of foreign key."));
}

void te::gdal::DataSetTypePersistence::drop(te::da::ForeignKey* /*fk*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of foreign key."));
}

void te::gdal::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::CheckConstraint* /*cc*/) 
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of check constraint."));
}

void te::gdal::DataSetTypePersistence::drop(te::da::CheckConstraint* /*cc*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of check constraint."));
}

void te::gdal::DataSetTypePersistence::create(te::da::Sequence* /*sequence*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of sequence."));
}

void te::gdal::DataSetTypePersistence::drop(te::da::Sequence* /*seq*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of sequence."));
}

te::da::DataSourceTransactor* te::gdal::DataSetTypePersistence::getTransactor() const
{ 
  return m_t; 
}
