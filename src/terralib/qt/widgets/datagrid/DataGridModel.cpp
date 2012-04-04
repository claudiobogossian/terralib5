/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file DataGridModel.cpp

  \brief It provides a standard interface for a model that represents their data as a data grid.
 */

// TerraLib
#include "../../../dataaccess.h"
#include "../../../maptools/DataGridOperation.h"

#include "DataGridModel.h"

te::qt::widgets::DataGridModel::DataGridModel(te::da::DataSetType* dt, te::da::DataSet* dataSet, 
                                              te::map::DataGridOperation* dataGridOp, QObject* parent)
  : m_dataSetType(dt), m_dataSet(dataSet), m_dataGridOp(dataGridOp), QAbstractTableModel(parent)
{
}

te::qt::widgets::DataGridModel::DataGridModel(te::map::DataGridOperation* dataGridOp, QObject* parent)
  : m_dataSetType(0), m_dataSet(0), m_dataGridOp(dataGridOp), QAbstractTableModel(parent)
{
}

void te::qt::widgets::DataGridModel::init(te::da::DataSetType* dt, te::da::DataSet* dataSet)
{
  m_dataSetType = dt;
  m_dataSet = dataSet;

  resetModel();
}

te::qt::widgets::DataGridModel::~DataGridModel()
{
}

int te::qt::widgets::DataGridModel::rowCount(const QModelIndex& /*parent*/) const
{
  if(m_dataSet)
    return m_dataSet->size();

  return 0;
}

int te::qt::widgets::DataGridModel::columnCount(const QModelIndex& /*parent*/) const
{
  if(m_dataSetType)
    return m_dataSetType->size();

  return 0;
}

QVariant te::qt::widgets::DataGridModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid() == false)
    return QVariant();

  int row = index.row();
  int column = index.column();

  // Get the logical row and the logical column
  int logicalRow;
  int logicalColumn;

  if(m_dataGridOp)
  {
    logicalRow = m_dataGridOp->getLogicalRow(row);
    logicalColumn = m_dataGridOp->getLogicalColumn(column);
  }
  else
  {
    logicalRow = row;
    logicalColumn = column;
  }

  if (role == Qt::TextAlignmentRole)
  {
    return int(Qt::AlignRight || Qt::AlignVCenter);
  }
  else if (role == Qt::DisplayRole)
  {
    std::string value;

    if(m_dataSet)
    {
      m_dataSet->move(logicalRow);
      if(m_dataSet->isNull(logicalColumn) == false) //Lauro
        value = m_dataSet->getAsString(logicalColumn);
    }

    return value.c_str();
  }

  return QVariant();
}

bool te::qt::widgets::DataGridModel::setData(const QModelIndex& index, const QVariant& /*v*/, int /*role*/)
{
  if (index.isValid() == false)
    return false;

  return false;
}

QVariant te::qt::widgets::DataGridModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal)
  {
    int logicalColumn;

    if(m_dataGridOp)
      logicalColumn = m_dataGridOp->getLogicalColumn(section);
    else
      logicalColumn = section;

    if (m_dataSetType && role == Qt::DisplayRole)
      return m_dataSetType->getProperty(logicalColumn)->getName().c_str();
  }

  return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags te::qt::widgets::DataGridModel::flags(const QModelIndex &index) const
{
  if (index.isValid() == false)
    return 0;

  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags = flags | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

  return flags;
}

void te::qt::widgets::DataGridModel::resetModel()
{
  reset();
}


