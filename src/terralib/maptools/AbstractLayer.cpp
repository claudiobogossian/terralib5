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
  \file terralib/maptools/AbstractLayer.cpp

  \brief This is the base class for Layers.
 */

// TerraLib
#include "../srs/Config.h"
#include "AbstractLayer.h"

te::map::AbstractLayer::AbstractLayer(AbstractLayer* parent)
  : te::common::TreeItem(parent),
    m_srid(TE_UNKNOWN_SRS),
    m_visibility(NOT_VISIBLE)
{
}

te::map::AbstractLayer::AbstractLayer(const std::string& id, AbstractLayer* parent)
  : te::common::TreeItem(parent),
    m_id(id),
    m_srid(TE_UNKNOWN_SRS),
    m_visibility(NOT_VISIBLE)
{
}

te::map::AbstractLayer::AbstractLayer(const std::string& id,
                                      const std::string& title,
                                      AbstractLayer* parent)
  : te::common::TreeItem(parent),
    m_id(id),
    m_title(title),
    m_srid(TE_UNKNOWN_SRS),
    m_visibility(NOT_VISIBLE)
{
}

te::map::AbstractLayer::~AbstractLayer()
{
}

const std::string& te::map::AbstractLayer::getId() const
{
  return m_id;
}

void te::map::AbstractLayer::setId(const std::string& id)
{
  m_id = id;
}

const std::string& te::map::AbstractLayer::getTitle() const
{
  return m_title;
}

void te::map::AbstractLayer::setTitle(const std::string& title)
{
  m_title = title;
}

te::map::Visibility te::map::AbstractLayer::getVisibility() const
{
  return m_visibility;
}

const te::gm::Envelope& te::map::AbstractLayer::getExtent() const
{
  return m_mbr;
}

void te::map::AbstractLayer::setExtent(const te::gm::Envelope& mbr)
{
  m_mbr = mbr;
}

int te::map::AbstractLayer::getSRID() const
{
  return m_srid;
}

void te::map::AbstractLayer::setSRID(int srid)
{
  m_srid = srid;
}

void te::map::AbstractLayer::setVisibility(Visibility v)
{
  m_visibility = v;

  setDescendantsVisibility(v);

  adjustAscendantsVisibility();
}

void te::map::AbstractLayer::setDescendantsVisibility(Visibility v)
{
  te::common::TreeItem::iterator it = begin();
  te::common::TreeItem::iterator it_end = end();

  while(it != it_end)
  {
    AbstractLayer* layer = dynamic_cast<AbstractLayer*>(it->get());

    if(layer)
    {
      layer->m_visibility = v;

      if(v != PARTIALLY_VISIBLE)
        layer->setDescendantsVisibility(v);
    }

    ++it;
  }
}

void te::map::AbstractLayer::adjustAscendantsVisibility()
{
  if(m_parent == 0)
    return;

  AbstractLayer* layer = 0;

  bool hasNotVisible = false;
  bool hasVisible = false;
  bool hasPartiallyVisible = false;

  te::common::TreeItem::iterator it = m_parent->begin();
  te::common::TreeItem::iterator it_end = m_parent->end();

  while(it != it_end)
  {
    layer = dynamic_cast<AbstractLayer*>(it->get());

    if(layer)
    {
      if(layer->getVisibility() == NOT_VISIBLE)
        hasNotVisible = true;
      else if(layer->getVisibility() == VISIBLE)
        hasVisible = true;
      else
        hasPartiallyVisible = true;
    }

    ++it;
  }

  Visibility parentVisibility = PARTIALLY_VISIBLE;

  if((hasNotVisible == true) && (hasVisible == false) && (hasPartiallyVisible == false))  // just not visible
    parentVisibility = NOT_VISIBLE;
  else if(hasNotVisible == false && hasVisible == true && hasPartiallyVisible == false)   // just visible
    parentVisibility = VISIBLE;

  layer = dynamic_cast<AbstractLayer*>(m_parent);

  layer->m_visibility = parentVisibility;

  layer->adjustAscendantsVisibility();
}

