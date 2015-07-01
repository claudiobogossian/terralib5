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
  \file GraphDataManager.cpp

  \brief This class defines a interface to access the graph
         elements inside a data source. Its use a implementation of
         Loader Strategy that defines how a data must be loaded.
         
*/

// Terralib Includes
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../core/Edge.h"
#include "../core/Vertex.h"
#include "../loader/AbstractGraphLoaderStrategy.h"
#include "../Config.h"
#include "../Exception.h"
#include "GraphDataManager.h"


te::graph::GraphDataManager::GraphDataManager(te::graph::AbstractGraph* g) : m_loadStrategy(0), m_graph(g)
{
}

te::graph::GraphDataManager::~GraphDataManager()
{
  delete m_loadStrategy;
}

te::graph::AbstractGraphLoaderStrategy* te::graph::GraphDataManager::getLoaderStrategy()
{
  return m_loadStrategy;
}

void te::graph::GraphDataManager::setLoaderStrategy(AbstractGraphLoaderStrategy* loaderStrategy)
{
  m_loadStrategy = loaderStrategy;
}

void te::graph::GraphDataManager::loadGraphDataByVertexId(int vertexId, te::graph::GraphCache* gc)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->loadDataByVertexId(vertexId, m_graph, gc);
  }
}

void te::graph::GraphDataManager::loadGraphDataByEdgeId(int edgeId, te::graph::GraphCache* gc)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->loadDataByEdgeId(edgeId, m_graph, gc);
  }
}

void te::graph::GraphDataManager::saveGraphData(GraphData* data)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->saveData(data);
  }
}

void te::graph::GraphDataManager::removeEdge(int id)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->removeEdge(id);
  }
}

void te::graph::GraphDataManager::removeVertex(int id)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->removeVertex(id);
  }
}
