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

bool te::qt::widgets::AbstractTreeItem::removeChildren(int i, int count)
{
  const QList<QObject*>& childrenList = children();
  int numChildren = childrenList.count();

  if (numChildren == 0)
    return false;

  assert(i <= numChildren);
  assert((i+count) <= numChildren);

  for (int j = i; j < (i + count); ++j)
  {
    QObject* item = childrenList.value(j);
    item->setParent(0);
    //delete item;
  }

  const QList<QObject*>& childrenList1 = children();
  numChildren = childrenList1.count();

  m_refLayer->removeChildren(i, count);

  return true;
}

void te::qt::widgets::AbstractTreeItem::addChild(int i, AbstractTreeItem* treeItem)
{
  int numChildren = children().count();

  assert(i <= numChildren);

  // Remove all the items from the ith position until the end of the list of children
  QList<QObject*> savedItemsList;

  for(int pos = i; pos < numChildren; ++pos)
    savedItemsList.append(getChild(pos));

  for(int j = 0; j < savedItemsList.count(); ++j)
  {
    QObject* child = savedItemsList.at(j);
    child->setParent(0);
  }

  // Insert the given item
  treeItem->setParent(this);  

  // Reinsert the saved items as children of this tree item
  for(int j = 0; j < savedItemsList.count(); ++j)
  {
    QObject* child = savedItemsList.at(j);
    child->setParent(this);
  }

  m_refLayer->addChild(i, treeItem->getRefLayer());
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

