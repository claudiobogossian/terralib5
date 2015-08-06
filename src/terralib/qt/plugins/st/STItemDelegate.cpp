/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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

#include "STItemDelegate.h"

// TerraLib
#include "../../widgets/layer/explorer/LayerItem.h"

te::qt::plugins::st::STItemDelegate::STItemDelegate(QStyledItemDelegate* decorated, QObject* parent) :
  te::common::Decorator<QStyledItemDelegate>(decorated, false)
{
  setParent(parent);

  m_obsIcon = QIcon::fromTheme("observation-layer");
  m_tmsIcon = QIcon::fromTheme("timeseries-layer");
  m_trjIcon = QIcon::fromTheme("trajectory-layer");
}

te::qt::plugins::st::STItemDelegate::~STItemDelegate()
{

}

void te::qt::plugins::st::STItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(index.internalPointer());

  if(item->getType() == "LAYER")
  {
    te::qt::widgets::LayerItem* li = (te::qt::widgets::LayerItem*)item;

    if(li->getLayer()->isValid())
    {
      QIcon ic = getIcon(li->getLayer().get());

      if(!ic.isNull())
      {
        QStyleOptionViewItem opt = option;
        opt.decorationSize = QSize(16, 16);
        opt.features |= QStyleOptionViewItem::HasDecoration;

        opt.icon = ic;

        QStyledItemDelegate::paint(painter, opt, index);

        return;
      }
    }
  }

  if(m_decorated != 0)
    m_decorated->paint(painter, option, index);
  else
    QStyledItemDelegate::paint(painter, option, index);
}

QIcon te::qt::plugins::st::STItemDelegate::getIcon(const te::map::AbstractLayer* l) const
{
  return (l->getType() == "TRAJECTORYDATASETLAYER") ? m_trjIcon :
  (l->getType() == "OBSERVATIONDATASETLAYER") ? m_obsIcon :
  (l->getType() == "TIMESERIESDATASETLAYER") ? m_tmsIcon :
  QIcon();
}
