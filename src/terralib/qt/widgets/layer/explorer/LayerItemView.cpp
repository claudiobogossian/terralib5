#include "LayerItemView.h"

#include "LayerItemModel.h"
#include "LayerViewDelegate.h"
#include "LayerViewMenuManager.h"
#include "LayerItem.h"

// Qt
#include <QMouseEvent>

bool Expand(const QModelIndex& idx, te::qt::widgets::LayerItemView* view)
{
  te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());

  return (item->getType() == "FOLDER");
}

std::list<te::map::AbstractLayerPtr> GetSelectedLayersOnly(te::qt::widgets::LayerItemView* view)
{
  std::list<te::map::AbstractLayerPtr> res;
  std::list<te::qt::widgets::TreeItem*> items = view->getSelectedItems();

  for(std::list<te::qt::widgets::TreeItem*>::iterator it = items.begin(); it != items.end(); ++it)
  if((*it)->getType() == "LAYER")
    res.push_back(((te::qt::widgets::LayerItem*)(*it))->getLayer());

  return res;
}

QModelIndex FindLayerIndex(QAbstractItemModel* model, const QModelIndex& parent, te::map::AbstractLayer* l)
{
  int cs = model->rowCount(parent);

  for(int i = 0; i < cs; i++)
  {
    QModelIndex cIdx = model->index(i, 0, parent);
    te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(cIdx.internalPointer());

    if(item->getType() == "LAYER")
    {
      if(((te::qt::widgets::LayerItem*)item)->getLayer().get() == l)
        return cIdx;
    }
    else if(item->getType() == "FOLDER")
    {
      QModelIndex fIdx = FindLayerIndex(model, cIdx, l);

      if(fIdx.isValid())
        return fIdx;
    }
  }

  return QModelIndex();
}

