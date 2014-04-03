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
  \file terralib/qt/widgets/datasource/explorer/DataSetItem.cpp

  \brief A class that represents a dataset in a TreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/Enums.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceCatalog.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../datasource/explorer/DataSetGroupItem.h"
#include "../../Exception.h"
#include "ConstraintsItem.h"
#include "DataSetItem.h"
#include "IndexGroupItem.h"
#include "PropertyGroupItem.h"

// STL
#include <memory>

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::DataSetItem::DataSetItem(const te::da::DataSetTypePtr& dt, te::da::DataSource* ds, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_dataset(dt),
    m_datasource(ds),
    m_checked(false)
{
  if(ds == 0 && dt->getCatalog())
    m_datasource = dt->getCatalog()->getDataSource();
}

te::qt::widgets::DataSetItem::~DataSetItem()
{
}

int te::qt::widgets::DataSetItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::DataSetItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
  {
    if((m_dataset->getCategory() == te::da::TABLE_TYPE) && m_dataset->hasGeom())
      return QVariant(QIcon::fromTheme("dataset-geotable"));
    else if((m_dataset->getCategory() == te::da::TABLE_TYPE) && !m_dataset->hasGeom())
      return QVariant(QIcon::fromTheme("dataset-table"));
    else if((m_dataset->getCategory() == te::da::VIEW_TYPE) && m_dataset->hasGeom())
      return QVariant(QIcon::fromTheme("dataset-geoview"));
    else if((m_dataset->getCategory() == te::da::VIEW_TYPE) && !m_dataset->hasGeom())
      return QVariant(QIcon::fromTheme("dataset-view"));
    else if((m_dataset->getCategory() == te::da::REGULAR_FILE_TYPE) && m_dataset->hasGeom())
      return QVariant(QIcon::fromTheme("dataset-geofile"));
    else if((m_dataset->getCategory() == te::da::REGULAR_FILE_TYPE) && !m_dataset->hasGeom())
      return QVariant(QIcon::fromTheme("dataset-file"));
    else
      return QVariant(QIcon::fromTheme("dataset"));
  }

  if(role == Qt::DisplayRole)
  {
    if(!m_dataset->getTitle().empty())
      return QVariant(m_dataset->getTitle().c_str());
    else
      return QVariant(m_dataset->getName().c_str());
  }

  if(role == Qt::CheckStateRole)
    return QVariant(m_checked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::DataSetItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenDataSet = m->addAction(tr("&Open dataset"));

  connect(aOpenDataSet, SIGNAL(triggered()), this, SLOT(openDataSet()));

  return m;
}

bool te::qt::widgets::DataSetItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::DataSetItem::flags() const
{
  return Qt::ItemIsUserCheckable;
}

void te::qt::widgets::DataSetItem::fetchMore()
{
  if(m_dataset.get() == 0)
    return;

  //if(!m_dataset->isFullLoaded() && (m_datasource != 0))
  //  te::da::LoadFull(m_dataset.get(), m_datasource);

  if(m_dataset->size() != 0)
    new PropertyGroupItem(this);

  if((!m_dataset->getNumberOfCheckConstraints() != 0) ||
      (!m_dataset->getNumberOfUniqueKeys() != 0) ||
      (!m_dataset->getPrimaryKey() != 0))
    new ConstraintsItem(this);

  if(m_dataset->getNumberOfIndexes() != 0)
    new IndexGroupItem(this);
}

bool te::qt::widgets::DataSetItem::hasChildren() const
{
  if(m_dataset.get() == 0)
    return false;

  if(m_dataset->size() != 0)
    return true;

  if(m_datasource == 0)
    return false;

  return true;  // assume at least one property will exist! otherwise we need to use catalog loader!
}

bool te::qt::widgets::DataSetItem::setData(const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    m_checked = value.toBool();

    DataSetGroupItem* p = dynamic_cast<DataSetGroupItem*>(parent());

    p->checkState();

    return true;
  }

  return false;
}

const te::da::DataSetTypePtr& te::qt::widgets::DataSetItem::getDataSet() const
{
  return m_dataset;
}

te::da::DataSource* te::qt::widgets::DataSetItem::getDataSource() const
{
  return m_datasource;
}

bool te::qt::widgets::DataSetItem::isChecked() const
{
  return m_checked;
}
