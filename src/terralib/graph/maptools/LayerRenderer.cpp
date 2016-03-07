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
  \file LayerRenderer.cpp

  \brief It renders the objects associated to a Layer.
 */

// TerraLib
#include "../../common/STLUtils.h"
#include "../../datatype/Enums.h"
#include "../../geometry/LineString.h"
#include "../../geometry/Point.h"
#include "../../geometry/Utils.h"
#include "../../maptools/AbstractLayer.h"
#include "../../maptools/Canvas.h"
#include "../../maptools/CanvasConfigurer.h"
#include "../../maptools/MarkRendererManager.h"
#include "../../se/Mark.h"
#include "../../se/Style.h"
#include "../../se/Rule.h"
#include "../../se/Utils.h"
#include "../core/AbstractGraph.h"
#include "../core/Edge.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../iterator/BoxIterator.h"
#include "../iterator/MemoryIterator.h"
#include "../iterator/SequenceIterator.h"
#include "Layer.h"
#include "LayerRenderer.h"


te::graph::LayerRenderer::LayerRenderer()
{}

te::graph::LayerRenderer::~LayerRenderer()
{}

void te::graph::LayerRenderer::draw(te::map::AbstractLayer* layer, te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid, const double& scale, bool* cancel)
{
  // Is our business?
  te::graph::Layer* l = dynamic_cast<Layer*>(layer);
  assert(l);

  if(!bbox.isValid())
    return;

  // The canvas configurer
  te::map::CanvasConfigurer cc(canvas);

  // Gets the associated layer style
  te::se::Style* style = l->getStyle();
  if(style == 0)
  {
    configDefaultLine(canvas);
    configDefaultPoint(canvas);
  }
  else
  {
    if(style->getRules().empty())
    {
      throw;
    }
    const te::se::Rule* rule = style->getRule(0);

    const std::vector<te::se::Symbolizer*> symbolizers = rule->getSymbolizers();

    for(size_t t= 0; t < symbolizers.size(); ++t)
    {
      te::se::Symbolizer* symb = symbolizers[t];

      cc.config(symb);
    }
  }

  te::graph::AbstractGraph* g = l->getGraph();

  if(g->getMetadata()->getDataSource())
    drawDataSourceGraph(g, canvas, bbox);
  else
    drawMemoryGraph(g, canvas, bbox, srid);
}

int te::graph::LayerRenderer::checkVertexGeometryProperty(te::graph::AbstractGraph* g)
{
  int idx = -1;

  for(int i = 0; i < g->getVertexPropertySize(); ++i)
  {
    te::dt::Property* p = g->getVertexProperty(i);
    if(p && p->getType() == te::dt::GEOMETRY_TYPE)
    {
      idx = i;
      break;
    }
  }

  return idx;
}

int te::graph::LayerRenderer::checkEdgeGeometryProperty(te::graph::AbstractGraph* g)
{
  int idx = -1;

  for(int i = 0; i < g->getEdgePropertySize(); ++i)
  {
    te::dt::Property* p = g->getEdgeProperty(i);
    if(p && p->getType() == te::dt::GEOMETRY_TYPE)
    {
      idx = i;
      break;
    }
  }

  return idx;
}

void te::graph::LayerRenderer::configDefaultLine(te::map::Canvas* canvas)
{
  canvas->setLineColor(te::color::RGBAColor(TE_SE_DEFAULT_STROKE_BASIC_COLOR, TE_OPAQUE));
  canvas->setLineWidth(TE_SE_DEFAULT_STROKE_BASIC_WIDTH);
  canvas->setLineDashStyle(te::map::SolidLine);
  canvas->setLineCapStyle(te::map::ButtCap);
  canvas->setLineJoinStyle(te::map::MiterJoin);
}

void te::graph::LayerRenderer::configDefaultPoint(te::map::Canvas* canvas)
{
  te::se::Mark* mark = te::se::CreateMark("circle", te::se::CreateStroke("#000000", "2"), te::se::CreateFill("#FFFF00", "1.0"));
  std::size_t size = 12;
  te::color::RGBAColor** rgba =  te::map::MarkRendererManager::getInstance().render(mark, size);
  canvas->setPointColor(te::color::RGBAColor(255, 0, 0, TE_TRANSPARENT));
  canvas->setPointPattern(rgba, size, size);

  te::common::Free(rgba, size);
  delete mark;
}

void te::graph::LayerRenderer::configLoopPoint(te::map::Canvas* canvas)
{
  te::se::Mark* mark = te::se::CreateMark("star", te::se::CreateStroke("#000000", "2"), te::se::CreateFill("#FFFF00", "1.0"));
  std::size_t size = 12;
  te::color::RGBAColor** rgba =  te::map::MarkRendererManager::getInstance().render(mark, size);
  canvas->setPointColor(te::color::RGBAColor(255, 0, 0, TE_TRANSPARENT));
  canvas->setPointPattern(rgba, size, size);

  te::common::Free(rgba, size);
  delete mark;
}

