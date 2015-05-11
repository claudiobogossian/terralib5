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
  \file terralib/maptools/MapDisplay.cpp

  \brief The map display controls how a set of layers are displayed.
*/

// TerraLib
#include "../srs/Config.h"
#include "MapDisplay.h"

te::map::MapDisplay::MapDisplay()
  : AbstractMapDisplay(),
    m_srid(TE_UNKNOWN_SRS),
    m_hAlign(te::map::Center),
    m_vAlign(te::map::Center)
{
}

te::map::MapDisplay::~MapDisplay()
{
}

void te::map::MapDisplay::setLayerList(const std::list<te::map::AbstractLayerPtr>& layers)
{
  m_layerList = layers;
}

te::map::AlignType te::map::MapDisplay::getHAlign() const
{
  return m_hAlign;
}

te::map::AlignType te::map::MapDisplay::getVAlign() const
{
  return m_vAlign;
}

void te::map::MapDisplay::setAlign(te::map::AlignType h, te::map::AlignType v)
{
  m_hAlign = h;
  m_vAlign = v;
}

const te::gm::Envelope& te::map::MapDisplay::getExtent() const
{
  return m_extent;
}

void te::map::MapDisplay::setExtent(te::gm::Envelope& e, bool /*doRefresh*/)
{
  m_extent = e;
}

int te::map::MapDisplay::getSRID() const
{
  return m_srid;
}

void te::map::MapDisplay::setSRID(const int& srid, bool doRefresh)
{
  if(m_srid == srid)
    return;

  if(m_srid != TE_UNKNOWN_SRS && srid != TE_UNKNOWN_SRS && m_extent.isValid())
    m_extent.transform(m_srid, srid);

  m_srid = srid;

  if(m_extent.isValid())
    setExtent(m_extent, doRefresh);
}
