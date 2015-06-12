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
  \file LFUCachePolicyFactory.cpp

  \brief This is the concrete factory for the LFU cache policy
*/

// TerraLib
#include "../Globals.h"
#include "LFUCachePolicy.h"
#include "LFUCachePolicyFactory.h"

// STL
#include <memory>

te::graph::LFUCachePolicyFactory* te::graph::LFUCachePolicyFactory::sm_factory(0);

const std::string& te::graph::LFUCachePolicyFactory::getType() const
{
  return Globals::sm_factoryCachePolicyTypeLFU;
}

void te::graph::LFUCachePolicyFactory::initialize()
{
  finalize();
  sm_factory = new LFUCachePolicyFactory;
}

void te::graph::LFUCachePolicyFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::graph::LFUCachePolicyFactory::LFUCachePolicyFactory()
  : te::graph::AbstractCachePolicyFactory(Globals::sm_factoryCachePolicyTypeLFU)
{
}

te::graph::AbstractCachePolicy* te::graph::LFUCachePolicyFactory::build()
{
  return new LFUCachePolicy;
}