void te::graph::LayerRenderer::drawDataSourceGraph(te::graph::AbstractGraph* g, te::map::Canvas* canvas, const te::gm::Envelope& bbox)
{
//set iterator
  te::gm::Envelope* box = new te::gm::Envelope(bbox);

  te::graph::BoxIterator* it = new te::graph::BoxIterator(g, box);

  te::graph::Edge* edge = it->getFirstEdge();

  int vertexGeomPropIdx = checkVertexGeometryProperty(g);
  int edgeGeomPropIdx = checkEdgeGeometryProperty(g);

  if(vertexGeomPropIdx == -1 && edgeGeomPropIdx == -1)
    return;
  
  while(it->isEdgeIteratorAfterEnd() == false)
  {
    if(edge)
    {
      //draw vertex from and vertex to
      te::graph::Vertex* vFrom = g->getVertex(edge->getIdFrom());
      te::graph::Vertex* vTo = g->getVertex(edge->getIdTo());

      if(vFrom && vTo)
      {
        te::gm::Geometry* geomVFrom = (te::gm::Geometry*)vFrom->getAttributes()[vertexGeomPropIdx];
        te::gm::Geometry* geomVTo = (te::gm::Geometry*)vTo->getAttributes()[vertexGeomPropIdx];

        //check if exist a geometry associated a edge... if not draw a simple line
        if(edgeGeomPropIdx != -1)
        {
          te::gm::Geometry* geomEdge = (te::gm::Geometry*)edge->getAttributes()[edgeGeomPropIdx];

          canvas->draw(geomEdge);

          if(vFrom->getId() == vTo->getId())
          {
            canvas->draw(geomVFrom);
          }
          else
          {
            canvas->draw(geomVFrom);
            canvas->draw(geomVTo);
          }
        }
        else 
        {
          if(vFrom->getId() == vTo->getId())
          {
            canvas->draw(geomVFrom);
          }
          else
          {
            te::gm::Point* pFrom = dynamic_cast<te::gm::Point*>(geomVFrom);
            te::gm::Point* pTo = dynamic_cast<te::gm::Point*>(geomVTo);

            if(pFrom && pTo)
            {
                te::gm::LineString* line = new te::gm::LineString(2, te::gm::LineStringType);
                line->setPoint(0, pFrom->getX(), pFrom->getY());
                line->setPoint(1, pTo->getX(), pTo->getY());

                canvas->draw(line);

                delete line;
          
            }

            canvas->draw(geomVFrom);
            canvas->draw(geomVTo);
          }
        }
      }
    }
    edge = it->getNextEdge();
  }

  delete it;
}

void te::graph::LayerRenderer::drawMemoryGraph(te::graph::AbstractGraph* g, te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
//set iterator
  te::graph::MemoryIterator* it = new te::graph::MemoryIterator(g);

  te::graph::Edge* edge = it->getFirstEdge();

  int vertexGeomPropIdx = checkVertexGeometryProperty(g);
  int edgeGeomPropIdx = checkEdgeGeometryProperty(g);

  if(vertexGeomPropIdx == -1 && edgeGeomPropIdx == -1)
    return;

  te::gm::Geometry* geomBox = te::gm::GetGeomFromEnvelope(&bbox, srid);
  
  while(it->isEdgeIteratorAfterEnd() == false)
  {
    if(edge)
    {
      //draw vertex from and vertex to
      te::graph::Vertex* vFrom = g->getVertex(edge->getIdFrom());
      te::graph::Vertex* vTo = g->getVertex(edge->getIdTo());

      if(vFrom && vTo)
      {
        te::gm::Geometry* geomVFrom = (te::gm::Geometry*)vFrom->getAttributes()[vertexGeomPropIdx];
        te::gm::Geometry* geomVTo = (te::gm::Geometry*)vTo->getAttributes()[vertexGeomPropIdx];

        //if(geomBox->contains(geomVFrom) && geomBox->contains(geomVTo))
        if(geomVFrom->within(geomBox) && geomVTo->within(geomBox))
        {
          //check if exist a geometry associated a edge... if not draw a simple line
          if(edgeGeomPropIdx != -1)
          {
            te::gm::Geometry* geomEdge = (te::gm::Geometry*)edge->getAttributes()[edgeGeomPropIdx];

            canvas->draw(geomEdge);

            if(vFrom->getId() == vTo->getId())
            {
              canvas->draw(geomVFrom);
            }
            else
            {
              canvas->draw(geomVFrom);
              canvas->draw(geomVTo);
            }
          }
          else 
          {
            if(vFrom->getId() == vTo->getId())
            {
              canvas->draw(geomVFrom);
            }
            else
            {
              te::gm::Point* pFrom = dynamic_cast<te::gm::Point*>(geomVFrom);
              te::gm::Point* pTo = dynamic_cast<te::gm::Point*>(geomVTo);

              if(pFrom && pTo)
              {
                  te::gm::LineString* line = new te::gm::LineString(2, te::gm::LineStringType);
                  line->setPoint(0, pFrom->getX(), pFrom->getY());
                  line->setPoint(1, pTo->getX(), pTo->getY());

                  canvas->draw(line);

                  delete line;
          
              }

              canvas->draw(geomVFrom);
              canvas->draw(geomVTo);
            }
          }
        }
      }
    }

    edge = it->getNextEdge();
  }

  delete geomBox;

  delete it;
}
