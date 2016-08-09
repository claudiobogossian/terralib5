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
  \file terralib/ws/ogc/wms/qtplugin/Plugin.cpp

  \brief Plugin implementation for the OGC Web Coverage Service (WMS) data source widget.

  \author Emerson Moraes
*/

#include "Plugin.h"

#include <iostream>

#include "../../../../core/translator/Translator.h"
#include "../../../../core/logger/Logger.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../qt/widgets/datasource/core/DataSourceTypeManager.h"
#include "../qt/WMSType.h"


te::ws::ogc::wms::qtplugin::Plugin::Plugin(const te::plugin::PluginInfo &pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::ws::ogc::wms::qtplugin::Plugin::~Plugin()
{
}

void te::ws::ogc::wms::qtplugin::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::widgets::DataSourceTypeManager::getInstance().add(new te::ws::ogc::wms::qt::WMSType);

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGC Web Map Service (WMS) widget startup!"));

  m_initialized = true;
}

void te::ws::ogc::wms::qtplugin::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te::da::DataSourceInfoManager::getInstance().removeByType("WMS2");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("WMS2");

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGC Web Ḿap Service (WMS) widget shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::ws::ogc::wms::qtplugin::Plugin)
