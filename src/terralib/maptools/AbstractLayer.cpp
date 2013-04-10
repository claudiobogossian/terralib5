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
  if(v == te::map::PARTIALLY_VISIBLE)
    return;

  m_visibility = v;

  if(hasChildren())
  {
    int numChildren = getChildrenCount();
    std::vector<AbstractLayer*> childrenVec(numChildren);

    for(int i = 0; i < numChildren; ++i)
    {
      childrenVec[i] = static_cast<AbstractLayer*>(getChild(i).get());
      childrenVec[i]->setVisibility(v);
    }
  }

  // Adjust the visibility of the ascendent layers that are folders
  AbstractLayer* parentLayer = static_cast<AbstractLayer*>(getParent());
  while(parentLayer != 0)
  {
    parentLayer->adjustVisibility();
    parentLayer = static_cast<AbstractLayer*>(parentLayer->getParent());
  }
}

void te::map::AbstractLayer::adjustVisibility()
{
  if(getType() != "FOLDERLAYER")
    return;

  if(hasChildren())
  {
    int numChildren = getChildrenCount();
    std::vector<AbstractLayer*> childrenVec(numChildren);

    for(int i = 0; i < numChildren; ++i)
    {
      childrenVec[i] = static_cast<AbstractLayer*>(getChild(i).get());
      childrenVec[i]->adjustVisibility();
    }

    bool allVisible = true;
    bool allNotVisible = true;

    for(int i = 0; i < numChildren; ++i)
    {
      if(childrenVec[i]->getVisibility() == te::map::NOT_VISIBLE)
        allVisible = false;
      else if(childrenVec[i]->getVisibility() == te::map::VISIBLE)
        allNotVisible = false;
      else if(childrenVec[i]->getVisibility() == te::map::PARTIALLY_VISIBLE)
      {
        allVisible = false;
        allNotVisible = false;
      }
    }

    m_visibility = te::map::PARTIALLY_VISIBLE;
    if(allVisible)
      m_visibility = te::map::VISIBLE;
    else if(allNotVisible)
      m_visibility = te::map::NOT_VISIBLE;
  }
}
