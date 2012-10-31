#include "DataSetModel.h"

//TerraLib include files
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/maptools/AbstractTable.h>

//Qt4 include files
#include <QModelIndex>
#include <QObject>
#include <QIcon>

bool isPkey(int col, const std::vector<size_t>& keys)
{
  std::vector<size_t>::const_iterator it;

  for(it=keys.begin(); it!=keys.end(); ++it)
    if(col == (int)*it)
      return true;

  return false;
}

namespace te 
{
  namespace qt 
  {
    namespace widgets 
    {
      DataSetModel::DataSetModel(te::map::AbstractTable * table, QObject * parent) :
          QAbstractTableModel(parent),
          m_table(table)
      {
      }

      DataSetModel::~DataSetModel()
      {
        delete m_table;
      }

      int DataSetModel::rowCount(const QModelIndex & parent) const
      {
        return (int) m_table->numRows();
      }

      int DataSetModel::columnCount(const QModelIndex & parent) const
      {
        return (int)m_table->numColumns();
      }

      QVariant DataSetModel::data(const QModelIndex & index, int role) const
      {
        if(!index.isValid())
          return QVariant();

        switch(role)
        {
        case Qt::TextAlignmentRole:
          return int(Qt::AlignCenter);
          break;

        case Qt::DisplayRole:
          return m_table->dataAsString(index.row(), index.column()).c_str();
          break;

        case PKEY:
          return m_table->map2PKey(index.row()).c_str();
          break;

        case GEOMETRY:
          return QVariant::fromValue<void*>((void*) m_table->getGeometry(index.row()));
          break;

        default:
          return QVariant();
        }
      }

      QVariant DataSetModel::headerData(int section, Qt::Orientation orientation, int role) const
      {
        if(orientation == Qt::Horizontal)
        {
          switch(role)
          {
            case Qt::DisplayRole:
              return m_table->getColumnName(section).c_str();
            break;

            case Qt::DecorationRole:
              return (isPkey(section, m_table->getPKeysColumns())) ?
                QIcon::fromTheme("key") :
                QVariant();
            break;

            default:
              return QAbstractTableModel::headerData(section, orientation, role);
            break;
          }
        }
        else
        {
          switch(role)
          {
            case Qt::DisplayRole:
              return section+1;
            break;

            default:
            break;
          }
        }

        return QVariant();
      }

      Qt::ItemFlags DataSetModel::flags(const QModelIndex & index) const
      {
        if (index.isValid() == false)
          return 0;

        Qt::ItemFlags flags = QAbstractItemModel::flags(index);
        flags = flags | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

        return flags;
      }
    }
  }
}
