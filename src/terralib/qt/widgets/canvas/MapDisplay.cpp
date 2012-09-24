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
  \file BasicMapDisplay.cpp

  \brief A Qt4 widget to control the display of a set of layers.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "MapDisplay.h"
#include "Canvas.h"
#include "ScopedCursor.h"

// Qt
#include <QtCore/QTimer>
#include <QtGui/QResizeEvent>

te::qt::widgets::MapDisplay::MapDisplay(const QSize& size, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    te::map::MapDisplay(),
    m_displayPixmap(new QPixmap(size)),
    m_draft(new QPixmap(size)),
    m_backgroundColor(Qt::white),
    m_resizePolicy(te::qt::widgets::MapDisplay::Fixed),
    m_timer(new QTimer(this)),
    m_interval(0)
{
  m_timer->setSingleShot(true);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(onResizeTimeout()));

  m_displayPixmap->fill(m_backgroundColor);
  m_draft->fill(Qt::transparent);

  resize(size);
}

te::qt::widgets::MapDisplay::~MapDisplay()
{
  delete m_displayPixmap;
  delete m_draft;
}

void te::qt::widgets::MapDisplay::setExtent(const te::gm::Envelope& e)
{
  te::map::MapDisplay::setExtent(e);

  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::qt::widgets::Canvas* canvas = it->second;
    canvas->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
    canvas->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
    canvas->clear();
  }

  draw();
}

QPixmap* te::qt::widgets::MapDisplay::getDisplayPixmap()
{
  return m_displayPixmap;
}

void te::qt::widgets::MapDisplay::setResizePolicy(const ResizePolicy& policy)
{
  m_resizePolicy = policy;
}

void te::qt::widgets::MapDisplay::setResizeInterval(int msec)
{
  m_interval = msec;
}

void te::qt::widgets::MapDisplay::draw()
{
  ScopedCursor cursor(Qt::WaitCursor);

  // Cleaning...
  m_displayPixmap->fill(m_backgroundColor);

  QPainter painter(m_displayPixmap);

  std::list<te::map::AbstractLayer*>::iterator it;
  for(it = m_layerList.begin(); it != m_layerList.end(); ++it) // for each layer
    draw(*it, painter);

  update();
}

void te::qt::widgets::MapDisplay::draw(te::map::AbstractLayer* layer, QPainter& painter)
{
  // Checking the visibility...
  if(layer->getVisibility() == te::map::NOT_VISIBLE)
    return;

  // Recursive draw
  for(std::size_t i = 0; i < layer->getChildrenCount(); ++i)
    draw(layer, painter);
  
  // Retrieves a canvas to current layer
  te::qt::widgets::Canvas* canvas = getCanvas(layer);

  // Draw the current layer
  layer->draw(canvas, *m_extent, m_srid);

  // Composes the result
  painter.drawPixmap(0, 0, *(canvas->getPixmap()));
}

te::qt::widgets::Canvas* te::qt::widgets::MapDisplay::getCanvas(te::map::AbstractLayer* layer)
{
  // Is there a canvas associated with the given layer?
  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it = m_layerCanvasMap.find(layer);
  if(it != m_layerCanvasMap.end())
    return it->second;

  // else, create one!
  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(m_displayPixmap->width(), m_displayPixmap->height());
  canvas->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
  canvas->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
  canvas->clear();

  m_layerCanvasMap[layer] = canvas;
  
  return canvas;
}

void te::qt::widgets::MapDisplay::resizeAllCanvas()
{
  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
    it->second->resize(m_displayPixmap->width(), m_displayPixmap->height());
}

void te::qt::widgets::MapDisplay::paintEvent(QPaintEvent* e)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, *m_displayPixmap);
  painter.drawPixmap(0, 0, *m_draft);
}

void te::qt::widgets::MapDisplay::resizeEvent(QResizeEvent* e)
{
  QWidget::resizeEvent(e);

  // Stores the old size
  if(!m_oldSize.isValid())
    e->oldSize().isValid() ? m_oldSize = e->oldSize() : m_oldSize = e->size();

  // Setups the timer controller
  m_timer->start(m_interval);
}

void te::qt::widgets::MapDisplay::onResizeTimeout()
{
  // Rebulding the map display pixmaps
  delete m_displayPixmap;
  m_displayPixmap = new QPixmap(size());
  m_displayPixmap->fill(m_backgroundColor);

  delete m_draft;
  m_draft = new QPixmap(size());
  m_draft->fill(Qt::transparent);

  // Resizing all canvas
  resizeAllCanvas();

  // Adjust the extent based on resize policy
  adjustExtent(m_oldSize, size());

  // Invalidate old size
  m_oldSize = QSize();
}

void te::qt::widgets::MapDisplay::adjustExtent(const QSize& oldSize, const QSize& size)
{
  if(m_extent == 0)
    return;

  te::gm::Envelope e = *m_extent;
  if(m_resizePolicy == te::qt::widgets::MapDisplay::Fixed)
    setExtent(e);

  double widthW = e.m_urx - e.m_llx;
  double heightW = e.m_ury - e.m_lly;

  double newWidthW = (size.width() * widthW) / oldSize.width();
  double newHeightW = (size.height() * heightW) / oldSize.height();

  switch(m_resizePolicy)
  {
    case te::qt::widgets::MapDisplay::Cut:
    {
      e.m_urx = e.m_llx + newWidthW;
      e.m_lly = e.m_ury - newHeightW;
    }
    break;

    case te::qt::widgets::MapDisplay::Center:
    {
      te::gm::Coord2D center = e.getCenter();
      e.m_llx = center.x - (newWidthW * 0.5);
      e.m_urx = center.x + (newWidthW * 0.5);
      e.m_lly = center.y - (newHeightW * 0.5);
      e.m_ury = center.y + (newHeightW * 0.5);
    }
    break;
  }

  setExtent(e);
}
