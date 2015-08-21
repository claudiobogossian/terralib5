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
  \file GraphMetadata.cpp

  \brief Class used to define the graph metadata informations.
*/

// TerraLib Includes
#include "../../geometry/Envelope.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../srs/Config.h"
#include "../Globals.h"
#include "EdgeProperty.h"
#include "VertexProperty.h"
#include "GraphMetadata.h"

te::graph::GraphMetadata::GraphMetadata(te::da::DataSource* ds) : 
  m_id(-1),
  m_srid(TE_UNKNOWN_SRS),
  m_extent(0),
  m_type(""),
  m_ds(ds),
  m_vertexProp(0),
  m_edgeProp(0)

{
  m_maxCacheSize = te::graph::Globals::sm_graphCacheDefaultMaxSize;
  m_maxVecCacheSize = te::graph::Globals::sm_graphVecCacheDefaultMaxSize;
  m_boxPercentSize = te::graph::Globals::sm_boxLoaderStrategyDefaultSize;
  m_memoryGraph = false;
}

te::graph::GraphMetadata::~GraphMetadata()
{
  if(m_ds)
    m_ds->close();

  delete m_ds;

  delete m_vertexProp;
  delete m_edgeProp;

  delete m_extent;
}

int te::graph::GraphMetadata::getId()
{
  return m_id;
}

void te::graph::GraphMetadata::setName(std::string name)
{
  m_name = name;
}

std::string te::graph::GraphMetadata::getName()
{
  return m_name;
}

void te::graph::GraphMetadata::setDescription(std::string desc)
{
  m_description = desc;
}

std::string te::graph::GraphMetadata::getDescription()
{
  return m_description;
}

void te::graph::GraphMetadata::setType(std::string graphType)
{
  m_type = graphType;
}

std::string te::graph::GraphMetadata::getType()
{
  return m_type;
}

void te::graph::GraphMetadata::setClass(GraphClass value)
{
  m_class = value;
}

te::graph::GraphClass te::graph::GraphMetadata::getClass()
{
  return m_class;
}

void te::graph::GraphMetadata::setStorageMode(GraphStorageMode value)
{
  m_mode = value;
}

te::graph::GraphStorageMode te::graph::GraphMetadata::getStorageMode()
{
  return m_mode;
}

te::da::DataSource* te::graph::GraphMetadata::getDataSource()
{
  return m_ds;
}

std::string te::graph::GraphMetadata::getVertexTableName()
{
  std::string tableName = "";
  
  if(getStorageMode() == te::graph::Vertex_List)
  {
    tableName = getName() + Globals::sm_tableVertexModelSufixName;
  }
  else if(getStorageMode() == te::graph::Edge_List)
  {
    tableName = getName() + Globals::sm_tableVertexAttributeModelSufixName;
  }

  return tableName;
}

std::string te::graph::GraphMetadata::getEdgeTableName()
{
  std::string tableName = "";
  
  if(getStorageMode() == te::graph::Vertex_List)
  {
    tableName = getName() + Globals::sm_tableEdgeAttributeModelSufixName;
  }
  else if(getStorageMode() == te::graph::Edge_List)
  {
    tableName = getName() + Globals::sm_tableEdgeModelSufixName;
  }

  return tableName;
}

void te::graph::GraphMetadata::addVertexProperty(te::dt::Property* p)
{
  if(m_vertexProp == 0)
  {
    m_vertexProp = new te::graph::VertexProperty();
  }

  m_vertexProp->add(p);
}

void te::graph::GraphMetadata::removeVertexProperty(int idx)
{
  if(m_vertexProp != 0)
  {
    te::dt::Property* p = getVertexProperty(idx);

    m_vertexProp->remove(p);
  }
}

te::dt::Property* te::graph::GraphMetadata::getVertexProperty(int idx)
{
  if(m_vertexProp != 0)
  {
    return m_vertexProp->getProperty(idx);
  }

  return 0;
}

int te::graph::GraphMetadata::getVertexPropertySize()
{
  if(m_vertexProp != 0)
  {
    return m_vertexProp->getProperties().size();
  }

  return 0;
}

void te::graph::GraphMetadata::addEdgeProperty(te::dt::Property* p)
{
  if(m_edgeProp == 0)
  {
    m_edgeProp = new te::graph::EdgeProperty();
  }

  m_edgeProp->add(p);
}

void te::graph::GraphMetadata::removeEdgeProperty(int idx)
{
  if(m_edgeProp != 0)
  {
    te::dt::Property* p = getEdgeProperty(idx);

    m_edgeProp->remove(p);
  }
}

te::dt::Property* te::graph::GraphMetadata::getEdgeProperty(int idx)
{
  if(m_edgeProp != 0)
  {
    return m_edgeProp->getProperty(idx);
  }

  return 0;
}

int te::graph::GraphMetadata::getEdgePropertySize()
{
  if(m_edgeProp != 0)
  {
    return m_edgeProp->getProperties().size();
  }

  return 0;
}

int te::graph::GraphMetadata::getSRID()
{
  return m_srid;
}

void te::graph::GraphMetadata::setSRID(int srid)
{
  m_srid = srid;
}

te::gm::Envelope* te::graph::GraphMetadata::getEnvelope()
{
  return m_extent;
}

void te::graph::GraphMetadata::setEnvelope(te::gm::Envelope& extent)
{
  delete m_extent;

  m_extent = new te::gm::Envelope(extent);
}
