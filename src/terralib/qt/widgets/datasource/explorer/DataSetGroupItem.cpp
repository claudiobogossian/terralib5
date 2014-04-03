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
  \file terralib/qt/widgets/datasource/explorer/DataSetGroupItem.cpp

  \brief A class used to group a set of dataset items in a DataSourceTreeModel.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSource.h"
//#include "../../../../dataaccess/datasource/DataSourceCatalog.h"
//#include "../../../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
//#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../Exception.h"
#include "DataSetGroupItem.h"
#include "DataSetItem.h"
#include "DataSourceItem.h"

// STL
#include <memory>

// Qt
#include <QAbstractItemModel>
#include <QMenu>
#include <QMessageBox>
#include <QWidget>

te::qt::widgets::DataSetGroupItem::DataSetGroupItem(const te::da::DataSourceInfoPtr& ds, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_ds(ds),
    m_checked(false),
    m_isInvalid(false)
{
}

te::qt::widgets::DataSetGroupItem::~DataSetGroupItem()
{
}

int te::qt::widgets::DataSetGroupItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::DataSetGroupItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("datasets"));

  if(role == Qt::DisplayRole)
    return QVariant(QString("datasets"));

  if(role == Qt::CheckStateRole)
    return QVariant(m_checked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::DataSetGroupItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenDataSets = m->addAction(tr("&Open datasets"));

  connect(aOpenDataSets, SIGNAL(triggered()), this, SLOT(openDataSets()));

  return m;
}

bool te::qt::widgets::DataSetGroupItem::canFetchMore() const
{
  if(!children().empty ())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::DataSetGroupItem::flags() const
{
  return Qt::ItemIsUserCheckable;
}

void te::qt::widgets::DataSetGroupItem::fetchMore()
{
  if(m_ds == 0)
    return;

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(m_ds->getId(), m_ds->getAccessDriver(), m_ds->getConnInfo());

  if(ds.get() == 0)
    return;

  std::vector<std::string> datasetNames = ds->getDataSetNames();

  const std::size_t ndatasets = datasetNames.size();

  for(std::size_t i = 0; i < ndatasets; ++i)
  {
    te::da::DataSetTypePtr dt(new te::da::DataSetType(datasetNames[i].c_str()));

    m_items.push_back(new DataSetItem(dt, ds.get(), this));
  }

  if(m_items.size() == 1)
  {
    m_items[0]->setData(true, Qt::CheckStateRole);
  }
}

bool te::qt::widgets::DataSetGroupItem::hasChildren() const
{
  if(m_isInvalid)
    return false;

  if(m_ds.get() == 0)
    return false;

  try
  {
    te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(m_ds->getId(), m_ds->getAccessDriver(), m_ds->getConnInfo());

    if(ds.get() == 0)
      return false;

    return ds->hasDataSets();
  }
  catch(...)
  {
    m_isInvalid = true;
    throw;
  }
}

bool te::qt::widgets::DataSetGroupItem::setData(const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    m_checked = value.toBool();

    for(std::size_t i = 0; i < m_items.size(); ++i)
    {
      m_items[i]->setData(value, role);
    }
    return true;
  }

  return false;
}

bool te::qt::widgets::DataSetGroupItem::isChecked() const
{
  return m_checked;
}

void te::qt::widgets::DataSetGroupItem::checkState()
{
  bool check = false;

  for(std::size_t i = 0; i < m_items.size(); ++i)
  {
    if(m_items[i]->isChecked())
    {
      check = true;
      break;
    }
  }

  m_checked = check;
}

const std::vector<te::qt::widgets::DataSetItem*>&  te::qt::widgets::DataSetGroupItem::getDataSetItems() const
{
  return m_items;
}
