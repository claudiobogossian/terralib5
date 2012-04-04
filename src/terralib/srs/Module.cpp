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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Spatial Reference System module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "Config.h"
#include "CoordinateSystemEPSGFactory.h"
#ifdef TE_USE_PROJ4
#include "CoordinateSystemProj4Factory.h"
#endif
#include "CoordinateSystemWKTFactory.h"
#include "Module.h"

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib SRS module is loaded.
//
//  \note This function doesn't perform operations for this module.
// */
//static void TeSRSInitialize()
//{
//  te::srs::CoordinateSystemEPSGFactory::initialize();
//  te::srs::CoordinateSystemWKTFactory::initialize();
//
//#ifdef TE_USE_PROJ4
//  te::srs::CoordinateSystemProj4Factory::initialize();
//#endif
//
//  TE_LOG_TRACE(TR_SRS("TerraLib SRS initialized!"));
//}

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib SRS module is unloaded.
//
//  \note This function doesn't perform operations for this module.
// */
//static void TeSRSFinalize()
//{
//#ifdef TE_USE_PROJ4
//  te::srs::CoordinateSystemProj4Factory::finalize();
//#endif
//
//  te::srs::CoordinateSystemWKTFactory::finalize();
//  te::srs::CoordinateSystemEPSGFactory::finalize();
//
//  TE_LOG_TRACE(TR_SRS("TerraLib SRS Finalized!"));
//}

const te::srs::Module& sm_module = te::srs::Module::getInstance();

te::srs::Module::Module()
{
  TerraLib::Module m = { TE_SRS_MODULE_NAME,
                         te::srs::Module::initialize,
                         te::srs::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib SRS module
 TE_ADD_TEXT_DOMAIN(TE_SRS_TEXT_DOMAIN, TE_SRS_TEXT_DOMAIN_DIR, "UTF-8");
}

te::srs::Module::~Module()
{
  TerraLib::getInstance().remove(TE_SRS_MODULE_NAME);
}

void te::srs::Module::initialize()
{
  te::srs::CoordinateSystemEPSGFactory::initialize();
  te::srs::CoordinateSystemWKTFactory::initialize();

#ifdef TE_USE_PROJ4
  te::srs::CoordinateSystemProj4Factory::initialize();
#endif

  TE_LOG_TRACE(TR_SRS("TerraLib SRS initialized!"));
}

void te::srs::Module::finalize()
{
#ifdef TE_USE_PROJ4
  te::srs::CoordinateSystemProj4Factory::finalize();
#endif

  te::srs::CoordinateSystemWKTFactory::finalize();
  te::srs::CoordinateSystemEPSGFactory::finalize();

  TE_LOG_TRACE(TR_SRS("TerraLib SRS Finalized!"));
}

//// TerraLib
//#include "../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_SRS_MODULE_NAME, TeSRSInitialize, TeSRSFinalize)


