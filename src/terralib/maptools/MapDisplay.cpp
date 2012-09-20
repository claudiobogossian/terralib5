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
  \file MapDisplay.cpp

  \brief The map display controls how a set of layers are displayed.
*/

// TerraLib
#include "../geometry/Envelope.h"
#include "MapDisplay.h"

te::map::MapDisplay::MapDisplay()
  : AbstractMapDisplay(),
    m_srid(-1),
    m_extent(0),
    m_hAlign(te::map::Center),
    m_vAlign(te::map::Center)
{}

te::map::MapDisplay::~MapDisplay()
{
  delete m_extent;
}

void te::map::MapDisplay::setLayerList(const std::list<te::map::AbstractLayer*>& order)
{
  if(&m_layerList != &order)
    m_layerList = order;
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

const te::gm::Envelope* te::map::MapDisplay::getExtent() const
{
  return m_extent;
}

void te::map::MapDisplay::setExtent(const te::gm::Envelope& e)
{
  delete m_extent;
  m_extent = 0;

  if(e.isValid())
    m_extent = new te::gm::Envelope(e);
}

int te::map::MapDisplay::getSRID() const
{
  return m_srid;
}

void te::map::MapDisplay::setSRID(const int& srid)
{
  if(m_srid == srid)
    return;

  if(m_extent)
  {
    m_extent->transform(m_srid, srid);
    setExtent(*m_extent);
  }
  m_srid = srid;
}
