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
  \file MultiThreadMapDisplay.cpp

  \brief A multi thread Qt4 widget to control the display of a set of layers.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "DrawLayerThread.h"
#include "MultiThreadMapDisplay.h"

// Qt
#include <QtGui/QPainter>

te::qt::widgets::MultiThreadMapDisplay::MultiThreadMapDisplay(const QSize& size, const bool& showFeedback, QWidget* parent, Qt::WindowFlags f)
  : te::qt::widgets::MapDisplay(size, parent, f),
    m_showFeedback(showFeedback),
    m_isDrawing(false)
{
  setAttribute(Qt::WA_OpaquePaintEvent, true);
}

te::qt::widgets::MultiThreadMapDisplay::~MultiThreadMapDisplay()
{
  te::common::FreeContents(m_threads);
}

void te::qt::widgets::MultiThreadMapDisplay::setLayerList(const std::list<te::map::AbstractLayerPtr>& layers)
{
  te::qt::widgets::MapDisplay::setLayerList(layers);

  if(layers.size() <= m_threads.size())
    return;

  std::size_t n = layers.size() - m_threads.size();
  for(std::size_t i = 0; i < n; ++i)
  {
    DrawLayerThread* thread = new DrawLayerThread;

    if(m_showFeedback) // Do you want show feedbacks?
      connect(thread, SIGNAL(feedback(QImage)), this, SLOT(showFeedback(QImage)));

    connect(thread, SIGNAL(drawLayerFinished(int, QImage)), this, SLOT(onDrawLayerFinished(int, QImage)));

    m_threads.push_back(thread);
  }
}

void te::qt::widgets::MultiThreadMapDisplay::setExtent(te::gm::Envelope& e, bool doRefresh)
{
  if(m_isDrawing)
    return;

  te::map::MapDisplay::setExtent(e);

  updateTransform(); /*  For while... I need the class CoordTransform! */

  e = m_extent;

  if(m_layerList.empty())
    return;

  m_isDrawing = true;

  if(doRefresh)
    refresh();

  emit extentChanged();
}

void te::qt::widgets::MultiThreadMapDisplay::refresh()
{
  // Cleaning...
  m_displayPixmap->fill(m_backgroundColor);

  std::size_t i = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it;
  for(it = m_layerList.begin(); it != m_layerList.end(); ++it) // for each layer
  {
    m_threads[i]->draw(it->get(), m_extent, m_srid, size(), i);
    i++;
  }
}

QPointF te::qt::widgets::MultiThreadMapDisplay::transform(const QPointF& p)
{
  if(!m_extent.isValid())
    return QPointF();

  return m_matrix.inverted().map(p);
}

void te::qt::widgets::MultiThreadMapDisplay::updateTransform()
{
  if(!m_extent.isValid())
    return;

  /*  Note: For while... I need the class CoordTransform! */
  
  // Compute aspect ratio
  double ww = m_extent.m_urx - m_extent.m_llx;
  double wh = m_extent.m_ury - m_extent.m_lly;

  double widthByHeight = static_cast<double>(width()) / static_cast<double>(height());

  if(widthByHeight > ww / wh)
  {
    double v = ww;
    ww = wh * widthByHeight;
    m_extent.m_llx = m_extent.m_llx - (ww - v) * 0.5;
    m_extent.m_urx = m_extent.m_llx + ww;
  }
  else
  {
    double v = wh;
    wh = ww / widthByHeight;
    m_extent.m_lly = m_extent.m_lly - (wh - v) * 0.5;
    m_extent.m_ury = m_extent.m_lly + wh;
  }

  // Bulding the transform matrix
  double xScale = static_cast<double>(width()) / (m_extent.m_urx - m_extent.m_llx);
  double yScale = static_cast<double>(height()) / (m_extent.m_ury - m_extent.m_lly);
  m_matrix.reset();
  m_matrix.scale(xScale, -yScale);
  m_matrix.translate(-m_extent.m_llx, -m_extent.m_ury);

  /* end note */
}

void te::qt::widgets::MultiThreadMapDisplay::showFeedback(const QImage& image)
{
  QPainter painter(m_displayPixmap);
  painter.setOpacity(0.1); // To improve user visual experience!
  painter.drawImage(0, 0, image);
  repaint(); // or update()? Which is the best here?!
}

void te::qt::widgets::MultiThreadMapDisplay::onDrawLayerFinished(const int& index, const QImage& image)
{
  m_images.insert(std::pair<int, QImage>(index, image));
  if(m_images.size() != m_layerList.size())
    return;

  m_displayPixmap->fill(m_backgroundColor);

  QPainter painter(m_displayPixmap);

  std::map<int, QImage>::iterator it;
  for(it = m_images.begin(); it != m_images.end(); ++it)
    painter.drawImage(0, 0, it->second);

  m_images.clear();

  repaint(); // or update()? Which is the best here?!

  m_isDrawing = false;
}
