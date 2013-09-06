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
  \file AbstractGraph.cpp

  \brief 
*/

// Terralib Includes
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../iterator/AbstractIterator.h"
#include "../Config.h"
#include "../Exception.h"
#include "AbstractGraph.h"

te::graph::AbstractGraph::AbstractGraph() :
  m_iterator(0)
{
}

te::graph::AbstractGraph::~AbstractGraph()
{
}

te::graph::Vertex* te::graph::AbstractGraph::getFirstVertex()
{
  if(m_iterator == 0)
  {
    throw Exception(TR_GRAPH("Iterator not defined"));
  }

  return m_iterator->getFirstVertex();
}

te::graph::Vertex* te::graph::AbstractGraph::getNextVertex()
{
  if(m_iterator == 0)
  {
    throw Exception(TR_GRAPH("Iterator not defined"));
  }

  return m_iterator->getNextVertex();
}

te::graph::Vertex* te::graph::AbstractGraph::getPreviousVertex()
{
  if(m_iterator == 0)
  {
    throw Exception(TR_GRAPH("Iterator not defined"));
  }

  return m_iterator->getPreviousVertex();
}

te::graph::Edge* te::graph::AbstractGraph::getFirstEdge()
{
  if(m_iterator == 0)
  {
    throw Exception(TR_GRAPH("Iterator not defined"));
  }

  return m_iterator->getFirstEdge();
}

te::graph::Edge* te::graph::AbstractGraph::getNextEdge()
{
  if(m_iterator == 0)
  {
    throw Exception(TR_GRAPH("Iterator not defined"));
  }

  return m_iterator->getNextEdge();
}

te::graph::Edge* te::graph::AbstractGraph::getPreviousEdge()
{
  if(m_iterator == 0)
  {
    throw Exception(TR_GRAPH("Iterator not defined"));
  }

  return m_iterator->getPreviousEdge();
}

void te::graph::AbstractGraph::setIterator(te::graph::AbstractIterator* i)
{
  m_iterator = i;
}

te::graph::AbstractIterator* te::graph::AbstractGraph::getIterator()
{
  return m_iterator;
}
