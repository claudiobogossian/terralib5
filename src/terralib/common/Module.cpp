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
#include "PlatformUtils.h"
#include "TerraLib.h"
#include "Translator.h"
#include "UnitsOfMeasureManager.h"

// STL
#include <cstdlib>

#if TE_USE_GNU_TEXT_UTILITIES
// GNU Text Utilities -> it is not true for now!
#include <locale.h>
#endif

const te::common::Module& sm_module = te::common::Module::getInstance();

te::common::Module::Module()
{
  TerraLib::Module m = { TE_COMMON_MODULE_NAME,
                         te::common::Module::initialize,
                         te::common::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// let's start all singletons in the right order and other stuffs that must be in the static initialization!

// initialize the translator singleton and the common runtime multilanguage support
  TE_ADD_TEXT_DOMAIN(TE_COMMON_TEXT_DOMAIN, TE_COMMON_TEXT_DOMAIN_DIR, "UTF-8");

// initialize the singleton LibraryManager
  LibraryManager::getInstance();

// initialize the singleton UnitsOfMeasureManager
  UnitsOfMeasureManager::getInstance();

// let's start the logger if the developer wants TerraLib to make it automatically during static initialization
#if defined(TERRALIB_LOGGER_DO_AUTOMATIC_INITIALIZATION) && defined(TERRALIB_LOGGER_DO_STATIC_INITIALIZATION)
  std::string loggerConfFile = FindInTerraLibPath(TERRALIB_LOGGER_DEFAULT_CONFIGURATION_FILE);
  te::common::Logger::initialize(TERRALIB_LOGGER_DEFAULT_NAME, TERRALIB_LOGGER_DEFAULT_CONFIG_FILE_TYPE, loggerConfFile);
#endif
}

te::common::Module::~Module()
{
#if defined(TERRALIB_LOGGER_DO_AUTOMATIC_INITIALIZATION) && defined(TERRALIB_LOGGER_DO_STATIC_INITIALIZATION)
  te::common::Logger::finalize(TERRALIB_LOGGER_DEFAULT_NAME);
#endif

  TerraLib::getInstance().remove(TE_COMMON_MODULE_NAME);
}

void te::common::Module::initialize()
{
// let's start the logger if developer want TerraLib to make it automatically during static initialization
#if defined(TERRALIB_LOGGER_DO_AUTOMATIC_INITIALIZATION) && !defined(TERRALIB_LOGGER_DO_STATIC_INITIALIZATION)
  std::string loggerConfFile = FindInTerraLibPath(TERRALIB_LOGGER_DEFAULT_CONFIGURATION_FILE);
  te::common::Logger::initialize(TERRALIB_LOGGER_DEFAULT_NAME, TERRALIB_LOGGER_DEFAULT_CONFIG_FILE_TYPE, loggerConfFile);
#endif

#ifdef TERRALIB_AUTOMATIC_INITIALIZATION
  UnitsOfMeasureManager::getInstance().init();
#endif

  TE_LOG_TRACE(TE_TR("TerraLib Common Runtime initialized!"));
}

void te::common::Module::finalize()
{
  TE_LOG_TRACE(TE_TR("TerraLib Common Runtime finalized!"));

#if defined(TERRALIB_LOGGER_DO_AUTOMATIC_INITIALIZATION) && !defined(TERRALIB_LOGGER_DO_STATIC_INITIALIZATION)
  te::common::Logger::finalize(TERRALIB_LOGGER_DEFAULT_NAME);
#endif
}


