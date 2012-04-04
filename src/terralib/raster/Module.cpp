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
  \file terralib/raster/Module.cpp
   
  \brief This singleton defines the TerraLib Raster module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Module.h"

const te::rst::Module& sm_module = te::rst::Module::getInstance();

te::rst::Module::Module()
{
  TerraLib::Module m = { TE_RASTER_MODULE_NAME,
                         te::rst::Module::initialize,
                         te::rst::Module::finalize
                       };

// Initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// It initializes the Translator support for the TerraLib Data Type support
  TE_ADD_TEXT_DOMAIN(TE_RASTER_TEXT_DOMAIN, TE_RASTER_TEXT_DOMAIN_DIR, "UTF-8");
}

te::rst::Module::~Module()
{
  TerraLib::getInstance().remove(TE_RASTER_MODULE_NAME);
}

void te::rst::Module::initialize()
{
  TE_LOG_TRACE(TR_RASTER("TerraLib Raster module initialized!"));
}

void te::rst::Module::finalize()
{
  TE_LOG_TRACE(TR_RASTER("TerraLib Raster module finalized!"));
}

