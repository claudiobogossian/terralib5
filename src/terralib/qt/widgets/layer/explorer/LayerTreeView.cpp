/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "AbstractTreeItem.h"
#include "LayerTreeModel.h"
#include "LayerTreeView.h"

// STL
#include <map>

// Boost
#include <boost/tuple/tuple.hpp>

// Qt
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QUrl>

/*!
  \class te::qt::widgets::LayerTreeView::Impl

  \brief LayerTreeView implementation.
*/
class te::qt::widgets::LayerTreeView::Impl
{
  public:

    typedef boost::tuple<QAction*, std::string, std::string, te::qt::widgets::LayerTreeView::ContextMenuSelectionType, bool> tuple_type;

    Impl(te::qt::widgets::LayerTreeView* ltv)
      : m_ltv(ltv)
    {
    }

    void add(QAction* action,
             const std::string& menu,
             const std::string& layerType,
             te::qt::widgets::LayerTreeView::ContextMenuSelectionType menuSelectionType)
    {
      m_menus.push_back(tuple_type(action, menu, layerType, menuSelectionType));
    }

    void remove(QAction* action)
    {
      std::list<tuple_type>::iterator it = m_menus.begin();

      while(it != m_menus.end())
      {
        if(it->get<0>() == action)
        {
          std::list<tuple_type>::iterator auxit = it;
          ++it;
          m_menus.erase(auxit);
        }
        else
        {
          ++it;
        }
      }
    }

    void showContextMenu(const QPoint& pos)
    {
      QMenu menu(m_ltv);

      std::list<AbstractTreeItem*> selectedItems = m_ltv->getSelectedItems();

      if(selectedItems.empty())
      {
        // if no items were selected, we only show the NO_ITEM_SELECTED actions
        for(std::list<tuple_type>::const_iterator it = m_menus.begin(); it != m_menus.end(); ++it)
        {
          QAction* action = it->get<0>();
          te::qt::widgets::LayerTreeView::ContextMenuSelectionType menuSelectionType = it->get<3>();

          if(menuSelectionType == te::qt::widgets::LayerTreeView::NO_ITEM_SELECTED)
            menu.addAction(action);
        }
      }
      else if(selectedItems.size() == 1)
      {
        // If just one item is selected we show their actions
        te::qt::widgets::AbstractTreeItem* selectedItem = selectedItems.front();

        te::map::AbstractLayerPtr layer = selectedItem->getLayer();

        // Get the type of the selected item
        std::string selectedItemType;

        if(layer && !layer->isValid())
        {
          QMessageBox::warning(0, tr("Warning"), tr("Is not possible to perform the action because this is an invalid layer!"));
          return;
        }

        if(layer && layer->getSchema().get() && layer->getSchema()->hasRaster())
          selectedItemType = "RASTER_LAYER_ITEM";
        else
          selectedItemType = selectedItem->getItemType();

        std::list<tuple_type>::const_iterator it;
        for(it = m_menus.begin(); it != m_menus.end(); ++it)
        {
          QAction* action = it->get<0>();
          std::string aItemType = it->get<2>();
          te::qt::widgets::LayerTreeView::ContextMenuSelectionType menuSelectionType = it->get<3>();

          if(menuSelectionType != te::qt::widgets::LayerTreeView::UNIQUE_ITEM_SELECTED)
            continue;

          if(!layer)
          {
            if((selectedItemType == "GROUPING_ITEM" || selectedItemType == "CHART_ITEM" || selectedItemType == "COLORMAP_ITEM") &&
                aItemType == "ITEM_OF_LAYER")
            {
              menu.addAction(action);
            }

            continue;
          }

          if(selectedItemType == "RASTER_LAYER_ITEM" || selectedItemType == "FOLDER_LAYER_ITEM")
          {
            if(aItemType == selectedItemType)
              menu.addAction(action);
          }
          else
          {
            if(aItemType.empty() || aItemType == selectedItemType)
              menu.addAction(action);
          }
        }
      }
      else
      {
        // If more than one layer is selected we must look for common actions depending on the layer types
        std::map<std::string, std::vector<QAction*> > actionsByLayerType;

        // Determine the layer types
        std::string layerType;

        std::list<AbstractTreeItem*>::const_iterator it;
        for(it = selectedItems.begin(); it != selectedItems.end(); ++it)
        {
          te::map::AbstractLayerPtr layer = (*it)->getLayer();
          if(!layer)
            return;

          if(layer && !layer->isValid())
          {
            QMessageBox::warning(0, tr("Warning"), tr("Is not possible to perform the action because there are invalid layers in the selection!"));
            return;
          }

          if(layer->getSchema().get() && layer->getSchema()->hasRaster())
            layerType = "RASTER_LAYER_ITEM";
          else
            layerType = (*it)->getItemType();

          actionsByLayerType[layerType] = std::vector<QAction*>();
        }

        // add actions to each group
        for(std::list<tuple_type>::const_iterator it = m_menus.begin(); it != m_menus.end(); ++it)
        {
          QAction* action = it->get<0>();
          std::string alayerType = it->get<2>();
          te::qt::widgets::LayerTreeView::ContextMenuSelectionType menuSelectionType = it->get<3>();

          if(menuSelectionType != te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED)
            continue;

          layerType = alayerType;

          if(layerType.empty())
          {
            for(std::map<std::string, std::vector<QAction*> >::iterator it = actionsByLayerType.begin();
                it != actionsByLayerType.end(); ++it)
            {
              it->second.push_back(action);
            }
          }
          else
            actionsByLayerType[layerType].push_back(action);
        }

        // determine the common list of actions
        std::vector<std::vector<QAction*> > setVec;

        for(std::map<std::string, std::vector<QAction*> >::iterator it = actionsByLayerType.begin();
            it != actionsByLayerType.end(); ++it)
        {
          setVec.push_back(it->second);
        }

        if(setVec.empty())
          return;

        std::vector<QAction*> commonActions = setVec[0];
        std::sort(commonActions.begin(), commonActions.end());

        for(std::size_t i = 1; i < setVec.size(); ++i)
        {
          std::sort(setVec[i].begin(), setVec[i].end());

          std::vector<QAction*> intersect;
          std::set_intersection(commonActions.begin(), commonActions.end(),
                                setVec[i].begin(), setVec[i].end(),
                                std::inserter(intersect, intersect.begin()));

          commonActions = intersect;
        }

        // add the actions to the popup menu
        for(std::size_t i = 0; i < commonActions.size(); ++ i)
          menu.addAction(commonActions[i]);
      }

      menu.exec(pos);
    }

