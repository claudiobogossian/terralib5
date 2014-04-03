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
#include "../../../../se/Description.h"
#include "../../../../se/Rule.h"
#include "../../se/SymbologyPreview.h"
#include "../../Exception.h"
#include "LegendItem.h"

// Qt
#include <QMenu>
#include <QPixmap>
#include <QWidget>

te::qt::widgets::LegendItem::LegendItem(const te::se::Rule* rule, QObject* parent)
  : AbstractTreeItem(parent),
    m_rule(rule),
    m_isCheckable(false),
    m_isChecked(false)
{
}

te::qt::widgets::LegendItem::~LegendItem()
{
}

int te::qt::widgets::LegendItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::LegendItem::data(int column, int role) const
{
  if(role == Qt::DecorationRole && column == 0)
  {
    return QVariant(QIcon(SymbologyPreview::build(m_rule, QSize(16, 16))));
  }

  if(role == Qt::DisplayRole)
  {
    if(column == 0)
    {
      if(m_rule->getDescription())
        return QVariant(QString::fromStdString(m_rule->getDescription()->getTitle()));

      if(m_rule->getName())
        return QVariant(QString::fromStdString(*(m_rule->getName())));

      return QVariant(QString(tr("Style")));
    }
  }

  if(role == Qt::CheckStateRole && m_isCheckable && column == 0)
    return QVariant(m_isChecked ? Qt::Checked : Qt::Unchecked);

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
  return (m_isCheckable ? (Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) : Qt::ItemIsEnabled);
}

void te::qt::widgets::LegendItem::fetchMore()
{
}

bool te::qt::widgets::LegendItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::LegendItem::setData(int column, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole && m_isCheckable)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));
    
    if(!ok)
      return false;

    m_isChecked = (checkState == Qt::Checked ? true : false);
  }

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

const std::string te::qt::widgets::LegendItem::getItemType() const
{
  return "LEGEND_ITEM";
}

//te::qt::widgets::AbstractTreeItem* te::qt::widgets::LegendItem::clone(QObject* parent)
//{
//  return new LegendItem(m_rule, parent);
//}

void te::qt::widgets::LegendItem::setCheckable(bool checkable)
{
  m_isCheckable = checkable;
}

bool te::qt::widgets::LegendItem::getCheckable()
{
  return m_isCheckable;
}