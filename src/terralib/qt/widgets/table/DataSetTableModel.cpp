/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/table/DataSetTableModel.cpp

  \brief A model based on te::da::DataSet.
*/

// TerraLib
#include "../../../common/Exception.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "DataSetTableModel.h"

// Qt
#include <QtGui/QIcon>

bool IsPkey(const int& column, const std::vector<size_t>& pkeys)
{
  std::vector<size_t>::const_iterator it;

  for(it=pkeys.begin(); it!=pkeys.end(); ++it)
    if(*it == column)
      return true;

  return false;
}

te::qt::widgets::DataSetTableModel::DataSetTableModel (QObject* parent)
  : QAbstractTableModel(parent),
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

void te::qt::widgets::DataSetTableModel::setPkeysColumns(const std::vector<size_t>& pkeys)
{
  m_pkeysColumns = pkeys;
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
      if(m_currentRow != index.row())
      {
        m_currentRow = index.row();
        m_dataset->move(m_currentRow);
      }
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

      case Qt::DecorationRole:
        return (IsPkey(section, m_pkeysColumns)) ?
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

bool te::qt::widgets::DataSetTableModel::setData (const QModelIndex & /*index*/, const QVariant & /*value*/, int /*role*/)
{
  return true;
}
