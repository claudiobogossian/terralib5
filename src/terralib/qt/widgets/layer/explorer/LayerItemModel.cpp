#include "LayerItemModel.h"

#include "FolderItem.h"
#include "LayerItem.h"

#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"

#include "../../../../maptools/FolderLayer.h"
#include "../../../../maptools/DataSetLayer.h"


// Qt
#include <QMimeData>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// STL
#include <memory>

void LoadLayer(te::map::AbstractLayerPtr layer, const std::string& idxPath)
{
  if(layer->getType() != "DATASETLAYER")
    return;

  te::map::DataSetLayer* l = dynamic_cast<te::map::DataSetLayer*>(layer.get());

  te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(l->getDataSourceId());

  te::da::DataSourcePtr dsrc = te::da::DataSourceManager::getInstance().get(info->getId(), info->getType(), info->getConnInfo());

  //if(!l->hasIndex())
  //  l->loadIndex(idxPath);
}

te::qt::widgets::TreeItem* GetFolder(te::common::TreeItemPtr l, const std::string& idxPath)
{
  te::qt::widgets::TreeItem* folder = new te::qt::widgets::FolderItem(dynamic_cast<te::map::FolderLayer*>(l.get())->getTitle());

  std::list<te::common::TreeItemPtr> children = l->getChildren();

  for(std::list<te::common::TreeItemPtr>::iterator it = children.begin(); it != children.end(); ++it)
  {
    te::common::TreeItemPtr item = *it;

    te::map::AbstractLayerPtr layer(boost::dynamic_pointer_cast<te::map::AbstractLayer>(item));

    if(layer->getType() == "FOLDERLAYER")
      folder->addChild(GetFolder(layer, idxPath));
    else if(layer->getType() == "DATASETLAYER")
    {
      folder->addChild(new te::qt::widgets::LayerItem(layer));
      LoadLayer(layer, idxPath);
    }
  }

  return folder;
}

void GetRootFolder(std::list<te::map::AbstractLayerPtr> layers, te::qt::widgets::TreeItem* root, const std::string& idxPath)
{
  for(std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
  {
    if((*it).get()->getType() == "FOLDERLAYER")
      root->addChild(GetFolder((*it), idxPath));
    else if((*it).get()->getType() == "DATASETLAYER")
    {
      root->addChild(new te::qt::widgets::LayerItem((*it)));

      LoadLayer((*it), idxPath);
    }
  }
}

void GetAllLayers(std::list<te::map::AbstractLayerPtr>& layers, const te::qt::widgets::TreeItem* item, const te::qt::widgets::TreeItem* root)
{  
  if(item->getType() == "FOLDER")
  {
    size_t c = item->getChildrenCount("");

    std::list<te::map::AbstractLayerPtr> ls;

    for(size_t i = 0; i < c; i++)
      GetAllLayers(ls, item->getChild(i), root);

    if(item->getParent() != 0)
    {
      static boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);

      te::map::FolderLayer* fd = new te::map::FolderLayer(id, item->getAsString());
      fd->setId(id);

      for(std::list<te::map::AbstractLayerPtr>::iterator it = ls.begin(); it != ls.end(); ++it)
        fd->add((*it));

      layers.push_back(te::map::FolderLayerPtr(fd));
    }
    else
      for(std::list<te::map::AbstractLayerPtr>::iterator it=ls.begin(); it!=ls.end(); ++it)
        layers.push_back((*it));
  }
  else if(item->getType() == "LAYER")
    layers.push_back(((te::qt::widgets::LayerItem*)item)->getLayer());
}

void GetVisibleLayers(std::list<te::map::AbstractLayerPtr>& layers, const te::qt::widgets::TreeItem* item)
{
  if(item->getType() == "FOLDER")
  {
    size_t c = item->getChildrenCount("");

    for(size_t i = 0; i < c; i++)
      GetVisibleLayers(layers, item->getChild(i));
  }
  else if(item->getType() == "LAYER" && item->isVisible() == te::qt::widgets::TOTALLY)
  {
    te::map::AbstractLayerPtr l = ((te::qt::widgets::LayerItem*)item)->getLayer();
    layers.push_back(l);
  }
}

bool ParentInRemoveList(const QModelIndexList& lst, const QModelIndex& idx)
{
  if(!idx.isValid())
    return false;

  if(lst.contains(idx.parent()))
    return true;

  return ParentInRemoveList(lst, idx.parent());
}

