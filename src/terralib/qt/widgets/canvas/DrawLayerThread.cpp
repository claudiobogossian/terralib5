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
  \file DrawLayerThread.cpp

  \brief This class represents a thread responsible to draw a given layer. Basically, this class receives draw layer requests and generates a QImage as result.
*/

// TerraLib
#include "../../../dataaccess/Exception.h"
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

  m_finishedWithSuccess = true;
  m_errorMessage.clear();

  // Creates the result image
  if(m_image.size() != size)
    m_image = QImage(size, QImage::Format_ARGB32_Premultiplied);

   m_image.fill(qRgba(0, 0, 0, 0));


  // Requests the thread execution!
  start();
}

bool te::qt::widgets::DrawLayerThread::finishedWithSuccess() const
{
  return m_finishedWithSuccess;
}

QString te::qt::widgets::DrawLayerThread::getErrorMessage() const
{
  return m_errorMessage;
}

te::map::AbstractLayer* te::qt::widgets::DrawLayerThread::getLayer() const
{
  return m_layer;
}

void te::qt::widgets::DrawLayerThread::run()
{
  // Prepares the canvas
  Canvas canvas(&m_image);
  //canvas.setRenderHint(QPainter::Antialiasing);
  canvas.calcAspectRatio(m_env.m_llx, m_env.m_lly, m_env.m_urx, m_env.m_ury);
  canvas.setWindow(m_env.m_llx, m_env.m_lly, m_env.m_urx, m_env.m_ury);
  canvas.clear();

  while(true)
  {
    // Let's draw!
    try
    {
      m_layer->draw(&canvas, m_env, m_srid);
      break;
    }
    catch(const te::da::Exception& e)
    {
      if(e.code() != te::common::NO_CONNECTION_AVAILABLE)
      {
        m_finishedWithSuccess = false;
        m_errorMessage = QString(tr("The layer") + " %1 " + tr("could not be drawn! Details:") + " %2").arg(m_layer->getTitle().c_str()).arg(e.what());
        break;
      }
      msleep(100);
    }
    catch(const std::exception& e)
    {
      m_finishedWithSuccess = false;
      m_errorMessage = QString(tr("The layer") + " %1 " + tr("could not be drawn! Details:") + " %2").arg(m_layer->getTitle().c_str()).arg(e.what());
      break;
    }
  }
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
