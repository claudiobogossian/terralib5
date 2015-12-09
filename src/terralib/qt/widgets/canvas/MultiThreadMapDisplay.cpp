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
  \file terralib/qt/widgets/canvas/MultiThreadMapDisplay.cpp

  \brief A multi thread Qt4 widget to control the display of a set of layers.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../maptools/Utils.h"
#include "DrawLayerThread.h"
#include "MultiThreadMapDisplay.h"

// Qt
#include <QApplication>
#include <QCursor>
#include <QPainter>

te::qt::widgets::MultiThreadMapDisplay::MultiThreadMapDisplay(const QSize& size, const bool& showFeedback, QWidget* parent, Qt::WindowFlags f)
  : te::qt::widgets::MapDisplay(size, parent, f),
    m_showFeedback(showFeedback),
    m_synchronous(false)
{
  setAttribute(Qt::WA_OpaquePaintEvent, true);
}

te::qt::widgets::MultiThreadMapDisplay::MultiThreadMapDisplay(QWidget* parent, const bool& showFeedback, Qt::WindowFlags f)
: te::qt::widgets::MapDisplay(parent, f),
m_showFeedback(showFeedback),
m_synchronous(false)
{
  setAttribute(Qt::WA_OpaquePaintEvent, true);
}

te::qt::widgets::MultiThreadMapDisplay::~MultiThreadMapDisplay()
{
  te::common::FreeContents(m_threads);
}

void te::qt::widgets::MultiThreadMapDisplay::setExtent(te::gm::Envelope& e, bool doRefresh)
{
  if(m_isDrawing)
    return;

  te::map::MapDisplay::setExtent(e);

  updateTransform();

  e = m_extent;

  if(doRefresh)
    refresh();

  emit extentChanged();
}

void te::qt::widgets::MultiThreadMapDisplay::refresh()
{
  if(m_isDrawing)
    return;

  // Cleaning...
  m_displayPixmap->fill(m_backgroundColor);
  m_draftPixmap->fill(Qt::transparent);

  // Considering only the visible layers
  m_visibleLayers.clear();
  te::map::GetVisibleLayers(m_layerList, m_visibleLayers);

  if(m_visibleLayers.empty())
  {
    repaint();
    return;
  }

  int n = m_visibleLayers.size() - m_threads.size();
  for(int i = 0; i < n; ++i)
  {
    DrawLayerThread* thread = new DrawLayerThread(this);

    if(m_showFeedback) // Do you want show feedbacks?
      connect(thread, SIGNAL(feedback(QImage)), this, SLOT(showFeedback(QImage)));

    connect(thread, SIGNAL(drawLayerFinished(int, QImage)), this, SLOT(onDrawLayerFinished(int, QImage)));

    m_threads.push_back(thread);
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  m_isDrawing = true;

  double curScale = getScale();

  std::size_t i = 0;
  std::list<te::map::AbstractLayerPtr>::reverse_iterator it;
  for(it = m_visibleLayers.rbegin(); it != m_visibleLayers.rend(); ++it) // for each layer
  {
    m_threads[i]->draw(it->get(), m_extent, m_srid, curScale, size(), i);
    i++;
  }

  if(m_synchronous)
  {
    QEventLoop wait;
    connect(this, SIGNAL(drawLayersFinished(const QMap<QString, QString>&)), &wait, SLOT(quit()));
    wait.exec();
  }
}

QPointF te::qt::widgets::MultiThreadMapDisplay::transform(const QPointF& p)
{
  if(!m_extent.isValid())
    return QPointF();

  return m_matrix.inverted().map(p);
}

void te::qt::widgets::MultiThreadMapDisplay::setSynchronous(bool on)
{
  m_synchronous = on;
}

void te::qt::widgets::MultiThreadMapDisplay::updateTransform()
{
  if(!m_extent.isValid())
    return;

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
}

void te::qt::widgets::MultiThreadMapDisplay::showFeedback(const QImage& image)
{
  QPainter painter(m_displayPixmap);
  painter.setOpacity(0.1); // To improve user visual experience!
  painter.drawImage(0, 0, image);
  //painter.drawPixmap(0, 0, QPixmap::fromImage(image));
  repaint(); // or update()? Which is the best here?!
}

void te::qt::widgets::MultiThreadMapDisplay::onDrawLayerFinished(const int& index, const QImage& image)
{
  m_images.insert(std::pair<int, QImage>(index, image));
  if(m_images.size() != m_visibleLayers.size())
  {
    QPainter painter(m_displayPixmap);
    painter.drawImage(0, 0, image);
    //painter.drawPixmap(0, 0, QPixmap::fromImage(image));
    painter.end();

    repaint();

    return;
  }

  m_displayPixmap->fill(m_backgroundColor);

  QPainter painter(m_displayPixmap);

  std::list<te::map::AbstractLayerPtr>::reverse_iterator itLayer = m_visibleLayers.rbegin();
  std::map<int, QImage>::iterator it;
  for(it = m_images.begin(); it != m_images.end(); ++it)
  {
    painter.setCompositionMode((QPainter::CompositionMode)itLayer->get()->getCompositionMode());

    painter.drawImage(0, 0, it->second);
    //painter.drawPixmap(0, 0, QPixmap::fromImage(it->second));

    ++itLayer;
  }

  painter.end();

  m_images.clear();

  repaint(); // or update()? Which is the best here?!

  m_isDrawing = false;

  QApplication::restoreOverrideCursor();

  // Building the error messages
  QMap<QString, QString> errors;
  for(std::size_t i = 0; i < m_threads.size(); ++i)
  {
    DrawLayerThread* t = m_threads[i];
    if(t->finishedWithSuccess())
      continue;
    errors.insert(t->getLayer()->getId().c_str(), t->getErrorMessage());
  }

  emit drawLayersFinished(errors);
}
