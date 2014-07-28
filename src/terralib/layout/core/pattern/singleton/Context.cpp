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
  \file Context.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Context.h"
#include "../factory/AbstractItemFactory.h"
#include "../../Utils.h"
#include "../factory/AbstractOutsideFactory.h"
#include "../../../../maptools/Canvas.h"
#include "../../../../geometry/Envelope.h"

te::layout::Context::Context() :
  m_mode(TypeNone),
  m_scene(0),
  m_canvas(0),
  m_zoomFactor(1.),
  m_defaultZoomFactor(0.7),
  m_oldZoomFactor(1.),
  m_itemFactory(0),
  m_outsideFactory(0),
  m_utils(0),
  m_dpiX(96),
  m_dpiY(96),
  m_templateFactory(0),
  m_version("TerraPrintQt4_1.0.0"),
  m_paperConfig(0),
  m_buildGraphicsItem(0),
  m_lineIntersectionMouseMode(TypeOffLinesIntersectionMouse)
{

}

te::layout::Context::~Context()
{

}

te::layout::LayoutMode te::layout::Context::getMode()
{
  return m_mode;
}

te::layout::AbstractScene* te::layout::Context::getScene()
{
  return m_scene;
}

void te::layout::Context::setMode( LayoutMode mode )
{
  m_mode = mode;
}

void te::layout::Context::setScene( AbstractScene* scene )
{
  m_scene = scene;
}

double te::layout::Context::getZoomFactor()
{
  return m_zoomFactor;
}

void te::layout::Context::setZoomFactor( double zoomFactor )
{
  m_zoomFactor = zoomFactor;
}

te::layout::AbstractItemFactory* te::layout::Context::getItemFactory()
{
  return m_itemFactory;
}

void te::layout::Context::setItemFactory( AbstractItemFactory* factory )
{
  m_itemFactory = factory;
}

te::layout::AbstractOutsideFactory* te::layout::Context::getOutsideFactory()
{
  return m_outsideFactory;
}

void te::layout::Context::setOutsideFactory( AbstractOutsideFactory* factory )
{
  m_outsideFactory = factory;
}

te::layout::AbstractTemplateFactory* te::layout::Context::getTemplateFactory()
{
  return m_templateFactory;
}

void te::layout::Context::setTemplateFactory( te::layout::AbstractTemplateFactory* factory )
{
  m_templateFactory = factory;
}

te::map::Canvas* te::layout::Context::getCanvas()
{
  return m_canvas;
}

void te::layout::Context::setCanvas( te::map::Canvas* canvas )
{
  m_canvas = canvas;
}

te::layout::Utils* te::layout::Context::getUtils()
{
  return m_utils;
}

void te::layout::Context::setUtils( Utils* utils )
{
  m_utils = utils;
}

te::layout::LayoutUnitsMetrics te::layout::Context::getUnitMetric()
{
  return m_unitMetric;
}

void te::layout::Context::setUnitMetric( LayoutUnitsMetrics unit )
{
  m_unitMetric = unit;
}

double te::layout::Context::getDpiX()
{
  return m_dpiX;
}

void te::layout::Context::setDpiX( double dpiX )
{
  m_dpiX = dpiX;
}

double te::layout::Context::getDpiY()
{
  return m_dpiY;
}

void te::layout::Context::setDpiY( double dpiY )
{
  m_dpiY = dpiY;
}

std::string te::layout::Context::getVersion()
{
  return m_version;
}

te::layout::PaperConfig* te::layout::Context::getPaperConfig() const
{
  return m_paperConfig;
}

void te::layout::Context::setPaperConfig( PaperConfig* config )
{
  m_paperConfig = config;
}

te::layout::BuildGraphicsItem* te::layout::Context::getBuildGraphicsItem()
{
  return m_buildGraphicsItem;
}

void te::layout::Context::setBuildGraphicsItem( BuildGraphicsItem* build )
{
  m_buildGraphicsItem = build;
}

te::layout::LayoutMode te::layout::Context::getLineIntersectionMouseMode()
{
  return m_lineIntersectionMouseMode;
}

void te::layout::Context::setLineIntersectionMouseMode( LayoutMode mode )
{
  m_lineIntersectionMouseMode = mode;
}

double te::layout::Context::getDefaultZoomFactor()
{
  return m_defaultZoomFactor;
}

void te::layout::Context::setDefaultZoomFactor( double zoomFactor )
{
  m_defaultZoomFactor = zoomFactor;
}

double te::layout::Context::getOldZoomFactor()
{
  return m_oldZoomFactor;
}

void te::layout::Context::setOldZoomFactor( double zoomFactor )
{
  m_oldZoomFactor = zoomFactor;
}