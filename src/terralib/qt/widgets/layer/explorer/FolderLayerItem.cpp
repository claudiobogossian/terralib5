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

/*!
  \file terralib/qt/widgets/layer/explorer/FolderLayerItem.cpp

  \brief The class that represents a folder layer item in a LayerTreeModel.
*/

// TerraLib
#include "../../../../maptools/FolderLayer.h"
#include "../../../../common/Translator.h"
#include "../../Exception.h"
#include "AbstractTreeItemFactory.h"
#include "FolderLayerItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::FolderLayerItem::FolderLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent)
  : AbstractTreeItem(parent)
{
  m_layer = boost::dynamic_pointer_cast<te::map::FolderLayer>(l);
}

te::qt::widgets::FolderLayerItem::~FolderLayerItem()
{
}

int te::qt::widgets::FolderLayerItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::FolderLayerItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("folder"));

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromStdString(m_layer->getTitle()));

  if(role == Qt::CheckStateRole)
  {
    if(m_layer->getVisibility() == te::map::VISIBLE)
      return QVariant(Qt::Checked);
    else if(m_layer->getVisibility() == te::map::NOT_VISIBLE)
      return QVariant(Qt::Unchecked);
    else
      return QVariant(Qt::PartiallyChecked);
  }

  return QVariant();
}

QMenu* te::qt::widgets::FolderLayerItem::getMenu(QWidget* /*parent*/) const
{
  //QMenu* m = new QMenu(parent);

  //QAction* aOpenDataSource = m->addAction(tr("&Open data source"));

  //connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  //return m;
  return 0;
}

bool te::qt::widgets::FolderLayerItem::canFetchMore() const
{
  return m_layer->hasChildren() && children().empty();
}

Qt::ItemFlags te::qt::widgets::FolderLayerItem::flags() const
{
  return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

void te::qt::widgets::FolderLayerItem::fetchMore()
{
  if(!children().empty())
    return;

  for(te::map::AbstractLayer::const_iterator it = m_layer->begin(); it != m_layer->end(); ++it)
  {
    AbstractTreeItemFactory::make(boost::dynamic_pointer_cast<te::map::AbstractLayer>(*it), this);
  }
}

bool te::qt::widgets::FolderLayerItem::hasChildren() const
{
  return m_layer->hasChildren();
}

bool te::qt::widgets::FolderLayerItem::setData(int column, const QVariant& value, int role)
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

te::map::AbstractLayerPtr te::qt::widgets::FolderLayerItem::getLayer() const
{
  return m_layer;
}

const std::string te::qt::widgets::FolderLayerItem::getItemType() const
{
  return "FOLDER_LAYER_ITEM";
}
