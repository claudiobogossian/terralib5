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
  \file terralib/edit/qt/tools/VertexTool.cpp

  \brief This class implements a concrete tool for vertex operations (move, add, etc.).
*/

// TerraLib
#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/LineString.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../geometry/Polygon.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Utils.h"
#include "../../../qt/widgets/Utils.h"
#include "../../Utils.h"
#include "VertexTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// STL
#include <cassert>
#include <memory>
#include <string>

te::edit::VertexTool::VertexTool(te::qt::widgets::MapDisplay* display, const QCursor& cursor, const te::map::AbstractLayerPtr& layer, QObject* parent)
  : AbstractTool(display, parent),
    m_layer(layer),
    m_geom(0),
    m_currentStage(GEOMETRY_SELECTION)
{
  assert(m_layer.get());

  setCursor(cursor);

  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));

  m_currentVertexIndex.makeInvalid();
}

te::edit::VertexTool::~VertexTool()
{
  delete m_geom;
}

bool te::edit::VertexTool::mousePressEvent(QMouseEvent* e)
{
  if(e->button() == Qt::LeftButton && m_currentStage == VERTEX_FOUND)
  {
    Qt::KeyboardModifiers keys = e->modifiers();

    // Moving...
    if(keys == Qt::NoModifier)
    {
      m_currentStage = VERTEX_MOVING;
      return true;
    }

    // Removing...
    if(keys & Qt::ControlModifier || keys & Qt::ShiftModifier)
    {
      RemoveVertex(m_lines, m_currentVertexIndex);

      m_currentVertexIndex.makeInvalid();

      m_currentStage = VERTEX_SEARCH;

      updateRTree();

      drawVertexes();

      return true;
    }
  }

  m_currentStage = GEOMETRY_SELECTION;

  return false;
}

bool te::edit::VertexTool::mouseMoveEvent(QMouseEvent* e)
{
  switch(m_currentStage)
  {
    case VERTEX_SEARCH:
    case VERTEX_FOUND:
    case VERTEX_READY_TO_ADD:
    {
      if(m_rtree.isEmpty())
        return false;

      te::gm::Envelope env = buildEnvelope(getPosition(e));

      // Search on rtree
      std::vector<VertexIndex> report;
      m_rtree.search(env, report);

      if(!report.empty()) // Vertex found!
      {
        m_currentStage = VERTEX_FOUND;

        m_currentVertexIndex = report[0];

        drawVertexes();

        return false;
      }

      // Else, try search "virtual vertex" at a segment
      m_currentVertexIndex.makeInvalid();

      std::auto_ptr<te::gm::Point> borderPoint(0);

      if(m_geom != 0)
      {
        std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&env, m_display->getSRID()));

        if(m_geom->intersects(geometryFromEnvelope.get()) && !m_geom->contains(geometryFromEnvelope.get())) // Mouse over a segment?
          borderPoint.reset(new te::gm::Point(env.getCenter().x, env.getCenter().y));
      }

      borderPoint.get() != 0 ? m_currentStage = VERTEX_READY_TO_ADD : m_currentStage = VERTEX_SEARCH;

      drawVertexes(borderPoint.get());

      return false;
    }

    case VERTEX_MOVING:
    {
      QPointF point = getPosition(e);
      point = m_display->transform(point);

      MoveVertex(m_lines, m_currentVertexIndex, point.x(), point.y());

      drawVertexes();

      return false;
    }

    default:
      return false;
  }

  return false;
}

bool te::edit::VertexTool::mouseReleaseEvent(QMouseEvent* e)
{
  switch(m_currentStage)
  {
    case GEOMETRY_SELECTION:
    {
      if(e->button() != Qt::LeftButton)
        return false;

      pickGeometry(m_layer, getPosition(e));

      if(m_geom)
        m_currentStage = VERTEX_SEARCH;

      return true;
    }

    case VERTEX_MOVING:
    {
      updateRTree();
      m_currentStage = VERTEX_SEARCH;
    }

    default:
      return false;
  }
}

bool te::edit::VertexTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  return false;
}

void te::edit::VertexTool::reset()
{
  delete m_geom;
  m_geom = 0;

  m_currentStage = GEOMETRY_SELECTION;

  m_lines.clear();

  m_currentVertexIndex.makeInvalid();

  m_rtree.clear();
}


