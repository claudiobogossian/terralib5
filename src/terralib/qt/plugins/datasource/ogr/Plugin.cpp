/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/plugins/datasource/ogr/Plugin.cpp

  \brief Plugin implementation for the OGR data source widget.
*/

// TerraLib
#include "../../../../common/Config.h"
#include "../../../../common/Translator.h"
#include "../../../../common/Logger.h"
#include "../../../widgets/datasource/core/DataSourceManager.h"
#include "../../../widgets/datasource/core/DataSourceTypeManager.h"
#include "OGRType.h"
#include "Plugin.h"

te::qt::plugins::ogr::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::qt::plugins::ogr::Plugin::~Plugin() 
{
}

void te::qt::plugins::ogr::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::widgets::DataSourceTypeManager::getInstance().add(new OGRType);

// it initializes the Translator support for the TerraLib PostGIS driver support
  TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_DATASOURCE_OGR_TEXT_DOMAIN, TE_QT_PLUGIN_DATASOURCE_OGR_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_QT_PLUGIN_DATASOURCE_OGR("TerraLib Qt OGR widget startup!"));

  m_initialized = true;
}

void te::qt::plugins::ogr::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te::qt::widgets::DataSourceManager::getInstance().removeByType("OGR");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("OGR");

  TE_LOG_TRACE(TE_QT_PLUGIN_DATASOURCE_OGR("TerraLib Qt OGR widget shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::ogr::Plugin)
