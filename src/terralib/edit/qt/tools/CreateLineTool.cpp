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
  \file terralib/edit/qt/tools/CreateLineTool.cpp

  \brief This class implements a concrete tool to create lines.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/LineString.h"
#include "../../../geometry/Point.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "CreateLineTool.h"

// Qt
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// STL
#include <cassert>
#include <memory>

te::edit::CreateLineTool::CreateLineTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, const QCursor& cursor, QObject* parent) 
  : AbstractTool(display, parent),
    m_layer(layer),
    m_continuousMode(false),
    m_isFinished(false)
{
  setCursor(cursor);

  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));

  draw();
}

te::edit::CreateLineTool::~CreateLineTool()
{
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);
}

bool te::edit::CreateLineTool::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
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

  return true;
}

bool te::edit::CreateLineTool::mouseMoveEvent(QMouseEvent* e)
{
  if(m_coords.size() < 1 || m_isFinished)
    return false;

  QPointF pos = GetPosition(e);

  QPointF pw = m_display->transform(pos);

  te::gm::Coord2D coord = te::gm::Coord2D(pw.x(), pw.y());

  TrySnap(coord, m_display->getSRID());

  m_coords.push_back(coord);

  m_lastPos = te::gm::Coord2D(coord.x, coord.y);

  Qt::KeyboardModifiers keys = e->modifiers();

  if(keys == Qt::NoModifier)
    m_continuousMode = false;
  else if(keys == Qt::ShiftModifier)
    m_continuousMode = true;

  draw();

  return false;
}

bool te::edit::CreateLineTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::edit::CreateLineTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_isFinished = true;

  storeNewGeometry();

  return true;
}

void te::edit::CreateLineTool::draw()
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
    te::gm::Geometry* line = buildLine();
    renderer.draw(line, true);

    if(m_continuousMode == false)
      m_coords.pop_back();
  }

  renderer.end();

  m_display->repaint();
}

void te::edit::CreateLineTool::clear()
{
  m_coords.clear();
}

te::gm::Geometry* te::edit::CreateLineTool::buildLine()
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

void te::edit::CreateLineTool::storeNewGeometry()
{
  RepositoryManager::getInstance().addGeometry(m_layer->getId(), buildLine());
}

void te::edit::CreateLineTool::onExtentChanged()
{
  if(m_coords.empty())
    return;

  m_coords.push_back(m_lastPos);

  draw();
}
