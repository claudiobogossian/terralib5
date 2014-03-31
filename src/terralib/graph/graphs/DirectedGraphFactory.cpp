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
  \file DirectedGraphFactory.h

  \brief This is the concrete factory for the directed Graph type.
*/

// TerraLib
#include "../core/GraphMetadata.h"
#include "../Exception.h"
#include "../Globals.h"
#include "DirectedGraph.h"
#include "DirectedGraphFactory.h"


// STL
#include <memory>

te::graph::DirectedGraphFactory* te::graph::DirectedGraphFactory::sm_factory(0);

const std::string& te::graph::DirectedGraphFactory::getType() const
{
  return Globals::sm_factoryGraphTypeDirectedGraph;
}

void te::graph::DirectedGraphFactory::getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const
{
  params.push_back(std::pair<std::string, std::string>("GRAPH_DATA_SOURCE_TYPE", ""));
  params.push_back(std::pair<std::string, std::string>("GRAPH_ID", ""));
  params.push_back(std::pair<std::string, std::string>("GRAPH_NAME", ""));
  params.push_back(std::pair<std::string, std::string>("GRAPH_DESCRIPTION", ""));
  params.push_back(std::pair<std::string, std::string>("GRAPH_STORAGE_MODE", ""));
  params.push_back(std::pair<std::string, std::string>("GRAPH_STRATEGY_LOADER", ""));
  params.push_back(std::pair<std::string, std::string>("GRAPH_CACHE_POLICY", ""));
}

void te::graph::DirectedGraphFactory::initialize()
{
  finalize();
  sm_factory = new DirectedGraphFactory;
}

void te::graph::DirectedGraphFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::graph::DirectedGraphFactory::DirectedGraphFactory()
  : te::graph::AbstractGraphFactory(Globals::sm_factoryGraphTypeDirectedGraph)
{
}

te::graph::AbstractGraph* te::graph::DirectedGraphFactory::iOpen(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  //create graph metadata
  te::graph::GraphMetadata* gMetadata = getMetadata(dsInfo, gInfo);

  if(gMetadata->m_memoryGraph)
    return 0;

  //get graph id
  int id = getId(dsInfo, gInfo);

  try
  {
    gMetadata->load(id);
  }
  catch(const std::exception& e)
  {
    std::string errorMessage = TE_TR("Error opening graph metadata: ");
    errorMessage += e.what();

    throw Exception(errorMessage);
  }

  //create cache policy strategy
  te::graph::AbstractCachePolicy* cp = getCachePolicy(gInfo);

  //create graph strategy
  te::graph::AbstractGraphLoaderStrategy* ls = getLoaderStrategy(gInfo, gMetadata);

  //create graph
  te::graph::AbstractGraph* g = new te::graph::DirectedGraph(cp, ls);

  return g;
}

te::graph::AbstractGraph* te::graph::DirectedGraphFactory::create(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  //create graph metadata
  te::graph::GraphMetadata* gMetadata = getMetadata(dsInfo, gInfo);

  setMetadataInformation(gInfo, gMetadata);

  try
  {
    gMetadata->save();
  }
  catch(const std::exception& e)
  {
    std::string errorMessage = TE_TR("Error saving graph metadata: ");
    errorMessage += e.what();

    throw Exception(errorMessage);
  }

  //create cache policy strategy
  te::graph::AbstractCachePolicy* cp = getCachePolicy(gInfo);

  //create graph strategy
  te::graph::AbstractGraphLoaderStrategy* ls = getLoaderStrategy(gInfo, gMetadata);

  //create graph
  te::graph::AbstractGraph* g = 0;

  if(gMetadata->m_memoryGraph)
    g = new te::graph::DirectedGraph(gMetadata);
  else
    g = new te::graph::DirectedGraph(cp, ls);

  return g;
}

te::graph::AbstractGraph* te::graph::DirectedGraphFactory::build()
{
  return new Graph;
}