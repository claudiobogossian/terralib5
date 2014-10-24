/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/explorer/QueryLayerItem.cpp

  \brief The class that represents a query layer item in a LayerTreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../maptools/QueryLayer.h"
#include "../../../../se/Style.h"
#include "../../Exception.h"
#include "LegendItem.h"
#include "QueryLayerItem.h"

// STL
#include <cassert>

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::QueryLayerItem::QueryLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent)
  : AbstractTreeItem(parent)
{
  m_layer = boost::dynamic_pointer_cast<te::map::QueryLayer>(l);
}

te::qt::widgets::QueryLayerItem::~QueryLayerItem()
{
}

int te::qt::widgets::QueryLayerItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::QueryLayerItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("query-layer"));

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromUtf8(m_layer->getTitle().c_str()));

  if(role == Qt::CheckStateRole)
    return QVariant(m_layer->getVisibility() == te::map::VISIBLE ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::QueryLayerItem::getMenu(QWidget* /*parent*/) const
{
  //QMenu* m = new QMenu(parent);

  //QAction* aOpenDataSource = m->addAction(tr("&Open data source"));

  //connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  //return m;
  return 0;
}

bool te::qt::widgets::QueryLayerItem::canFetchMore() const
{
  return (m_layer->getStyle() != 0) && (m_layer->getStyle()->getRules().empty() == false) && children().isEmpty();
}

Qt::ItemFlags te::qt::widgets::QueryLayerItem::flags() const
{
  return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;;
}

void te::qt::widgets::QueryLayerItem::fetchMore()
{
  if(!children().isEmpty())
    return;

  if(m_layer->getStyle())
  {
    const std::vector<te::se::Rule*>& rules = m_layer->getStyle()->getRules();

    for(std::size_t i = 0; i != rules.size(); ++i)
      new LegendItem(rules[i], this);
  }
}

bool te::qt::widgets::QueryLayerItem::hasChildren() const
{
  return (m_layer->getStyle() != 0) && (m_layer->getStyle()->getRules().empty() == false);
}

bool te::qt::widgets::QueryLayerItem::setData(int column, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());
    if(checkState == Qt::Checked)
      m_layer->setVisibility(te::map::VISIBLE);
    else if(checkState == Qt::Unchecked)
      m_layer->setVisibility(te::map::NOT_VISIBLE);

    m_layer->updateVisibilityOfAncestors();

    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::widgets::QueryLayerItem::getLayer() const
{
  return m_layer;
}

const std::string te::qt::widgets::QueryLayerItem::getItemType() const
{
  return "QUERY_LAYER_ITEM";
}
