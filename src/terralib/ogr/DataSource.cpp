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
#include "DataSource.h"
#include "Globals.h"
#include "Transactor.h"
#include "Utils.h"

#include "../common/BoostUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/query/SQLDialect.h"

// OGR
#include <ogrsf_frmts.h>

// Boost
#include <boost/filesystem/operations.hpp>

te::da::SQLDialect* te::ogr::DataSource::sm_myDialect(0);

te::ogr::DataSource::DataSource() :
te::da::DataSource(),
m_ogrDS(0),
m_isValid(false)
{
}

te::ogr::DataSource::~DataSource()
{
  close();
}

std::string te::ogr::DataSource::getType() const
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

std::auto_ptr<te::da::DataSourceTransactor> te::ogr::DataSource::getTransactor()
{
  return std::auto_ptr<te::da::DataSourceTransactor>(new Transactor(this));
}

void te::ogr::DataSource::open()
{
  close();

  if(m_connectionInfo.empty())
    throw Exception(TR_OGR("There is no information about the data source")); 

  std::string path = te::common::ConvertLatin1UTFString(m_connectionInfo.begin()->second);
  m_ogrDS = OGRSFDriverRegistrar::Open(path.c_str());
  
  if(m_ogrDS == 0)
  {
    m_isValid = false;
    throw Exception(TR_OGR("Error open data source: " + path));
  }

  m_isValid = true;
}

void te::ogr::DataSource::close()
{
  OGRDataSource::DestroyDataSource(m_ogrDS);
  m_ogrDS = 0;  
  
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

const te::da::DataSourceCapabilities& te::ogr::DataSource::getCapabilities() const
{
  return m_capabilities;
}

const te::da::SQLDialect* te::ogr::DataSource::getDialect() const
{
  return sm_myDialect;
}

void te::ogr::DataSource::setDialect(te::da::SQLDialect* dialect)
{
  if(sm_myDialect != 0)
    delete sm_myDialect;

  sm_myDialect = dialect;
}

OGRDataSource* te::ogr::DataSource::getOGRDataSource()
{
  return m_ogrDS;
}

void te::ogr::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  std::string path = dsInfo.begin()->second;
  OGRSFDriverRegistrar* driverManager = OGRSFDriverRegistrar::GetRegistrar();
  OGRSFDriver* driver = driverManager->GetDriverByName(GetDriverName(path).c_str());

  if (driver == 0)
    throw(Exception(TR_OGR("Driver not found.")));

  if(!driver->TestCapability(ODrCCreateDataSource))
    throw(Exception(TR_OGR("The Driver does not have create capability.")));

  m_ogrDS = driver->CreateDataSource(path.c_str());

  if(m_ogrDS == 0)
    throw(Exception(TR_OGR("Error when attempting create the data source.")));   

  setConnectionInfo(dsInfo);

  close();
}

void te::ogr::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  std::string path = dsInfo.begin()->second;

  if(m_ogrDS!=0 && path.compare(m_ogrDS->GetName()) == 0)
    close();

  OGRSFDriverRegistrar* driverManager = OGRSFDriverRegistrar::GetRegistrar();
  OGRSFDriver* driver = driverManager->GetDriverByName(GetDriverName(path).c_str());

  if (driver == 0)
    throw(Exception(TR_OGR("Driver not found.")));

  if(!driver->TestCapability(ODrCDeleteDataSource))
    throw(Exception(TR_OGR("The Driver does not have drop capability.")));

  if(driver->DeleteDataSource(path.c_str()) != OGRERR_NONE)
    throw(Exception(TR_OGR("Error when dropping the data source.")));   
}

bool te::ogr::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  return boost::filesystem::exists(dsInfo.begin()->second);
}

std::vector<std::string> te::ogr::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  std::vector<std::string> names;

  names.push_back(dsInfo.begin()->second);

  return names;
}

std::vector<std::string> te::ogr::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  std::vector<std::string> encodings;

  return encodings;
}

te::ogr::DataSource* te::ogr::Build()
{
  return new DataSource;
}