void CleanRemoveList(QModelIndexList& lst)
{
  std::vector<QModelIndex> idxs;

  foreach(QModelIndex idx, lst)
  {
    if((static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer()))->getType() == "LEGEND")
      idxs.push_back(idx);
    else if(ParentInRemoveList(lst, idx))
      idxs.push_back(idx);
  }

  for(std::vector<QModelIndex>::iterator it = idxs.begin(); it != idxs.end(); ++it)
    lst.removeAll((*it));
}

QModelIndex FindIndex(const te::qt::widgets::LayerItemModel* model, const te::qt::widgets::TreeItem* item, const QModelIndex& idx)
{
  try
  {
    if(idx.internalPointer() == item)
      throw idx;
    else
    {
      int c = model->rowCount(idx);

      for(int i = 0; i < c; i++)
        FindIndex(model, item, model->index(i, 0, idx));
    }
  }
  catch(const QModelIndex& idxE)
  {
    if(!idx.isValid())
      return idxE;
    else
      throw idxE;
  }

  return idx;
}

QModelIndex FindInsertInformation(const QModelIndex& par, int& row, te::qt::widgets::LayerItemModel* model)
{
  // If the parent was not given
  if(!par.isValid())
  {
    row = model->rowCount();

    return QModelIndex();
  }

  te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(par.internalPointer());

  // If the parent is a FolderItem
  if(item->getType() == "FOLDER")
  {
    row = model->rowCount(par);

    return par;
  }

  // If the parent is a LayerItem
  if(item->getType() == "LAYER")
  {
    row = par.row() + 1;

    return model->parent(par);
  }

  // If the parent is a LegendItem
  QModelIndex parP = model->parent(par);
  row = parP.row() + 1;

  return model->parent(parP);
}

te::qt::widgets::TreeItem* FindParent(te::qt::widgets::TreeItem* item)
{
  if(item->getType() == "FOLDER")
    return item;

  return FindParent(item->getParent());
}

te::qt::widgets::LayerItemModel::LayerItemModel(QObject *parent):
  QAbstractItemModel(parent)
{
  m_root.reset(new FolderItem("Layers"));
}

te::qt::widgets::LayerItemModel::~LayerItemModel()
{
}

void te::qt::widgets::LayerItemModel::addLayer(te::map::AbstractLayerPtr layer, const std::string& idxPath)
{
  addLayer(layer, QModelIndex(), idxPath);
}

void te::qt::widgets::LayerItemModel::addLayer(te::map::AbstractLayerPtr layer, const QModelIndex& parent, const std::string& idxPath)
{
  int row;

  QModelIndex flIdx = FindInsertInformation(parent, row, this);

  TreeItem* item = (flIdx.isValid()) ? static_cast<TreeItem*>(flIdx.internalPointer()) : m_root.get();

  if(layer->getType() == "FOLDERLAYER")
    item->insertChild(GetFolder(layer, idxPath), (size_t)row);
  else if(layer->getType() == "DATASETLAYER")
  {
    item->insertChild(new LayerItem(layer), (size_t)row);

    //if(!layer->hasIndex())
    //  layer->loadIndex(idxPath);
  }
}

void te::qt::widgets::LayerItemModel::addLayers(const std::list<te::map::AbstractLayerPtr>& layers, const std::string& idxPath)
{
  addLayers(layers, QModelIndex(), idxPath);
}

void te::qt::widgets::LayerItemModel::addLayers(const std::list<te::map::AbstractLayerPtr>& layers, const QModelIndex& parent, const std::string& idxPath)
{
  int row;

  QModelIndex par = FindInsertInformation(parent, row, this);

  TreeItem* item = (par.isValid()) ? static_cast<TreeItem*>(par.internalPointer()) : m_root.get();

  beginInsertRows(par, row, row + (int)layers.size() - 1);

  if(!item->hasChildren())
    GetRootFolder(layers, item, idxPath);
  else
  {
    int cont = row;

    for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
    {
      te::map::AbstractLayerPtr layer = *it;

      if(layer->getType() == "FOLDERLAYER")
        item->insertChild(GetFolder(layer, idxPath), (size_t)cont);
      else if(layer->getType() == "DATASETLAYER")
      {
        item->insertChild(new LayerItem(layer), (size_t)cont);

        //if(!layer->hasIndex())
        //  layer->loadIndex(idxPath);
      }

      cont++;
    }
  }

  endInsertRows();
}

void te::qt::widgets::LayerItemModel::setLayers(const std::list<te::map::AbstractLayerPtr>& layers)
{
  beginResetModel();

  m_root->removeAllChilds();

  endResetModel();

  addLayers(layers);
}

