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
  \file LDDGraphBuilder.cpp

  \brief This class defines the LDD strategy to build a graph.

        This strategy is based on Serio Rosim method, using this
        "mask" is possible extract a graph from a LDD image.

                    -------------------
                    |  32 |  64 | 128 |
                    -------------------
                    |  16 |  *  |  1  |
                    -------------------
                    |  8  |  4  |  2  |
                    -------------------

*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/progress/TaskProgress.h"
#include "../../geometry/Coord2D.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/Point.h"
#include "../../raster/Grid.h"
#include "../../raster/Raster.h"
#include "../core/AbstractGraphFactory.h"
#include "../core/Edge.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../graphs/Graph.h"
#include "../Config.h"
#include "../Exception.h"
#include "LDDGraphBuilder.h"

te::graph::LDDGraphBuilder::LDDGraphBuilder() : AbstractGraphBuilder()
{
  m_edgeId = 0;
}

te::graph::LDDGraphBuilder::~LDDGraphBuilder()
{
}

bool te::graph::LDDGraphBuilder::build(te::rst::Raster* raster, const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo)
{
  m_raster = raster;

  //create output graph
  m_graph.reset(te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo));

  assert(m_graph);

  //create graph attrs
  te::gm::GeometryProperty* gProp = new te::gm::GeometryProperty("coords");
  gProp->setId(0);
  gProp->setGeometryType(te::gm::PointType);
  gProp->setSRID(raster->getSRID());

  m_graph->addVertexProperty(gProp);

  te::common::TaskProgress t;

  t.setTotalSteps(m_raster->getNumberOfRows());
  t.setMessage(TR_GRAPH("LLD Graph Builder..."));
  t.useTimer(true);

  //extract graph
  for(std::size_t r = 1; r < m_raster->getNumberOfRows(); r++)
  {
    for(std::size_t c = 1; c < m_raster->getNumberOfColumns(); c++)
    {
      //get pixel value
      double val = 255.;
      m_raster->getValue(c, r, val);

      int value = (int) val;

      //create vertex
      int vId;
      
      if(getVertexId(r, c, vId) == false)
      {
        continue;
      }

      Vertex* v = new Vertex(vId);
      v->setAttributeVecSize(m_graph->getVertexPropertySize());

      te::gm::Coord2D coord = m_raster->getGrid()->gridToGeo(c, r);
      te::gm::Point* p = new te::gm::Point(coord.x, coord.y, gProp->getSRID());

      v->addAttribute(0, p);

      m_graph->add(v);

      //value to be ignored
      if(value == 255 || value == 0)
      {
        continue;
      }

      //create edge

      //right
      if(value & 1)
      {
        int vFrom = vId;
        int vTo;
        if(getVertexId(r, c + 1, vTo))
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFrom, vTo);

          m_graph->add(e);
        }
      }

      //lower right
      if(value & 2)
      {
        int vFrom = vId;
        int vTo;
        if(getVertexId(r + 1, c + 1, vTo))
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFrom, vTo);

          m_graph->add(e);
        }
      }

      //down
      if(value & 4)
      {
        int vFrom = vId;
        int vTo;
        if(getVertexId(r + 1, c, vTo))
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFrom, vTo);

          m_graph->add(e);
        }
      }

      //lower left
      if(value & 8)
      {
        int vFrom = vId;
        int vTo;
        if(getVertexId(r + 1, c - 1, vTo))
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFrom, vTo);

          m_graph->add(e);
        }
      }

      //left
      if(value & 16)
      {
        int vFrom = vId;
        int vTo;
        if(getVertexId(r, c - 1, vTo))
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFrom, vTo);

          m_graph->add(e);
        }
      }

      //upper left
      if(value & 32)
      {
        int vFrom = vId;
        int vTo;
        if(getVertexId(r - 1, c - 1, vTo))
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFrom, vTo);

          m_graph->add(e);
        }
      }

      //up
      if(value & 64)
      {
        int vFrom = vId;
        int vTo;
        if(getVertexId(r - 1, c, vTo))
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFrom, vTo);

          m_graph->add(e);
        }
      }

      //upper right
      if(value & 128)
      {
        int vFrom = vId;
        int vTo;
        if(getVertexId(r - 1, c + 1, vTo))
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFrom, vTo);

          m_graph->add(e);
        }
      }
    }

    t.pulse();
  }

  return true;
}

bool te::graph::LDDGraphBuilder::getVertexId(int row, int col, int& id)
{
  if( row < 1 || row > (int)m_raster->getNumberOfRows() - 1)
  {
    return false;
  }

  if( col < 1 || row > (int)m_raster->getNumberOfColumns() - 1)
  {
    return false;
  }

  if(m_raster)
  {
    id = (row * m_raster->getNumberOfColumns()) + col;

    return true;
  }

  return false;
}

int  te::graph::LDDGraphBuilder::getEdgeId()
{
  int id = m_edgeId;

  m_edgeId++;

  return id;
}