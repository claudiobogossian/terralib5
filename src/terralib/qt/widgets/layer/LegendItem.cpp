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
  \file LegendItem.cpp

  \brief A widget designed to show the information of a Legend in a tree oriented way.
 */

// TerraLib
#include "../../../maptools/Layer.h"
#include "../../../maptools/Grouping.h"
#include "../../../maptools/LegendItem.h"
#include "LayerItem.h"
#include "LegendItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::LegendItem::LegendItem(te::map::LegendItem* refLegendItem, QObject* parent)
  : AbstractTreeItem(parent), m_refLegendItem(refLegendItem)
{
  m_refLayer = static_cast<AbstractTreeItem*>(parent)->getRefLayer();

  te::map::Grouping* g = m_refLayer->getGrouping();
  if((g->getType() == te::map::STD_DEVIATION && refLegendItem->getUpperLimit().empty()) == false)
  {
    QPixmap pixmap(16, 12);

    te::color::RGBAColor color = refLegendItem->getColor();
    QColor qColor(color.getRgba());
    pixmap.fill(qColor);
    m_icon.addPixmap(pixmap);
  }
}

te::qt::widgets::LegendItem::~LegendItem()
{
}

bool te::qt::widgets::LegendItem::isLayerItem() const
{
  return false;
}

bool te::qt::widgets::LegendItem::isLegendItem() const
{
  return true;
}

QVariant te::qt::widgets::LegendItem::data(int role) const
{
  if(role == Qt::DisplayRole)
  {
    std::string title = m_refLegendItem->getTitle();

    if(title.empty())
      return QVariant("");

    return QVariant(title.c_str());
  }
  else if(role == Qt::DecorationRole)
  {
    if(m_icon.isNull())
      return QVariant();
    else
      return m_icon;
  }

  return QVariant();
}

QMenu* te::qt::widgets::LegendItem::getMenu(QWidget* parent) const
{
  return 0;
}

const QIcon& te::qt::widgets::LegendItem::getIcon() const
{
  return m_icon;
}

void te::qt::widgets::LegendItem::setIcon(const QIcon& icon)
{
  m_icon = icon;
}
