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
  \file terralib/tools/dscopy/qt/DSCopyDialogController.cpp

  \brief Data Source Copy GUI Controller
 */

// DSCopy
#include "DSCopyDialogController.h"
#include "../core/DSCopy.h"
#include "../core/Utils.h"

// TerraLib
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/dataaccess/datasource/DataSourceCatalogLoader.h>

te::tools::dscopy::DSCopyDialogController::DSCopyDialogController()
{
  std::string errorMessage;
  Utils::loadModules(errorMessage);
}

te::tools::dscopy::DSCopyDialogController::~DSCopyDialogController()
{

}

bool te::tools::dscopy::DSCopyDialogController::setOriginConnectionInfo(std::string dstype, std::string conn, std::string& errorMessage)
{

  try
  {
    if(dstype == "OGR")
    {
      m_dsOrigin = te::da::DataSourceFactory::make("OGR");
      m_dsOrigin->open("connection_string="+conn);
    }
    else if(dstype == "GDAL")
    {
      m_dsOrigin = te::da::DataSourceFactory::make("GDAL");
      m_dsOrigin->open("URI="+conn);    
    }
    else if(dstype == "POSTGIS")
    {
      m_dsOrigin = te::da::DataSourceFactory::make("POSTGIS");
      m_dsOrigin->open(conn);    
    }
    else
    {
      errorMessage = "There is not a implementation for this Data Source Type yet!";
      return false;
    }

    if(!m_dsOrigin)
    {
      errorMessage = "Data Source invalid!";
        return false;
    }
    else
      return true;
  }
  catch(std::exception& e)
  {
    errorMessage = "Error ocurred: " + std::string(e.what());
    return false;
  }
  catch(...)
  {
    errorMessage = "Error ocurred: unexpected!";
    return false;
  }

}

bool te::tools::dscopy::DSCopyDialogController::setOriginConnectionInfo(std::string dstype, std::string host, 
  std::string port, std::string dsname, std::string user, std::string pw, std::string& errorMessage)
{

  if(dstype == "POSTGIS")
  {
    std::string connStr = "host=" + host;
    connStr += "&port=" + port;
    connStr += "&dbname=" + dsname;
    connStr += "&user=" + user;
    connStr += "&password=" + pw;

    try
    {

      m_dsOrigin = te::da::DataSourceFactory::make("POSTGIS");
      m_dsOrigin->open(connStr);

      if(!m_dsOrigin)
      {
        errorMessage = "Data Source invalid!";
        return false;
      }
      else
        return true;

    }
    catch(std::exception& e)
    {
      errorMessage = "Error ocurred: " + std::string(e.what());
      return false;
    }
    catch(...)
    {
      errorMessage = "Error ocurred: unexpected!";
      return false;
    }
  }
  else
  {
    errorMessage = "There is not a implementation for this Data Source Type yet!";
    return false;
  }
}

bool te::tools::dscopy::DSCopyDialogController::setDestinationConnectionInfo(std::string dstype, std::string conn, std::string& errorMessage)
{

  try
  {
    if(dstype == "OGR")
    {
      m_dsDestination = te::da::DataSourceFactory::make("OGR");
      m_dsDestination->open("connection_string="+conn);
    }
    else if(dstype == "GDAL")
    {
      m_dsDestination = te::da::DataSourceFactory::make("GDAL");
      m_dsDestination->open("URI="+conn);    
    }
    else if(dstype == "POSTGIS")
    {
      m_dsDestination = te::da::DataSourceFactory::make("POSTGIS");
      m_dsDestination->open(conn);    
    }
    else
    {
      errorMessage = "There is not a implementation for this Data Source Type yet!";
      return false;
    }

    if(!m_dsDestination)
    {
      errorMessage = "Data Source invalid!";
        return false;
    }
    else
      return true;
  }
  catch(std::exception& e)
  {
    errorMessage = "Error ocurred: " + std::string(e.what());
    return false;
  }
  catch(...)
  {
    errorMessage = "Error ocurred: unexpected!";
    return false;
  }

}

bool te::tools::dscopy::DSCopyDialogController::setDestinationConnectionInfo(std::string dstype, std::string host, 
  std::string port, std::string dsname, std::string user, std::string pw, std::string& errorMessage)
{

  if(dstype == "POSTGIS")
  {
    std::string connStr = "host=" + host;
    connStr += "&port=" + port;
    connStr += "&dbname=" + dsname;
    connStr += "&user=" + user;
    connStr += "&password=" + pw;

    try
    {

      m_dsDestination = te::da::DataSourceFactory::make("POSTGIS");
      m_dsDestination->open(connStr);

      if(!m_dsDestination)
      {
        errorMessage = "Data Source invalid!";
        return false;
      }
      else
        return true;

    }
    catch(std::exception& e)
    {
      errorMessage = "Error ocurred: " + std::string(e.what());
      return false;
    }
    catch(...)
    {
      errorMessage = "Error ocurred: unexpected!";
      return false;
    }
  }
  else
  {
    errorMessage = "There is not a implementation for this Data Source Type yet!";
    return false;
  }
}

std::vector<std::string> te::tools::dscopy::DSCopyDialogController::getDictionary()
{
  std::vector<std::string> dictionary;

  te::da::DataSourceFactory::dictionary_type::const_iterator it = te::da::DataSourceFactory::getDictionary().begin();

  while(it != te::da::DataSourceFactory::getDictionary().end())
  {  
    if(it->first != "MEM")
    {
      dictionary.push_back(it->first.c_str());
    }

    ++it;
  }

  return dictionary;
}

std::vector<std::string*> te::tools::dscopy::DSCopyDialogController::getDatasetsName()
{
  std::vector<std::string*> dataSetsNames;

  te::da::DataSourceTransactor* transactor = m_dsOrigin->getTransactor();

  te::da::DataSourceCatalogLoader* catalogLoader = transactor->getCatalogLoader();
  
  catalogLoader->getDataSets(dataSetsNames);

  return dataSetsNames;
}

void te::tools::dscopy::DSCopyDialogController::setDatasetsToCopy(std::vector<std::string> datasets)
{
  m_datasets = datasets;
}

bool te::tools::dscopy::DSCopyDialogController::copy(std::string& errorMessage)
{
  std::vector<std::string*> datasetsP;
    for(size_t i = 0; i < m_datasets.size(); i++)
      datasetsP.push_back(&m_datasets[i]);

  try
  {
    DSCopy ds;
    //ds.init();
    return ds.copy(m_dsOrigin, m_dsDestination, datasetsP, errorMessage);

  }
  catch(std::exception& e)  
  {
    errorMessage = "Error ocurred: " + std::string(e.what());
    return false;
  }
  catch(...)
  {
    errorMessage = "Error ocurred: unexpected!";
    return false;
  }
}