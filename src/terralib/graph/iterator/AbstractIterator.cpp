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
#include "../iterator/AbstractIterator.h"
#include "../Config.h"
#include "../Exception.h"
#include "../Globals.h"

te::graph::AbstractIterator::AbstractIterator(te::graph::AbstractGraph* g) : 
  m_graph(g),
  m_vertexQuery(0),
  m_edgeQuery(0),
  m_listIsolatedVertex(true)
{
}

te::graph::AbstractIterator::~AbstractIterator()
{
}

te::graph::Vertex* te::graph::AbstractIterator::getNextVertex()
{
  if(m_vertexQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  int id;

  if(m_vertexQuery->moveNext())
  {
    id = m_vertexQuery->getInt32(Globals::sm_tableVertexModelAttrId);
  }
  else
  {
    return 0;
  }

  return m_graph->getVertex(id);
}

te::graph::Vertex* te::graph::AbstractIterator::getPreviousVertex()
{
  if(m_vertexQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  int id;

  if(m_vertexQuery->movePrevious())
  {
    id = m_vertexQuery->getInt32(Globals::sm_tableVertexModelAttrId);
  }
  else
  {
    return 0;
  }

  return m_graph->getVertex(id);
}

bool te::graph::AbstractIterator::isVertexIteratorAfterEnd()
{
  if(m_vertexQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  return m_vertexQuery->isAfterEnd();
}

size_t te::graph::AbstractIterator::getVertexInteratorCount()
{
  if(m_vertexQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  return m_vertexQuery->size();
}

void te::graph::AbstractIterator::listIsolatedVertex(bool flag)
{
  m_listIsolatedVertex = flag;
}

te::graph::Edge* te::graph::AbstractIterator::getNextEdge()
{
  if(m_edgeQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  int id;

  if(m_edgeQuery->moveNext())
  {
    id = m_edgeQuery->getInt32(Globals::sm_tableEdgeModelAttrId);
  }
  else
  {
    return 0;
  }

  return m_graph->getEdge(id);
}

te::graph::Edge* te::graph::AbstractIterator::getPreviousEdge()
{
  if(m_edgeQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  int id;

  if(m_edgeQuery->movePrevious())
  {
    id = m_vertexQuery->getInt32(Globals::sm_tableEdgeModelAttrId);
  }
  else
  {
    return 0;
  }

  return m_graph->getEdge(id);
}

bool te::graph::AbstractIterator::isEdgeIteratorAfterEnd()
{
  if(m_edgeQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  return m_edgeQuery->isAfterEnd();
}

size_t te::graph::AbstractIterator::getEdgeInteratorCount()
{
  if(m_edgeQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  return m_edgeQuery->size();
}
