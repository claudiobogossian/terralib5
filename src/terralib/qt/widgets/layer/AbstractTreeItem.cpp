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
  \file AbstractTreeItem.cpp

  \brief A widget designed to show the information of a tree item.
 */

// TerraLib
#include "AbstractTreeItem.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/LegendItem.h"

// STL
#include <cassert>

te::qt::widgets::AbstractTreeItem::AbstractTreeItem(QObject* parent)
  : QObject(parent), m_refLayer(0)
{
}

te::qt::widgets::AbstractTreeItem::~AbstractTreeItem()
{}

te::map::AbstractLayer* te::qt::widgets::AbstractTreeItem::getRefLayer() const
{
  return m_refLayer;
}

const QList<QObject*>& te::qt::widgets::AbstractTreeItem::getChildren() const
{
  return children();
}

te::qt::widgets::AbstractTreeItem* te::qt::widgets::AbstractTreeItem::getChild(int pos)
{
  te::qt::widgets::AbstractTreeItem* item = 0;

  if(children().count() != 0)
    item = static_cast<te::qt::widgets::AbstractTreeItem*>(children().value(pos));

  return item;
}

int te::qt::widgets::AbstractTreeItem::getIndex() const
{
  te::qt::widgets::AbstractTreeItem* parentItem = static_cast<te::qt::widgets::AbstractTreeItem*>(parent());
  int index = parentItem->children().indexOf((AbstractTreeItem*)this, 0);
  return index;
}

te::map::AbstractLayer* te::qt::widgets::AbstractTreeItem::removeChild(int row)
{
  const QList<QObject*>& childrenList = children();
  int numChildren = childrenList.count();

  if (numChildren == 0 || row < 0 || row >= numChildren)
    return false;

  te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(childrenList.at(row));
  numChildren = m_refLayer->getChildrenCount();

  te::map::AbstractLayer* itemRefLayer = static_cast<te::map::AbstractLayer*>(m_refLayer->takeChild(row));
  item->setParent(0);

  numChildren = m_refLayer->getChildrenCount();

  delete item;

  numChildren = m_refLayer->getChildrenCount();


  return itemRefLayer;
}

bool te::qt::widgets::AbstractTreeItem::removeChildren(int i, int count)
{
  const QList<QObject*> childrenList = children();

  int numChildren = childrenList.count();

  if (numChildren == 0)
    return false;

  assert(i <= numChildren);
  assert((i+count) <= numChildren);

  for (int j = i; j < (i + count); ++j)
  {
    QObject* item = childrenList.at(j);
    item->setParent(0);
  }

  m_refLayer->takeChildren(i, count);

  return true;
}

void te::qt::widgets::AbstractTreeItem::insertChild(int pos, AbstractTreeItem* item)
{
  int numChildren = children().count();

  assert(pos <= numChildren);

  // Get the children of the tree item and disconnect them from the tree item
  QList<QObject*> savedItemsList = children();

  for(int i = 0; i < numChildren; ++i)
    savedItemsList.at(i)->setParent(0);

  // Insert the given item into the saved tree
  savedItemsList.insert(pos, item);

  // Reinsert the saved items into the tree
  for(int i = 0; i < savedItemsList.count(); ++i)
    savedItemsList.at(i)->setParent(this);

  m_refLayer->insertChild(pos, item->getRefLayer());
}

void te::qt::widgets::AbstractTreeItem::append(AbstractTreeItem* treeItem)
{
  treeItem->setParent(this);
}

int te::qt::widgets::AbstractTreeItem::getChildRow(AbstractTreeItem* treeItem)
{
  return children().indexOf(treeItem);
}

bool te::qt::widgets::AbstractTreeItem::isLayerItem() const
{
  return true;
}

bool te::qt::widgets::AbstractTreeItem::isLegendItem() const
{
  return false;
}
