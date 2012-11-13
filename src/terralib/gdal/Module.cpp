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
  \file Module.h
   
  \brief The TerraLib OGR driver is a plugin.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"
#include "Module.h"
#include "RasterFactory.h"

// GDAL
#include <gdal_priv.h>

te::gdal::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::gdal::Module::~Module()
{
}

void te::gdal::Module::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib GDAL driver support
  TE_ADD_TEXT_DOMAIN(TE_GDAL_TEXT_DOMAIN, TE_GDAL_TEXT_DOMAIN_DIR, "UTF-8");

  GDALAllRegister();

  DataSourceFactory::initialize();

  RasterFactory::initialize();

  std::map<std::string, std::string> gdalCapabilities;
  gdalCapabilities["ACCESS_POLICY"] = "RW_ACCESS";
  gdalCapabilities["TRANSACTION"] = "FALSE";
  gdalCapabilities["DATASET_PERSISTENCE"] = "TRUE";
  gdalCapabilities["DATASETTYPE_PERSISTENCE"] = "TRUE";
  gdalCapabilities["PREPARED_QUERY"] = "TRUE";
  gdalCapabilities["BATCH_EXECUTOR"] = "TRUE";
  gdalCapabilities["BIT_DT"] = "FALSE";
  gdalCapabilities["CHAR_DT"] = "FALSE";
  gdalCapabilities["UCHAR_DT"] = "FALSE";
  gdalCapabilities["INT16_DT"] = "FALSE";
  gdalCapabilities["UINT16_DT"] = "FALSE";
  gdalCapabilities["INT32_DT"] = "FALSE";
  gdalCapabilities["UINT32_DT"] = "FALSE";
  gdalCapabilities["INT64_DT"] = "FALSE";
  gdalCapabilities["UINT64_DT"] = "FALSE";
  gdalCapabilities["BOOLEAN_DT"] = "FALSE";
  gdalCapabilities["FLOAT_DT"] = "FALSE";
  gdalCapabilities["DOUBLE_DT"] = "FALSE";
  gdalCapabilities["NUMERIC_DT"] = "FALSE";
  gdalCapabilities["STRING_DT"] = "FALSE";
  gdalCapabilities["BYTE_ARRAY_DT"] = "FALSE";
  gdalCapabilities["GEOMETRY_DT"] = "FALSE";
  gdalCapabilities["DATETIME_DT"] = "FALSE";
  gdalCapabilities["ARRAY_DT"] = "FALSE";
  gdalCapabilities["COMPOSITE_DT"] = "FALSE";
  gdalCapabilities["RASTER_DT"] = "TRUE";
  gdalCapabilities["DATASET_DT"] = "FALSE";

  gdalCapabilities["PRIMARY_KEY"] = "FALSE";
  gdalCapabilities["UNIQUE_KEY"] = "FALSE";
  gdalCapabilities["FOREIGN_KEY"] = "FALSE";
  gdalCapabilities["SEQUENCE"] = "FALSE";
  gdalCapabilities["CHECK_CONSTRAINTS"] = "FALSE";
  gdalCapabilities["INDEX"] = "FALSE";
  gdalCapabilities["RTREE_INDEX"] = "FALSE";
  gdalCapabilities["BTREE_INDEX"] = "FALSE";
  gdalCapabilities["HASH_INDEX"] = "FALSE";
  gdalCapabilities["QUADRTREE_INDEX"] = "FALSE";

  gdalCapabilities["DATASET_BIDIRECTIONAL"] = "FALSE";
  gdalCapabilities["EFFICIENT_MOVE_PREVIOUS"] = "FALSE";
  gdalCapabilities["EFFICIENT_MOVE_FIRST"] = "FALSE";
  gdalCapabilities["EFFICIENT_MOVE_BEFORE_FIRST"] = "FALSE";
  gdalCapabilities["EFFICIENT_MOVE_LAST"] = "FALSE";
  gdalCapabilities["EFFICIENT_MOVE_AFTER_LAST"] = "FALSE";
  gdalCapabilities["EFFICIENT_MOVE"] = "FALSE";
  gdalCapabilities["EFFICIENT_DATASET_SIZE"] = "TRUE";
  gdalCapabilities["DATASET_INSERTION"] = "FALSE";
  gdalCapabilities["DATASET_UPDATE"] = "FALSE";
  gdalCapabilities["DATASET_DELETION"] = "FALSE";

  gdalCapabilities["SQL_DIALECT"] = "FALSE";
  gdalCapabilities["INSERT_QUERY"] = "FALSE";
  gdalCapabilities["UPDATE_QUERY"] = "FALSE";
  gdalCapabilities["DELETE_QUERY"] = "FALSE";
  gdalCapabilities["CREATE_QUERY"] = "FALSE";
  gdalCapabilities["DROP_QUERY"] = "FALSE";
  gdalCapabilities["ALTER_QUERY"] = "FALSE";
  gdalCapabilities["SELECT_QUERY"] = "FALSE";
  gdalCapabilities["SELECT_INTO_QUERY"] = "FALSE";

  te::gdal::DataSource::setCapabilities(gdalCapabilities);

  TE_LOG_TRACE(TR_GDAL("TerraLib GDAL driver startup!"));

  m_initialized = true;
}

void te::gdal::Module::shutdown()
{
  if(!m_initialized)
    return;

//! it finalizes the GDAL factory support.
  te::gdal::DataSourceFactory::finalize();

  RasterFactory::finalize();

//! free GDAL registered drivers
  te::da::DataSourceManager::getInstance().detachAll(GDAL_DRIVER_IDENTIFIER);
 
  TE_LOG_TRACE(TR_GDAL("TerraLib GDAL driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::gdal::Module)

