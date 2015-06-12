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
  \file Vertex.cpp

  \brief From the point of view of graph theory, vertices are treated 
        as featureless and indivisible objects, although they may have 
        additional structure depending on the application from which the 
        graph arises;for instance, a semantic network is a graph in 
        which the vertices represent concepts or classes of objects.
*/

// Terralib Includes
#include "../../common/STLUtils.h"
#include "../../datatype/AbstractData.h"
#include "Vertex.h"


te::graph::Vertex::Vertex(int id, bool isNew) : m_vertexId(id), m_dirty(false), m_new(isNew)
{
}

te::graph::Vertex::Vertex(te::graph::Vertex* rhs)
{
  m_vertexId = rhs->getId();
  m_dirty = false;
  m_new = true;

  m_predecessors.insert(rhs->getPredecessors().begin(), rhs->getPredecessors().end());
  m_successors.insert(rhs->getSuccessors().begin(), rhs->getSuccessors().end());
  m_neighborhood.insert(rhs->getNeighborhood().begin(), rhs->getNeighborhood().end());

  this->setAttributeVecSize(rhs->getAttributes().size());

  for(size_t t = 0; t < rhs->getAttributes().size(); ++ t)
  {
    te::dt::AbstractData* ad = rhs->getAttributes()[t]->clone();
    
    this->addAttribute(t, ad);
  }
}

te::graph::Vertex::~Vertex()
{
  te::common::FreeContents(m_attrs);

  m_predecessors.clear();
  m_successors.clear();
  m_neighborhood.clear();
}

int te::graph::Vertex::getId()
{
  return m_vertexId;
}

std::vector<te::dt::AbstractData*>& te::graph::Vertex::getAttributes()
{
  return m_attrs;
}

void te::graph::Vertex::setAttributeVecSize(int size)
{
  m_attrs.resize(size);
}

void te::graph::Vertex::addAttribute(int idx, te::dt::AbstractData* ad)
{
  if(m_attrs[idx])
    delete m_attrs[idx];
    
   m_attrs[idx] = ad;

   return;
}

void te::graph::Vertex::removeAttribute(int idx)
{
  delete m_attrs[idx];

  return;
}

std::set<int>& te::graph::Vertex::getPredecessors() 
{
  return m_predecessors;
}

std::set<int>& te::graph::Vertex::getSuccessors() 
{
  return m_successors;
}

std::set<int>& te::graph::Vertex::getNeighborhood() 
{
  return m_neighborhood;
}

void te::graph::Vertex::setDirty(bool flag)
{
  m_dirty = flag;
}

bool te::graph::Vertex::isDirty()
{
  return m_dirty;
}

bool te::graph::Vertex::isNew()
{
  return m_new;
}