te::qt::widgets::LayerItemView::LayerItemView(QWidget* parent):
  QTreeView(parent),
  m_outterFilter(0)
{
  setAcceptDrops(true);
  setDragEnabled(true);

  setDefaultDropAction(Qt::MoveAction);

  setRootIsDecorated(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);

  viewport()->setAutoFillBackground(true);

  m_model = new LayerItemModel(this);

  m_mnuMger = new LayerViewMenuManager(this);

  installEventFilter(m_mnuMger);

  setModel(m_model);

  setItemDelegate(new LayerViewDelegate((QStyledItemDelegate*)itemDelegate(), this));

  connect(m_model, SIGNAL(visibilityChanged()), SIGNAL(visibilityChanged()));
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

void te::qt::widgets::LayerItemView::setLayers(const std::list<te::map::AbstractLayerPtr>& layers)
{
  m_model->setLayers(layers);
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerItemView::getAllLayers() const
{
  return m_model->getAllLayers();
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerItemView::getVisibleLayers() const
{
  return m_model->getVisibleLayers();
}

std::list<te::qt::widgets::TreeItem*> te::qt::widgets::LayerItemView::getSelectedItems() const
{
  std::list<te::qt::widgets::TreeItem*> res;

  QModelIndexList lst = QTreeView::selectedIndexes();

  qSort(lst);

  for(QModelIndexList::ConstIterator it = lst.begin(); it != lst.end(); ++it)
    res.push_back(static_cast<te::qt::widgets::TreeItem*>((*it).internalPointer()));

  return res;
}

void te::qt::widgets::LayerItemView::addFolder(const std::string& name, const QModelIndex& idx)
{
  m_model->addFolder(name, idx);

  if(idx.isValid() && Expand(idx, this))
    expand(idx);

  selectionModel()->clearSelection();
}

void te::qt::widgets::LayerItemView::updateChart(const QModelIndex& idx)
{
  std::vector<TreeItem*> items;
  TreeItem* item = static_cast<TreeItem*>(idx.internalPointer());

  item->getChildren(items, "CHART");

  if(!items.empty())
  {
    TreeItem* chart = *items.begin();
    int pos = chart->getPosition();

    QModelIndex cIdx = idx.child(pos, 0);

    QModelIndexList ls;

    ls << cIdx;

    removeItems(ls);
  }

  ((LayerItem*)item)->updateChart();

  m_model->insertRows(1, 1, idx);

  QTreeView::expand(idx);
  QTreeView::expand(idx.child(1, 0));
}

void te::qt::widgets::LayerItemView::updateGrouping(const QModelIndex& idx)
{
  std::vector<TreeItem*> items;
  TreeItem* item = static_cast<TreeItem*>(idx.internalPointer());
  QModelIndexList ls;

  item->getChildren(items, "GROUPING");

  if(!items.empty())
  {
    TreeItem* g = *items.begin();
    int pos = g->getPosition();

    QModelIndex cIdx = idx.child(pos, 0);
    ls << cIdx;
  }

  item->getChildren(items, "COLORMAP");

  if(!items.empty())
  {
    TreeItem* c = *items.begin();
    int pos = c->getPosition();

    QModelIndex cIdx = idx.child(pos, 0);
    ls << cIdx;
  }

  if(!ls.isEmpty())
    removeItems(ls);

  int pos = ((LayerItem*)item)->updateGrouping();

  m_model->insertRows(pos, 1, idx);

  QTreeView::expand(idx);
  QTreeView::expand(idx.child(pos, 0));
}

void te::qt::widgets::LayerItemView::updateLegend(te::map::AbstractLayer* l)
{
  QModelIndex lIdx = FindLayerIndex(model(), QModelIndex(), l);

  if(lIdx.isValid())
  {
    LayerItem* li = static_cast<LayerItem*>(lIdx.internalPointer());
    li->updateLegend();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

    QVector<int> roles;

    roles << Qt::DecorationRole;

    dataChanged(lIdx.child(0, 0), lIdx.child(0, 0), roles);
#else
    dataChanged(lIdx.child(0, 0), lIdx.child(0, 0));
#endif

  }
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

void te::qt::widgets::LayerItemView::addInvalidLayerAction(QAction* act)
{
  m_mnuMger->addAction(LayerViewMenuManager::INVALID_LAYERS, act);
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

void te::qt::widgets::LayerItemView::removeDelegate(QStyledItemDelegate* d)
{
  te::common::Decorator<QStyledItemDelegate>* delegate =
      dynamic_cast< te::common::Decorator<QStyledItemDelegate>* >(itemDelegate());

  if(delegate != 0)
  {
    QStyledItemDelegate* newDelegate = delegate->removeDecorator(d);

    if(newDelegate != 0)
      setItemDelegate(newDelegate);
  }
}

void te::qt::widgets::LayerItemView::setMenuEventHandler(QObject* obj)
{
  if(m_outterFilter != 0)
    removeEventFilter(m_outterFilter);

  if(obj != m_mnuMger)
  {
    removeEventFilter(m_mnuMger);
    m_outterFilter = obj;
  }
  else
    m_outterFilter = 0;

  installEventFilter(obj);
}

QObject* te::qt::widgets::LayerItemView::getMenuEventHandler() const
{
  return (m_outterFilter != 0) ? m_outterFilter : m_mnuMger;
}

void te::qt::widgets::LayerItemView::removeMenuEventHandler(QObject* handler)
{
  if(m_outterFilter == 0)
    return;

  te::common::Decorator<QObject>* h = dynamic_cast< te::common::Decorator<QObject>* >(m_outterFilter);

  if(h != 0)
  {
    QObject* newH = h->removeDecorator(handler);

    if(newH != 0)
      setMenuEventHandler(newH);
  }
}

void te::qt::widgets::LayerItemView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
  QTreeView::selectionChanged(selected, deselected);

  emit selectedLayersChanged(GetSelectedLayersOnly(this));
}

void te::qt::widgets::LayerItemView::dropEvent(QDropEvent * event)
{
  QTreeView::dropEvent(event);

  emit layerOrderChanged();
}

void te::qt::widgets::LayerItemView::mouseDoubleClickEvent(QMouseEvent* event)
{
  QPoint pos = event->globalPos();
  QModelIndex idx = indexAt(viewport()->mapFromGlobal(pos));

  if(idx.isValid())
  {
    TreeItem* item = static_cast<TreeItem*>(idx.internalPointer());

    if(item->getType() == "LEGEND" && item->getParent()->getType() == "LAYER")
      emit doubleClicked(((LayerItem*)item->getParent())->getLayer());
  }
}
