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
  \file terralib/raster/VectorizerPolygonStructure.cpp

  \brief A polygon container node class.
*/

// TerraLib
#include "TileIndexer.h"
#include "VectorizerPolygonStructure.h"

// STL
#include <iostream>

te::rst::VectorizerPolygonStructure::VectorizerPolygonStructure()
{
  init();
}

te::rst::VectorizerPolygonStructure::VectorizerPolygonStructure(const VectorizerPolygonStructure& rhs)
{
  init();

  reset( rhs.m_polygonPtr.get() ? (te::gm::Polygon*)rhs.m_polygonPtr->clone() : 
    (te::gm::Polygon*)0, rhs.m_value, rhs.m_tileIndexerDY);
}

te::rst::VectorizerPolygonStructure::VectorizerPolygonStructure( te::gm::Polygon* polPtr,
  const unsigned int& v, const double& tidy)
{
  init();

  reset(polPtr, v, tidy);
}

te::rst::VectorizerPolygonStructure::~VectorizerPolygonStructure()
{
  clear();
}

void te::rst::VectorizerPolygonStructure::init()
{
  m_value = 0;
  m_tileIndexerDY = 1.0;
}

void te::rst::VectorizerPolygonStructure::clear()
{
  m_indexerPtr.reset();
  m_polygonPtr.reset();
  init();
}

void te::rst::VectorizerPolygonStructure::reset( te::gm::Polygon* polPtr, 
  const unsigned int& v, const double& tidy)
{
  clear();

  m_polygonPtr.reset( polPtr );
  m_value = v;
  m_tileIndexerDY = tidy;

  if( polPtr )
  {
    m_indexerPtr.reset( new te::rst::TileIndexer(*m_polygonPtr, m_tileIndexerDY) );
  }
}

te::rst::VectorizerPolygonStructure& te::rst::VectorizerPolygonStructure::operator=(
  const te::rst::VectorizerPolygonStructure& rhs)
{
  reset( rhs.m_polygonPtr.get() ? (te::gm::Polygon*)rhs.m_polygonPtr->clone() : 
    (te::gm::Polygon*)0, rhs.m_value, rhs.m_tileIndexerDY);

  return *this;
}
