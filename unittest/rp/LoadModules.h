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

#ifndef __TERRALIB_UNITTEST_RP_LOADMODULES_H
#define __TERRALIB_UNITTEST_RP_LOADMODULES_H

#include <terralib/common.h>
#include <terralib/plugin.h>
#include <terralib/Config.h>

/*!
  \brief Load terralib modules.
 */
void LoadModules()
{
  {
    #if TE_USE_OGR == 1
      te::plugin::PluginInfo info;
      info.m_name = "OGR DataSource Driver";
      info.m_description = "This data source driver supports...";

      #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        info.m_type = "dll";
        #ifdef NDEBUG
              info.m_mainFile = "terralib_ogr.dll";
        #else
              info.m_mainFile = "terralib_ogr_d.dll";
        #endif
      #elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
        #ifdef NDEBUG
          info.m_type = "s.o.";
          info.m_mainFile = "libterralib_ogr.so";
        #else
          info.m_type = "s.o.";
          info.m_mainFile = "libterralib_ogr_d.so";
        #endif
      #elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
        info.m_type = "dylib";
        info.m_mainFile = "libterralib_ogr.dylib";
      #else
        #error "Platform not supported yet"
      #endif

      te::plugin::PluginManager::getInstance().loadPlugin(info);
    #endif
  }

  {
    #if TE_USE_GDAL
      te::plugin::PluginInfo info;
      info.m_name = "GDAL DataSource Driver";
      info.m_description = "This data source driver supports...";

      #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        info.m_type = "dll";
        #ifdef NDEBUG
              info.m_mainFile = "terralib_gdal.dll";
        #else
              info.m_mainFile = "terralib_gdal_d.dll";
        #endif            
      #elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
        #ifdef NDEBUG
          info.m_type = "s.o.";
          info.m_mainFile = "libterralib_gdal.so";
        #else
          info.m_type = "s.o.";
          info.m_mainFile = "libterralib_gdal_d.so";
        #endif
      #elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
        info.m_type = "dylib";
        info.m_mainFile = "libterralib_gdal.dylib";
      #else
        #error "Platform not supported yet"
      #endif

      te::plugin::PluginManager::getInstance().loadPlugin(info);
    #endif
  }

};

#endif  // __TERRALIB_UNITTEST_RP_LOADMODULES_H
