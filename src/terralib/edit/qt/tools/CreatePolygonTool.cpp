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
  \file terralib/edit/qt/tools/CreatePolygonTool.cpp

  \brief This class implements a concrete tool to create polygons.
*/

// TerraLib
#include "../../../geometry.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "../core/command/AddCommand.h"
#include "../core/UndoStackManager.h"
#include "CreatePolygonTool.h"

// Qt
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// STL
#include <cassert>
#include <memory>

te::edit::CreatePolygonTool::CreatePolygonTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, const QCursor& cursor, Qt::MouseButton sideToClose, QObject* parent)
: GeometriesUpdateTool(display, layer.get(), parent),
    m_continuousMode(false),
    m_isFinished(false),
    m_addWatches(0),
    m_currentIndex(0),
    m_sideToClose(sideToClose)
{
  setCursor(cursor);

  draw();
}

te::edit::CreatePolygonTool::~CreatePolygonTool()
{
  delete m_feature;
  clear();
}

bool te::edit::CreatePolygonTool::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
    return false;

  if(m_isFinished) // Is Finished?! So, start again...
  {
    clear();
    m_isFinished = false;
  }

  QPointF pw = m_display->transform(GetPosition(e));

  te::gm::Coord2D coord = te::gm::Coord2D(pw.x(), pw.y());

  TrySnap(coord, m_display->getSRID());

  m_coords.push_back(coord);

  storeUndoCommand();

  return true;
}

bool te::edit::CreatePolygonTool::mouseMoveEvent(QMouseEvent* e)
{
  if(m_coords.size() < 1 || m_isFinished)
    return false;

  QPointF pos = GetPosition(e);
  pos += QPointF(0.0001, 0.0001); // To avoid collinear points on polygon

  QPointF pw = m_display->transform(pos);

  te::gm::Coord2D coord = te::gm::Coord2D(pw.x(), pw.y());

  TrySnap(coord, m_display->getSRID());

  m_coords.push_back(coord);

  m_lastPos = te::gm::Coord2D(coord.x, coord.y);

  if (e->buttons() & Qt::LeftButton)
  {
    m_continuousMode = true;

    storeUndoCommand();
  }
  else
    m_continuousMode = false;

  draw();

  return false;
}

bool te::edit::CreatePolygonTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  if (m_sideToClose != Qt::LeftButton)
    return false;

  if (e->button() != Qt::LeftButton)
    return false;

  if(m_coords.size() < 3) // Can not stop yet...
    return false;

  m_isFinished = true;

  storeFeature();

  return true;
}

bool te::edit::CreatePolygonTool::mouseReleaseEvent(QMouseEvent* e)
{
  if (m_sideToClose != Qt::RightButton)
    return false;

  if (e->button() != Qt::RightButton)
    return false;

  if (m_coords.size() < 3) // Can not stop yet...
    return false;

  m_isFinished = true;

  storeFeature();

  UndoStackManager::getInstance().getUndoStack()->clear();

  return true;
}

void te::edit::CreatePolygonTool::draw()
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

  if(!m_coords.empty())
  {
    // Draw the geometry being created
    if(m_coords.size() < 3)
      drawLine();
    else if (m_currentIndex > -1)
      drawPolygon();

    if(m_continuousMode == false)
      m_coords.pop_back();
  }

  renderer.end();

  m_display->repaint();
}

void te::edit::CreatePolygonTool::drawPolygon()
{
  // Build the geometry
  te::gm::Geometry* polygon = buildPolygon();

  // Draw the current geometry and the vertexes
  Renderer& renderer = Renderer::getInstance();
  renderer.draw(polygon, true);

  delete polygon;
}

void te::edit::CreatePolygonTool::drawLine()
{
  // Build the geometry
  te::gm::Geometry* line = buildLine();

  // Draw the current geometry and the vertexes
  Renderer& renderer = Renderer::getInstance();
  renderer.draw(line, true);

  delete line;
}

