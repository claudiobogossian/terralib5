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
  \file AbstractGraphFactory.cpp

  \brief This is the abstract factory for Graphs.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Exception.h"
#include "AbstractGraphFactory.h"
#include "GraphMetadata.h"

// STL
#include <memory>

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::make()
{
  return te::common::AbstractFactory<AbstractGraph, std::string>::make(TE_DEFAULT_GRAPH_TYPE);
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::make(const std::string& gType)
{
  std::string ucase = te::common::Convert2UCase(gType);

  return te::common::AbstractFactory<AbstractGraph, std::string>::make(ucase);
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::make(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  return make(TE_DEFAULT_GRAPH_TYPE, dsInfo, gInfo);
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::make(const std::string& gType, const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  std::string ucase = te::common::Convert2UCase(gType);

  te::common::AbstractFactory<AbstractGraph, std::string>::dictionary_type& d = te::common::AbstractFactory<AbstractGraph, std::string>::getDictionary();

  AbstractGraphFactory* f = static_cast<AbstractGraphFactory*>(d.find(ucase));

  if(f == 0)
    throw Exception(TR_GRAPH("Could not find concrete factory! Check if it was initialized!"));

  AbstractGraph* g = f->create(dsInfo, gInfo);

  g->getMetadata()->setType(ucase);

  return g;
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::open(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  return open(TE_DEFAULT_GRAPH_TYPE, dsInfo, gInfo);
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::open(const std::string& gType, const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  std::string ucase = te::common::Convert2UCase(gType);

  te::common::AbstractFactory<AbstractGraph, std::string>::dictionary_type& d = te::common::AbstractFactory<AbstractGraph, std::string>::getDictionary();

  AbstractGraphFactory* f = static_cast<AbstractGraphFactory*>(d.find(ucase));

  if(f == 0)
    throw Exception(TR_GRAPH("Could not find concrete factory! Check if it was initialized!"));

  AbstractGraph* g = f->iOpen(dsInfo, gInfo);

  g->getMetadata()->setType(ucase);

  return g;
}

te::graph::AbstractGraphFactory::AbstractGraphFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::graph::AbstractGraph, std::string>(factoryKey)
{
}



