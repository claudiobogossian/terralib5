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
  \file terralib/edit/qt/tools/CreatePointTool.cpp

  \brief This class implements a concrete tool to create points.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Point.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "CreatePointTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// STL
#include <cassert>
#include <memory>

te::edit::CreatePointTool::CreatePointTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, const QCursor& cursor, QObject* parent)
  : GeometriesUpdateTool(display, layer.get(), parent),
  m_isFinished(false)
{
  setCursor(cursor);
}

te::edit::CreatePointTool::~CreatePointTool()
{
}

bool te::edit::CreatePointTool::mousePressEvent(QMouseEvent* e)
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

  clear();

  m_coords.push_back(coord);

  draw();

  return true;
}

bool te::edit::CreatePointTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_isFinished = true;

  storeFeature();

  return true;
}

void te::edit::CreatePointTool::draw()
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
    te::gm::Geometry* point = buildPoint();
    renderer.draw(point);
  }

  renderer.end();

  m_display->repaint();
}

void te::edit::CreatePointTool::clear()
{
  m_coords.clear();
}

te::gm::Geometry* te::edit::CreatePointTool::buildPoint()
{
  te::gm::Point* point = new te::gm::Point(m_coords[0].x, m_coords[0].y, m_layer->getSRID());

  point->setSRID(m_display->getSRID());

  if (point->getSRID() == m_layer->getSRID())
    return point;

  // else, need conversion...
  point->transform(m_layer->getSRID());

  return point;
}

void te::edit::CreatePointTool::storeFeature()
{
  RepositoryManager::getInstance().addGeometry(m_layer->getId(), buildPoint(), te::edit::TO_ADD);
}

void te::edit::CreatePointTool::resetVisualizationTool()
{
  clear();
}