void te::edit::CreatePolygonTool::clear()
{
  m_feature = 0;
  m_currentIndex = 0;
  m_coords.clear();
  m_addWatches.clear();

  UndoStackManager::getInstance().getUndoStack()->clear();
}

te::gm::Geometry* te::edit::CreatePolygonTool::buildPolygon()
{
  // Build the geometry
  te::gm::LinearRing* ring = new te::gm::LinearRing(m_coords.size() + 1, te::gm::LineStringType);
  for(std::size_t i = 0; i < m_coords.size(); ++i)
    ring->setPoint(i, m_coords[i].x, m_coords[i].y);
  ring->setPoint(m_coords.size(), m_coords[0].x, m_coords[0].y); // Closing...

  te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
  polygon->setRingN(0, ring);

  polygon->setSRID(m_display->getSRID());

  if(polygon->getSRID() == m_layer->getSRID())
    return polygon;

  // else, need conversion...
  polygon->transform(m_layer->getSRID());

  return polygon;
}

te::gm::Geometry* te::edit::CreatePolygonTool::buildLine()
{
  te::gm::LineString* line = new te::gm::LineString(m_coords.size(), te::gm::LineStringType);
  for(std::size_t i = 0; i < m_coords.size(); ++i)
    line->setPoint(i, m_coords[i].x, m_coords[i].y);

  line->setSRID(m_display->getSRID());

  if(line->getSRID() == m_layer->getSRID())
    return line;

  // else, need conversion...
  line->transform(m_layer->getSRID());

  return line;
}

void te::edit::CreatePolygonTool::storeFeature()
{
  RepositoryManager::getInstance().addFeature(m_layer->getId(), m_feature->clone());

  emit geometriesEdited();
}

void te::edit::CreatePolygonTool::onExtentChanged()
{
  if(m_coords.empty())
    return;

  m_coords.push_back(m_lastPos);

  draw();
}

void te::edit::CreatePolygonTool::storeUndoCommand()
{
  if (buildPolygon()->getNPoints() < 4)
    return;

  if (m_coords.empty())
    return;

  m_coords.push_back(m_lastPos);

  if (m_feature == 0)
    m_feature = new Feature();

  m_feature->setGeometry(convertGeomType(m_layer,buildPolygon()));
  m_feature->setOperation(te::edit::GEOMETRY_CREATE);
  m_feature->setCoords(m_coords);

  m_addWatches.push_back(m_feature->clone());

  if (m_currentIndex < (int)(m_addWatches.size() - 2))
  {
    std::size_t i = 0;
    while (i < m_addWatches.size())
    {
      m_addWatches.pop_back();
      i = (m_currentIndex + 1);
    }
    m_addWatches.push_back(m_feature->clone()); 
  }

  m_currentIndex = (int)(m_addWatches.size() - 1);

  QUndoCommand* command = new AddCommand(m_addWatches, m_currentIndex, m_layer);
  connect(dynamic_cast<AddCommand*>(command), SIGNAL(geometryAcquired(te::gm::Geometry*, std::vector<te::gm::Coord2D>)), SLOT(onGeometryAcquired(te::gm::Geometry*, std::vector<te::gm::Coord2D>)));

  UndoStackManager::getInstance().addUndoStack(command);
}

void te::edit::CreatePolygonTool::onGeometryAcquired(te::gm::Geometry* geom, std::vector<te::gm::Coord2D> coords)
{
  m_feature->setGeometry(convertGeomType(m_layer, geom));

  m_coords.clear();
  m_coords = coords;

  if (m_isFinished)
  {
    if (m_currentIndex > -1)
      RepositoryManager::getInstance().addFeature(m_layer->getId(), m_feature->clone());
    else
      RepositoryManager::getInstance().removeFeature(m_layer->getId(), m_feature->getId());
  }

  draw();
}

void te::edit::CreatePolygonTool::resetVisualizationTool()
{
  clear();
}
