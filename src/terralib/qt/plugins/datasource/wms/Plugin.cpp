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
  \file terralib/qt/plugins/datasource/wms/Plugin.cpp

  \brief Plugin implementation for the OGC Web Map Service (WMS) data source widget.
*/

// TerraLib
#include "../../../../common/Config.h"
#include "../../../../common/Translator.h"
#include "../../../../common/Logger.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../widgets/datasource/core/DataSourceTypeManager.h"
#include "WMSType.h"
#include "Plugin.h"

te::qt::plugins::wms::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::qt::plugins::wms::Plugin::~Plugin() 
{
}

void te::qt::plugins::wms::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::widgets::DataSourceTypeManager::getInstance().add(new WMSType);

// it initializes the Translator support for the TerraLib PostGIS driver support
  TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_DATASOURCE_WMS_TEXT_DOMAIN, TE_QT_PLUGIN_DATASOURCE_WMS_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGC Web Map Service (WMS) widget startup!"));

  m_initialized = true;
}

void te::qt::plugins::wms::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te::da::DataSourceInfoManager::getInstance().removeByType("WMS");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("WMS");

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGC Web Map Service (WMS) widget shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::wms::Plugin)
