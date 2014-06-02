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
  \file terralib/gml/Module.cpp

  \brief This singleton defines the TerraLib GML module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Module.h"

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib GML module is loaded.
//
//  \note This function doesn't perform operations for this module.
//*/
//static void TeGmlInitialize()
//{
//  TE_LOG_TRACE(TR_GML("TerraLib GML module initialized!"));
//}
//
///*!
//  \brief This is a helper function that will be automatically called when the TerraLib GML module is unloaded.
//
//  \note This function doesn't perform operations for this module.
//*/
//static void TeGmlFinalize()
//{
//  TE_LOG_TRACE(TR_GML("TerraLib GML module finalized!"));
//}

const te::gml::Module& sm_module = te::gml::Module::getInstance();

te::gml::Module::Module()
{
  TerraLib::Module m = { TE_GML_MODULE_NAME,
                         te::gml::Module::initialize,
                         te::gml::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::gml::Module::~Module()
{
  TerraLib::getInstance().remove(TE_GML_MODULE_NAME);
}

void te::gml::Module::initialize()
{
  TE_LOG_TRACE(TE_TR("TerraLib GML module initialized!"));
}

void te::gml::Module::finalize()
{
  TE_LOG_TRACE(TE_TR("TerraLib GML module finalized!"));
}

//// TerraLib
//#include "../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_GML_MODULE_NAME, TeGmlInitialize, TeGmlFinalize)

