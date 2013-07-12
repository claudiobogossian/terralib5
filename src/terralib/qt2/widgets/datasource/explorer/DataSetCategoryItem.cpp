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
  \file terralib/qt/widgets/datasource/explorer/DataSetCategoryItem.cpp

  \brief A class used to group a set of dataset items in a DataSourceTreeModel.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess2/Enums.h"
#include "../../../../dataaccess2/dataset/DataSetType.h"
#include "../../../../dataaccess2/datasource/DataSource.h"
//#include "../../../../dataaccess/datasource/DataSourceCatalog.h"
//#include "../../../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../../../dataaccess2/datasource/DataSourceManager.h"
#include "../../../../dataaccess2/datasource/DataSourceInfo.h"
//#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess2/utils/Utils.h"
#include "../../Exception.h"
#include "DataSetCategoryItem.h"
#include "DataSetItem.h"
#include "DataSourceItem.h"

// STL
#include <memory>

// Qt
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>

te::qt::widgets::DataSetCategoryItem::DataSetCategoryItem(const te::da::DataSourceInfoPtr& ds, int category, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_ds(ds),
    m_checked(false),
    m_isInvalid(false),
    m_category(category)
{
}

te::qt::widgets::DataSetCategoryItem::~DataSetCategoryItem()
{
}

int te::qt::widgets::DataSetCategoryItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::DataSetCategoryItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme(te::da::GetDataSetCategoryName(m_category).c_str()));

  if(role == Qt::DisplayRole)
    return QVariant(QString(te::da::GetDataSetCategoryName(m_category).c_str()));

  if(role == Qt::CheckStateRole)
    return QVariant(m_checked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::DataSetCategoryItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenDataSets = m->addAction(tr("&Open datasets"));

  connect(aOpenDataSets, SIGNAL(triggered()), this, SLOT(openDataSets()));

  return m;
}

bool te::qt::widgets::DataSetCategoryItem::canFetchMore() const
{
  if(!children().empty ())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::DataSetCategoryItem::flags() const
{
  return Qt::ItemIsUserCheckable;
}

void te::qt::widgets::DataSetCategoryItem::fetchMore()
{
  if(m_ds == 0)
    return;

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(m_ds->getId(), m_ds->getAccessDriver(), m_ds->getConnInfo());

  if(ds.get() == 0)
    return;

  boost::ptr_vector<std::string> datasets;

  te::da::GetDataSets(datasets, ds.get());

  const std::size_t ndatasets = datasets.size();

  for(std::size_t i = 0; i < ndatasets; ++i)
  {
    te::da::DataSetTypePtr dt(new te::da::DataSetType(datasets[i]));

    if(dt->getCategory() == m_category)
      new DataSetItem(dt, ds.get(), this);
  }
}

bool te::qt::widgets::DataSetCategoryItem::hasChildren() const
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

    return te::da::HasDataSet(ds.get());
  }
  catch(...)
  {
    m_isInvalid = true;
    throw;
  }
}

bool te::qt::widgets::DataSetCategoryItem::setData(const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    m_checked = value.toBool();
    return true;
  }

  return false;
}

bool te::qt::widgets::DataSetCategoryItem::isChecked() const
{
  return m_checked;
}

