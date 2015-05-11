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
  \file Platform.h
   
  \brief A utility class to initialize and terminate TerraLib GRIB driver support.  
 */

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "RasterFactory.h"
#include "Platform.h"

bool te::grib::Platform::sm_gribInitialized = false;

te::grib::Platform::Platform(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::CppPlugin(pluginInfo)
{
}

te::grib::Platform::~Platform()
{
}

void te::grib::Platform::startup()
{
  te::grib::Platform::initialize();
}

void te::grib::Platform::shutdown()
{
  te::grib::Platform::finalize();
}

void te::grib::Platform::initialize()
{
  if(sm_gribInitialized)
    return;

// it initializes the Translator support for the TerraLib GRIB driver support
  TE_ADD_TEXT_DOMAIN(TE_GRIB_TEXT_DOMAIN, TE_GRIB_TEXT_DOMAIN_DIR, "UTF-8");

// it initializes the GRIB Factory support
  te::grib::RasterFactory::initialize();

// if we are here... the common is initialized!
  sm_gribInitialized = true;
}

void te::grib::Platform::finalize()
{
  if(!sm_gribInitialized)
    return;

// it finalizes the GRIB factory support.
  te::grib::RasterFactory::finalize();
 
  sm_gribInitialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::grib::Platform)

