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
   
  \brief This singleton defines the TerraLib's Google JavaScript V8 Common Runtime module entry.
 */

// TerraLib
#include "../../../common/Logger.h"
#include "../../../common/TerraLib.h"
#include "../../../common/Translator.h"
#include "Config.h"
#include "JsContextManager.h"
#include "Module.h"

// Google V8
#include <v8.h>

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib V8 Common Rutime module is loaded.
//
//  \note This method doesn't perform operations.
// */
//static void Tev8CommonInitialize()
//{
//  TE_LOG_TRACE(TR_V8COMMON("The TerraLib's Google JavaScript V8 Common Runtime module initialized!"));
//}
//
///*!
//  \brief This is a helper function that will be automatically called when the TerraLib V8 Common Rutime module is unloaded.
//
//  \note This method doesn't perform operations.
// */
//static void TeV8CommonFinalize()
//{
//  TE_LOG_TRACE(TR_V8COMMON("The TerraLib's Google JavaScript V8 Common Runtime module finalized!"));
//}

const te::v8::common::Module& sm_module = te::v8::common::Module::getInstance();

te::v8::common::Module::Module()
{
  TerraLib::Module m = { TE_V8_COMMON_MODULE_NAME,
                         te::v8::common::Module::initialize,
                         te::v8::common::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib V8 Common Runtime support
 TE_ADD_TEXT_DOMAIN(TE_V8COMMON_TEXT_DOMAIN, TE_V8COMMON_TEXT_DOMAIN_DIR, "UTF-8");

// it initializes the JVMManager singleton
  JsContextManager::getInstance();
}

te::v8::common::Module::~Module()
{
// it clears class cache managed by the singleton JsContextManager
  JsContextManager::getInstance().clear();

// call V8 dispose...
  ::v8::V8::Dispose();

  TerraLib::getInstance().remove(TE_V8_COMMON_MODULE_NAME);
}

void te::v8::common::Module::initialize()
{
  TE_LOG_TRACE(TR_V8COMMON("The TerraLib's Google JavaScript V8 Common Runtime module initialized!"));
}

void te::v8::common::Module::finalize()
{
  TE_LOG_TRACE(TR_V8COMMON("The TerraLib's Google JavaScript V8 Common Runtime module finalized!"));
}

//// TerraLib
//#include "../../../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_V8_COMMON_MODULE_NAME, Tev8CommonInitialize, TeV8CommonFinalize)


