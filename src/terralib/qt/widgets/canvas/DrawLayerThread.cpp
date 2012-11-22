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
  \file DrawLayerThread.cpp

  \brief This class represents a thread responsible to draw a given layer. Basically, this class receives draw layer requests and generates a QImage as result.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "DrawLayerThread.h"
#include "Canvas.h"

te::qt::widgets::DrawLayerThread::DrawLayerThread(QObject* parent)
  : QThread(parent),
    m_interval(100)
{
  // Signals & slots
  connect(&m_feedback, SIGNAL(timeout()), SLOT(sendFeedback()));
  connect(this, SIGNAL(started()), SLOT(onStarted()));
  connect(this, SIGNAL(finished()), SLOT(onFinished()));
}

te::qt::widgets::DrawLayerThread::~DrawLayerThread()
{
  wait();
}

void te::qt::widgets::DrawLayerThread::draw(te::map::AbstractLayer* layer, const te::gm::Envelope& box, int srid, const QSize& size, const int& index)
{
  /* Note: For while... Actually, I would like to can stop the current draw process and restart with the new request box! te::common::TaskProgress?! */
  if(isRunning())
    return;

  // Storing the values
  m_layer = layer;
  m_env = box;
  m_srid = srid;
  m_index = index;

  // Creates the result image
  if(m_image.size() != size)
  {
    m_image = QImage(size, QImage::Format_ARGB32_Premultiplied);
    m_image.fill(qRgba(0, 0, 0, 0));
  }

  // Requests the thread execution!
  start();
}

void te::qt::widgets::DrawLayerThread::run()
{
  // Prepares the canvas
  Canvas canvas(&m_image);
  canvas.calcAspectRatio(m_env.m_llx, m_env.m_lly, m_env.m_urx, m_env.m_ury);
  canvas.setWindow(m_env.m_llx, m_env.m_lly, m_env.m_urx, m_env.m_ury);
  canvas.clear();

  // Let's draw!
  m_layer->draw(&canvas, m_env, m_srid);
}

void te::qt::widgets::DrawLayerThread::onStarted()
{
  m_feedback.start(m_interval);
}

void te::qt::widgets::DrawLayerThread::sendFeedback()
{
  emit feedback(m_image);
}

void te::qt::widgets::DrawLayerThread::onFinished()
{
  emit drawLayerFinished(m_index, m_image);
  m_image.fill(qRgba(0, 0, 0, 0));
  m_feedback.stop();
}
