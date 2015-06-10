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
  \file AbstractCachePolicyFactory.cpp

  \brief This is the abstract factory for cache policy.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../Config.h"
#include "../Exception.h"
#include "AbstractCachePolicyFactory.h"

// STL
#include <memory>

te::graph::AbstractCachePolicy* te::graph::AbstractCachePolicyFactory::make()
{
  return te::common::AbstractFactory<AbstractCachePolicy, std::string>::make(TE_DEFAULT_CACHE_POLICY_TYPE);
}

te::graph::AbstractCachePolicy* te::graph::AbstractCachePolicyFactory::make(const std::string& gType)
{
  std::string ucase = te::common::Convert2UCase(gType);

  return te::common::AbstractFactory<AbstractCachePolicy, std::string>::make(ucase);
}

te::graph::AbstractCachePolicyFactory::AbstractCachePolicyFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::graph::AbstractCachePolicy, std::string>(factoryKey)
{
}