  private:

    std::list<tuple_type> m_menus;          //!< A list of information about context menus.
    te::qt::widgets::LayerTreeView* m_ltv;  //!< The layer tree view associated to this implementation.
};

te::qt::widgets::LayerTreeView::LayerTreeView(QWidget* parent)
  : QTreeView(parent),
    m_pImpl(0)
{
  setAcceptDrops(true);
  setDragEnabled(true);

  setDefaultDropAction(Qt::MoveAction);

  setRootIsDecorated(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);

  viewport()->setAutoFillBackground(true);

  // Signals and slots
  connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(onItemActivated(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(onItemClicked(const QModelIndex&)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(onItemDoubleClicked(const QModelIndex&)));
  connect(this, SIGNAL(entered(const QModelIndex&)), SLOT(onItemEntered(const QModelIndex&)));
  connect(this, SIGNAL(pressed(const QModelIndex&)), SLOT(onItemPressed(const QModelIndex&)));

  m_pImpl = new Impl(this);
}

te::qt::widgets::LayerTreeView::~LayerTreeView()
{
  delete m_pImpl;
}

std::list<te::qt::widgets::AbstractTreeItem*> te::qt::widgets::LayerTreeView::getSelectedItems() const
{
  std::list<AbstractTreeItem*> selectedItems;

  QModelIndexList indexes = selectedIndexes();

  QModelIndex idx;

  foreach(idx, indexes)
  {
    AbstractTreeItem* selectedItem = static_cast<AbstractTreeItem*>(idx.internalPointer());

    if(selectedItem)
      selectedItems.push_back(selectedItem);
  }

  return selectedItems;
}

std::list<te::qt::widgets::AbstractTreeItem*> te::qt::widgets::LayerTreeView::getSelectedLayerItems() const
{
  std::list<te::qt::widgets::AbstractTreeItem*> selectedItems = getSelectedItems();

  std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems;

  std::list<te::qt::widgets::AbstractTreeItem*>::const_iterator it;
  for(it = selectedItems.begin(); it != selectedItems.end(); ++it)
  {
    te::qt::widgets::AbstractTreeItem* selectedItem = *it;

    if(selectedItem->getLayer())
      selectedLayerItems.push_back(selectedItem);
  }

  return selectedLayerItems;
}

std::list<te::qt::widgets::AbstractTreeItem*> te::qt::widgets::LayerTreeView::getSelectedSingleLayerItems() const
{
  std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = getSelectedLayerItems();

  std::list<te::qt::widgets::AbstractTreeItem*> selectedSingleLayerItems;

  std::list<te::qt::widgets::AbstractTreeItem*>::const_iterator it;
  for(it = selectedLayerItems.begin(); it != selectedLayerItems.end(); ++it)
  {
    te::qt::widgets::AbstractTreeItem* selectedLayerItem = *it;

    if(selectedLayerItem->getLayer() && selectedLayerItem->getItemType() != "FOLDER_ITEM_TYPE")
      selectedSingleLayerItems.push_back(selectedLayerItem);
  }

  return selectedSingleLayerItems;
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerTreeView::getSelectedSingleLayers() const
{
  std::list<te::map::AbstractLayerPtr> selectedSingleLayers;

  std::list<te::qt::widgets::AbstractTreeItem*> selectedItems = getSelectedItems();

  std::list<te::qt::widgets::AbstractTreeItem*>::const_iterator it;
  for(it = selectedItems.begin(); it != selectedItems.end(); ++it)
  {
    AbstractTreeItem* item = *it;

    te::map::AbstractLayerPtr layer = (*it)->getLayer();

    if(layer && layer->getType() != "FOLDERLAYER")
      selectedSingleLayers.push_back(layer);
  }

  return selectedSingleLayers;
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerTreeView::getSelectedAndVisibleSingleLayers() const
{
  std::list<te::map::AbstractLayerPtr> selectedAndVisibleSingleLayers;

  std::list<te::map::AbstractLayerPtr> selectedSingleLayers = getSelectedSingleLayers();

  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  for(it = selectedSingleLayers.begin(); it != selectedSingleLayers.end(); ++it)
  {
    te::map::AbstractLayerPtr layer = *it;
    if(layer->getVisibility() == te::map::VISIBLE)
      selectedAndVisibleSingleLayers.push_back(layer);
  }

  return selectedAndVisibleSingleLayers;
}

void te::qt::widgets::LayerTreeView::onSelectedLayersChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
  std::list<te::map::AbstractLayerPtr> selectedLayers;

  std::list<te::qt::widgets::AbstractTreeItem*> selectedItems = getSelectedItems();

  std::list<te::qt::widgets::AbstractTreeItem*>::const_iterator it;
  for(it = selectedItems.begin(); it != selectedItems.end(); ++it)
  {
    te::map::AbstractLayerPtr selectedLayer = (*it)->getLayer();

    if(selectedLayer)
      selectedLayers.push_back(selectedLayer);
  }

  if(!selectedLayers.empty())
    emit selectedLayersChanged(selectedLayers);
}

void te::qt::widgets::LayerTreeView::add(QAction* action,
                                         const std::string& menu,
                                         const std::string& itemType,
                                         ContextMenuSelectionType menuSelectionType)
{
  m_pImpl->add(action, menu, itemType, menuSelectionType);
}

void te::qt::widgets::LayerTreeView::remove(QAction* action)
{
  m_pImpl->remove(action);
}

void te::qt::widgets::LayerTreeView::onItemActivated(const QModelIndex& index)
{
  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

  emit activated(item);
}

void te::qt::widgets::LayerTreeView::onItemClicked(const QModelIndex& index)
{
  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

  emit clicked(item);

  LayerTreeModel* model = dynamic_cast<LayerTreeModel*>(this->model());

  if(model == 0)
    return;

  if(!model->isCheckable())
    return;

  // If the item visibility was changed, emit the signal of visibilityChanged for this item,
  // for their descendants(if any) and for their ancestors
  te::map::AbstractLayerPtr itemLayer = item->getLayer();
  if((itemLayer != 0) && itemLayer->hasVisibilityChanged())
  {
    if(itemLayer->getType() != "FOLDERLAYER")
      emit visibilityChanged(itemLayer);

    // For their descendants
    std::vector<AbstractTreeItem*> descendantItems = item->getDescendants();
    for(std::size_t i = 0; i < descendantItems.size(); ++i)
    {
      te::map::AbstractLayerPtr descendantLayer = descendantItems[i]->getLayer();

      if((descendantLayer != 0) && descendantLayer->hasVisibilityChanged())
      {
        if(descendantLayer->getType() != "FOLDERLAYER")
          emit visibilityChanged(descendantLayer);
      }
    }

    // For their ancestors
    std::vector<AbstractTreeItem*> ancestorItems = item->getAncestors();
    for(std::size_t i = 0; i < ancestorItems.size(); ++i)
    {
      te::map::AbstractLayerPtr ancestorLayer = ancestorItems[i]->getLayer();

      if((ancestorLayer != 0) && ancestorLayer->hasVisibilityChanged())
      {
        if(ancestorLayer->getType() != "FOLDERLAYER")
          emit visibilityChanged(ancestorLayer);
      }
    }
  }
}

void te::qt::widgets::LayerTreeView::onItemDoubleClicked(const QModelIndex& index)
{
  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

  emit doubleClicked(item);
}

void te::qt::widgets::LayerTreeView::onItemEntered(const QModelIndex& index)
{
  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

  emit entered(item);
}

void te::qt::widgets::LayerTreeView::onItemPressed(const QModelIndex& index)
{
  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

  te::map::AbstractLayerPtr layer = static_cast<te::map::AbstractLayer*>(item->getLayer().get());

  if(layer)
    layer->setVisibilityAsChanged(false);

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
  if(e->keyboardModifiers() == Qt::ControlModifier)
    e->setDropAction(Qt::CopyAction);
  else
    e->setDropAction(Qt::MoveAction);

  e->accept();

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

void te::qt::widgets::LayerTreeView::contextMenuEvent(QContextMenuEvent* e)
{
  assert(e);

  m_pImpl->showContextMenu(e->globalPos());
}
