/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file LayerItem.cpp

  \brief A widget designed to show the information of a Layer in a tree oriented way.
 */

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/Layer.h"
#include "../../../maptools/LegendItem.h"
#include "LayerItem.h"
#include "LegendItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>


te::qt::widgets::LayerItem::LayerItem(te::map::AbstractLayer* refLayer, QObject* parent)
  : AbstractTreeItem(parent)
{
  m_refLayer = refLayer;

  te::map::AbstractLayer::const_iterator it = m_refLayer->begin();
  te::map::AbstractLayer::const_iterator it_end = m_refLayer->end();

  while(it != it_end)
  {
    te::map::AbstractLayer* childReflayer = static_cast<te::map::AbstractLayer*>(*it);

    new LayerItem(childReflayer, this);
    ++it;
  }

  if(refLayer->hasLegend() == true)
  {
    std::vector<te::map::LegendItem*> legend = refLayer->getLegend();
    for(std::size_t i = 0; i < legend.size(); ++i)
      new te::qt::widgets::LegendItem(legend[i], this);
  }
}

te::qt::widgets::LayerItem::~LayerItem()
{
}

QVariant te::qt::widgets::LayerItem::data(int role) const
{
 if (m_refLayer == 0)
   return QVariant();

  if(role == Qt::DisplayRole)
  {
    if(m_refLayer->getTitle().empty())
      return QVariant("");

    return QVariant(m_refLayer->getTitle().c_str());
  }
  else if(role == Qt::CheckStateRole)
  {
    if(m_refLayer->getVisibility() == te::map::VISIBLE)
      return QVariant(Qt::Checked);
    else if(m_refLayer->getVisibility() == te::map::NOT_VISIBLE)
      return QVariant(Qt::Unchecked);
    else
      return QVariant(Qt::PartiallyChecked);
  }

  return QVariant();
}

bool te::qt::widgets::LayerItem::isLayerItem() const
{
  return true;
}

QMenu* te::qt::widgets::LayerItem::getMenu(QWidget* parent) const
{
  return new QMenu(parent);
}

void te::qt::widgets::LayerItem::removeLegend()
{
  const QList<QObject*> childrenList = children();
  int numChildren = childrenList.count();

  if(numChildren == 0)
    return;

  for(int i = 0; i < numChildren; ++i)
  {
    QObject* item = childrenList.at(i);
    item->setParent(0);
    delete item;
  }
    
  m_refLayer->removeLegend();
}

void te::qt::widgets::LayerItem::insertLegend(const std::vector<te::map::LegendItem*>& legend)
{
  removeLegend();

  for(std::size_t i = 0; i < legend.size(); ++i)
    new te::qt::widgets::LegendItem(legend[i], this);

  m_refLayer->insertLegend(legend);
}
