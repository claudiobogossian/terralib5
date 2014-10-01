/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/qt/PropertyItem.cpp

  \brief A class that represents a Property in a LayerTreeModel.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../datatype/Property.h"
#include "../Exception.h"
#include "LayerItem.h"
#include "PropertyItem.h"

// Qt
#include <QWidget>

te::vp::PropertyItem::PropertyItem(te::dt::Property* p, AbstractTreeItem* parent)
  : AbstractTreeItem(parent),
    m_property(p),
    m_selected(false)
{
}

te::vp::PropertyItem::~PropertyItem()
{
  if(m_property && (m_property->getParent() == 0))
    delete m_property;
}

int te::vp::PropertyItem::columnCount() const
{
  return 2;
}

QVariant te::vp::PropertyItem::data(int column, int role) const
{
  LayerItem* litem = dynamic_cast<LayerItem*>(parent()->parent());

  if(role == Qt::DisplayRole && column == 0)
    return QVariant(m_property->getName().c_str());

  if(role == Qt::CheckStateRole && column == 0 && litem != 0)
    return ((m_selected && litem->isSelected()) ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::vp::PropertyItem::getMenu(QWidget* parent) const
{
  /*QMenu* m = new QMenu(parent);

  QAction* aOpenProperty = m->addAction(tr("&Open property"));

  connect(aOpenProperty, SIGNAL(triggered()), this, SLOT(openProperty()));*/

  return 0;
}

bool te::vp::PropertyItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::vp::PropertyItem::flags() const
{
  return Qt::ItemIsUserCheckable;
}

void te::vp::PropertyItem::fetchMore()
{
}

bool te::vp::PropertyItem::hasChildren() const
{
  return false;
}

bool te::vp::PropertyItem::setData(int column, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));

    if(!ok)
      return false;

    if(column == 0)
    {
      if(checkState == Qt::Checked)
        m_selected = true;
      else if(checkState == Qt::Unchecked)
        m_selected = false;
    }    
    return true;
  }

  return true;
}

te::dt::Property* te::vp::PropertyItem::getProperty() const
{
  return m_property;
}

te::map::AbstractLayerPtr te::vp::PropertyItem::getLayer() const
{
  return 0;
}

void te::vp::PropertyItem::setSelected(bool selected)
{
  m_selected = selected;
}

bool te::vp::PropertyItem::isSelected()
{
  return m_selected;
}

const std::string te::vp::PropertyItem::getItemType() const
{
  return "LEGEND_ITEM";
}
