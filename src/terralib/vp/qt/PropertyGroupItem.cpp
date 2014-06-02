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
  \file terralib/vp/qt/PropertyGroupItem.cpp

  \brief A class that represents a group of properties in a LayerTreeModel.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../datatype/Property.h"
#include "../Exception.h"
#include "LayerItem.h"
#include "PropertyGroupItem.h"
#include "PropertyItem.h"

// Qt
#include <QWidget>

te::vp::PropertyGroupItem::PropertyGroupItem(std::vector<te::dt::Property*> properties, AbstractTreeItem* parent)
  : AbstractTreeItem(parent)
{
  if(properties.empty())
    return;

  for(size_t i = 0; i < properties.size(); ++i)
  {
    te::dt::Property* prop = properties[i]->clone();

    if(prop->getType() == te::dt::GEOMETRY_TYPE)
      continue;

    te::qt::widgets::AbstractTreeItem* item = new PropertyItem(prop, this);

    m_items.push_back(item);
  }
}

te::vp::PropertyGroupItem::~PropertyGroupItem()
{
  //if(m_property && (m_property->getParent() == 0))
  //  delete m_property;
}

int te::vp::PropertyGroupItem::columnCount() const
{
  return 2;
}

QVariant te::vp::PropertyGroupItem::data(int column, int role) const
{
  if(role == Qt::DisplayRole && column == 0)
    return QVariant(TE_TR("Properties"));

  return QVariant();
}

QMenu* te::vp::PropertyGroupItem::getMenu(QWidget* parent) const
{
  /*QMenu* m = new QMenu(parent);

  QAction* aOpenProperty = m->addAction(tr("&Open property"));

  connect(aOpenProperty, SIGNAL(triggered()), this, SLOT(openProperty()));*/

  return 0;
}

bool te::vp::PropertyGroupItem::canFetchMore() const
{
  if(!children().empty ())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::vp::PropertyGroupItem::flags() const
{
  return Qt::ItemIsUserCheckable;
}

void te::vp::PropertyGroupItem::fetchMore()
{
  /*if(parent() == 0)
    return;

  LayerItem* parentItem = dynamic_cast<LayerItem*>(parent());

  if(!parentItem->hasChildren())
    return;

  te::map::AbstractLayerPtr layer = parentItem->getLayer();

  if(layer->hasChildren())
  {
    for(size_t i = 0; i < layer->getChildrenCount(); ++i)
    {
      te::map::AbstractLayerPtr child = boost::dynamic_pointer_cast<te::map::AbstractLayer>(layer->getChild(i));
      new LayerItem(child, this);
    }
  }
  else if(layer->getSchema()->getProperties().size() > 0)
  {
    const te::map::LayerSchema* schema = parentItem->getLayer()->getSchema();
    
    if(schema == 0)
      return;

    std::vector<te::dt::Property*> properties = schema->getProperties();

    for(size_t i = 0; i < properties.size(); ++i)
    {
      te::dt::Property* p = schema->getProperty(i);
      
      if(p == 0)
        continue;

      new PropertyItem(properties[i], this);
    }
  }*/
}

bool te::vp::PropertyGroupItem::hasChildren() const
{
  return m_items.size() > 0 ? true : false;
}

bool te::vp::PropertyGroupItem::setData(int column, const QVariant& value, int role)
{
  return true;
}

te::map::AbstractLayerPtr te::vp::PropertyGroupItem::getLayer() const
{
  return 0;
}

std::vector<te::qt::widgets::AbstractTreeItem*> te::vp::PropertyGroupItem::getItems() const
{
  return m_items;
}

const std::string te::vp::PropertyGroupItem::getItemType() const
{
  return "GROUPING_ITEM";
}
