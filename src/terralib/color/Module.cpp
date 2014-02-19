/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/color/Module.cpp

  \brief This singleton defines the TerraLib Color module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "ColorSchemeCatalogManager.h"
#include "Config.h"
#include "Module.h"

const te::color::Module& sm_module = te::color::Module::getInstance();

te::color::Module::Module()
{
  TerraLib::Module m = { TE_COLOR_MODULE_NAME,
                         te::color::Module::initialize,
                         te::color::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Color module
 TE_ADD_TEXT_DOMAIN(TE_COLOR_TEXT_DOMAIN, TE_COLOR_TEXT_DOMAIN_DIR, "UTF-8");
}

te::color::Module::~Module()
{
  TerraLib::getInstance().remove(TE_COLOR_MODULE_NAME);
}

void te::color::Module::initialize()
{
#ifdef TE_AUTOMATIC_INITIALIZATION
  te::color::ColorSchemeCatalogManager::getInstance().init();
#endif

  TE_LOG_TRACE(TR_COLOR("TerraLib Color module initialized!"));
}

void te::color::Module::finalize()
{
  TE_LOG_TRACE(TR_COLOR("TerraLib Color module finalized!"));
}

