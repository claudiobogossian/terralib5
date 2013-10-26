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
#include "AbstractTreeItem.h"
#include "LayerExplorer.h"
#include "LayerTreeModel.h"

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

  // Signals and slots
  connect(m_treeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), m_treeView, SLOT(onSelectedLayersChanged(const QItemSelection&, const QItemSelection&)));
  connect(m_treeView, SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)), SLOT(onSelectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)));
  connect(m_treeView, SIGNAL(visibilityChanged(const te::map::AbstractLayerPtr&)), SLOT(onVisibilityChanged(const te::map::AbstractLayerPtr&)));
  connect(m_treeView, SIGNAL(doubleClicked(te::qt::widgets::AbstractTreeItem*)), SLOT(onItemDoubleClicked(te::qt::widgets::AbstractTreeItem*)));
  connect(m_treeModel, SIGNAL(visibilityChanged(const te::map::AbstractLayerPtr&)), SLOT(onVisibilityChanged(const te::map::AbstractLayerPtr&)));
  connect(m_treeModel, SIGNAL(layerOrderChanged()), SLOT(onLayerOrderChanged()));
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

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerExplorer::getTopLayers() const
{
  std::list<te::map::AbstractLayerPtr> topLayers;

  std::vector<te::map::AbstractLayerPtr> modelTopLayers = m_treeModel->getTopLayers();

  for(std::size_t i = 0; i < modelTopLayers.size(); ++i)
    topLayers.push_back(modelTopLayers[i]);

  return topLayers;
}

std::list<te::qt::widgets::AbstractTreeItem*> te::qt::widgets::LayerExplorer::getSelectedItems() const
{
  return m_treeView->getSelectedItems();
}

std::list<te::qt::widgets::AbstractTreeItem*> te::qt::widgets::LayerExplorer::getSelectedLayerItems() const
{
  return m_treeView->getSelectedLayerItems();
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerExplorer::getSelectedSingleLayers() const
{
  return m_treeView->getSelectedSingleLayers();
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerExplorer::getSelectedAndVisibleSingleLayers() const
{
  return m_treeView->getSelectedAndVisibleSingleLayers();
}

void te::qt::widgets::LayerExplorer::add(const te::map::AbstractLayerPtr& layer)
{
  if(m_treeModel == 0)
    return;

  m_treeModel->add(layer);
}

void te::qt::widgets::LayerExplorer::remove(AbstractTreeItem* item)
{
  if(m_treeModel == 0)
    return;

  m_treeModel->remove(item);
}

te::qt::widgets::LayerTreeView::ContextMenuType te::qt::widgets::LayerExplorer::getMenuType(int menuType) const
{
  return static_cast<te::qt::widgets::LayerTreeView::ContextMenuType>(menuType);
}

void te::qt::widgets::LayerExplorer::add(QAction* action,
                                         const QString& menu,
                                         const QString& layerType,
                                         te::qt::widgets::LayerTreeView::ContextMenuType menuType)
{
  if(m_treeView == 0)
    return;

  m_treeView->add(action, menu, layerType, menuType);
}

void te::qt::widgets::LayerExplorer::onSelectedLayersChanged(const std::list<te::map::AbstractLayerPtr>& selectedLayers)
{
  emit selectedLayersChanged(selectedLayers);
}

void te::qt::widgets::LayerExplorer::onVisibilityChanged(const te::map::AbstractLayerPtr& layer)
{
  emit visibilityChanged(layer);
}

void te::qt::widgets::LayerExplorer::onLayerOrderChanged()
{
  emit layerOrderChanged();
}

void te::qt::widgets::LayerExplorer::onItemDoubleClicked(te::qt::widgets::AbstractTreeItem* item)
{
  emit doubleClicked(item);
}
