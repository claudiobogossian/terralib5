#include "DataSetTableModel.h"

// TerraLib
#include "../../../common/Exception.h"
#include "../../../dataaccess/dataset/DataSet.h"

void MoveDataSet(te::da::DataSet* dset, const int& currentRow, const int& nextRow)
{
  if(currentRow != nextRow)
  {
    if(nextRow == (currentRow+1))
    {
      if(!dset->moveNext())
        throw te::common::Exception("Fail to move next on data set.");
    }
    else if(!dset->move(nextRow))
      throw te::common::Exception("Fail to move dataSet.");
  }
}

te::qt::widgets::DataSetTableModel::DataSetTableModel (QObject* parent) :
QAbstractTableModel(parent),
m_dataset(0),
m_currentRow(-1)
{
}

te::qt::widgets::DataSetTableModel::~DataSetTableModel()
{
  delete m_dataset;
}

void te::qt::widgets::DataSetTableModel::setDataSet(te::da::DataSet* dset)
{
  beginResetModel();

  delete m_dataset;

  m_dataset = dset;

  endResetModel();
}

int te::qt::widgets::DataSetTableModel::rowCount(const QModelIndex & parent) const
{
  return (m_dataset == 0) ? 0 : (int)m_dataset->size();
}

int te::qt::widgets::DataSetTableModel::columnCount(const QModelIndex & parent) const
{
  return (m_dataset == 0) ? 0 : (int) m_dataset->getNumProperties();
}

QVariant te::qt::widgets::DataSetTableModel::data(const QModelIndex & index, int role) const
{
  if(!index.isValid())
    return QVariant();

  switch (role)
  {
    case Qt::TextAlignmentRole:
      return (int)(Qt::AlignCenter);
    break;

    case Qt::DisplayRole:
      MoveDataSet(m_dataset, m_currentRow, index.row());
      m_currentRow = index.row();
      return m_dataset->getAsString(index.column()).c_str();
    break;

    default:
      return QVariant();
    break;
  }
}

QVariant te::qt::widgets::DataSetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(orientation == Qt::Horizontal)
  {
    switch(role)
    {
      case Qt::DisplayRole:
        return m_dataset->getPropertyName(section).c_str();
      break;

      default:
        return QAbstractTableModel::headerData(section, orientation, role);
      break;
    }
  }
  else
  {
    switch (role)
    {
      case Qt::DisplayRole:
        return section+1;
      break;

      default:
        return QAbstractTableModel::headerData(section, orientation, role);
      break;
    }
  }

  return QVariant();
}

Qt::ItemFlags te::qt::widgets::DataSetTableModel::flags(const QModelIndex & index) const
{
  if (index.isValid() == false)
    return 0;

  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

  return flags;
}

bool te::qt::widgets::DataSetTableModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
  return true;
}
