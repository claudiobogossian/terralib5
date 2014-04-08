/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ContextLayoutItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ContextLayoutItem.h"

te::layout::ContextLayoutItem::ContextLayoutItem() :
  m_zoomFactor(1.),
  m_pixmap(0),
  m_show(false),
  m_wait(false),
  m_resizeCanvas(true)
{

}

te::layout::ContextLayoutItem::ContextLayoutItem( LayoutParams params, double zoomFactor ) :
  m_params(params),
  m_zoomFactor(zoomFactor),
  m_pixmap(0),
  m_show(false),
  m_wait(false),
  m_resizeCanvas(true)
{

}

te::layout::ContextLayoutItem::~ContextLayoutItem()
{

}

void te::layout::ContextLayoutItem::setParams( LayoutParams params )
{
  m_params = params;
}

te::layout::LayoutParams te::layout::ContextLayoutItem::getParams()
{
  return m_params;
}

void te::layout::ContextLayoutItem::setZoomFactor( double zoomFactor )
{
  m_zoomFactor = zoomFactor;
}

double te::layout::ContextLayoutItem::getZoomFactor()
{
  return m_zoomFactor;
}

void te::layout::ContextLayoutItem::setPixmap( te::color::RGBAColor** pixmap )
{
  m_pixmap = pixmap;
}

te::color::RGBAColor** te::layout::ContextLayoutItem::getPixmap()
{
  return m_pixmap;
}

void te::layout::ContextLayoutItem::setShow( bool show )
{
  m_show = show;
}

bool te::layout::ContextLayoutItem::getShow()
{
  return m_show;
}

void te::layout::ContextLayoutItem::setWait( bool wait )
{
  m_wait = wait;
}

bool te::layout::ContextLayoutItem::getWait()
{
  return m_wait;
}

bool te::layout::ContextLayoutItem::isResizeCanvas()
{
  return m_resizeCanvas;
}

void te::layout::ContextLayoutItem::setResizeCanvas( bool clear )
{
  m_resizeCanvas = clear;
}

