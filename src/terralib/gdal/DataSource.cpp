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
  \file terralib/gdal/DataSource.cpp

  \brief The implementation of a DataSource that consists of datasets that can be decoded by the GDAL Library.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../geometry/Envelope.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"
#include "DataSetUseCounter.h"

// GDAL
#include <gdal_priv.h>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

te::da::DataSourceCapabilities te::gdal::DataSource::sm_capabilities;

te::gdal::DataSource::DataSource():
  m_straccess(""),
  m_isOpened(false)
{
}

te::gdal::DataSource::~DataSource()
{}

std::string te::gdal::DataSource::getType() const
{
  return TE_GDAL_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::gdal::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::gdal::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{ 
  m_connectionInfo = connInfo;
}

// open methods retrieves the names of the datasets in the data source
void te::gdal::DataSource::open()
{
  if (m_isOpened)
    return;

  if (m_connectionInfo.empty())
      throw Exception((boost::format(TE_TR("Empty data source connection information"))).str());
  
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("SOURCE");
  if (it != m_connectionInfo.end())
    m_straccess = it->second;
  else
  {
    it = m_connectionInfo.find("URI");
    if (it != m_connectionInfo.end())
      m_straccess = it->second;
    else
      throw Exception((boost::format(TE_TR("Invalid data source connection information"))).str());
  }
  m_isOpened = true;
}

void te::gdal::DataSource::close()
{
}

bool te::gdal::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::gdal::DataSource::isValid() const
{
  if(m_connectionInfo.empty())
    return false;
  
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("SOURCE");
  
  // if URI used, check if it is a valid directory or file name
  if(it != m_connectionInfo.end())
  {
    if(boost::filesystem::is_directory(it->second))
      return true;    
  }
  else  // if it is another GDAL string let's check it
  {
    it = m_connectionInfo.find("URI");
    
    if(it == m_connectionInfo.end())
      return false;
    
    DataSetUseCounter dsUseCounter( GetParentDataSetName( it->second ), 
                                    DataSetsManager::MultipleAccessType );
    
    GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
    
    if(gds)
    {
      GDALClose(gds);
      
      return true;
    }
  }
  return false;
}

const te::da::DataSourceCapabilities& te::gdal::DataSource::getCapabilities() const
{
  return sm_capabilities;
}


std::auto_ptr<te::da::DataSourceTransactor> te::gdal::DataSource::getTransactor()
{
  if (!m_isOpened)
    throw Exception((boost::format(TE_TR("Data source is not open."))).str());    
  
  return std::auto_ptr<te::da::DataSourceTransactor>(new te::gdal::Transactor(m_straccess));
}

void te::gdal::DataSource::setCapabilities(const te::da::DataSourceCapabilities& capabilities)
{
  sm_capabilities = capabilities;
}

void te::gdal::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  m_connectionInfo = dsInfo;
  
  // create the needed directory
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("SOURCE");
  if(it != m_connectionInfo.end())
  {
    try 
    {      
      if(!boost::filesystem::is_directory(it->second))
        boost::filesystem::create_directory(it->second);
    } 
    catch(const boost::filesystem::filesystem_error& e) 
    { 
      throw Exception((boost::format(TE_TR("Could not create the data source due to the following error: %1%.")) % e.what()).str());
    }
  }
  else 
  {
    throw Exception((boost::format(TE_TR("Data source creation is supported only for directory data sources"))).str());
  }
}

bool te::gdal::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("SOURCE"); // expects a directory
  if(it != dsInfo.end())   
  {
    if (boost::filesystem::exists(it->second) && boost::filesystem::is_directory(it->second)) 
      return true;
    
    return false;
  }
  
  it = dsInfo.find("URI"); // expects a file?
  if(it != dsInfo.end())   
  {  
    DataSetUseCounter dsUseCounter( GetParentDataSetName( it->second ), DataSetsManager::MultipleAccessType );
    
    GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
  
    if (gds)
    {
      GDALClose(gds);
      
      return true;
    }
  }
  
  return false; // it is an invalid file
}
 
void te::gdal::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("URI");
  if (it == dsInfo.end())
    it = dsInfo.find("SOURCE");
  
  if (it == dsInfo.end())
    return;  // nothing to be done
  
  try 
  {    
    boost::filesystem::remove(it->second);
  }
  catch(const boost::filesystem::filesystem_error& /*e*/) 
  {}
}

std::vector<std::string> te::gdal::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  std::vector<std::string> dsnames;
  
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("URI");
  if (it != dsInfo.end())
    dsnames.push_back(it->second);
  else
  {
    it = dsInfo.find("SOURCE");
    if (it != dsInfo.end())
      dsnames.push_back(it->second);
    else
      throw Exception((boost::format(TE_TR("Empty ou invalid data source connection information"))).str());
  }
  return dsnames;
}

std::vector<te::common::CharEncoding> te::gdal::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  return std::vector<te::common::CharEncoding>();
}
