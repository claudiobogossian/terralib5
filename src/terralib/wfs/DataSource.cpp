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
  \file terralib/wfs/DataSource.cpp

  \brief Implementation of the data source for the WFS driver.
*/

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../common/StringUtils.h"
#include "../core/translator/Translator.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>
#include <ogr_core.h>

te::da::DataSourceCapabilities te::wfs::DataSource::sm_capabilities;

te::da::SQLDialect* te::wfs::DataSource::sm_dialect(0);

te::wfs::DataSource::DataSource(const std::string& connInfo)
  : te::da::DataSource(connInfo),
    m_ogrDS(0),
    m_isOpened(false)
{
}

te::wfs::DataSource::DataSource(const te::core::URI& uri)
  : te::da::DataSource(uri),
  m_ogrDS(0),
  m_isOpened(false)
{
}

te::wfs::DataSource::~DataSource()
{
  close();
}

std::string te::wfs::DataSource::getType() const
{
  return TE_WFS_DRIVER_IDENTIFIER;
}

std::auto_ptr<te::da::DataSourceTransactor> te::wfs::DataSource::getTransactor()
{
  if(!m_isOpened)
    throw Exception(TE_TR("The data source is not opened!"));

  return std::auto_ptr<te::da::DataSourceTransactor>(new Transactor(this));
}

void te::wfs::DataSource::open()
{
  if(m_isOpened)
    return;

  verifyConnectionInfo();

  m_ogrDS = (GDALDataset*)GDALOpenEx(m_uri.uri().c_str(), GDAL_OF_READONLY, NULL, NULL, NULL);

  if(m_ogrDS == 0)
    throw Exception(TE_TR("Could not open the WFS data source!"));

  m_isOpened = true;
}

void te::wfs::DataSource::close()
{
//  if (m_ogrDS)
    //OGRDataSource::DestroyDataSource(m_ogrDS);
//    GDALClose(m_ogrDS);

  m_ogrDS = 0;

  m_isOpened = false;
}

bool te::wfs::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::wfs::DataSource::isValid() const
{
  if (m_isOpened)
    return true;

  verifyConnectionInfo();

  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpenEx(m_uri.uri().c_str(), GDAL_OF_READONLY, NULL, NULL, NULL));
  if (gds == 0)
    return false;

  GDALClose(gds);

  return true;
}

const te::da::DataSourceCapabilities& te::wfs::DataSource::getCapabilities() const
{
  return sm_capabilities;
}

void te::wfs::DataSource::setCapabilities(const te::da::DataSourceCapabilities& capabilities)
{
  sm_capabilities = capabilities;
}

const te::da::SQLDialect* te::wfs::DataSource::getDialect() const
{
  return sm_dialect;
}

void te::wfs::DataSource::setDialect(te::da::SQLDialect* dialect)
{
  delete sm_dialect;
  sm_dialect = dialect;
}

//OGRDataSource* te::wfs::DataSource::getOGRDataSource()
GDALDataset* te::wfs::DataSource::getOGRDataSource()
{
  return m_ogrDS;
}

const std::vector<te::wfs::WFSLayerInfo>& te::wfs::DataSource::getLayersInfo()
{
  if(!m_layersInfo.empty())
    return m_layersInfo;

  buildLayersInfo();

  return m_layersInfo;
}

void te::wfs::DataSource::create(const std::string& connInfo)
{
  throw Exception(TE_TR("The create() method is not supported by the WFS driver!"));
}

void te::wfs::DataSource::drop(const std::string& connInfo)
{
  throw Exception(TE_TR("The drop() method is not supported by the WFS driver!"));
}

bool te::wfs::DataSource::exists(const std::string& connInfo)
{
  if (connInfo.empty())
    return false;

  const te::core::URI aux(connInfo);
  if (!aux.isValid())
    return false;

  std::string path = aux.path();
  if (path.empty())
    return false;

  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(path.c_str(), GA_ReadOnly));
  if (gds == 0)
    return false;

  GDALClose(gds);

  return true;
}

std::vector<std::string> te::wfs::DataSource::getDataSourceNames(const std::string& /*connInfo*/)
{
  return std::vector<std::string>();
}

void te::wfs::DataSource::verifyConnectionInfo() const
{
  if (!m_uri.isValid())
    throw Exception(TE_TR("The connection information is invalid!"));

  if (m_uri.path().empty())
    throw Exception(TE_TR("The connection information is invalid. Missing the path parameter!"));
}

void te::wfs::DataSource::buildLayersInfo()
{
  open();

  OGRLayer* wfsMetadata = m_ogrDS->GetLayerByName("WFSLayerMetadata");

  if(wfsMetadata == 0)
    throw Exception(TE_TR("Could not retrieve the metadata from WFS server!"));

  OGRFeature* f;
  wfsMetadata->ResetReading();
  while((f = wfsMetadata->GetNextFeature()) != NULL)
  {
    WFSLayerInfo info;
    info.m_name = f->GetFieldAsString("layer_name");
    info.m_title = f->GetFieldAsString("title");
    info.m_abstract = f->GetFieldAsString("abstract");

    m_layersInfo.push_back(info);
  }
}
