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
  \file terralib/qt/widgets/layer/explorer/LayerTreeView.cpp

  \brief A tree view for the layers of an application.
*/

// TerraLib
#include "AbstractLayerTreeItem.h"
#include "LayerTreeModel.h"
#include "LayerTreeView.h"

// Qt
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QMessageBox>

te::qt::widgets::LayerTreeView::LayerTreeView(QWidget* parent)
  : QTreeView(parent)
{
  setAcceptDrops(true);
  setDragEnabled(true);
  setRootIsDecorated(true);

  viewport()->setAutoFillBackground(true);

  connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(itemActivated(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(itemClicked(const QModelIndex&)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(itemDoubleClicked(const QModelIndex&)));
  connect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(itemEntered(const QModelIndex&)));
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(itemPressed(const QModelIndex&)));
}

te::qt::widgets::LayerTreeView::~LayerTreeView()
{
}

std::list<te::qt::widgets::AbstractLayerTreeItem*> te::qt::widgets::LayerTreeView::getSelectedItems() const
{
  std::list<AbstractLayerTreeItem*> layers;

  QModelIndexList indexes = selectedIndexes();

  QModelIndex idx;

  foreach(idx, indexes)
  {
    AbstractLayerTreeItem* layer = static_cast<AbstractLayerTreeItem*>(idx.internalPointer());

    if(layer != 0)
      layers.push_back(layer);
  }

  return layers;
}

void te::qt::widgets::LayerTreeView::add(const te::map::AbstractLayerPtr& layer)
{
  LayerTreeModel* model = dynamic_cast<LayerTreeModel*>(this->model());

  if(model == 0)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Can not add a layer to an empty model!"));
    return;
  }

  model->add(layer);
}

void te::qt::widgets::LayerTreeView::itemActivated(const QModelIndex & index)
{
  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  emit activated(item);
}

void te::qt::widgets::LayerTreeView::itemClicked(const QModelIndex & index)
{
  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  emit clicked(item);

  LayerTreeModel* model = dynamic_cast<LayerTreeModel*>(this->model());

  if(!model->isCheckable())
    return;

  QVariant value = item->data(0, Qt::CheckStateRole);

  if(value.isNull())
    return;

  emit toggled(item, value.toBool());
}

void te::qt::widgets::LayerTreeView::itemDoubleClicked(const QModelIndex & index)
{
  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  emit doubleClicked(item);
}

void te::qt::widgets::LayerTreeView::itemEntered(const QModelIndex & index)
{
  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  emit entered(item);
}

void te::qt::widgets::LayerTreeView::itemPressed(const QModelIndex & index)
{
  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  emit pressed(item);
}

void te::qt::widgets::LayerTreeView::dragEnterEvent(QDragEnterEvent* e)
{
  //viewport()->setStyleSheet("background-color: lightblue");
  //viewport()->setBackgroundRole(QPalette::Highlight);
  //e->acceptProposedAction();

  QTreeView::dragEnterEvent(e);
}

void te::qt::widgets::LayerTreeView::dragMoveEvent(QDragMoveEvent* e)
{
  e->acceptProposedAction();

  QTreeView::dragMoveEvent(e);
}

void te::qt::widgets::LayerTreeView::dragLeaveEvent(QDragLeaveEvent* e)
{
  //viewport()->setStyleSheet("background-color: white");
  //viewport()->setBackgroundRole(QPalette::Base);
  //e->accept();

  QTreeView::dragLeaveEvent(e);
}

void te::qt::widgets::LayerTreeView::dropEvent(QDropEvent* e)
{
  //viewport()->setBackgroundRole(QPalette::Base);
  ////viewport()->setStyleSheet("background-color: white");

  //const QMimeData* mimeData = e->mimeData();

  //if(mimeData->hasUrls())
  //{
  //  QList<QUrl> urlList = mimeData->urls();

  //  QString text;

  //  for(QList<QUrl>::Iterator it = urlList.begin(); it != urlList.end(); ++it)
  //  {
  //    QString url = it->path();
  //  }
  //}

  QTreeView::dropEvent(e);
}
