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
  \file terralib/common/TreeItem.cpp

  \brief A base class for data organized as a tree.
*/

// TerraLib
#include "TreeItem.h"

// STL
#include <algorithm>
#include <cassert>

const std::size_t te::common::TreeItem::npos(static_cast<std::size_t>(-1));

te::common::TreeItem::TreeItem(TreeItem* parent)
  : m_parent(parent)
{
  if(m_parent)
    m_parent->m_children.push_back(TreeItemPtr(this));
}

te::common::TreeItem::TreeItem(const TreeItemPtr& parent)
{
  if(parent.get())
    m_parent->m_children.push_back(TreeItemPtr(this));
}

te::common::TreeItem::~TreeItem()
{
  iterator it = begin();
  iterator itend = end();

  while(it != itend)
  {
    (*it)->m_parent = 0;
    ++it;
  }
}

bool te::common::TreeItem::hasChildren() const
{
  return !m_children.empty();
}

te::common::TreeItem* te::common::TreeItem::getParent() const
{
  return m_parent;
}

const std::list<te::common::TreeItemPtr>& te::common::TreeItem::getChildren() const
{
  return m_children;
}

const te::common::TreeItemPtr& te::common::TreeItem::getChild(std::size_t i) const
{
  assert(i < m_children.size());

  const_iterator it = begin();

  std::advance(it, i);

  return *it;
}

const te::common::TreeItemPtr& te::common::TreeItem::operator[](std::size_t i) const
{
  return getChild(i);
}

void te::common::TreeItem::add(const TreeItemPtr& childItem)
{
  assert(childItem.get());

  m_children.push_back(childItem);

  childItem->m_parent = this;
}

void te::common::TreeItem::insert(std::size_t i, const TreeItemPtr& childItem)
{
  assert(i <= m_children.size());
  assert(childItem);

  //childItem->disconnect();

  childItem->m_parent = this;

  iterator it = begin();

  std::advance(it, i);

  m_children.insert(it, childItem);
}

te::common::TreeItemPtr te::common::TreeItem::remove(std::size_t i)
{
  assert(i < m_children.size());

  iterator it = begin();

  std::advance(it, i);

  TreeItemPtr childItem(*it);

  //childItem->disconnect();

  m_children.erase(it);

  childItem.get()->m_parent = 0;

  return childItem;
}

std::list<te::common::TreeItemPtr> te::common::TreeItem::remove(std::size_t i, std::size_t count)
{
  assert(i < m_children.size());
  assert(count > 0);
  assert((i + count - 1) < m_children.size());

  iterator firstIt = begin();

  std::advance(firstIt, i);

  iterator lastIt = firstIt;

  std::advance(lastIt, count);

  std::list<TreeItemPtr> removedItems;

  for(iterator it = firstIt; it != lastIt; ++it)
  {
    TreeItemPtr childItem = *it;

    //childItem->disconnect();

    removedItems.push_back(childItem);
  }

  m_children.erase(firstIt, lastIt);

  return removedItems;
}    

te::common::TreeItemPtr te::common::TreeItem::replace(std::size_t i, const TreeItemPtr& childItem)
{
  assert(i < m_children.size());
  assert(childItem);

  TreeItemPtr replacedItem = getChild(i);
  replacedItem->disconnect();

  insert(i, childItem);

  return replacedItem;
}

bool te::common::TreeItem::isSibling(const TreeItem* item) const
{
  return getParent() == item->getParent();
}

std::size_t te::common::TreeItem::getChildrenCount() const
{
  return m_children.size();
}

void te::common::TreeItem::getDescendantsCount(std::size_t& count) const
{
  count += m_children.size();

  for(const_iterator it = begin(); it != end(); ++it)
  {
    TreeItemPtr item = *it;
    item->getDescendantsCount(count);
  }
}

void te::common::TreeItem::disconnect()
{
  if(m_parent == 0)
    return;

  int index = getIndex();

  if(index < 0)
    return;

  iterator it = m_parent->begin();

  std::advance(it, index);

  m_parent->m_children.erase(it);

  m_parent = 0;
}

std::size_t te::common::TreeItem::getIndex() const
{
  if(m_parent == 0)
    return npos;

  const std::list<TreeItemPtr>& children = m_parent->m_children;

  std::size_t k = 0;

  for(const_iterator it = children.begin(); it != children.end(); ++it)
  {
    TreeItemPtr childItem = *it;

    if(childItem.get() == this)
      return k;

    ++k;
  }

  return npos;
}

void te::common::TreeItem::swap(const TreeItemPtr& firstChild, const TreeItemPtr& secondChild)
{
  assert(firstChild->getParent() == this && secondChild->getParent() == this);

  std::size_t fidx = firstChild->getIndex();
  std::size_t sidx = secondChild->getIndex();

  iterator firstIt = begin();

  std::advance(firstIt, fidx);

  iterator secondIt = begin();

  std::advance(secondIt, sidx);

  *firstIt = secondChild;
  *secondIt = firstChild;
}

void te::common::TreeItem::swap(const TreeItemPtr& sibling)
{
  assert(m_parent);

  m_parent->swap(this, sibling);
}

te::common::TreeItem::const_iterator te::common::TreeItem::begin() const
{
  return m_children.begin();
}

te::common::TreeItem::iterator te::common::TreeItem::begin()
{
  return m_children.begin();
}

te::common::TreeItem::const_iterator te::common::TreeItem::end() const
{
  return m_children.end();
}

te::common::TreeItem::iterator te::common::TreeItem::end()
{
  return m_children.end();
}

void te::common::TreeItem::setParent(TreeItem* parent)
{
  if(m_parent == parent)
    return;

  if(m_parent)
    disconnect();

  m_parent = parent;

  if(parent)
    parent->m_children.push_back(this);
}