void te::qt::widgets::LayerItemModel::addItems(const std::vector<TreeItem*> items, TreeItem* parent, const int& pos)
{
  std::vector<TreeItem*> aux;
  QModelIndexList ls;
  QModelIndex parentIdx = FindIndex(this, parent, QModelIndex());

  for(std::vector<TreeItem*>::const_iterator it = items.begin(); it != items.end(); ++it)
    ls << FindIndex(this, (*it), QModelIndex());

  CleanRemoveList(ls);

  qSort(ls.begin(), ls.end());

  std::list<QModelIndex> lIdx = ls.toStdList();

  int beforePos = 0;
  size_t pC = parent->getChildrenCount("");

  for(std::list<QModelIndex>::reverse_iterator it = lIdx.rbegin(); it != lIdx.rend(); ++it)
  {
    beginRemoveRows((*it).parent(), (*it).row(), (*it).row());

    QModelIndex auxIdx = (*it).parent();

    TreeItem* auxPar = (auxIdx.isValid()) ? static_cast<TreeItem*>(auxIdx.internalPointer()) : m_root.get();

    TreeItem* item = static_cast<TreeItem*>((*it).internalPointer());

    if((item->getParent() == parent) && (item->getPosition() < pos))
      beforePos++;

    aux.push_back(auxPar->removeChild((*it).row()));

    endRemoveRows();
  }
  
  if((size_t)pos < pC)
    pC = pos;

  pC -= beforePos;

  for(std::vector<TreeItem*>::iterator it = aux.begin(); it != aux.end(); ++it)
  {
    beginInsertRows(parentIdx, (int)pC, (int)pC);

    parent->insertChild((*it), pC);

    endInsertRows();
  }
}

void te::qt::widgets::LayerItemModel::addFolder(const std::string& name, TreeItem* parent)
{
  addFolder(name, FindIndex(this, parent, QModelIndex()));
}

void te::qt::widgets::LayerItemModel::addFolder(const std::string& name, const QModelIndex& idx)
{
  int row;
  TreeItem* fItem = new FolderItem(name);

  QModelIndex par = FindInsertInformation(idx, row, this);

  TreeItem* parItem = (par.isValid()) ? static_cast<TreeItem*>(par.internalPointer()) : m_root.get();

  beginInsertRows(par, row, row);

  parItem->insertChild(fItem, (size_t)row);

  endInsertRows();
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerItemModel::getAllLayers()
{
  std::list<te::map::AbstractLayerPtr> ls;

  GetAllLayers(ls, m_root.get(), m_root.get());

  return ls;
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerItemModel::getVisibleLayers()
{
  std::list<te::map::AbstractLayerPtr> ls;

  GetVisibleLayers(ls, m_root.get());

  return ls;
}

int te::qt::widgets::LayerItemModel::columnCount(const QModelIndex&) const
{
  return 1;
}

bool te::qt::widgets::LayerItemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    TreeItem* itm = static_cast<TreeItem*>(index.internalPointer());

    bool ok;

    int v = value.toInt(&ok);

    if(!ok)
      return false;

    itm->setVisible((v == Qt::Checked) ? TOTALLY : NONE, true, true);

    // Find first parent
    QModelIndex f = parent(index);

    while(f.parent().isValid())
      f = f.parent();

    // Find the last row
    int count = rowCount(index);

    QModelIndex l = (count == 0) ? index : index.child(count - 1, 0);

    while(true)
    {
      count = rowCount(l);

      if(count == 0)
        break;

      l = l.child(count - 1, 0);
    }

    emit dataChanged(f, l);
  }

  return true;
}

bool te::qt::widgets::LayerItemModel::removeRows(int row, int count, const QModelIndex& parent)
{
  int last = row + count -1;

  beginRemoveRows(parent, row, last);

  TreeItem* parItem = (parent.isValid()) ? static_cast<TreeItem*>(parent.internalPointer()) : m_root.get();

  for(int i = row; i <= last; i++)
    std::auto_ptr<TreeItem> aux(parItem->removeChild(i));

  endRemoveRows();

  return true;
}

Qt::DropActions te::qt::widgets::LayerItemModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

QStringList te::qt::widgets::LayerItemModel::mimeTypes() const
{
  QStringList types;
  types << "application/layers";
  return types;
}

QMimeData* te::qt::widgets::LayerItemModel::mimeData(const QModelIndexList& indexes) const
{
  QMimeData *mimeData = new QMimeData();
  std::vector<TreeItem*>* aux = new std::vector<TreeItem*>();

  foreach (QModelIndex idx, indexes)
    if(idx.isValid())
      aux->push_back(static_cast<TreeItem*>(idx.internalPointer()));

  QString s;
  s.setNum((qulonglong)aux);

  QByteArray encodedData(s.toStdString().c_str());

  mimeData->setData("application/layers", encodedData);

  return mimeData;
}

