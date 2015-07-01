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
   
  \brief This singleton defines the TerraLib ST module entry.  
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"

#include "loader/STDataLoaderFromDSFactory.h"
#include "loader/STDataLoaderFromMemDSFactory.h"
#include "Config.h"
#include "Module.h"
#include "Globals.h"

const te::st::Module& sm_module = te::st::Module::getInstance();

te::st::Module::Module()
{
  TerraLib::Module m = { TE_ST_MODULE_NAME,
                         te::st::Module::initialize,
                         te::st::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::st::Module::~Module()
{
  TerraLib::getInstance().remove(TE_ST_MODULE_NAME);
}

void te::st::Module::initialize()
{
  // it initializes the st data loader factories
  te::st::STDataLoaderFromDSFactory::initialize();
  te::st::STDataLoaderFromMemDSFactory::initialize();

  TE_LOG_TRACE(TE_TR("TerraLib ST module initialized!"));
}

void te::st::Module::finalize()
{
  // it finalizes the st data loader factory support.
  te::st::STDataLoaderFromDSFactory::finalize();
  te::st::STDataLoaderFromMemDSFactory::finalize();

  TE_LOG_TRACE(TE_TR("TerraLib ST module finalized!"));
}

