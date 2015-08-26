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
  \file unittest/rp/LoadModules.cpp
 
  \brief Load terralib modules.
 */

#ifndef __TERRALIB_UNITTEST_RP_LOADMODULES_H
#define __TERRALIB_UNITTEST_RP_LOADMODULES_H

#include <terralib/common.h>
#include <terralib/plugin.h>
#include <terralib/BuildConfig.h>

/*!
  \brief Load TerraLib modules.
 */

void LoadModules()
{
  std::string plugins_path = te::common::FindInTerraLibPath("share/terralib/plugins");

  te::plugin::PluginInfo* info;
  
#ifdef TERRALIB_MOD_OGR_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.ogr.teplg");
  te::plugin::PluginManager::getInstance().add(info); 
#endif

#ifdef TERRALIB_MOD_GDAL_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.gdal.teplg");
  te::plugin::PluginManager::getInstance().add(info);
#endif

  te::plugin::PluginManager::getInstance().loadAll();  
}

#endif  // __TERRALIB_UNITTEST_RP_LOADMODULES_H