void te::edit::VertexTool::pickGeometry(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
{
  te::gm::Envelope env = buildEnvelope(pos);
  pickGeometry(m_layer, env);
}

void te::edit::VertexTool::pickGeometry(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& env)
{
  reset();

  if(layer->getVisibility() != te::map::VISIBLE || !layer->isValid())
    return;

  std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());

  if(!schema->hasGeom())
    return;

  te::gm::Envelope reprojectedEnvelope(env);

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_display->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != m_display->getSRID()))
    reprojectedEnvelope.transform(m_display->getSRID(), layer->getSRID());

  if(!reprojectedEnvelope.intersects(layer->getExtent()))
    return;

  try
  {
    te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(schema.get());

    // Gets the dataset
    std::auto_ptr<te::da::DataSet> dataset = layer->getData(gp->getName(), &reprojectedEnvelope, te::gm::INTERSECTS);
    assert(dataset.get());

    // Generates a geometry from the given extent. It will be used to refine the results
    std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&reprojectedEnvelope, layer->getSRID()));

    // The restriction point. It will be used to refine the results
    te::gm::Coord2D center = reprojectedEnvelope.getCenter();
    te::gm::Point point(center.x, center.y, layer->getSRID());

    while(dataset->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gp->getName()));

      if(g->contains(&point) || g->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(g.get()))
      {
        // Geometry found!
        m_geom = g.release();
        break;
      }
    }

    m_lines.clear();

    GetLines(m_geom, m_lines);

    updateRTree();

    drawVertexes();
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
    // TODO: catch the exceptions...
  }
}

void te::edit::VertexTool::drawVertexes(te::gm::Point* virtualVertex)
{
  const te::gm::Envelope& env = m_display->getExtent();
  if(!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Prepares the canvas
  te::qt::widgets::Canvas canvas(m_display->width(), m_display->height());
  canvas.setDevice(draft, false);
  canvas.setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

  if(m_lines.empty())
  {
    m_display->repaint();
    return;
  }

  switch(m_geom->getGeomTypeId())
  {
    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
    {
      te::qt::widgets::Config2DrawPolygons(&canvas, QColor(0, 255, 0, 80), Qt::black, 1);
    }
    break;

    case te::gm::LineStringType:
    case te::gm::LineStringZType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZMType:
    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZMType:
    {
      te::qt::widgets::Config2DrawLines(&canvas, QColor(0, 0, 0, 80), 5);
    }
    break;
  }

  canvas.draw(m_geom);

  te::qt::widgets::Config2DrawPoints(&canvas, "circle", 8, Qt::red, Qt::red, 1);

  // Draw all vertexes
  for(std::size_t i = 0; i < m_lines.size(); ++i)
  {
    te::gm::LineString* line = m_lines[i];

    for(std::size_t j = 0; j < line->getNPoints(); ++j)
    {
      std::auto_ptr<te::gm::Point> point(line->getPointN(j));
      canvas.draw(point.get());
    }
  }

  // Draw the current vertex
  if(m_currentVertexIndex.isValid())
  {
    te::qt::widgets::Config2DrawPoints(&canvas, "circle", 16, Qt::transparent, Qt::blue, 3);

    te::gm::LineString* line = m_lines[m_currentVertexIndex.m_line];

    std::auto_ptr<te::gm::Point> point(line->getPointN(m_currentVertexIndex.m_pos));

    canvas.draw(point.get());
  }

  if(virtualVertex)
  {
    assert(virtualVertex);

    te::qt::widgets::Config2DrawPoints(&canvas, "square", 16, Qt::transparent, Qt::green, 3);
    canvas.draw(virtualVertex);
  }

  m_display->repaint();
}

void te::edit::VertexTool::onExtentChanged()
{
  drawVertexes();
}

QPointF te::edit::VertexTool::getPosition(QMouseEvent* e)
{
#if QT_VERSION >= 0x050000
  return e->localPos();
#else
  return e->posF();
#endif
}

te::gm::Envelope te::edit::VertexTool::buildEnvelope(const QPointF& pos)
{
  QPointF pixelOffset(4.0, 4.0);

  QRectF rect(pos - pixelOffset, pos + pixelOffset);

  // Converts rect boundary to world coordinates
  QPointF ll(rect.left(), rect.bottom());
  QPointF ur(rect.right(), rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  te::gm::Envelope env(ll.x(), ll.y(), ur.x(), ur.y());

  return env;
}

void te::edit::VertexTool::updateRTree()
{
  m_rtree.clear();

  for(std::size_t i = 0; i < m_lines.size(); ++i)
  {
    te::gm::LineString* line = m_lines[i];

    for(std::size_t j = 0; j < line->getNPoints(); ++j)
    {
      te::gm::Envelope e(line->getX(j), line->getY(j), line->getX(j), line->getY(j));

      VertexIndex index;
      index.setIndex(i, j);

      m_rtree.insert(e, index);
    }
  }
}
