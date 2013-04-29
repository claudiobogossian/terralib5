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
  \file terralib/vp/qt/LayerItem.cpp

  \brief ????
*/

// TerraLib
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../qt/widgets/datasource/explorer/PropertyItem.h"
#include "LayerItem.h"
#include "PropertyItem.h"

#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::vp::LayerItem::LayerItem(te::map::AbstractLayerPtr layer, QObject* parent)
  : te::qt::widgets::AbstractLayerTreeItem(parent),
    m_layer(layer),
    m_selected(false),
    m_onlySelecteds(false)
{
  if(layer->hasChildren())
  {
    for(size_t i = 0; i < layer->getChildrenCount(); i++)
    {
      te::map::AbstractLayerPtr layerChild = boost::dynamic_pointer_cast<te::map::AbstractLayer>(layer->getChild(i));

      if(layerChild->isValid())
        new LayerItem(layerChild, this);
    }
  }
  else if(m_layer->getSchema()->getProperties().size() > 0)
  {
    const te::map::LayerSchema* schema = m_layer->getSchema();

    if(schema == 0)
      return;

    std::vector<te::dt::Property*> properties = schema->getProperties();

    for(size_t i = 0; i < properties.size(); i++)
    {
      te::dt::Property* p = schema->getProperty(i);
      
      if(p == 0)
        continue;

      new PropertyItem(properties[i], this);
    }
  }
}

te::vp::LayerItem::~LayerItem()
{

}

int te::vp::LayerItem::columnCount() const
{
  return 2;
}

QVariant te::vp::LayerItem::data(int column, int role) const
{
  if(role == Qt::DisplayRole && column == 0)
    return QVariant(QString::fromStdString(m_layer->getTitle()));

  if(role == Qt::CheckStateRole && column == 0)
    return (m_selected ? Qt::Checked : Qt::Unchecked);

  if(role == Qt::CheckStateRole && column == 1)
    return (m_onlySelecteds ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::vp::LayerItem::getMenu(QWidget* parent) const
{
  //QMenu* m = new QMenu(parent);

  //QAction* aOpenDataSource = m->addAction(tr("&Open layer"));

  //connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  //return m;
  return 0;
}

bool te::vp::LayerItem::canFetchMore() const
{
  return m_layer->hasChildren() || (m_layer->getSchema()->getProperties().size() > 0);
}

void te::vp::LayerItem::fetchMore()
{
  if(parent() == 0)
    return;

  LayerItem* parentItem = dynamic_cast<LayerItem*>(parent());

  if(!parentItem->hasChildren())
    return;

  te::map::AbstractLayerPtr layer = parentItem->getLayer();

  if(layer->hasChildren())
  {
    for(size_t i = 0; i < layer->getChildrenCount(); i++)
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

    for(size_t i = 0; i < properties.size(); i++)
    {
      te::dt::Property* p = schema->getProperty(i);
      
      if(p == 0)
        continue;

      new PropertyItem(properties[i], this);
    }
  }
}

Qt::ItemFlags te::vp::LayerItem::flags() const
{
  return Qt::ItemIsUserCheckable;
}

bool te::vp::LayerItem::hasChildren() const
{
  return m_layer->hasChildren() || !children().isEmpty();
}

bool te::vp::LayerItem::setData(int column, const QVariant& value, int role)
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
    else if(column == 1)
    {
      if(checkState == Qt::Checked)
        m_onlySelecteds = true;
      else if(checkState == Qt::Unchecked)
        m_onlySelecteds = false;
    }
    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::vp::LayerItem::getLayer() const
{
  return m_layer;
}