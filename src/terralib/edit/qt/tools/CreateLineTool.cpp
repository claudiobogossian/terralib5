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
  \file terralib/edit/qt/tools/CreateLineTool.cpp

  \brief This class implements a concrete tool to create lines.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/LineString.h"
#include "../../../geometry/Point.h"
#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../Utils.h"
#include "CreateLineTool.h"

// Qt
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// STL
#include <cassert>
#include <memory>

te::edit::CreateLineTool::CreateLineTool(te::qt::widgets::MapDisplay* display, const QCursor& cursor, QObject* parent) 
  : AbstractTool(display, parent),
    m_continuousMode(false),
    m_isFinished(false)
{
  setCursor(cursor);

  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));
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
  m_coords.push_back(te::gm::Coord2D(pw.x(), pw.y()));

  return true;
}

bool te::edit::CreateLineTool::mouseMoveEvent(QMouseEvent* e)
{
  if(m_coords.size() < 1 || m_isFinished)
    return false;

  QPointF pos = GetPosition(e);

  QPointF pw = m_display->transform(pos);

  m_coords.push_back(te::gm::Coord2D(pw.x(), pw.y()));

  m_lastPos = te::gm::Coord2D(pw.x(), pw.y());

  Qt::KeyboardModifiers keys = e->modifiers();

  if(keys == Qt::NoModifier)
    m_continuousMode = false;
  else if(keys == Qt::ShiftModifier)
    m_continuousMode = true;

  drawLine();

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

  return true;
}

void te::edit::CreateLineTool::drawLine()
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

  // Let's draw!
  drawLine(canvas);

  if(m_continuousMode == false)
    m_coords.pop_back();

  m_display->repaint();
}

void te::edit::CreateLineTool::drawLine(te::qt::widgets::Canvas& canvas)
{
  // Build the geometry
  te::gm::LineString* line = new te::gm::LineString(m_coords.size(), te::gm::LineStringType);
  for(std::size_t i = 0; i < m_coords.size(); ++i)
    line->setPoint(i, m_coords[i].x, m_coords[i].y);

  // Let's draw!
  DrawGeometry(&canvas, line, m_display->getSRID());

  drawVertexes(canvas);

  delete line;
}

void te::edit::CreateLineTool::drawVertexes(te::qt::widgets::Canvas& canvas)
{
  te::qt::widgets::Config2DrawPoints(&canvas, "circle", 8, Qt::red, Qt::red, 1);

  DrawVertexes(&canvas, m_coords, m_display->getSRID(), m_display->getSRID());
}

void te::edit::CreateLineTool::clear()
{
  m_coords.clear();

  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);
    
  m_display->repaint();
}

void te::edit::CreateLineTool::onExtentChanged()
{
  if(m_coords.empty())
    return;

  m_coords.push_back(m_lastPos);

  drawLine();
}
