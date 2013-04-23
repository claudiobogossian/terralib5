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
  \file terralib/vp/qt/FolderLayerItem.cpp

  \brief The class that represents a folder layer item in a LayerTreeModel.
*/

// TerraLib
#include "../../maptools/FolderLayer.h"
#include "../../common/Translator.h"
#include "../core/Exception.h"
#include "AbstractLayerTreeItemFactory.h"
#include "FolderLayerItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::vp::FolderLayerItem::FolderLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent)
  : AbstractLayerTreeItem(parent)
{
  m_layer = boost::dynamic_pointer_cast<te::map::FolderLayer>(l);

  for(te::map::AbstractLayer::const_iterator it = l->begin(); it != l->end(); ++it)
  {
    /*AbstractLayerTreeItem* litem = */AbstractLayerTreeItemFactory::make(boost::dynamic_pointer_cast<te::map::AbstractLayer>(*it), this);
  }
}

te::vp::FolderLayerItem::~FolderLayerItem()
{
}

int te::vp::FolderLayerItem::columnCount() const
{
  return 1;
}

QVariant te::vp::FolderLayerItem::data(int /*column*/, int role) const
{
  //if(role == Qt::DecorationRole)
  //  return QVariant(QIcon::fromTheme("folder-layer"));

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromUtf8(m_layer->getTitle().c_str()));

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

QMenu* te::vp::FolderLayerItem::getMenu(QWidget* /*parent*/) const
{
  //QMenu* m = new QMenu(parent);

  //QAction* aOpenDataSource = m->addAction(tr("&Open data source"));

  //connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  //return m;
  return 0;
}

bool te::vp::FolderLayerItem::canFetchMore() const
{
  return !children().empty();
}

Qt::ItemFlags te::vp::FolderLayerItem::flags() const
{
  return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

void te::vp::FolderLayerItem::fetchMore()
{
}

bool te::vp::FolderLayerItem::hasChildren() const
{
  return !children().empty();
}

bool te::vp::FolderLayerItem::setData(const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));

    if(!ok)
      return false;

    if(checkState == Qt::Checked)
      m_layer->setVisibility(te::map::VISIBLE);
    else if(checkState == Qt::Unchecked)
      m_layer->setVisibility(te::map::NOT_VISIBLE);
    else if (checkState == Qt::PartiallyChecked)
      m_layer->setVisibility(te::map::PARTIALLY_VISIBLE);

    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::vp::FolderLayerItem::getLayer() const
{
  return m_layer;
}

//te::vp::AbstractLayerTreeItem* te::vp::FolderLayerItem::clone(QObject* parent)
//{
//  return new FolderLayerItem(m_layer, parent);
//}


