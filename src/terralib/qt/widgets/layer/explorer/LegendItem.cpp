/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/explorer/LegendItem.cpp

  \brief ???
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../se/Rule.h"
#include "../../Exception.h"
#include "LegendItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::LegendItem::LegendItem(const te::se::Rule* rule, QObject* parent)
  : AbstractLayerTreeItem(parent),
    m_rule(rule)
{
}

te::qt::widgets::LegendItem::~LegendItem()
{
}

int te::qt::widgets::LegendItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::LegendItem::data(int /*column*/, int role) const
{
  //if(role == Qt::DecorationRole)
  //  return QVariant(QIcon::fromTheme("layer"));

  //if(role == Qt::DisplayRole)
  //  return QVariant(QString::fromUtf8(m_layer->getTitle().c_str()));

  //if(role == Qt::CheckStateRole)
  //  return QVariant(m_layer->getVisibility() == te::map::VISIBLE ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::LegendItem::getMenu(QWidget* /*parent*/) const
{
  //QMenu* m = new QMenu(parent);

  //QAction* aOpenDataSource = m->addAction(tr("&Open data source"));

  //connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  //return m;
  return 0;
}

bool te::qt::widgets::LegendItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::qt::widgets::LegendItem::flags() const
{
  return Qt::ItemIsUserCheckable;
}

void te::qt::widgets::LegendItem::fetchMore()
{
}

bool te::qt::widgets::LegendItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::LegendItem::setData(const QVariant& value, int role)
{
  //if(role == Qt::CheckStateRole)
  //{
  //  bool vis = value.toBool();

  //  m_layer->setVisibility(vis ? te::map::VISIBLE : te::map::NOT_VISIBLE);
  //  return true;
  //}

  return false;
}

