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
  \file LoadModules.h
 
  \brief Load terralib modules.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_LOADMODULES_H
#define __TERRALIB_UNITTEST_DATAACCESS_LOADMODULES_H

#include <terralib/common.h>
#include <terralib/plugin.h>
#include "Config.h"

/*!
  \brief Load terralib modules.
 */
void LoadModules()
{
  try
  {   
    te::plugin::PluginInfo* info;
      
    info = te::plugin::GetInstalledPlugin(PLUGINS_PATH + std::string("/plugin_ogr_info.xml"));
    te::plugin::PluginManager::getInstance().add(info); 

    info = te::plugin::GetInstalledPlugin(PLUGINS_PATH + std::string("/plugin_gdal_info.xml"));
    te::plugin::PluginManager::getInstance().add(info);
       
    info = te::plugin::GetInstalledPlugin(PLUGINS_PATH + std::string("/plugin_pgis_info.xml"));
    te::plugin::PluginManager::getInstance().add(info);       
  
    te::plugin::PluginManager::getInstance().loadAll(); 
  }
  catch(const te::common::Exception& e)
  {
    throw e;
  }  

};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_LOADMODULES_H
