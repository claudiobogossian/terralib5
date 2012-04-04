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
  \file AbstractLayerItem.cpp

  \brief A widget designed to show the information of a given Layer in a tree oriented way.
 */

// TerraLib
#include "AbstractLayerItem.h"
#include "../../../maptools/AbstractLayer.h"

// STL
#include <cassert>

te::qt::widgets::AbstractLayerItem::AbstractLayerItem(te::map::AbstractLayer* refLayer, QObject* parent)
  : QObject(parent), m_refLayer(refLayer)
{
  if (parent)
  {
    te::qt::widgets::AbstractLayerItem* layerParent;
    layerParent = static_cast<te::qt::widgets::AbstractLayerItem*>(parent);

    layerParent->append(this);
  }
}

te::qt::widgets::AbstractLayerItem::~AbstractLayerItem()
{}

te::map::AbstractLayer* te::qt::widgets::AbstractLayerItem::getRefLayer() const
{
  return m_refLayer;
}

const QList<te::qt::widgets::AbstractLayerItem*>& te::qt::widgets::AbstractLayerItem::getChildren() const
{
  return m_children;
}

te::qt::widgets::AbstractLayerItem* te::qt::widgets::AbstractLayerItem::getChild(int pos)
{
  return m_children.value(pos);
}

int te::qt::widgets::AbstractLayerItem::getIndex() const
{
  te::qt::widgets::AbstractLayerItem* parentItem = static_cast<te::qt::widgets::AbstractLayerItem*>(parent());
  int index = parentItem->getChildren().indexOf((AbstractLayerItem*)this, 0);
  return index;
}

bool te::qt::widgets::AbstractLayerItem::removeChildren(int i, int count)
{
  int numChildren = m_children.count();

  if (numChildren == 0)
    return false;

  assert(i <= numChildren);
  assert((i+count) <= numChildren);

  for (int j = i; j < (i + count); ++j)
    m_children.removeAt(j);

  m_refLayer->removeChildren(i, count);

  return true;
}

void te::qt::widgets::AbstractLayerItem::addChild(int i, AbstractLayerItem* layerItem)
{
  assert(i <= m_children.count());

  m_children.insert(i, layerItem);

  m_refLayer->addChild(i, layerItem->getRefLayer());
}

void te::qt::widgets::AbstractLayerItem::append(AbstractLayerItem* layerItem)
{
  m_children.append(layerItem);
}

void te::qt::widgets::AbstractLayerItem::moveChild(int fromPos, int toPos)
{
  assert(fromPos <= m_children.count() && toPos <= m_children.count());

  m_children.move(fromPos, toPos);
}

int te::qt::widgets::AbstractLayerItem::getChildRow(AbstractLayerItem* layerItem)
{
  return m_children.indexOf(layerItem);
}
