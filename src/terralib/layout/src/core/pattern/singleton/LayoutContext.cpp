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
  \file LayoutContext.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutContext.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutUtils.h"
#include "LayoutAbstractOutsideFactory.h"
#include "../../../../../maptools/Canvas.h"
#include "../../../../../geometry/Envelope.h"

// STL
#include <stddef.h>  // defines NULL

// Global static pointer used to ensure a single instance of the class.
te::layout::LayoutContext* te::layout::LayoutContext::_instance = NULL;

/** This function is called to create an instance of the class. 
Calling the constructor publicly is not allowed. The constructor 
is private and is only called by this Instance function.
*/
te::layout::LayoutContext* te::layout::LayoutContext::getInstance()
{
  if(!_instance)
  {
    _instance = new LayoutContext;
    return _instance;
  }
  return _instance;
}

te::layout::LayoutMode te::layout::LayoutContext::getMode()
{
  return m_mode;
}

te::layout::LayoutScene* te::layout::LayoutContext::getScene()
{
  return m_scene;
}

void te::layout::LayoutContext::setMode( LayoutMode mode )
{
  m_mode = mode;
}

void te::layout::LayoutContext::setScene( LayoutScene* scene )
{
  m_scene = scene;
}

double te::layout::LayoutContext::getZoomFactor()
{
  return m_zoomFactor;
}

void te::layout::LayoutContext::setZoomFactor( double zoomFactor )
{
  m_zoomFactor = zoomFactor;
}

te::layout::LayoutAbstractItemFactory* te::layout::LayoutContext::getItemFactory()
{
  return m_itemFactory;
}

void te::layout::LayoutContext::setItemFactory( LayoutAbstractItemFactory* factory )
{
  m_itemFactory = factory;
}

te::layout::LayoutAbstractOutsideFactory* te::layout::LayoutContext::getOutsideFactory()
{
  return m_outsideFactory;
}

void te::layout::LayoutContext::setOutsideFactory( LayoutAbstractOutsideFactory* factory )
{
  m_outsideFactory = factory;
}

te::layout::LayoutAbstractTemplateFactory* te::layout::LayoutContext::getTemplateFactory()
{
  return m_templateFactory;
}

void te::layout::LayoutContext::setTemplateFactory( te::layout::LayoutAbstractTemplateFactory* factory )
{
  m_templateFactory = factory;
}

te::layout::LayoutAbstractPaperType te::layout::LayoutContext::getTypePaper()
{
  return m_typePaper;
}

void te::layout::LayoutContext::setTypePaper( LayoutAbstractPaperType type )
{
  m_typePaper = type;
}

te::gm::Envelope* te::layout::LayoutContext::getBoxPaper()
{
  return m_paperBox;
}

void te::layout::LayoutContext::setBoxPaper( te::gm::Envelope* box )
{
  m_paperBox = box;
}

te::map::Canvas* te::layout::LayoutContext::getCanvas()
{
  return m_canvas;
}

void te::layout::LayoutContext::setCanvas( te::map::Canvas* canvas )
{
  m_canvas = canvas;
}

te::layout::LayoutUtils* te::layout::LayoutContext::getUtils()
{
  return m_utils;
}

void te::layout::LayoutContext::setUtils( LayoutUtils* utils )
{
  m_utils = utils;
}

te::layout::LayoutUnitsMetrics te::layout::LayoutContext::getUnitMetric()
{
  return m_unitMetric;
}

void te::layout::LayoutContext::setUnitMetric( LayoutUnitsMetrics unit )
{
  m_unitMetric = unit;
}

double te::layout::LayoutContext::getDpiX()
{
  return m_dpiX;
}

void te::layout::LayoutContext::setDpiX( double dpiX )
{
  m_dpiX = dpiX;
}

double te::layout::LayoutContext::getDpiY()
{
  return m_dpiY;
}

void te::layout::LayoutContext::setDpiY( double dpiY )
{
  m_dpiY = dpiY;
}

