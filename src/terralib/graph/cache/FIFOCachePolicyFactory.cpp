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
  \file FIFOCachePolicyFactory.cpp

  \brief This is the concrete factory for the FIFO cache policy
*/

// TerraLib
#include "../Globals.h"
#include "FIFOCachePolicy.h"
#include "FIFOCachePolicyFactory.h"

// STL
#include <memory>

te::graph::FIFOCachePolicyFactory* te::graph::FIFOCachePolicyFactory::sm_factory(0);

const std::string& te::graph::FIFOCachePolicyFactory::getType() const
{
  return Globals::sm_factoryCachePolicyTypeFIFO;
}

void te::graph::FIFOCachePolicyFactory::initialize()
{
  finalize();
  sm_factory = new FIFOCachePolicyFactory;
}

void te::graph::FIFOCachePolicyFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::graph::FIFOCachePolicyFactory::FIFOCachePolicyFactory()
  : te::graph::AbstractCachePolicyFactory(Globals::sm_factoryCachePolicyTypeFIFO)
{
}

te::graph::AbstractCachePolicy* te::graph::FIFOCachePolicyFactory::build()
{
  return new FIFOCachePolicy;
}