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
  \file terralib/gdal/DataSource.cpp

  \brief The implementation of a DataSource that consists of datasets that can be decoded by the GDAL Library.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Globals.h"

// GDAL
#include <gdal_priv.h>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

te::da::DataSourceCapabilities te::gdal::DataSource::sm_capabilities;

te::gdal::DataSource::DataSource()
  : m_catalog(0),
    m_isOpened(false),
    m_isDirectory(false)
{
  m_catalog = new te::da::DataSourceCatalog();
  m_catalog->setDataSource(this);
}

te::gdal::DataSource::~DataSource()
{
  delete m_catalog;
}

const std::string& te::gdal::DataSource::getType() const
{
  return Globals::getInstance().getDriverID();
}

void te::gdal::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connectionInfo = connInfo;
}

const std::map<std::string, std::string>& te::gdal::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

const te::da::DataSourceCapabilities& te::gdal::DataSource::getCapabilities() const
{
  return sm_capabilities;
}

const te::da::SQLDialect* te::gdal::DataSource::getDialect() const
{
   return 0;
}

void te::gdal::DataSource::open()
{
  if(m_isOpened)
    return;

// by default assume datasource doesn't refer to a directory
  m_isDirectory = false;

// check if the datasource is a directory: GDAL doesn't give support on this
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");

  if(it != m_connectionInfo.end())
  {
    if(!boost::filesystem::exists(it->second))
      throw Exception((boost::format(TR_GDAL("Invalid data source connection information: %1%.")) % it->second).str());

    if(boost::filesystem::is_directory(it->second))
      m_isDirectory = true;
  }
  else
  {
    it = m_connectionInfo.find("SOURCE");

    if(it == m_connectionInfo.end())
      throw Exception(TR_GDAL("Invalid data source connection information!"));
  }

  m_isOpened = true;
}

void te::gdal::DataSource::close()
{
  m_catalog->clear();

  m_isOpened = false;
}

bool te::gdal::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::gdal::DataSource::isValid() const
{
  if(m_connectionInfo.empty())
    return false;

  bool isValid = false;

  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");

// if URI used, check if it is a valid directory or file name
  if(it != m_connectionInfo.end())
  {
    if(!boost::filesystem::exists(it->second))
      return false;

    if(boost::filesystem::is_directory(it->second))
      return true;    
  }
  else  // if it is another GDAL string let's check it
  {
    it = m_connectionInfo.find("SOURCE");

    if(it == m_connectionInfo.end())
      return false;
  }

  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));

  if(gds)
    isValid = true;

  GDALClose(gds);

  return isValid;
}

te::da::DataSourceCatalog* te::gdal::DataSource::getCatalog() const
{
  return m_catalog;
}

te::da::DataSourceTransactor* te::gdal::DataSource::getTransactor()
{
  return new te::gdal::DataSourceTransactor(this);
}

void te::gdal::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSource::setCapabilities(const te::da::DataSourceCapabilities& capabilities)
{
  sm_capabilities = capabilities;
}

void te::gdal::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  setConnectionInfo(dsInfo);
  
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");

  if(it != m_connectionInfo.end())
  {
    try 
    {      
      if(!boost::filesystem::is_directory(it->second))
        boost::filesystem::create_directory(it->second);
    } 
    catch(const boost::filesystem::filesystem_error& e) 
    { 
      throw Exception((boost::format(TR_GDAL("Could not create the data source due to the following error: %1%.")) % e.what()).str());
    }
  }

  close();
}
 
void te::gdal::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("URI");

  if(it != dsInfo.end())
  {
    try 
    {    
      boost::filesystem::remove(it->second);
    }
    catch(const boost::filesystem::filesystem_error& e) 
    { 
      throw Exception((boost::format(TR_GDAL("Could not drop the data source due to the following error: %1%.")) % e.what()).str());
    }
  }
}

bool te::gdal::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw Exception(TR_GDAL("Not implemented yet!"));
}
