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
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"

#include "DataSetTableModel.h"
#include "Promoter.h"

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
    m_currentRow(-1),
    m_promoter(0),
    m_OIdsVisible(false)
{
}

te::qt::widgets::DataSetTableModel::~DataSetTableModel()
{
  delete m_dataset;
  delete m_promoter;
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

void te::qt::widgets::DataSetTableModel::setPromotionEnable(const bool& enable)
{
  if(enable)
  {
    if(m_promoter == 0)
      m_promoter = new Promoter;

    m_promoter->preProcessKeys(m_dataset, m_pkeysColumns);
  }
  else
  {
    delete m_promoter;
    m_promoter = 0;
  }
}

void te::qt::widgets::DataSetTableModel::promote(const te::da::ObjectIdSet* oids)
{
  setPromotionEnable(true);

  m_promoter->promote(oids);
}

void te::qt::widgets::DataSetTableModel::orderByColumns(const std::vector<int>& cols)
{
  if(m_promoter == 0)
    m_promoter = new Promoter;

  m_promoter->sort(m_dataset, cols);

  reset();
}

bool te::qt::widgets::DataSetTableModel::isPromotionEnabled()
{
  return m_promoter != 0 && m_promoter->isPromotionEnabled();
}

te::qt::widgets::Promoter* te::qt::widgets::DataSetTableModel::getPromoter()
{
  return m_promoter;
}

void te::qt::widgets::DataSetTableModel::showOIdsVisible(const bool& visible)
{
  m_OIdsVisible = visible;
}

te::da::ObjectIdSet* te::qt::widgets::DataSetTableModel::getObjectIdSet (const int& initRow, const int& finalRow)
{
  te::da::ObjectIdSet* oids = new te::da::ObjectIdSet;

  // Mounting oidset
  std::vector<size_t>::iterator it;

  for(it=m_pkeysColumns.begin(); it!=m_pkeysColumns.end(); ++it)
    oids->addProperty(m_dataset->getPropertyName(*it), *it, m_dataset->getPropertyDataType(*it));

  // Loading oid set.
  int row;

  for(int i=initRow; i<=finalRow; i++)
  {
    row = (m_promoter == 0) ? i : m_promoter->getLogicalRow(i);
    m_dataset->move(row);

    oids->add(te::da::GenerateOID(m_dataset, oids->getPropertyNames()));
  }

  return oids;
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

        int row = (m_promoter == 0) ? m_currentRow : (int)m_promoter->getLogicalRow(m_currentRow);
        m_dataset->move(row);
      }

      return (m_dataset->isNull(index.column())) ? 
        tr("Null") : 
        m_dataset->getAsString(index.column()).c_str();

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
        return (m_OIdsVisible && IsPkey(section, m_pkeysColumns)) ?
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
