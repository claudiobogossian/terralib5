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
  \file terralib/edit/qt/tools/VertexTool.cpp

  \brief This class implements a concrete tool for vertex operations (move, add, etc.).
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/LineString.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../geometry/Polygon.h"
#include "../../../geometry/Utils.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "../core/command/UpdateCommand.h"
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

te::edit::VertexTool::VertexTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
: GeometriesUpdateTool(display, layer.get(), parent),
  m_currentStage(FEATURE_SELECTION),
  m_updateWatches(0)
{

  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));

  m_currentVertexIndex.makeInvalid();

  updateCursor();

  draw();
}

te::edit::VertexTool::~VertexTool()
{
  delete m_feature;
  m_updateWatches.clear();
}

bool te::edit::VertexTool::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
  {
    if (m_feature)
      emit readyToSave();

    return false;
  }

  Qt::KeyboardModifiers keys = e->modifiers();

  if(e->button() == Qt::LeftButton && m_currentStage == VERTEX_FOUND)
  {
    // Moving...
    if(keys == Qt::NoModifier)
    {
      setStage(VERTEX_MOVING);
      return true;
    }

    // Removing...
    if(keys == Qt::ShiftModifier)
    {
      RemoveVertex(m_lines, m_currentVertexIndex);

      storeEditedFeature();

      m_currentVertexIndex.makeInvalid();

      setStage(VERTEX_SEARCH);

      updateRTree();

      draw();

      return true;
    }

  }

  // This operation will be handled by mouse double click
  if(m_currentStage == VERTEX_READY_TO_ADD)
    return false;

  // Else...
  setStage(FEATURE_SELECTION);

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

      te::gm::Envelope env = buildEnvelope(GetPosition(e));

      // Search on rtree
      std::vector<VertexIndex> report;
      m_rtree.search(env, report);

      if(!report.empty()) // Vertex found!
      {
        setStage(VERTEX_FOUND);

        m_currentVertexIndex = report[0];

        draw();

        return false;
      }

      // Else, try search "virtual vertex" at a segment
      m_currentVertexIndex.makeInvalid();

      std::auto_ptr<te::gm::Point> borderPoint(0);

      if(m_feature != 0)
      {
        std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&env, m_display->getSRID()));

        if(m_feature->getGeometry()->intersects(geometryFromEnvelope.get()) && !m_feature->getGeometry()->contains(geometryFromEnvelope.get())) // Mouse over a segment?
          borderPoint.reset(new te::gm::Point(env.getCenter().x, env.getCenter().y));
      }

      borderPoint.get() != 0 ? setStage(VERTEX_READY_TO_ADD) : setStage(VERTEX_SEARCH);

      draw(borderPoint.get());

      return false;
    }

    case VERTEX_MOVING:
    {
      QPointF point = GetPosition(e);
      point = m_display->transform(point);

      MoveVertex(m_lines, m_currentVertexIndex, point.x(), point.y());

      storeEditedFeature();

      draw();

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
    case FEATURE_SELECTION:
    {
      if(e->button() != Qt::LeftButton)
        return false;

      pickFeature(m_layer, GetPosition(e));

      if (m_feature)
      {
        setStage(VERTEX_SEARCH);
      }

      storeUndoCommand();

      return true;
    }

    case VERTEX_MOVING:
    {
      updateRTree();

      setStage(VERTEX_SEARCH);

      storeUndoCommand();
    }

    default:
      return false;
  }

  
}

bool te::edit::VertexTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  if(e->button() == Qt::LeftButton && m_currentStage == VERTEX_READY_TO_ADD)
  {
    // Added point
    QPointF point = GetPosition(e);

    // Added point extent
    te::gm::Envelope e = buildEnvelope(point);

    // Convert to world coordinates
    point = m_display->transform(point);

    AddVertex(m_lines, point.x(), point.y(), e, m_display->getSRID());

    storeEditedFeature();

    m_currentVertexIndex.makeInvalid();

    setStage(VERTEX_SEARCH);

    updateRTree();

    draw();

    return true;
  }

  return false;
}

void te::edit::VertexTool::reset()
{
  delete m_feature;
  m_feature = 0;

  setStage(FEATURE_SELECTION);

  m_lines.clear();

  m_currentVertexIndex.makeInvalid();

  m_rtree.clear();
}

void te::edit::VertexTool::pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
{
  te::gm::Envelope env = buildEnvelope(pos);
  pickFeature(layer, env);
}

void te::edit::VertexTool::pickFeature(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& env)
{
  reset();

  try
  {
    m_feature = PickFeature(layer, env, m_display->getSRID(), te::edit::GEOMETRY_UPDATE);

    m_lines.clear();

    if(m_feature != 0)
      GetLines(m_feature->getGeometry(), m_lines);

    updateRTree();

    draw();
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
}

void te::edit::VertexTool::draw(te::gm::Point* virtualVertex)
{

  const te::gm::Envelope& env = m_display->getExtent();
  if(!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Initialize the renderer
  Renderer& renderer = Renderer::getInstance();
  renderer.begin(draft, env, m_display->getSRID());

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  if(m_lines.empty())
  {
    renderer.end();
    m_display->repaint();
    return;
  }

  // Draw the vertexes
  if (RepositoryManager::getInstance().hasIdentify(m_layer->getId(), m_feature->getId()) == false)
    renderer.draw(m_feature->getGeometry(), true);
  else
    renderer.drawVertexes(m_feature->getGeometry());

  // Draw the current vertex
  if(m_currentVertexIndex.isValid())
  {
    te::gm::LineString* line = m_lines[m_currentVertexIndex.m_line];
    std::auto_ptr<te::gm::Point> point(line->getPointN(m_currentVertexIndex.m_pos));

    renderer.setPointStyle("circle", Qt::transparent, Qt::blue, 3, 24);
    renderer.draw(point.get());
  }

  // Draw the virtual vertex
  if(virtualVertex)
  {
    assert(virtualVertex);

    renderer.setPointStyle("circle", Qt::transparent, Qt::darkGreen, 3, 24);
    renderer.draw(virtualVertex);
  }

  renderer.end();

  m_display->repaint();

}

void te::edit::VertexTool::onExtentChanged()
{
  draw();
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

void te::edit::VertexTool::setStage(StageType stage)
{
  m_currentStage = stage;
  updateCursor();
}

void te::edit::VertexTool::updateCursor()
{
  switch(m_currentStage)
  {
    case FEATURE_SELECTION:
    case VERTEX_SEARCH:
       m_display->setCursor(Qt::ArrowCursor);
    break;

    case VERTEX_FOUND:
      m_display->setCursor(Qt::OpenHandCursor);
    break;

    case VERTEX_MOVING:
      m_display->setCursor(Qt::ClosedHandCursor);
    break;

    default:
      m_display->setCursor(Qt::ArrowCursor);
  }
}

void te::edit::VertexTool::storeEditedFeature()
{
  RepositoryManager::getInstance().addFeature(m_layer->getId(), m_feature->clone());

  emit geometriesEdited();

}

void te::edit::VertexTool::storeUndoCommand()
{
    std::size_t count = 0;

    if (m_feature == 0)
      return;

    m_updateWatches.push_back(m_feature->clone());

    for (std::size_t i = 0; i < m_updateWatches.size(); ++i)
    {
      if (m_updateWatches[i]->getId()->getValueAsString() == m_feature->getId()->getValueAsString())
        count++;
    }

    // only to store the first "feature" of geometry
    if (count == 1)
      return;

    QUndoCommand* command = new UpdateCommand(m_updateWatches, m_display, m_layer);
    UndoStackManager::getInstance().addUndoStack(command);

}