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
  \file DataSource.cpp

  \brief The OGR data provider.  
 */

// TerraLib
#include "../common/Exception.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../geometry/Geometry.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Globals.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

// STL
#include <cassert>
#include <cstring>

// Boost
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

te::ogr::DataSource::DataSource()
  : m_catalog(0),
    m_ogrDS(0),
    m_isValid(false)
{
  m_catalog = new te::da::DataSourceCatalog;
  m_catalog->setDataSource(this);
}

te::ogr::DataSource::~DataSource()
{
  close();
}

const std::string& te::ogr::DataSource::getType() const
{
  return Globals::m_driverIdentifier;
}

const std::map<std::string, std::string>& te::ogr::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::ogr::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connectionInfo = connInfo;
}

const te::da::DataSourceCapabilities& te::ogr::DataSource::getCapabilities() const
{
  return m_capabilities;
}

const te::da::SQLDialect* te::ogr::DataSource::getDialect() const
{
  return 0; // Throw an exception? OGR Library not supports sql dialect
}

void te::ogr::DataSource::open()
{
  close();

  if(m_connectionInfo.empty())
    throw te::common::Exception(TR_OGR("There is no information about the data source")); 

  std::string path = m_connectionInfo.begin()->second;
  m_ogrDS = OGRSFDriverRegistrar::Open(path.c_str());
  
  if(m_ogrDS == 0)
  {
    m_isValid = false;
    throw te::common::Exception(TR_OGR("Error open data source: " + path));
  }

  m_isValid = true;
}

void te::ogr::DataSource::close()
{
  OGRDataSource::DestroyDataSource(m_ogrDS);
  m_ogrDS = 0;  
  
  m_catalog->clear();
    
  m_isValid = false;
}

bool te::ogr::DataSource::isOpened() const
{
  return m_ogrDS != 0;    
}

bool te::ogr::DataSource::isValid() const
{
  return m_isValid;
}

te::da::DataSourceCatalog* te::ogr::DataSource::getCatalog() const
{
  return m_catalog;
}

te::da::DataSourceTransactor* te::ogr::DataSource::getTransactor()
{
  return new DataSourceTransactor(this, m_ogrDS);
}

void te::ogr::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw te::common::Exception(TR_OGR("Not implemented yet!"));
}

void te::ogr::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  std::string path = dsInfo.begin()->second;
 
  if (boost::filesystem::is_directory(path.c_str()))
  {  
    try
    {
      boost::filesystem::create_directory(path);
    }
    catch (boost::filesystem::filesystem_error &e)
    { 
      std::string m = TR_OGR("Could not create directory. ");
                  m += e.what();
      throw te::common::Exception(m);
    }
    setConnectionInfo(dsInfo);
    open();
  }
  else
  {
    OGRSFDriverRegistrar* driverManager = OGRSFDriverRegistrar::GetRegistrar();
    OGRSFDriver* driver = driverManager->GetDriverByName(GetDriverName(path).c_str());
    if (driver == 0)
      throw(te::common::Exception(TR_OGR("Driver not found.")));

    if(!driver->TestCapability(ODrCCreateDataSource))
      throw(te::common::Exception(TR_OGR("The Driver does not have create capability.")));

    m_ogrDS = driver->CreateDataSource(path.c_str());
    if(m_ogrDS == 0)
      throw(te::common::Exception(TR_OGR("Error when attempting create the data source.")));   

    //OGRDataSource::DestroyDataSource(newDS);
  }
}

void te::ogr::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  //std::string path = m_connectionInfo.begin()->second;
 
  //if (boost::filesystem::is_directory(path.c_str()))
  //{  
  //  try
  //  {
  //    boost::filesystem::remove(path);
  //  }
  //  catch (boost::filesystem::filesystem_error &e)
  //  { 
  //    std::string m = TR_OGR("Could not drop directory. ");
  //                m += e.what();
  //    throw te::common::Exception(m);
  //  }
  //}
  //else
  //{
  //  OGRSFDriverRegistrar* driverManager = OGRSFDriverRegistrar::GetRegistrar();
  //  OGRSFDriver* driver = driverManager->GetDriverByName(te::ogr::Platform::getDriverName(path).c_str());
  //  if(driver == 0)
  //    throw(te::common::Exception(TR_OGR("Driver not found.")));

  //  if(!driver->TestCapability(ODrCDeleteDataSource))
  //    throw(te::common::Exception(TR_OGR("The Driver does not have drop capability.")));

  //  if(driver->DeleteDataSource(path.c_str()) != OGRERR_NONE)
  //    throw(te::common::Exception(TR_OGR("Error when attempting drop the data source.")));

  //  if(driver->DeleteDataSource("") != OGRERR_NONE)
  //    throw(te::common::Exception(TR_OGR("Error when attempting drop the data source.")));
  //}
}

bool te::ogr::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw te::common::Exception(TR_OGR("Not implemented yet!"));
}

