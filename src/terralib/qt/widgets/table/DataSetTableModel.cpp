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
#include "../../../datatype/Enums.h"
#include "../../../datatype/SimpleData.h"
#include "../../../memory/DataSet.h"
#include "../../../memory/DataSetItem.h"
#include "../Utils.h"
#include "DataSetTableModel.h"
#include "Promoter.h"

// Qt
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>

bool IsPkey(const int& column, const std::vector<size_t>& pkeys)
{
  std::vector<size_t>::const_iterator it;

  for(it=pkeys.begin(); it!=pkeys.end(); ++it)
    if(*it == column)
      return true;

  return false;
}

QString ColumnType(const int& type)
{

  switch(type)
  {
    case te::dt::VOID_TYPE:
      return QObject::tr("Void");
    break;

    case te::dt::BIT_TYPE:
      return QObject::tr("Bit");
    break;

    case te::dt::CHAR_TYPE:
      return QObject::tr("Char");
    break;

    case te::dt::UCHAR_TYPE:
      return QObject::tr("Unsigned char");
    break;

    case te::dt::INT16_TYPE:
    case te::dt::INT32_TYPE:
    case te::dt::INT64_TYPE:
      return QObject::tr("Integer");
    break;

    case te::dt::UINT16_TYPE:
    case te::dt::UINT32_TYPE:
    case te::dt::UINT64_TYPE:
      return QObject::tr("Unsigned integer");
    break;

    case te::dt::BOOLEAN_TYPE:
      return QObject::tr("Boolean");
    break;

    case te::dt::FLOAT_TYPE:
      return QObject::tr("Float");
    break;

    case te::dt::DOUBLE_TYPE:
      return QObject::tr("Double");
    break;

    case te::dt::NUMERIC_TYPE:
      return QObject::tr("Numeric");
    break;

    case te::dt::STRING_TYPE:
      return QObject::tr("String");
    break;

    case te::dt::BYTE_ARRAY_TYPE:
      return QObject::tr("Byte array");
    break;

    case te::dt::GEOMETRY_TYPE:
      return QObject::tr("Geometry");
    break;

    case te::dt::DATETIME_TYPE:
      return QObject::tr("Date time");
    break;

    default:
      return QObject::tr("Unknown");
    break;
  }
}

class Editor
{
  public:
    Editor()
    {

    }

    void clear()
    {
      m_editions.clear();
    }

    void setValue(const int& row, const int& column, const std::string& value)
    {
      std::pair<int, int> key(row, column);
      m_editions[key] = value;
    }

    bool isEdited(const int& row, const int& column) const
    {
      return m_editions.find(std::pair<int, int>(row, column)) != m_editions.end();
    }

    std::string getValue(const int& row, const int& column)
    {
      return m_editions[std::pair<int, int>(row, column)];
    }

    bool hasEditions() const
    {
      return !m_editions.empty();
    }

    void getEditedDataSet(te::da::DataSet* in, te::mem::DataSet* out, std::vector< std::set<int> >& fields)
    {
      std::vector<int> rows;

      fields = getEditedLists(rows);

      // Creating the items
      for(int i = 0; i < (int)rows.size(); ++i)
      {
        te::mem::DataSetItem* item = new te::mem::DataSetItem(out);
        std::set<int> ef = fields[i];

        in->move(rows[i]);

        // For each property
        for(int j = 0; j < (int) in->getNumProperties(); ++j)
        {
          bool edited = ef.find(j) != ef.end();

          if(!edited)
            item->setValue(j, in->getValue(j).release());
          else
          {
            std::string data = getValue(rows[i], j);

            switch(in->getPropertyDataType(j))
            {
              case te::dt::INT16_TYPE:
                 item->setValue(j, new te::dt::Int16(atoi(data.c_str())));
              break;

              case te::dt::INT32_TYPE:
                 item->setValue(j, new te::dt::Int32(atoi(data.c_str())));
              break;

              case te::dt::INT64_TYPE:
                 item->setValue(j, new te::dt::Int64(atoi(data.c_str())));
              break;

              case te::dt::FLOAT_TYPE:
                 item->setValue(j, new te::dt::Float(atof(data.c_str())));
              break;

              case te::dt::DOUBLE_TYPE:
                 item->setValue(j, new te::dt::Double(atof(data.c_str())));
              break;

              case te::dt::NUMERIC_TYPE:
                 item->setValue(j, new te::dt::Numeric(data));
              break;

              case te::dt::STRING_TYPE:
                 item->setValue(j, new te::dt::String(data));
              break;

              default:
                break;
            }
          }
        }

        out->add(item);
      }
    }

  protected:

    std::vector< std::set<int> > getEditedLists(std::vector<int>& rows)
    {
      std::vector< std::set<int> > res;

      if(!m_editions.empty())
      {
        std::set<int> subRes;
        int current = m_editions.begin()->first.first;
        rows.push_back(current);
        std::map< std::pair<int, int>, std::string >::iterator it;

        for(it=m_editions.begin(); it!= m_editions.end(); ++it)
        {
          int cR = it->first.first;

          if(cR != current)
          {
            rows.push_back(cR);
            res.push_back(subRes);
            subRes.clear();
            current = cR;
          }

          subRes.insert(it->first.second);
        }

        if(rows.size() != res.size())
          res.push_back(subRes);
      }

      return res;
    }

    std::map< std::pair<int, int>, std::string > m_editions;
};

