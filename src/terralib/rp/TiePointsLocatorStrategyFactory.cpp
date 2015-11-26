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
  \file terralib/rp/TiePointsLocatorStrategyFactory.cpp
  \brief Tie-points locator strategy factory.
*/

#include "TiePointsLocatorStrategyFactory.h"

namespace te
{
  namespace rp
  {
    TiePointsLocatorStrategyFactory::TiePointsLocatorStrategyFactory( 
      const std::string& factoryKey )
    : te::common::AbstractFactory< te::rp::TiePointsLocatorStrategy, std::string,
      std::less<std::string> >( factoryKey )
    {
    }
    
    TiePointsLocatorStrategyFactory::~TiePointsLocatorStrategyFactory()
    {
    }
  } // end namespace rp
}   // end namespace te    