bool te::qt::widgets::LayerItemModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
  Q_UNUSED(action);
  Q_UNUSED(row);
  Q_UNUSED(parent);

  if (!data->hasFormat("application/layers"))
    return false;

  if (column > 0)
    return false;

  return true;
}

bool te::qt::widgets::LayerItemModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
    return false;

  if (action == Qt::IgnoreAction)
    return true;

  QByteArray encodedData = data->data("application/layers");

  qulonglong dataValue = encodedData.toULongLong();

  std::auto_ptr< std::vector<TreeItem*> > changed(reinterpret_cast< std::vector<TreeItem*>* >(dataValue));

  TreeItem* newPar;

  if(row == -1) // Dropped over an item
  {
    if(parent.isValid())
    {
      TreeItem* aux = static_cast<TreeItem*>(parent.internalPointer());

      if(aux->getType() == "LAYER")
      {
        row = parent.row() + 1;
        if(parent.parent().isValid())
          newPar = static_cast<TreeItem*>(parent.parent().internalPointer());
        else
          newPar = m_root.get();
      }
      else if(aux->getType() == "FOLDER")
      {
        newPar = aux;
        row = (int)newPar->getChildrenCount("");
      }
      else if(aux->getType() == "LEGEND")
      {
        newPar = aux->getParent()->getParent();
        row = aux->getParent()->getPosition() + 1;
      }
    }
    else
      newPar = m_root.get();
  }
  else // Dropped on a line
    newPar = (parent.isValid()) ? FindParent(static_cast<TreeItem*>(parent.internalPointer())) : m_root.get();

  addItems(*changed.get(), newPar, row);

  return true;
}

void te::qt::widgets::LayerItemModel::removeItems(const QModelIndexList& lst)
{
  if(lst.isEmpty())
    return;

  QModelIndexList idxs = lst;

  CleanRemoveList(idxs);

  qSort(idxs);

  while(!idxs.isEmpty())
  {
    QModelIndex idx = idxs.takeLast();
    QModelIndex par = idx.parent();

    removeRow(idx.row(), par);
  }
}


QVariant te::qt::widgets::LayerItemModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  QModelIndex par = parent(index);

  if(index.row() >= rowCount(par))
    return QVariant();

  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

  if(role == Qt::CheckStateRole)
  {
    if(item->getType() == "LEGEND")
      return QVariant();
    else
      return QVariant((item->isVisible() == NONE) ? Qt::Unchecked : (item->isVisible() == PARTIALLY) ? Qt::PartiallyChecked : Qt::Checked);
  }

  if(role == Qt::DisplayRole)
    return QString::fromStdString(item->getAsString());

  return QVariant();
}

Qt::ItemFlags te::qt::widgets::LayerItemModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags def;

  if(index.isValid())
  {
    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    if(item->getType() == "LAYER" || item->getType() == "FOLDER")
      def = Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
    else
      def = Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
  }
  else
    def = Qt::ItemIsDropEnabled;

  return def;
}

QVariant te::qt::widgets::LayerItemModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return tr("Layer name");

  return QVariant();
}

QModelIndex te::qt::widgets::LayerItemModel::index(int row, int column, const QModelIndex &parent) const
{
  size_t c = m_root->getChildrenCount("");

  if(c == 0)
    return QModelIndex();

  if(!parent.isValid())
  {
    if(static_cast<std::size_t>(row) >= c)
      return QModelIndex();

    // row and column is about a top-level item?
    TreeItem* item = m_root->getChild((int)row);

    return createIndex(row, column, item);
  }

  TreeItem* parentItem = static_cast<TreeItem*>(parent.internalPointer());

  if(parentItem == 0)
    throw te::common::Exception(tr("Invalid data associated to the layer model!").toStdString());

  if(row >= (size_t)parentItem->getChildrenCount(""))
    return QModelIndex();

  TreeItem* item = parentItem->getChild(row);

  if(item == 0)
    throw te::common::Exception(tr("The layer item is not an AbstractTreeItem!").toStdString());

  return createIndex(row, column, item);
}

QModelIndex te::qt::widgets::LayerItemModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
  TreeItem *parentItem = childItem->getParent();

  if (parentItem == m_root.get())
    return QModelIndex();

  return createIndex(parentItem->getPosition(), 0, parentItem);
}

int te::qt::widgets::LayerItemModel::rowCount(const QModelIndex &parent) const
{
  TreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = m_root.get();
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  return ((int) parentItem->getChildrenCount(""));
}
