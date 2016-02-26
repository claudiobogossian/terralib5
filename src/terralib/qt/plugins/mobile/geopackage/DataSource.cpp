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
#include "DataSource.h"
#include "Globals.h"
#include "Transactor.h"
#include "Utils.h"

#include "../../../../common/Translator.h"
#include "../../../../dataaccess/query/SQLDialect.h"

// OGR
#include <ogrsf_frmts.h>
#include <ogr_core.h>
#include <gdal_priv.h>
#include <ogr_api.h>
#include <gdal.h>

// Boost
#include <boost/filesystem/operations.hpp>

te::da::SQLDialect* te::gpkg::DataSource::sm_myDialect(0);

void GetDataSetTypeCapabilities(te::da::DataSourceCapabilities& caps)
{
  te::da::DataTypeCapabilities dt_caps;

  dt_caps.setSupportInt32(true);
  dt_caps.setSupportArray(true);
  dt_caps.setSupportString(true);
  dt_caps.setSupportDouble(true);
  dt_caps.setSupportNumeric(true);
  dt_caps.setSupportByteArray(true);
  dt_caps.setSupportDateTime(true);
  dt_caps.setSupportGeometry(true);

  caps.setDataTypeCapabilities(dt_caps);
}

void GetQueryCapabilities(te::da::DataSourceCapabilities& caps)
{
  te::da::QueryCapabilities qy_caps;

  // Spatial Topologic Operators
  qy_caps.addSpatialTopologicOperator("st_envelopeintersects");

  // Comparsion Operators
  qy_caps.addComparsionOperator("=");
  qy_caps.addComparsionOperator("<>");
  qy_caps.addComparsionOperator("<");
  qy_caps.addComparsionOperator(">");
  qy_caps.addComparsionOperator("<=");
  qy_caps.addComparsionOperator(">=");

  qy_caps.addLogicalOperator("and");
  qy_caps.addLogicalOperator("or");

  caps.setQueryCapabilities(qy_caps);
}

//void GetCapabilities(OGRDataSource* ds, te::da::DataSourceCapabilities& caps)
void GetCapabilities(GDALDataset* ds, te::da::DataSourceCapabilities& caps)
{
  // DataSet
  if(!ds || ds->GetLayerCount() <= 0)
    return;

  te::da::DataSetCapabilities ds_caps;

  OGRLayer* l = ds->GetLayer(0);
  
  ds_caps.setSupportEfficientMove((l->TestCapability(OLCFastSetNextByIndex)) ? true : false);
  ds_caps.setSupportRandomTraversing((l->TestCapability(OLCRandomRead)) ? true : false);
  ds_caps.setSupportBidirectionalTraversing((l->TestCapability(OLCRandomRead)) ? true : false);
  ds_caps.setSupportEfficientDataSetSize((l->TestCapability(OLCFastGetExtent)) ? true : false);

  caps.setDataSetCapabilities(ds_caps);
}


te::gpkg::DataSource::DataSource() :
  te::da::DataSource(),
  m_ogrDS(0),
  m_isValid(false)
{
}

te::gpkg::DataSource::~DataSource()
{
  close();
}

std::string te::gpkg::DataSource::getType() const
{
  return Globals::m_driverIdentifier;
}

const std::map<std::string, std::string>& te::gpkg::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::gpkg::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connectionInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> te::gpkg::DataSource::getTransactor()
{
  return std::auto_ptr<te::da::DataSourceTransactor>(new Transactor(this));
}

void te::gpkg::DataSource::open()
{
  close();

  if(m_connectionInfo.empty())
    throw Exception(TE_TR("There is no information about the data source")); 

  std::string path;
  std::map<std::string, std::string>::const_iterator it;
  
  it = m_connectionInfo.find("URI");
  if (it==m_connectionInfo.end())
    throw(Exception(TE_TR("Not enough information to open the data source.")));

  path = it->second;

  if (boost::filesystem::exists(path))
    //m_ogrDS = OGRSFDriverRegistrar::Open(path.c_str(), 1);
    m_ogrDS = (GDALDataset*)GDALOpenEx(path.c_str(), GDAL_OF_UPDATE, NULL, NULL, NULL);
  
  // let's try to open it without update permission
  if (!m_ogrDS)
  {
    //m_ogrDS = OGRSFDriverRegistrar::Open(path.c_str(), 0);
    m_ogrDS = (GDALDataset*)GDALOpenEx(path.c_str(), GDAL_OF_READONLY, NULL, NULL, NULL);
    if (m_ogrDS)
      m_capabilities.setAccessPolicy(te::common::RAccess);
  }
  else
    m_capabilities.setAccessPolicy(te::common::RWAccess);

  m_isValid = true;

  GetDataSetTypeCapabilities(m_capabilities);
  GetQueryCapabilities(m_capabilities);

  if (m_ogrDS)
    GetCapabilities(m_ogrDS, m_capabilities);
}

void te::gpkg::DataSource::close()
{
  if (m_ogrDS)
    //OGRDataSource::DestroyDataSource(m_ogrDS);
    GDALClose(m_ogrDS);

  m_ogrDS = 0;  
  
  m_isValid = false;
}