te::qt::widgets::DataSetTableModel::DataSetTableModel (QObject* parent)
  : QAbstractTableModel(parent),
    m_dataset(0),
    m_currentRow(-1),
    m_OIdsVisible(true),
    m_enabled(true)
{
  m_promoter = new Promoter;
  m_editor.reset(new Editor);
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

  m_editor->clear();

  m_rowCount = (m_dataset == 0 || !m_enabled) ? 0 : (int)m_dataset->size();

  endResetModel();
}

void te::qt::widgets::DataSetTableModel::setPkeysColumns(const std::vector<size_t>& pkeys)
{
  m_pkeysColumns = pkeys;
}

void te::qt::widgets::DataSetTableModel::promote(const te::da::ObjectIdSet* oids)
{
  beginResetModel();

  m_promoter->promote(oids);

  endResetModel();
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
    row = (int)m_promoter->getLogicalRow(i);
    m_dataset->move(row);

    oids->add(te::da::GenerateOID(m_dataset, oids->getPropertyNames()));
  }

  return oids;
}

void te::qt::widgets::DataSetTableModel::setEnabled(const bool& enabled)
{
  beginResetModel();

  m_enabled = enabled;

  m_rowCount = (m_enabled && m_dataset != 0) ? (int)m_dataset->size() : 0;

  endResetModel();
}

void te::qt::widgets::DataSetTableModel::setEditable(const bool &editable)
{
  m_isEditable = editable;
}

std::auto_ptr<te::da::DataSet> te::qt::widgets::DataSetTableModel::getEditions(const te::da::DataSetType *type, std::vector< std::set<int> > &ps)
{
  std::auto_ptr<te::da::DataSet> dset;
  te::mem::DataSet* md = new te::mem::DataSet(type);

  m_editor->getEditedDataSet(m_dataset, md, ps);

  dset.reset(md);

  return dset;
}

bool te::qt::widgets::DataSetTableModel::hasEditions() const
{
  return m_editor->hasEditions();
}

void te::qt::widgets::DataSetTableModel::discardEditions()
{
  m_editor->clear();
}

int te::qt::widgets::DataSetTableModel::rowCount(const QModelIndex & parent) const
{
  return m_rowCount;
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
    {
      if(m_currentRow != index.row())
      {
        m_currentRow = index.row();

        int row = (int)m_promoter->getLogicalRow(m_currentRow);
        m_dataset->move(row);
      }

      if(!m_dataset->isNull(index.column()))
      {
        if(m_editor->isEdited(m_promoter->getLogicalRow(index.row()), index.column()))
          return m_editor->getValue(m_promoter->getLogicalRow(index.row()), index.column()).c_str();

        if(m_dataset->getPropertyDataType(index.column()) == te::dt::STRING_TYPE)
        {
          std::string value = m_dataset->getString(index.column());
          te::common::CharEncoding encoding = m_dataset->getPropertyCharEncoding(index.column());
          if (encoding == te::common::UNKNOWN_CHAR_ENCODING)
            return value.c_str();
          else
			      return Convert2Qt(value, encoding);
        }
        else
          return m_dataset->getAsString(index.column(), 6).c_str();
      }
    }
    break;

    case Qt::FontRole:
      if(m_editor->isEdited(m_promoter->getLogicalRow(index.row()), index.column()))
      {
        QFont f;
        f.setBold(true);
        f.setItalic(true);
        return f;
      }
    break;

    case Qt::EditRole:
      return data(index, Qt::DisplayRole);
    break;

    default:
      break;
  }

  return QVariant();
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

      case Qt::ToolTipRole:
       return m_dataset->getPropertyName(section).c_str() + QString(" : ") + ColumnType(m_dataset->getPropertyDataType(section));
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

  flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  if(m_isEditable && !IsPkey(index.column(), m_pkeysColumns))
    flags |= Qt::ItemIsEditable;

  return flags;
}

bool te::qt::widgets::DataSetTableModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
  if(role == Qt::EditRole)
  {
    try
    {
      switch(m_dataset->getPropertyDataType(index.column()))
      {
        case te::dt::INT16_TYPE:
        case te::dt::UINT16_TYPE:
        case te::dt::INT32_TYPE:
        case te::dt::UINT32_TYPE:
        case te::dt::INT64_TYPE:
        case te::dt::UINT64_TYPE:
        {
          bool ok;
          value.toInt(&ok);

          if(!ok)
            throw te::common::Exception(tr("Invalid int value.").toStdString());
        }
        break;

        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
        case te::dt::NUMERIC_TYPE:
        {
          bool ok;
          value.toDouble(&ok);

          if(!ok)
            throw te::common::Exception(tr("Invalid double value.").toStdString());
        }
        break;
      }

      QString curV = data(index, Qt::DisplayRole).toString();
      QString newV = value.toString();

      if(curV != newV)
        m_editor->setValue(m_promoter->getLogicalRow(index.row()), index.column(), value.toString().toStdString());

      return true;
    }
    catch(te::common::Exception& e)
    {
      QMessageBox::warning(0, tr("Edition Failure"), e.what());
      return false;
    }
  }

  return true;
}

bool te::qt::widgets::DataSetTableModel::insertColumns(int column, int count, const QModelIndex& parent)
{
  beginInsertColumns(parent, column, column+count);

  endInsertColumns();

  return true;
}

bool te::qt::widgets::DataSetTableModel::removeColumns(int column, int count, const QModelIndex& parent)
{
  beginRemoveColumns(parent, column, column+count);

  endRemoveColumns();

  return true;
}
