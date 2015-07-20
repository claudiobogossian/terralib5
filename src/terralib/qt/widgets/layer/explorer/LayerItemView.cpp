#include "LayerItemView.h"

#include "LayerItemModel.h"
#include "LayerViewDelegate.h"
#include "RasterLayerDelegate.h"
#include "ThemeViewDelegate.h"
#include "LayerViewMenuManager.h"
#include "TreeItem.h"

bool Expand(const QModelIndex& idx, te::qt::widgets::LayerItemView* view)
{
  te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());

  return (item->getType() == "FOLDER");
}


te::qt::widgets::LayerItemView::LayerItemView(QWidget* parent):
  QTreeView(parent)
{
  setAcceptDrops(true);
  setDragEnabled(true);

  setDefaultDropAction(Qt::MoveAction);

  setRootIsDecorated(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);

  viewport()->setAutoFillBackground(true);

  m_model = new LayerItemModel(this);

//  m_delegate = new LayerViewDelegate((QStyledItemDelegate*)itemDelegate(), this);

  m_mnuMger = new LayerViewMenuManager(this);

  installEventFilter(m_mnuMger);

  setModel(m_model);

  // Mostrando icone só pro raster
//  setItemDelegate((new ThemeViewDelegate(((QStyledItemDelegate*)itemDelegate()), this)));
  setItemDelegate(new RasterLayerDelegate(new ThemeViewDelegate(new LayerViewDelegate(((QStyledItemDelegate*)itemDelegate()), this), this), this));

  //Mostrando icones do terraamazon
//  setItemDelegate(new LayerViewDelegate((QStyledItemDelegate*)itemDelegate(), this));

  //Mostrando icones do terraamazon e do raster
//  setItemDelegate(new RasterLayerDelegate(m_delegate, this));

  // Mostrando icones do terraview
//  setItemDelegate(new RasterLayerDelegate(new ThemeViewDelegate(m_delegate, this), this));
}

te::qt::widgets::LayerItemView::~LayerItemView()
{

}

void te::qt::widgets::LayerItemView::addLayers(const std::list<te::map::AbstractLayerPtr>& layers, const QModelIndex& idx, const std::string& idxPath)
{
  m_model->addLayers(layers, idx, idxPath);

  if(idx.isValid() && Expand(idx, this))
    expand(idx);

  selectionModel()->clearSelection();
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerItemView::getAllLayers() const
{
  return m_model->getAllLayers();
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerItemView::getVisibleLayers() const
{
  return m_model->getVisibleLayers();
}

void te::qt::widgets::LayerItemView::addFolder(const std::string& name, const QModelIndex& idx)
{
  m_model->addFolder(name, idx);

  if(idx.isValid() && Expand(idx, this))
    expand(idx);

  selectionModel()->clearSelection();
}

void te::qt::widgets::LayerItemView::addNoLayerAction(QAction* act)
{
  m_mnuMger->addAction(LayerViewMenuManager::NO_LAYERS, act);
}

void te::qt::widgets::LayerItemView::addAllLayerAction(QAction* act)
{
  m_mnuMger->addAction(LayerViewMenuManager::ALL_LAYERS, act);
}

void te::qt::widgets::LayerItemView::addVectorLayerAction(QAction* act)
{
  m_mnuMger->addAction(LayerViewMenuManager::VECTOR_LAYER, act);
}

void te::qt::widgets::LayerItemView::addRasterLayerAction(QAction* act)
{
  m_mnuMger->addAction(LayerViewMenuManager::RASTER_LAYER, act);
}

void te::qt::widgets::LayerItemView::addFolderLayerAction(QAction* act)
{
  m_mnuMger->addAction(LayerViewMenuManager::FOLDER_LAYER, act);
}

void te::qt::widgets::LayerItemView::addMultipleSelectionAction(QAction* act)
{
  m_mnuMger->addAction(LayerViewMenuManager::MULTI_LAYERS, act);
}

void te::qt::widgets::LayerItemView::removeItems(const QModelIndexList& idxs)
{
  m_model->removeItems(idxs);
  selectionModel()->clearSelection();
}

void te::qt::widgets::LayerItemView::removeSelectedItems()
{
  removeItems(selectionModel()->selectedIndexes());
}
