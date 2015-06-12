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
  \file BoxLoaderStrategyFactory.cpp

  \brief This is the concrete factory for the database box loader strategy.
*/

// TerraLib
#include "../Globals.h"
#include "BoxLoaderStrategy.h"
#include "BoxLoaderStrategyFactory.h"

// STL
#include <memory>

te::graph::BoxLoaderStrategyFactory* te::graph::BoxLoaderStrategyFactory::sm_factory(0);

const std::string& te::graph::BoxLoaderStrategyFactory::getType() const
{
  return Globals::sm_factoryLoaderStrategyTypeBox;
}

void te::graph::BoxLoaderStrategyFactory::initialize()
{
  finalize();
  sm_factory = new BoxLoaderStrategyFactory;
}

void te::graph::BoxLoaderStrategyFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::graph::BoxLoaderStrategyFactory::BoxLoaderStrategyFactory()
  : te::graph::AbstractGraphLoaderStrategyFactory(Globals::sm_factoryLoaderStrategyTypeBox)
{
}

te::graph::AbstractGraphLoaderStrategy* te::graph::BoxLoaderStrategyFactory::create(GraphMetadata* gm)
{
  te::graph::AbstractGraphLoaderStrategy* s = new te::graph::BoxLoaderStrategy(gm);

  return s;
}

te::graph::AbstractGraphLoaderStrategy* te::graph::BoxLoaderStrategyFactory::build()
{
  return new BoxLoaderStrategy(0);
}