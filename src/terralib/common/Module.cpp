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
  \file terralib/common/Module.cpp
   
  \brief This singleton defines an entry in the Platform for the TerraLib Common Runtime module.
*/

// TerraLib
#include "Enums.h"
#include "LibraryManager.h"
#include "Logger.h"
#include "Module.h"
#include "TerraLib.h"
#include "Translator.h"
#include "UnitOfMeasure.h"
#include "UnitsOfMeasureManager.h"

// STL
#include <cstdlib>

#if TE_USE_GNU_TEXT_UTILITIES
// GNU Text Utilities -> it is not true for now!
#include <locale.h>
#endif

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Common Runtime module is loaded.
//
//  The initialization includes:
//  <ul>
//  <li>Logger initialization when automatic dynamic initialization is enabled.</li>
//  </ul>
//*/
//static void TeCommonInitialize()
//{
//// let's start the logger if developer want TerraLib to make it automatically during static initialization
//#if TE_LOGGER_DO_AUTOMATIC_INITIALIZATION && !TE_LOGGER_DO_STATIC_INITIALIZATION
//  TE_LOGGER_MAKE_DEFAULT_INITIALIZATION();
//#endif
//
//  TE_LOG_TRACE(TR_COMMON("TerraLib Common Runtime initialized!"));
//}

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Common Runtime module is unloaded.
//
//  The finalization includes:
//  <ul>
//  <li>Logger finalization when automatic dynamic initialization is enabled.</li>
//  </ul>
//*/
//static void TeCommonFinalize()
//{
//  TE_LOG_TRACE(TR_COMMON("TerraLib Common Runtime finalized!"));
//
//#if TE_LOGGER_DO_AUTOMATIC_INITIALIZATION && !TE_LOGGER_DO_STATIC_INITIALIZATION
//  TE_LOGGER_MAKE_DEFAULT_FINALIZATION();
//#endif
//}

const te::common::Module& sm_module = te::common::Module::getInstance();

te::common::Module::Module()
{
  TerraLib::Module m = { TE_COMMON_MODULE_NAME,
                         te::common::Module::initialize,
                         te::common::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

//
  setlocale(LC_ALL, "");

// let's start all singletons in the right order and other stuffs that must be in the static initialization!

// initialize the translator singleton and the common runtime multilanguage support
  TE_ADD_TEXT_DOMAIN(TE_COMMON_TEXT_DOMAIN, TE_COMMON_TEXT_DOMAIN_DIR, "UTF-8");

// initialize the singleton LibraryManager
  LibraryManager::getInstance();

// initialize the singleton CharSetManager
//  CharEncodingManager::getInstance();

// initialize the singleton CountryManager
//  CountryManager::getInstance();

// initialize the singleton LanguageManager
//  LanguageManager::getInstance();

// initialize the singleton UnitsOfMeasureManager
  UnitsOfMeasureManager::getInstance();
  std::vector<std::string> altnames;
  altnames.push_back("meter");
  altnames.push_back("metro");
  te::common::UnitOfMeasure* umeter = new te::common::UnitOfMeasure(te::common::UOM_Metre,"metre","m",te::common::Length,"SI standard unit for linear measures");
  te::common::UnitsOfMeasureManager::getInstance().insert(umeter, altnames);
  
  te::common::UnitOfMeasure* ukmeter = new te::common::UnitOfMeasure(te::common::UOM_Kilometre,"kilometre","km",te::common::Length, te::common::UOM_Metre,1000);
  te::common::UnitsOfMeasureManager::getInstance().insert(ukmeter);
  
  te::common::UnitOfMeasure* uradian = new te::common::UnitOfMeasure(te::common::UOM_Radian,"radian","rad",te::common::Angle,"SI standard unit for angular measures");
  te::common::UnitsOfMeasureManager::getInstance().insert(uradian);
  
  te::common::UnitOfMeasure* udegree = new te::common::UnitOfMeasure(te::common::UOM_Degree,"degree","deg",te::common::Angle,te::common::UOM_Radian,3.141592653589,0,0,180);
  te::common::UnitsOfMeasureManager::getInstance().insert(udegree);
  
  te::common::UnitOfMeasure* uunity = new te::common::UnitOfMeasure(te::common::UOM_Unity,"unity","unity",te::common::Scale,"EPSG standard unit for scale measures");
  te::common::UnitsOfMeasureManager::getInstance().insert(uunity);
  

// let's start the logger if the developer wants TerraLib to make it automatically during static initialization
#if TE_LOGGER_DO_AUTOMATIC_INITIALIZATION && TE_LOGGER_DO_STATIC_INITIALIZATION
  TE_LOGGER_MAKE_DEFAULT_INITIALIZATION();
#endif
}

te::common::Module::~Module()
{
#if TE_LOGGER_DO_AUTOMATIC_INITIALIZATION && TE_LOGGER_DO_STATIC_INITIALIZATION
   TE_LOGGER_MAKE_DEFAULT_FINALIZATION();
#endif

  TerraLib::getInstance().remove(TE_COMMON_MODULE_NAME);
}

void te::common::Module::initialize()
{
// let's start the logger if developer want TerraLib to make it automatically during static initialization
#if TE_LOGGER_DO_AUTOMATIC_INITIALIZATION && !TE_LOGGER_DO_STATIC_INITIALIZATION
  TE_LOGGER_MAKE_DEFAULT_INITIALIZATION();
#endif

  TE_LOG_TRACE(TR_COMMON("TerraLib Common Runtime initialized!"));
}

void te::common::Module::finalize()
{
  TE_LOG_TRACE(TR_COMMON("TerraLib Common Runtime finalized!"));

#if TE_LOGGER_DO_AUTOMATIC_INITIALIZATION && !TE_LOGGER_DO_STATIC_INITIALIZATION
  TE_LOGGER_MAKE_DEFAULT_FINALIZATION();
#endif
}

//// TerraLib
//#include "ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_COMMON_MODULE_NAME, TeCommonInitialize, TeCommonFinalize)
//
