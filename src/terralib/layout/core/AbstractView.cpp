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
  \file AbstractView.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractView.h"
#include "enum/Enums.h"

te::layout::AbstractView::AbstractView():
  m_visibleRulers(true),
  m_currentZoom(50),
  m_defaultZoom(50),
  m_oldZoom(50),
  m_maxZoomLimit(900),
  m_minZoomLimit(10),
  m_currentMode(0),
  m_oldMode(0)
{
  m_zooms[42] = "42%";
  m_zooms[50] = "50%";
  m_zooms[70] = "70%";
  m_zooms[100] = "100%";
  m_zooms[150] = "150%";
  m_zooms[200] = "200%";
  m_zooms[300] = "300%";

  if(Enums::getInstance().getEnumModeType())
  {
    m_currentMode = Enums::getInstance().getEnumModeType()->getModeNone();
    m_oldMode = m_currentMode;
  }
}

bool te::layout::AbstractView::isVisibleRulers()
{
  return m_visibleRulers;
}

void te::layout::AbstractView::setVisibleRulers( bool visible )
{
  m_visibleRulers = visible;
}

void te::layout::AbstractView::addZoom( int zoom, std::string text )
{
  m_zooms[zoom] = text;
}

void te::layout::AbstractView::removeZoom( int zoom )
{
  std::map<int, std::string>::iterator it;

  it = m_zooms.find(zoom);

  if(it != m_zooms.end())
  {
    m_zooms.erase(it);
  }
}

void te::layout::AbstractView::clearZoomList()
{
  m_zooms.clear();
}

int te::layout::AbstractView::nextZoom()
{
  int zoom = 0;
  std::map<int, std::string>::iterator it;
  
  it = m_zooms.find(m_currentZoom);

  if(it != m_zooms.end())
  {
    ++it;
    if(it != m_zooms.end())
    {
      if(m_currentZoom != it->first)
      {
        zoom = it->first;
      }
    }
  }
  return zoom;
}

int te::layout::AbstractView::previousZoom()
{
  int zoom = 0;
  std::map<int, std::string>::iterator it;

  it = m_zooms.find(m_currentZoom);

  if(it != m_zooms.end())
  {
    --it;
    if(it != m_zooms.end())
    {
      if(m_currentZoom != it->first)
      {
        zoom = it->first;
      }
    }
  }
  return zoom;
}

void te::layout::AbstractView::setDefaultZoom( int zoom )
{
  m_defaultZoom = zoom;
}

int te::layout::AbstractView::getDefaultZoom()
{
  return m_defaultZoom;
}

int te::layout::AbstractView::getCurrentZoom()
{
  return m_currentZoom;
}

int te::layout::AbstractView::getOldZoom()
{
  return m_oldZoom;
}

bool te::layout::AbstractView::isLimitExceeded( int newZoom )
{
  bool result = false;

  //Zooming In
  if(newZoom > m_maxZoomLimit || newZoom < m_minZoomLimit)
    result = true;

  return result;
}

void te::layout::AbstractView::setMaxZoomLimit( int zoom )
{
  m_maxZoomLimit = zoom;
}

int te::layout::AbstractView::getMaxZoomLimit()
{
  return m_maxZoomLimit;
}

void te::layout::AbstractView::setMinZoomLimit( int zoom )
{
  m_minZoomLimit = zoom;
}

int te::layout::AbstractView::getMinZoomLimit()
{
  return m_minZoomLimit;
}

void te::layout::AbstractView::setCurrentZoom( int zoom )
{
  if(m_currentZoom == zoom)
  {
    return;
  }
  m_oldZoom = m_currentZoom;
  m_currentZoom = zoom;
}

te::layout::EnumType* te::layout::AbstractView::getCurrentMode()
{
  return m_currentMode;
}

te::layout::EnumType* te::layout::AbstractView::getOldMode()
{
  return m_oldMode;
}

void te::layout::AbstractView::setCurrentMode( EnumType* mode )
{
  m_oldMode = m_currentMode;
  m_currentMode = mode;
}