bool te::gpkg::DataSource::isOpened() const
{
  return m_ogrDS != 0;    
}

bool te::gpkg::DataSource::isValid() const
{
  return m_isValid;
}

const te::da::DataSourceCapabilities& te::gpkg::DataSource::getCapabilities() const
{
  return m_capabilities;
}

const te::da::SQLDialect* te::gpkg::DataSource::getDialect() const
{
  return sm_myDialect;
}

void te::gpkg::DataSource::setDialect(te::da::SQLDialect* dialect)
{
  if(sm_myDialect != 0)
    delete sm_myDialect;

  sm_myDialect = dialect;
}

//OGRDataSource* te::gpkg::DataSource::getOGRDataSource()
GDALDataset* te::gpkg::DataSource::getOGRDataSource()
{
  return m_ogrDS;
}

void te::gpkg::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  setConnectionInfo(dsInfo);

  close();
}

void  te::gpkg::DataSource::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  if (!m_ogrDS)
  {
    std::string path;
    std::map<std::string, std::string>::const_iterator it;
    
    it = m_connectionInfo.find("URI");
    if (it==m_connectionInfo.end())
      throw(Exception(TE_TR("Not enough information to create data set.")));
    path = it->second;
    
    boost::filesystem::path bpath(path);
    std::string dir = bpath.parent_path().string();
    if (!boost::filesystem::exists(dir))
      boost::filesystem::create_directory(dir);
    
    //OGRSFDriverRegistrar* driverManager = OGRSFDriverRegistrar::GetRegistrar();
    //OGRSFDriver* driver;

    GDALDriverManager* driverManager = GetGDALDriverManager();
    GDALDriver* driver;
    
    it = m_connectionInfo.find("DRIVER");
    if (it!=m_connectionInfo.end())
      driver = driverManager->GetDriverByName(it->second.c_str());
    else
      driver = driverManager->GetDriverByName(GetDriverName(path).c_str());
    
    if (driver == 0)
      throw(Exception(TE_TR("Driver not found.")));
    
    //if(!driver->TestCapability(ODrCCreateDataSource))
    //  throw(Exception(TE_TR("The driver has no capability for creating a datasource!")));

    if (!OGR_Dr_TestCapability(driver, ODrCCreateDataSource))
      throw(Exception(TE_TR("The driver has no capability for creating a datasource!")));
    
    char** papszOptions = 0;
    it = m_connectionInfo.begin();
    while(it != m_connectionInfo.end())
    {
      if(it->first == "URI" || it->first == "SOURCE" || it->first == "DRIVER")
      {
        ++it;
        continue;
      }
      papszOptions = CSLSetNameValue(papszOptions, it->first.c_str(), it->second.c_str());
      ++it;
    }
    
    //m_ogrDS = driver->CreateDataSource(path.c_str(), papszOptions);
    m_ogrDS = driver->Create(path.c_str(), 0, 0, 0, GDT_Unknown, papszOptions);

    if(papszOptions)
      CSLDestroy(papszOptions);
  }
  
  if (!m_ogrDS)
    throw(Exception(TE_TR("Error creating the dataset!")));
  
  std::auto_ptr<te::da::DataSourceTransactor> t = getTransactor();
  return t->createDataSet(dt, options);
}

void te::gpkg::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  std::string path = dsInfo.begin()->second;

  //if (m_ogrDS != 0 && path.compare(m_ogrDS->GetName()) == 0)
  if (m_ogrDS != 0 && path.compare(m_ogrDS->GetDescription()) == 0)
    close();

  //OGRSFDriverRegistrar* driverManager = OGRSFDriverRegistrar::GetRegistrar();
  //OGRSFDriver* driver = driverManager->GetDriverByName(GetDriverName(path).c_str());

  GDALDriverManager* driverManager = GetGDALDriverManager();
  GDALDriver* driver = driverManager->GetDriverByName(GetDriverName(path).c_str());

  if (driver == 0)
    throw(Exception(TE_TR("Driver not found!")));

//  if(!driver->TestCapability(ODrCDeleteDataSource))
//    throw(Exception(TE_TR("The Driver does not have drop capability.")));

  if (!OGR_Dr_TestCapability(driver, ODrCDeleteDataSource))
    throw(Exception(TE_TR("The driver has no drop capability!")));

//  if(driver->DeleteDataSource(path.c_str()) != OGRERR_NONE)
//    throw(Exception(TE_TR("Error when dropping the data source.")));
  GDALClose(driver);
}

bool te::gpkg::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  return boost::filesystem::exists(dsInfo.begin()->second);
}

std::vector<std::string> te::gpkg::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  std::vector<std::string> names;

  names.push_back(dsInfo.begin()->second);

  return names;
}

std::vector<te::common::CharEncoding> te::gpkg::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  return std::vector<te::common::CharEncoding>();
}

te::gpkg::DataSource* te::gpkg::Build()
{
  return new DataSource;
}
