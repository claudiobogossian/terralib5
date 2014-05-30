/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file AbstractGraphLoaderStrategyFactory.cpp

  \brief This is the abstract factory for graph loader strategy.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../Config.h"
#include "../Exception.h"
#include "AbstractGraphLoaderStrategyFactory.h"

// STL
#include <memory>


te::graph::AbstractGraphLoaderStrategy* te::graph::AbstractGraphLoaderStrategyFactory::make()
{
  return te::common::AbstractFactory<AbstractGraphLoaderStrategy, std::string>::make(TE_DEFAULT_GRAPH_LOADER_STRATEGY_TYPE);
}

te::graph::AbstractGraphLoaderStrategy* te::graph::AbstractGraphLoaderStrategyFactory::make(const std::string& lsType)
{
  std::string ucase = te::common::Convert2UCase(lsType);

  return te::common::AbstractFactory<AbstractGraphLoaderStrategy, std::string>::make(ucase);
}

te::graph::AbstractGraphLoaderStrategy* te::graph::AbstractGraphLoaderStrategyFactory::make(const std::string& lsType, GraphMetadata* gm)
{
  std::string ucase = te::common::Convert2UCase(lsType);

  te::common::AbstractFactory<AbstractGraphLoaderStrategy, std::string>::dictionary_type& d = te::common::AbstractFactory<AbstractGraphLoaderStrategy, std::string>::getDictionary();

  AbstractGraphLoaderStrategyFactory* f = static_cast<AbstractGraphLoaderStrategyFactory*>(d.find(ucase));

  if(f == 0)
    throw Exception(TE_TR("Could not find concrete factory! Check if it was initialized!"));

  AbstractGraphLoaderStrategy* g = f->create(gm);

  return g;
}

te::graph::AbstractGraphLoaderStrategyFactory::AbstractGraphLoaderStrategyFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::graph::AbstractGraphLoaderStrategy, std::string>(factoryKey)
{
}

