/*  Copyright (C) 2001-2016 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ws/ogc/wms/qt/WMSLayerDisplay.cpp

  \brief A display QFrame to draw previews for WMS data.

  \author Emerson Moraes
*/

#include "WMSLayerDisplay.h"

//TerraLib
#include "../../../core/Utils.h"

// STL
#include <cassert>

// Qt
#include <QResizeEvent>

te::ws::ogc::wms::qt::WMSLayerDisplay::WMSLayerDisplay(QWidget *parent, Qt::WindowFlags f)
  : QFrame(parent, f)
{

}

te::ws::ogc::wms::qt::WMSLayerDisplay::~WMSLayerDisplay()
{

}

void te::ws::ogc::wms::qt::WMSLayerDisplay::draw(const te::ws::ogc::wms::WMSGetMapResponse &response)
{
  if(m_canvas.get() == 0)
  {
    m_canvas.reset(new te::qt::widgets::Canvas(width(), height()));
  }

  if(response.m_size == 0 || response.m_format.empty())
  {
    return;
  }

  te::map::ImageType imageType = te::ws::core::FormatToImageType(response.m_format);

  m_canvas->drawImage(const_cast<char*>(response.m_buffer.c_str()), (std::size_t) response.m_size, imageType);

  repaint();
}

void te::ws::ogc::wms::qt::WMSLayerDisplay::clear()
{
  if(m_canvas.get() == 0)
    m_canvas.reset(new te::qt::widgets::Canvas(width(), height()));

  m_canvas->setBackgroundColor(te::color::RGBAColor(255, 255, 255, 0));
  m_canvas->clear();

  repaint();
}

void te::ws::ogc::wms::qt::WMSLayerDisplay::paintEvent(QPaintEvent *e)
{
  if(m_canvas.get() == 0)
    return;

  QPainter painter(this);

  painter.drawPixmap(0, 0, *(m_canvas->getPixmap()));

  painter.end();
}

void te::ws::ogc::wms::qt::WMSLayerDisplay::resizeEvent(QResizeEvent *e)
{
  assert(e);

  if(m_canvas.get() == 0)
    return;

  m_canvas->resize(e->size().width(), e->size().height());

  QFrame::resizeEvent(e);
}
