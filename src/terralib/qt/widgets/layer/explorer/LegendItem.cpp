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
#include "../../../../geometry/Point.h"
#include "../../../../geometry/Polygon.h"
#include "../../../../geometry/LinearRing.h"
#include "../../../../geometry/LineString.h"
#include "../../../../se/Description.h"
#include "../../../../se/Rule.h"
#include "../../se/SymbologyPreview.h"
#include "../../Exception.h"
#include "LegendItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QPixmap>
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
  if(role == Qt::DecorationRole)
  {
    /*te::gm::Polygon pol(1, te::gm::PolygonType);

    te::gm::LinearRing* ring = new te::gm::LinearRing(6, te::gm::LineStringType);

    ring->setPoint(0, 2, 2);
    ring->setPoint(1, 2, 8);
    ring->setPoint(2, 8, 14);
    ring->setPoint(3, 14, 8);
    ring->setPoint(4, 14, 2);
    ring->setPoint(5, 2, 2);

    pol.setRingN(0, ring);

    return QVariant(QIcon(SymbologyPreview::build(m_rule, &pol, QSize(16, 16))));*/

    return QVariant(QIcon(SymbologyPreview::build(m_rule, QSize(16, 16))));
  }

  if(role == Qt::DisplayRole)
  {
    if(m_rule->getDescription())
      return QVariant(QString::fromStdString(m_rule->getDescription()->getTitle()));

    if(m_rule->getName())
      return QVariant(QString::fromStdString(*(m_rule->getName())));

    return QVariant(QString(tr("No Description")));
  }

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
  return Qt::ItemIsEnabled;
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

te::map::AbstractLayerPtr te::qt::widgets::LegendItem::getLayer() const
{
  return te::map::AbstractLayerPtr(0);
}

//te::qt::widgets::AbstractLayerTreeItem* te::qt::widgets::LegendItem::clone(QObject* parent)
//{
//  return new LegendItem(m_rule, parent);
//}

