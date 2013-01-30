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
  \file AbstractLayer.cpp

  \brief This is the base class for Layers.
 */

// TerraLib
#include "../geometry/Envelope.h"
#include "../se/Style.h"
#include "../xlink/SimpleLink.h"
#include "AbstractLayer.h"
#include "Grouping.h"
#include "LegendItem.h"

te::map::AbstractLayer::AbstractLayer(const std::string& id,
                                      const std::string& title,
                                      AbstractLayer* parent)
  : te::common::TreeItem(parent),
    m_id(id),
    m_title(title),
    m_visibility(NOT_VISIBLE),
    m_icon(0)
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

void te::map::AbstractLayer::setVisibility(Visibility v)
{
  m_visibility = v;
  setDescendantsVisibility(v);
  setAscendantsVisibility();
}

te::xl::SimpleLink* te::map::AbstractLayer::getIcon() const
{
  return m_icon.get();
}

void te::map::AbstractLayer::setIcon(te::xl::SimpleLink* icon)
{
  m_icon.reset(icon);
}

bool te::map::AbstractLayer::isSibling(te::map::AbstractLayer* layer) const
{
  if(getParent() == layer->getParent())
    return true;

  return false;
}

te::map::Grouping* te::map::AbstractLayer::getGrouping() const
{
  return 0;
}

void te::map::AbstractLayer::setGrouping(te::map::Grouping* grouping)
{
}

bool te::map::AbstractLayer::hasLegend()
{
  return false;
}

std::vector<te::map::LegendItem*> te::map::AbstractLayer::getLegend() const
{
  std::vector<te::map::LegendItem*> legend;
  return legend;
}

void te::map::AbstractLayer::removeLegend()
{
}

void te::map::AbstractLayer::insertLegend(const std::vector<LegendItem*>& /*legend*/)
{
}

void te::map::AbstractLayer::setDescendantsVisibility(Visibility v)
{
  te::common::TreeItem::iterator it = begin();
  te::common::TreeItem::iterator it_end = end();

  while(it != it_end)
  {
    AbstractLayer* layer = static_cast<AbstractLayer*>(it->get());

    layer->m_visibility = v;

    if(v != PARTIALLY_VISIBLE)
      layer->setDescendantsVisibility(v);

    ++it;
  }
}

void te::map::AbstractLayer::setAscendantsVisibility()
{
  if(m_parent == 0)
    return;

  te::common::TreeItem::iterator it = begin();
  te::common::TreeItem::iterator it_end = end();

  te::common::TreeItem* parent = m_parent;

  it = parent->begin();
  it_end = parent->end();

  bool hasNotVisible = false;
  bool hasVisible = false;
  bool hasPartiallyVisible = false;

  while(it != it_end)
  {
    AbstractLayer* layer = static_cast<AbstractLayer*>(it->get());

    if(layer->getVisibility() == NOT_VISIBLE)
      hasNotVisible = true;
    else if(layer->getVisibility() == VISIBLE)
      hasVisible = true;
    else
      hasPartiallyVisible = true;

    ++it;
  }

  Visibility parentVisibility;

  if(hasNotVisible == true && hasVisible == false && hasPartiallyVisible == false)
    parentVisibility = NOT_VISIBLE;
  else if(hasNotVisible == false && hasVisible == true && hasPartiallyVisible == false)
    parentVisibility = VISIBLE;
  else
    parentVisibility = PARTIALLY_VISIBLE;

  AbstractLayer* parentLayer = static_cast<AbstractLayer*>(parent);
  parentLayer->m_visibility = parentVisibility;

  parentLayer->setAscendantsVisibility();
}
