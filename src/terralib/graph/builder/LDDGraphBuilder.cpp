#include "LDDGraphBuilder.h"

#include "AbstractGraphFactory.h"
#include "Config.h"
#include "Edge.h"
#include "Exception.h"
#include "Graph.h"
#include "GraphMetadata.h"
#include "Vertex.h"
#include "VertexProperty.h"

// TerraLib
#include "../dataaccess.h"
#include "../datatype.h"
#include "../raster.h"
#include "../geometry.h"

#include "../common/Translator.h"
#include "../common/progress/TaskProgress.h"

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
  m_graph = te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo);

  assert(m_graph);

  m_graph->getMetadata()->m_maxVecCacheSize = 5;
  m_graph->getMetadata()->m_maxCacheSize = 100000;

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