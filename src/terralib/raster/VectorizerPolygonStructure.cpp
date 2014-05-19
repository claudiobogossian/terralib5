/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  : m_polygon(0, te::gm::PolygonType)
{
  init();
}

te::rst::VectorizerPolygonStructure::VectorizerPolygonStructure(const VectorizerPolygonStructure& rhs)
  : m_polygon(0, te::gm::PolygonType)
{
  init();

  reset(rhs.m_polygon, rhs.m_value, rhs.m_tileIndexerDY);
}

te::rst::VectorizerPolygonStructure::VectorizerPolygonStructure(const te::gm::Polygon& p, const int& v, const double& tidy)
  : m_polygon(0, te::gm::PolygonType)
{
  init();

  reset(p, v, tidy);
}

te::rst::VectorizerPolygonStructure::~VectorizerPolygonStructure()
{
  clear();
}

void te::rst::VectorizerPolygonStructure::init()
{
  m_value = 0;
  m_tileIndexerDY = 1.0;
  m_indexer = 0;
}

void te::rst::VectorizerPolygonStructure::clear()
{
  if(m_indexer)
    delete m_indexer;

  init();
}

void te::rst::VectorizerPolygonStructure::reset(const te::gm::Polygon& p, const int& v, const double& tidy)
{
  clear();

  m_polygon = p;
  m_value = v;
  m_tileIndexerDY = tidy;

  m_indexer = new te::rst::TileIndexer(m_polygon, m_tileIndexerDY);
}

te::rst::VectorizerPolygonStructure& te::rst::VectorizerPolygonStructure::operator=(const te::rst::VectorizerPolygonStructure& rhs)
{
  reset(rhs.m_polygon, rhs.m_value, rhs.m_tileIndexerDY);

  return *this;
}
