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
  \file terralib/qt/widgets/layer/explorer/LayerExplorer.cpp

  \brief A widget designed to explore the layers of an application.
*/

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "LayerExplorer.h"
#include "LayerTreeModel.h"
#include "LayerTreeView.h"

// Qt
//#include <QtCore/QMimeData>
//#include <QtCore/QUrl>
//#include <QtGui/QDragEnterEvent>
//#include <QtGui/QDragLeaveEvent>
//#include <QtGui/QDragMoveEvent>
//#include <QtGui/QDropEvent>
//#include <QtGui/QMessageBox>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>

te::qt::widgets::LayerExplorer::LayerExplorer(QWidget* parent, Qt::WindowFlags flags)
  : QDockWidget(tr("Layer Explorer"), parent, flags),
    m_treeView(0),
    m_treeModel(0)
{
  //setAcceptDrops(true);
  //setAutoFillBackground(true);

  m_treeView = new LayerTreeView(this);

  m_treeModel = new LayerTreeModel(this);

  m_treeModel->setCheckable(true);

  setWidget(m_treeView);

  m_treeView->setModel(m_treeModel);

  m_treeView->setHeaderHidden(true);
  m_treeView->setIndentation(TE_QTWIDGETS_DEFAULT_TREEVIEW_IDENTATION);
}

te::qt::widgets::LayerExplorer::~LayerExplorer()
{
}

te::qt::widgets::LayerTreeView* te::qt::widgets::LayerExplorer::getTreeView() const
{
  return m_treeView;
}

te::qt::widgets::LayerTreeModel* te::qt::widgets::LayerExplorer::getTreeModel() const
{
  return m_treeModel;
}

void te::qt::widgets::LayerExplorer::set(const std::list<te::map::AbstractLayerPtr>& layers)
{
  m_treeModel->set(layers);
}

std::list<te::qt::widgets::AbstractTreeItem*> te::qt::widgets::LayerExplorer::getSelectedItems() const
{
  if(m_treeView == 0)
    return std::list<te::qt::widgets::AbstractTreeItem*>();

  return m_treeView->getSelectedItems();
}

void te::qt::widgets::LayerExplorer::add(const te::map::AbstractLayerPtr& layer)
{
  if(m_treeView == 0)
    return;

  m_treeView->add(layer);
}

void te::qt::widgets::LayerExplorer::remove(AbstractTreeItem* item)
{
  if(m_treeView == 0)
    return;

  m_treeModel->remove(item);
}

//void te::qt::widgets::LayerExplorer::dragEnterEvent(QDragEnterEvent* e)
//{
//  //setBackgroundRole(QPalette::Highlight);
//  setStyleSheet("background-color: yellow");
//  e->acceptProposedAction();
//}
//
//void te::qt::widgets::LayerExplorer::dragMoveEvent(QDragMoveEvent* e)
//{
//  e->acceptProposedAction();
//}
//
//void te::qt::widgets::LayerExplorer::dragLeaveEvent(QDragLeaveEvent* e)
//{
//  setBackgroundRole(QPalette::Window);
//  e->accept();
//}
//
//void te::qt::widgets::LayerExplorer::dropEvent(QDropEvent* e)
//{
//  const QMimeData* mimeData = e->mimeData();
//
//  if(mimeData->hasUrls())
//  {
//    QList<QUrl> urlList = mimeData->urls();
//
//    QString text;
//
//    for(QList<QUrl>::Iterator it = urlList.begin(); it != urlList.end(); ++it)
//    {
//      QString url = it->path();
//
//      QMessageBox::warning(this,
//                         tr("TerraLib Qt Components"),
//                         url);
//    }
//  }
//
//  //setBackgroundRole(QPalette::Window);
//}
