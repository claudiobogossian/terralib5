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
  \file MemoryGraphFactory.cpp

  \brief This is the concrete factory for the memory Graph type.
*/

// TerraLib
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../drivers/memory/MemoryGraphMetadata.h"
#include "../Exception.h"
#include "../Globals.h"
#include "MemoryGraph.h"
#include "MemoryGraphFactory.h"

// STL
#include <memory>

te::graph::MemoryGraphFactory* te::graph::MemoryGraphFactory::sm_factory(0);

const std::string& te::graph::MemoryGraphFactory::getType() const
{
  return Globals::sm_factoryGraphTypeMemoryGraph;
}

void te::graph::MemoryGraphFactory::getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const
{
  params.push_back(std::pair<std::string, std::string>("GRAPH_MEMORY", ""));
  params.push_back(std::pair<std::string, std::string>("GRAPH_NAME", ""));
  params.push_back(std::pair<std::string, std::string>("GRAPH_DESCRIPTION", ""));
}

void te::graph::MemoryGraphFactory::initialize()
{
  finalize();
  sm_factory = new MemoryGraphFactory;
}

void te::graph::MemoryGraphFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::graph::MemoryGraphFactory::MemoryGraphFactory()
  : te::graph::AbstractGraphFactory(Globals::sm_factoryGraphTypeMemoryGraph)
{
}

te::graph::AbstractGraph* te::graph::MemoryGraphFactory::iOpen(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  return 0;
}

te::graph::AbstractGraph* te::graph::MemoryGraphFactory::create(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  std::map<std::string, std::string>::const_iterator it;
  std::map<std::string, std::string>::const_iterator itend = gInfo.end();

   //check memory parameter
  it = gInfo.find("GRAPH_MEMORY");

  if(it == itend || it->second != "TRUE")
  {
    return 0;
  }

  //create graph metadata
  te::graph::GraphMetadata* gMetadata = new te::graph::MemoryGraphMetadata();

  it = gInfo.find("GRAPH_NAME");
  if(it != itend)
  {
    gMetadata->setName(it->second);
  }

  it = gInfo.find("GRAPH_DESCRIPTION");
  if(it != itend)
  {
    gMetadata->setDescription(it->second);
  }

  //create graph
  te::graph::AbstractGraph* g = new te::graph::MemoryGraph(gMetadata);

  return g;
}

te::graph::AbstractGraph* te::graph::MemoryGraphFactory::build()
{
  return new MemoryGraph;
}