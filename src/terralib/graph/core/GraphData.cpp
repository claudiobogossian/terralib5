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
  \file GraphData.cpp

  \brief This class define a important struct used to group
         a map of vertex and edges. A flag is used to indicate
         if any element of this group was changed.
*/

// Terralib Includes
#include "../../common/STLUtils.h"
#include "../core/Edge.h"
#include "../core/Vertex.h"
#include "GraphData.h"


te::graph::GraphData::GraphData(int id): 
  m_id(id),
  m_dirty(false)
{
}

te::graph::GraphData::~GraphData()
{
  te::common::FreeContents(m_vertexMap);
  te::common::FreeContents(m_edgeMap);

  m_vertexMap.clear();
  m_edgeMap.clear();
}

int te::graph::GraphData::getId()
{
  return m_id;
}

te::graph::GraphData::VertexMap& te::graph::GraphData::getVertexMap()
{
  return m_vertexMap;
}

void te::graph::GraphData::setVertexMap(const VertexMap& map)
{
  m_vertexMap = map;
}

te::graph::GraphData::EdgeMap& te::graph::GraphData::getEdgeMap()
{
  return m_edgeMap;
}

void te::graph::GraphData::setEdgeMap(const EdgeMap& map)
{
  m_edgeMap = map;
}

void te::graph::GraphData::setDirty(bool status)
{
  m_dirty = status;
}

bool te::graph::GraphData::isDirty()
{
  return m_dirty;
}
