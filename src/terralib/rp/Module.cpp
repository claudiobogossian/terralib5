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
  \file terralib/rp/Module.cpp
   
  \brief This singleton defines the TerraLib Raster Processing module entry.
 */

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Module.h"

/*!
  \brief This is a helper function that will be automatically called when the TerraLib DataType module is loaded.

  \note This method doesn't perform operations.
 */
//static void TeRasterProcessingInitialize()
//{
//  TE_LOG_TRACE(TR_RP("TerraLib Raster Processing module initialized!"));
//}

/*!
  \brief This is a helper function that will be automatically called when the TerraLib Raster Processing module is unloaded.

  \note This method doesn't perform operations.
 */
//static void TeRasterProcessingFinalize()
//{
//  TE_LOG_TRACE(TR_RP("TerraLib Raster Processing module finalized!"));
//}

const te::rp::Module& sm_module = te::rp::Module::getInstance();

te::rp::Module::Module()
{
  TerraLib::Module m = { TE_RP_MODULE_NAME,
                         te::rp::Module::initialize,
                         te::rp::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Data Type support
  TE_ADD_TEXT_DOMAIN(TE_RP_TEXT_DOMAIN, TE_RP_TEXT_DOMAIN_DIR, "UTF-8");
}

te::rp::Module::~Module()
{
  TerraLib::getInstance().remove(TE_RP_MODULE_NAME);
}

void te::rp::Module::initialize()
{
  TE_LOG_TRACE(TR_RP("TerraLib Raster Processing module initialized!"));
}

void te::rp::Module::finalize()
{
  TE_LOG_TRACE(TR_RP("TerraLib Raster Processing module finalized!"));
}

// TerraLib
//#include "../common/ModuleUtils.h"

//TE_REGISTER_MODULE(TE_RP_MODULE_NAME, TeRasterProcessingInitialize, TeRasterProcessingFinalize)

