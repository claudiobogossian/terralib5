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
#include "../../enum/Enums.h"
#include "../proxy/AbstractProxyProject.h"

te::layout::Context::Context() :
  m_scene(0),
  m_canvas(0),
  m_utils(0),
  m_version("MAPLayoutQt5_1.0.0"),
  m_systematicConfig(0),
  m_proxyProject(0),
  m_itemUtils(0)
{
  
}

te::layout::Context::~Context()
{

}

te::layout::AbstractScene* te::layout::Context::getScene()
{
  return m_scene;
}

void te::layout::Context::setScene( AbstractScene* scene )
{
  m_scene = scene;
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

std::string te::layout::Context::getVersion()
{
  return m_version;
}

te::layout::SystematicScaleConfig* te::layout::Context::getSystematicScaleConfig()
{
  return m_systematicConfig;
}

void te::layout::Context::setSystematicScaleConfig( SystematicScaleConfig* scale )
{
  m_systematicConfig = scale;
}

void te::layout::Context::setProxyProject( AbstractProxyProject* project )
{
  m_proxyProject = project;
}

te::layout::AbstractProxyProject* te::layout::Context::getProxyProject()
{
  return m_proxyProject;
}

void te::layout::Context::setItemUtils( ItemUtils* utils )
{
  m_itemUtils = utils;
}

te::layout::ItemUtils* te::layout::Context::getItemUtils()
{
  return m_itemUtils;
}


