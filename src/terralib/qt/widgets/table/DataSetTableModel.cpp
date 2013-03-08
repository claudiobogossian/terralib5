/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSetTableModel.cpp

  \brief A table model for a dataset.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../maptools/DataSetTable.h"
#include "DataSetTableModel.h"

te::qt::widgets::DataSetTableModel::DataSetTableModel(te::da::DataSet* dataset, QObject* parent)
  : QAbstractTableModel(parent),
    m_table(0)
{
  m_table.reset(new te::map::DataSetTable(dataset));
}

te::qt::widgets::DataSetTableModel::~DataSetTableModel()
{
}

int te::qt::widgets::DataSetTableModel::columnCount(const QModelIndex& /*parent*/) const
{
  return m_table->numColumns();
}

QVariant te::qt::widgets::DataSetTableModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
    return QVariant();

  if(role != Qt::DisplayRole)
    return QVariant();

  int row = index.row();

  int col = index.column();

  QString s(m_table->dataAsString(row, col).c_str());

  if(s.isEmpty())
    return QVariant();

  return QVariant(s);
}

QVariant te::qt::widgets::DataSetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role != Qt::DisplayRole)
    return QVariant();

  if(orientation == Qt::Horizontal)
  {
    std::string colName = m_table->getColumnName(section);

    return QVariant(colName.c_str());
  }
  else  // if(orientation == Qt::Vertical)
  {
    return QVariant(section);
  }
}

int te::qt::widgets::DataSetTableModel::rowCount(const QModelIndex& /*parent*/) const
{
  return m_table->numRows();
}

void te::qt::widgets::DataSetTableModel::setDataSet(te::da::DataSet* dataset)
{
  beginResetModel();
  m_table.reset(new te::map::DataSetTable(dataset));
  endResetModel();
}

