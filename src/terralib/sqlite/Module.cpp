/*  Copyright (C) 2009-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/Module.cpp
   
  \brief The TerraLib SQLite Data Access driver implements the plugin interface.
*/

// TerraLib
#include "Module.h"

// Boost
#include <boost/filesystem.hpp>

// SQLite
#include <sqlite3.h>

#ifdef TE_ENABLE_SPATIALITE
// SpatiaLite
#include <spatialite.h>
#endif


te::sqlite::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::sqlite::Module::~Module()
{
}

void te::sqlite::Module::startup() 
{
  if(m_initialized)
    return;

  sqlite3_initialize();
  
#ifdef TE_ENABLE_SPATIALITE
  spatialite_init(0);
#endif

  m_initialized = true;
}

void te::sqlite::Module::shutdown() 
{
  if(!m_initialized)
    return;

  sqlite3_shutdown();

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::sqlite::Module)

