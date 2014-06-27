/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/graph/Module.cpp
   
  \brief This singleton defines the TerraLib Graph module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "cache/FIFOCachePolicyFactory.h"
#include "cache/LFUCachePolicyFactory.h"
#include "graphs/BidirectionalGraphFactory.h"
#include "graphs/DirectedGraphFactory.h"
#include "graphs/GraphFactory.h"
#include "graphs/UndirectedGraphFactory.h"
#include "loader/BoxLoaderStrategyFactory.h"
#include "loader/SequenceLoaderStrategyFactory.h"
#include "Config.h"
#include "Module.h"

const te::graph::Module& sm_module = te::graph::Module::getInstance();

te::graph::Module::Module()
{
  TerraLib::Module m = { TE_GRAPH_MODULE_NAME,
                         te::graph::Module::initialize,
                         te::graph::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::graph::Module::~Module()
{
  TerraLib::getInstance().remove(TE_GRAPH_MODULE_NAME);
}

void te::graph::Module::initialize()
{
  // cache factories
  FIFOCachePolicyFactory::initialize();
  LFUCachePolicyFactory::initialize();

  // loader strategy factories
  BoxLoaderStrategyFactory::initialize();
  SequenceLoaderStrategyFactory::initialize();

  // graph factories
  GraphFactory::initialize();
  BidirectionalGraphFactory::initialize();
  DirectedGraphFactory::initialize();
  UndirectedGraphFactory::initialize();

  TE_LOG_TRACE(TE_TR("TerraLib Graph module initialized!"));
}

void te::graph::Module::finalize()
{
  // cache factories
  FIFOCachePolicyFactory::finalize();
  LFUCachePolicyFactory::finalize();

  // loader strategy factories
  BoxLoaderStrategyFactory::finalize();
  SequenceLoaderStrategyFactory::finalize();

  // graph factories
  GraphFactory::finalize();
  BidirectionalGraphFactory::finalize();
  DirectedGraphFactory::finalize();
  UndirectedGraphFactory::finalize();

  TE_LOG_TRACE(TE_TR("TerraLib Graph module finalized!"));
}
