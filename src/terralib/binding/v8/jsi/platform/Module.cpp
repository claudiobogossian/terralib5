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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib JavaScript-API module entry.
 */

// TerraLib
#include "../../../../common/Logger.h"
#include "../../../../common/TerraLib.h"
#include "../../../../common/Translator.h"
#include "../Config.h"
#include "Module.h"

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib V8-API is loaded.
//
//  \note This method doesn't perform operations.
// */
//static void TeV8JSIInitialize()
//{
//  TE_LOG_TRACE(TR_V8JSI("TerraLib's Google JavaScript V8 engine module initialized!"));
//}
//
///*!
//  \brief This is a helper function that will be automatically called when the TerraLib V8-API is unloaded.
//
//  \note This method doesn't perform operations.
// */
//static void TeV8JSIFinalize()
//{
//  TE_LOG_TRACE(TR_V8JSI("TerraLib's Google JavaScript V8 engine module finalized!"));
//}

const te::v8::jsi::Module& sm_module = te::v8::jsi::Module::getInstance();

te::v8::jsi::Module::Module()
{
  TerraLib::Module m = { TE_V8_JSI_MODULE_NAME,
                         te::v8::jsi::Module::initialize,
                         te::v8::jsi::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib V8 Plugin support
 TE_ADD_TEXT_DOMAIN(TE_V8JSI_TEXT_DOMAIN, TE_V8JSI_TEXT_DOMAIN_DIR, "UTF-8");
}

te::v8::jsi::Module::~Module()
{
  TerraLib::getInstance().remove(TE_V8_JSI_MODULE_NAME);
}

void te::v8::jsi::Module::initialize()
{
  TE_LOG_TRACE(TR_V8JSI("TerraLib's Google JavaScript V8 engine module initialized!"));
}

void te::v8::jsi::Module::finalize()
{
  TE_LOG_TRACE(TR_V8JSI("TerraLib's Google JavaScript V8 engine module finalized!"));
}

//// TerraLib
//#include "../../../../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_V8_JSI_MODULE_NAME, TeV8JSIInitialize, TeV8JSIFinalize)


