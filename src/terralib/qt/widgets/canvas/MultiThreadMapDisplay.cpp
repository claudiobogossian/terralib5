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
#include "../../../maptools/AbstractLayer.h"
#include "../../../se/Style.h"
#include "MultiThreadMapDisplay.h"
#include "Canvas.h"
#include "DrawThread.h"
#include "ThreadManager.h"

// Qt
#include <QApplication>
#include <QImage>
#include <QPainter>

void RemoveImage(const std::string lId, std::map<std::string, QImage*>& imgs)
{
  std::map<std::string, QImage*>::iterator it = imgs.find(lId);

  if(it != imgs.end())
  {
    delete it->second;
    imgs.erase(lId);
  }
}

void RemoveImage(te::map::AbstractLayer* layer, std::map<std::string, QImage*>& imgs)
{
  std::string lId = layer->getId();
  RemoveImage(lId, imgs);
}

te::qt::widgets::MultiThreadMapDisplay::MultiThreadMapDisplay(const QSize& size, const bool& showFeedback, QWidget* parent, Qt::WindowFlags f)
  : te::qt::widgets::MapDisplay(size, parent, f),
    m_showFeedback(showFeedback),
    m_synchronous(false),
    m_tmger(0)
{
  setAttribute(Qt::WA_OpaquePaintEvent, true);
}

te::qt::widgets::MultiThreadMapDisplay::MultiThreadMapDisplay(QWidget* parent, const bool& showFeedback, Qt::WindowFlags f)
: te::qt::widgets::MapDisplay(parent, f),
m_showFeedback(showFeedback),
m_synchronous(false),
m_tmger(0)
{
  setAttribute(Qt::WA_OpaquePaintEvent, true);
}

te::qt::widgets::MultiThreadMapDisplay::~MultiThreadMapDisplay()
{
  te::common::FreeContents(m_images);
  m_images.clear();
}

void te::qt::widgets::MultiThreadMapDisplay::setExtent(te::gm::Envelope& e, bool doRefresh)
{
  if(m_isDrawing)
    return;

  m_extent = e;

  updateTransform();

  e = m_extent;

  if(doRefresh)
    refresh(doRefresh);

  emit extentChanged();
}

void te::qt::widgets::MultiThreadMapDisplay::refresh(bool redraw)
{
  m_cancel = false;

  if(m_isDrawing)
  {
    m_cancel = true;

    m_tmger->stopProccess();

    te::common::FreeContents(m_images);
    m_images.clear();

    te::common::FreeContents(m_threads);
    m_threads.clear();

    m_cancel = false;
    m_isDrawing = false;
  }

  m_oldCursor = cursor();

  setCursor(Qt::BusyCursor);

//  MapDisplay::blockSignals(true);

  m_draftPixmap->fill(Qt::transparent);

  // Considering only the visible layers
  m_visibleLayers.clear();

  te::map::GetVisibleLayers(m_layerList, m_visibleLayers);

  if(m_visibleLayers.empty())
  {
    te::common::FreeContents(m_images);
    m_images.clear();

    m_displayPixmap->fill(m_backgroundColor);

    repaint();

    m_isDrawing = false;

    return;
  }

  m_isDrawing = true;

  if(redraw)
  {
    te::common::FreeContents(m_images);
    m_images.clear();
  }

  std::map<std::string, QImage*> imgs;

  double scale = getScale();

  for(std::list<te::map::AbstractLayerPtr>::iterator it = m_visibleLayers.begin(); it != m_visibleLayers.end(); ++it)
  {
    std::string lId = (*it)->getId();

    std::map<std::string, QImage*>::iterator imgIt = m_images.find(lId);

    if(imgIt != m_images.end())
    {
      imgs[lId] = imgIt->second;
      m_images.erase(lId);
    }
    else
    {
      QImage* img = new QImage(size(), QImage::Format_ARGB32_Premultiplied);
      imgs[lId] = img;

      DrawThread* thread = new DrawThread(imgs[lId], (*it).get(), &m_extent, m_backgroundColor, m_srid, scale, m_hAlign, m_vAlign);
      m_threads.push_back(thread);
    }
  }

  if(!m_images.empty())
  {
    te::common::FreeContents(m_images);
    m_images.clear();
  }

  m_images = imgs;

  if(m_tmger != 0)
    delete m_tmger;
    
  m_tmger = 0;

  if(!m_threads.empty())
  {
    int interval = (m_showFeedback) ? 1000 : -1;

    m_tmger = new ThreadManager(m_threads, interval);

    connect(m_tmger, SIGNAL(showFeedback()), SLOT(showFeedback()));

    connect(m_tmger, SIGNAL(finished()), SLOT(onRenderingFinished()));

    m_tmger->run();
  }
  else
    onRenderingFinished();
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

void te::qt::widgets::MultiThreadMapDisplay::updateLayer(te::map::AbstractLayerPtr layer, bool redraw)
{
  RemoveImage(layer.get(), m_images);

  if(redraw)
    refresh();
}

void te::qt::widgets::MultiThreadMapDisplay::resizeEvent(QResizeEvent* e)
{
  if(!m_images.empty())
  {
    te::common::FreeContents(m_images);
    m_images.clear();
  }

  MapDisplay::resizeEvent(e);
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

void te::qt::widgets::MultiThreadMapDisplay::showFeedback()
{
  m_displayPixmap->fill(m_backgroundColor);

  QPainter painter(m_displayPixmap);

  for(std::list<te::map::AbstractLayerPtr>::reverse_iterator it = m_visibleLayers.rbegin(); it != m_visibleLayers.rend(); ++it)
  {
    te::map::AbstractLayer* l = (*it).get();

    painter.setCompositionMode((QPainter::CompositionMode)l->getCompositionMode());

    painter.drawImage(0, 0, *static_cast<QImage*>(m_images[l->getId()]));
  }

  repaint(); // or update()? Which is the best here?!
}

void te::qt::widgets::MultiThreadMapDisplay::onDrawLayerFinished(const int& index, const QImage& image)
{
}

void te::qt::widgets::MultiThreadMapDisplay::onRenderingFinished()
{
  showFeedback();

  m_isDrawing = false;

  QMap<QString, QString> errors;

  for(std::vector<QRunnable*>::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
  {
    DrawThread* th = (DrawThread*)*it;

    if(!th->hasFinished())
    {
      QString lId = th->layerId();
      QString errorm = th->errorMessage();

      if(!errorm.isEmpty())
        errors[lId] = errorm;

      RemoveImage(lId.toStdString(), m_images);
    }
  }

  //if(m_tmger != 0)
  //  m_tmger->stopProccess();

  if(!m_threads.empty())
  {
    te::common::FreeContents(m_threads);
    m_threads.clear();
  }

  setCursor(m_oldCursor);

  emit drawLayersFinished(errors);
}

void te::qt::widgets::MultiThreadMapDisplay::onDrawCanceled()
{
  m_cancel = true;
  m_tmger->stopProccess();
//  onRenderingFinished();
}

