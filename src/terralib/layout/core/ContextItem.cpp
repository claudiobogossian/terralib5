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
  \file ContextItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ContextItem.h"

te::layout::ContextItem::ContextItem() :
  m_zoom(100),
  m_pixmap(0),
  m_resizeCanvas(true),
  m_show(false),
  m_utils(0),
  m_canvas(0),
  m_dpiX(96),
  m_dpiY(96),
  m_changePos(false)
{

}

te::layout::ContextItem::ContextItem( ParamsCreate params, int zoom) :
  m_params(params),
  m_zoom(zoom),
  m_pixmap(0),
  m_resizeCanvas(true),
  m_show(false),
  m_utils(0),
  m_canvas(0),
  m_dpiX(96),
  m_dpiY(96),
  m_changePos(false)
{

}

te::layout::ContextItem::~ContextItem()
{

}

void te::layout::ContextItem::setParams( ParamsCreate params )
{
  m_params = params;
}

te::layout::ParamsCreate te::layout::ContextItem::getParams()
{
  return m_params;
}

void te::layout::ContextItem::setZoom( int zoom )
{
  m_zoom = zoom;
}

int te::layout::ContextItem::getZoom()
{
  return m_zoom;
}

void te::layout::ContextItem::setPixmap( te::color::RGBAColor** pixmap )
{
  m_pixmap = pixmap;
}

te::color::RGBAColor** te::layout::ContextItem::getPixmap()
{
  return m_pixmap;
}

bool te::layout::ContextItem::isResizeCanvas()
{
  return m_resizeCanvas;
}

void te::layout::ContextItem::setResizeCanvas( bool clear )
{
  m_resizeCanvas = clear;
}

te::map::Canvas* te::layout::ContextItem::getCanvas()
{
  return m_canvas;
}

void te::layout::ContextItem::setCanvas( te::map::Canvas* canvas )
{
  m_canvas = canvas;
}

te::layout::Utils* te::layout::ContextItem::getUtils()
{
  return m_utils;
}

void te::layout::ContextItem::setUtils( Utils* utils )
{
  m_utils = utils;
}

double te::layout::ContextItem::getDpiX()
{
  return m_dpiX;
}

void te::layout::ContextItem::setDpiX( double dpi )
{
  m_dpiX = dpi;
}

double te::layout::ContextItem::getDpiY()
{
  return m_dpiY;
}

void te::layout::ContextItem::setDpiY( double dpi )
{
  m_dpiY = dpi;
}

void te::layout::ContextItem::setShow( bool show )
{
  m_show = show;
}

bool te::layout::ContextItem::isShow()
{
  return m_show;
}

void te::layout::ContextItem::setPos( te::gm::Coord2D pos )
{
  m_pos = pos;
}

te::gm::Coord2D te::layout::ContextItem::getPos()
{
  return m_pos;
}

void te::layout::ContextItem::setChangePos( bool change )
{
  m_changePos = change;
}

bool te::layout::ContextItem::isChangePos()
{
  return m_changePos;
}
