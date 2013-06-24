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

// STL
#include <map>

// Boost
#include <boost/tuple/tuple.hpp>

// Qt
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>

/*!
  \class te::qt::widgets::LayerTreeView::Impl

  \brief LayerTreeView implementation.
*/
class te::qt::widgets::LayerTreeView::Impl
{
  public:

    typedef boost::tuple<QAction*, QString, QString, te::qt::widgets::LayerTreeView::ContextMenuType> tuple_type;

    Impl(te::qt::widgets::LayerTreeView* ltv)
      : m_ltv(ltv)
    {
    }

    void add(QAction* action,
             const QString& menu,
             const QString& layerType,
             te::qt::widgets::LayerTreeView::ContextMenuType menuType)
    {
      m_menus.push_back(tuple_type(action, menu, layerType, menuType));
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

      std::list<AbstractLayerTreeItem*> selectedLayers = m_ltv->getSelectedItems();

      if(selectedLayers.empty())
      {
        // if no layers were selected, we only show the NO_LAYER_SELECTED actions
        for(std::list<tuple_type>::const_iterator it = m_menus.begin();
            it != m_menus.end();
            ++it)
        {
          QAction* action = it->get<0>();
          //QString menuName = it->get<1>();
          //QString layerType = it->get<2>();
          te::qt::widgets::LayerTreeView::ContextMenuType menuType = it->get<3>();

          if(menuType == te::qt::widgets::LayerTreeView::NO_LAYER_SELECTED)
          {
            menu.addAction(action);
          }
        }
      }
      else if(selectedLayers.size() == 1)
      {
        // If just one layer is selected we show their actions
        QString layerType(QString::fromStdString(selectedLayers.front()->getLayer()->getType()));

        for(std::list<tuple_type>::const_iterator it = m_menus.begin();
            it != m_menus.end();
            ++it)
        {
          QAction* action = it->get<0>();
          //QString menuName = it->get<1>();
          QString alayerType = it->get<2>();
          te::qt::widgets::LayerTreeView::ContextMenuType menuType = it->get<3>();

          if(menuType == te::qt::widgets::LayerTreeView::SINGLE_LAYER_SELECTED &&
             ((alayerType == layerType) || alayerType.isEmpty()))
          {
            menu.addAction(action);
          }
        }
      }
      else
      {
        // if more than one layer is selected we must look for common actions depending on the layer types
        std::map<std::string, std::vector<QAction*> > actionsByLayerType;

        // determine the layer types
        for(std::list<AbstractLayerTreeItem*>::const_iterator it = selectedLayers.begin();
            it != selectedLayers.end();
            ++it)
        {
          te::map::AbstractLayerPtr layer = (*it)->getLayer();

          actionsByLayerType[layer->getType()] = std::vector<QAction*>();
        }

        // add actions to each group
        for(std::list<tuple_type>::const_iterator it = m_menus.begin();
            it != m_menus.end();
            ++it)
        {
          QAction* action = it->get<0>();
          //QString menuName = it->get<1>();
          QString alayerType = it->get<2>();
          te::qt::widgets::LayerTreeView::ContextMenuType menuType = it->get<3>();

          if(menuType != te::qt::widgets::LayerTreeView::MULTIPLE_LAYERS_SELECTED)
            continue;

          std::string layerType = alayerType.toStdString();

          if(layerType.empty())
          {
            for(std::map<std::string, std::vector<QAction*> >::iterator it = actionsByLayerType.begin();
                it != actionsByLayerType.end();
                ++it)
            {
              it->second.push_back(action);
            }
          }
          else
          {
            actionsByLayerType[layerType].push_back(action);
          }
        }

        // determine the common list of actions
        std::vector<std::vector<QAction*> > setVec;
        for(std::map<std::string, std::vector<QAction*> >::iterator it = actionsByLayerType.begin();
            it != actionsByLayerType.end(); ++it)
        {
          setVec.push_back(it->second);
        }

        std::vector<QAction*> commonActions = setVec[0];
        for(std::size_t i = 1; i < setVec.size(); ++i)
        {
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
  setRootIsDecorated(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);

  viewport()->setAutoFillBackground(true);

  connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(itemActivated(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(itemClicked(const QModelIndex&)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(itemDoubleClicked(const QModelIndex&)));
  connect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(itemEntered(const QModelIndex&)));
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(itemPressed(const QModelIndex&)));

  m_pImpl = new Impl(this);
}

te::qt::widgets::LayerTreeView::~LayerTreeView()
{
  delete m_pImpl;
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

QModelIndexList te::qt::widgets::LayerTreeView::getSelectedIndexes() const
{
  return selectedIndexes();
}

void te::qt::widgets::LayerTreeView::add(const te::map::AbstractLayerPtr& layer)
{
  LayerTreeModel* model = dynamic_cast<LayerTreeModel*>(this->model());

  if(model == 0)
  {
    QMessageBox::warning(this,
                         tr("TerraLib"),
                         tr("Can not add a layer to an empty model!"));
    return;
  }

  model->add(layer);
}

void te::qt::widgets::LayerTreeView::remove(const QModelIndex& index)
{
  LayerTreeModel* model = dynamic_cast<LayerTreeModel*>(this->model());

  if(model == 0)
  {
    QMessageBox::warning(this,
                         tr("TerraLib"),
                         tr("Can not remove a layer from an empty model!"));
    return;
  }

  model->remove(index);
}

void te::qt::widgets::LayerTreeView::add(QAction* action,
                                         const QString& menu,
                                         const QString& layerType,
                                         ContextMenuType menuType)
{
  m_pImpl->add(action, menu, layerType, menuType);
}

void te::qt::widgets::LayerTreeView::remove(QAction* action)
{
  m_pImpl->remove(action);
}

void te::qt::widgets::LayerTreeView::itemActivated(const QModelIndex & index)
{
  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  emit activated(item);
}

void te::qt::widgets::LayerTreeView::itemClicked(const QModelIndex& index)
{
  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  emit clicked(item);

  LayerTreeModel* model = dynamic_cast<LayerTreeModel*>(this->model());

  if(model == 0)
    return;

  if(!model->isCheckable())
    return;

  // If the item visibility was changed, emit the signal of visibilityChanged for this item,
  // for their descendants(if any) and for their ancestors
  te::map::AbstractLayer* itemLayer = item->getLayer().get();
  if((itemLayer != 0) && itemLayer->hasVisibilityChanged())
  {
    emit visibilityChanged(item);

    // For their descendants
    std::vector<AbstractLayerTreeItem*> descendantItems = item->getDescendants();
    for(std::size_t i = 0; i < descendantItems.size(); ++i)
    {
      te::map::AbstractLayer* descendantLayer = descendantItems[i]->getLayer().get();
      if((descendantLayer != 0) && descendantLayer->hasVisibilityChanged())
        emit visibilityChanged(descendantItems[i]);
    }

    // For their ancestors
    std::vector<AbstractLayerTreeItem*> ancestorItems = item->getAncestors();
    for(std::size_t i = 0; i < ancestorItems.size(); ++i)
    {
      te::map::AbstractLayer* ancestorLayer = ancestorItems[i]->getLayer().get();
      if((ancestorLayer != 0) && ancestorLayer->hasVisibilityChanged())
        emit visibilityChanged(ancestorItems[i]);
    }
  }
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

  te::map::AbstractLayer* layer = static_cast<te::map::AbstractLayer*>(item->getLayer().get());
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

void te::qt::widgets::LayerTreeView::contextMenuEvent(QContextMenuEvent* e)
{
  assert(e);

  m_pImpl->showContextMenu(e->globalPos());
}

