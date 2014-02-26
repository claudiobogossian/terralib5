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
  \file AbstractIterator.cpp

  \brief 
*/

// Terralib Includes
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../core/AbstractGraph.h"
#include "../graphs/MemoryGraph.h"
#include "../Config.h"
#include "../Exception.h"
#include "../Globals.h"
#include "MemoryIterator.h"

te::graph::MemoryIterator::MemoryIterator(te::graph::AbstractGraph* g) : 
  te::graph::AbstractIterator(g)
{
  te::graph::MemoryGraph* mg = dynamic_cast<te::graph::MemoryGraph*>(g);

  if(mg)
  {
    m_vertexMap = mg->getVertexMap();
    m_edgeMap = mg->getEdgeMap();

    m_vertexMapIt = m_vertexMap.begin();
    m_edgeMapIt = m_edgeMap.begin();
  }
}

te::graph::MemoryIterator::~MemoryIterator()
{
}

te::graph::Vertex* te::graph::MemoryIterator::getFirstVertex()
{
  m_vertexMapIt = m_vertexMap.begin();

  return m_vertexMapIt->second;
}

te::graph::Vertex* te::graph::MemoryIterator::getNextVertex()
{
  ++m_vertexMapIt;

  if(m_vertexMapIt != m_vertexMap.end())
    return m_vertexMapIt->second;
  
  return 0;
}

te::graph::Vertex* te::graph::MemoryIterator::getPreviousVertex()
{
  --m_vertexMapIt;

  return m_vertexMapIt->second;
}

bool te::graph::MemoryIterator::isVertexIteratorAfterEnd()
{
  return m_vertexMapIt == m_vertexMap.end();
}

size_t te::graph::MemoryIterator::getVertexInteratorCount()
{
  return m_vertexMap.size();
}

te::graph::Edge* te::graph::MemoryIterator::getFirstEdge()
{
  m_edgeMapIt = m_edgeMap.begin();

  return m_edgeMapIt->second;
}

te::graph::Edge* te::graph::MemoryIterator::getNextEdge()
{
  ++m_edgeMapIt;

  if(m_edgeMapIt != m_edgeMap.end())
    return m_edgeMapIt->second;
  
  return 0;
}

te::graph::Edge* te::graph::MemoryIterator::getPreviousEdge()
{
  --m_edgeMapIt;

  return m_edgeMapIt->second;
}

bool te::graph::MemoryIterator::isEdgeIteratorAfterEnd()
{
  return m_edgeMapIt == m_edgeMap.end();
}

size_t te::graph::MemoryIterator::getEdgeInteratorCount()
{
  return m_edgeMap.size();
}
