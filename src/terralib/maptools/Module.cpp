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
   
  \brief This singleton defines the TerraLib MapTools module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "AbstractLayerRendererFactory.h"
#include "DataSetLayerRendererFactory.h"
#include "Config.h"
#include "Module.h"
#include "QueryEncoder.h"
#include "QueryLayerRendererFactory.h"
#include "RasterLayerRendererFactory.h"

const te::map::Module& sm_module = te::map::Module::getInstance();

te::map::Module::Module()
{
  TerraLib::Module m = { TE_MAPTOOLS_MODULE_NAME,
                         te::map::Module::initialize,
                         te::map::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::map::Module::~Module()
{
  TerraLib::getInstance().remove(TE_MAPTOOLS_MODULE_NAME);
}

void te::map::Module::initialize()
{
// initialize renderers
  AbstractLayerRendererFactory::initialize();
  DataSetLayerRendererFactory::initialize();
  QueryLayerRendererFactory::initialize();
  RasterLayerRendererFactory::initialize();

  QueryEncoder::initialize();

  TE_LOG_TRACE(TE_TR("TerraLib Map Tools module initialized!"));
}

void te::map::Module::finalize()
{
// finalize renderers
  AbstractLayerRendererFactory::finalize();
  DataSetLayerRendererFactory::finalize();
  QueryLayerRendererFactory::finalize();
  RasterLayerRendererFactory::finalize();

  TE_LOG_TRACE(TE_TR("TerraLib Map Tools module finalized!"));
}

