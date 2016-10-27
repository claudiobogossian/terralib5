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
  \file terralib/plugin/Module.cpp

  \brief This singleton defines the TerraLib Plugin module entry.
*/

// TerraLib
#include "../core/logger/Logger.h"
#include "../common/TerraLib.h"
#include "../core/translator/Translator.h"
#include "AbstractPlugin.h"
#include "CppPluginEngineFactory.h"
#include "Module.h"
#include "PluginInfo.h"
#include "PluginManager.h"
#include "Utils.h"

const te::plugin::Module& sm_module = te::plugin::Module::getInstance();

te::plugin::Module::Module()
{
  TerraLib::Module m = { TE_PLUGIN_MODULE_NAME,
                         te::plugin::Module::initialize,
                         te::plugin::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// just to make our singleton alive!
  PluginManager::getInstance();
}

te::plugin::Module::~Module()
{

}

void te::plugin::Module::initialize()
{

}

void te::plugin::Module::finalize()
{

}



