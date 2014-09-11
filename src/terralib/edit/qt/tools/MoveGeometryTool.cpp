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
  \file terralib/edit/qt/tools/MoveGeometryTool.cpp

  \brief This class implements a concrete tool to move geometries.
*/

// TerraLib
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../IdGeometry.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "MoveGeometryTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// STL
#include <cassert>
#include <memory>

te::edit::MoveGeometryTool::MoveGeometryTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
  : AbstractTool(display, parent),
    m_layer(layer),
    m_geom(0),
    m_moveStarted(false)
{
  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));

  updateCursor();
}

te::edit::MoveGeometryTool::~MoveGeometryTool()
{
  delete m_geom;
}

bool te::edit::MoveGeometryTool::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  pickGeometry(m_layer, GetPosition(e));

  if(m_geom)
    m_moveStarted = true;

  m_origin = m_display->transform(GetPosition(e));

  m_delta *= 0;

  return true;
}

bool te::edit::MoveGeometryTool::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_moveStarted)
    return false;

  QPointF currentPosition = m_display->transform(GetPosition(e));

  // Calculates the delta value
  m_delta = currentPosition - m_origin;

  // Move geometry using the current delta
  MoveGeometry(m_geom->getGeometry(), m_delta.x(), m_delta.y());

  storeEditedGeometry();

  drawGeometry();

  m_origin = currentPosition;

  return false;
}

bool te::edit::MoveGeometryTool::mouseReleaseEvent(QMouseEvent* e)
{
  m_moveStarted = false;

  return false;
}

bool te::edit::MoveGeometryTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  return false;
}

void te::edit::MoveGeometryTool::reset()
{
  delete m_geom;
  m_geom = 0;

  m_moveStarted = false;

  m_origin *= 0;
  m_delta *= 0;
}

void te::edit::MoveGeometryTool::pickGeometry(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
{
  reset();

  te::gm::Envelope env = buildEnvelope(pos);

  try
  {
    m_geom = PickGeometry(m_layer, env, m_display->getSRID());

    drawGeometry();
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
}

te::gm::Envelope te::edit::MoveGeometryTool::buildEnvelope(const QPointF& pos)
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

void te::edit::MoveGeometryTool::drawGeometry()
{
  const te::gm::Envelope& env = m_display->getExtent();
  if(!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  if(m_geom == 0)
  {
    m_display->repaint();
    return;
  }

  // Initialize the renderer
  Renderer& renderer = Renderer::getInstance();
  renderer.begin(draft, env, m_display->getSRID());

  // Draw the current geometry and the vertexes
  renderer.draw(m_geom->getGeometry(), true);

  renderer.end();

  m_display->repaint();
}

void te::edit::MoveGeometryTool::updateCursor()
{
  m_display->setCursor(Qt::ArrowCursor);
}

void te::edit::MoveGeometryTool::onExtentChanged()
{
  drawGeometry();
}

void te::edit::MoveGeometryTool::storeEditedGeometry()
{
  RepositoryManager::getInstance().addEditedGeometry(m_layer->getId(), m_geom->getId()->clone(), dynamic_cast<te::gm::Geometry*>(m_geom->getGeometry()->clone()));
}
