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
  \file terralib/vp/qt/LayerItem.cpp

  \brief A class that represents a Layer in a LayerTreeModel.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../maptools/DataSetLayer.h"
#include "../../se/Style.h"
#include "LayerItem.h"
#include "PropertyGroupItem.h"
#include "PropertyItem.h"
#include "LegendGroupItem.h"

// Qt
#include <QWidget>

// STL
#include <memory>

te::vp::LayerItem::LayerItem(te::map::AbstractLayerPtr layer, QObject* parent)
  : te::qt::widgets::AbstractTreeItem(parent),
    m_layer(layer),
    m_propertyGroup(0),
    m_legendGroup(0),
    m_selected(false),
    m_onlySelecteds(false),
    m_OnlyLegend(false)
{
  if(layer->hasChildren())
  {
    for(size_t i = 0; i < layer->getChildrenCount(); ++i)
    {
      te::map::AbstractLayerPtr layerChild = boost::dynamic_pointer_cast<te::map::AbstractLayer>(layer->getChild(i));

      if(layerChild->isValid())
      {
        te::qt::widgets::AbstractTreeItem* litem =  new te::vp::LayerItem(layerChild, this);
        m_items.push_back(litem);
      }
    }
  }
  else if(m_layer->getSchema()->getProperties().size() > 0)
  {
    std::auto_ptr<te::map::LayerSchema> schema = m_layer->getSchema();

    if(schema.get() == 0)
      return;

    std::vector<te::dt::Property*> properties = schema->getProperties();

    m_propertyGroup = new PropertyGroupItem(properties, this);
  }
  
  te::map::DataSetLayer* dataSetLayer = 0;
  dataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_layer.get());

  if(dataSetLayer != 0)
  {
    if(dataSetLayer->getStyle())
    {
      const std::vector<te::se::Rule*>& rules = dataSetLayer->getStyle()->getRules();

      m_legendGroup = new LegendGroupItem(rules, this);
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

  for(size_t i = 0; i < m_items.size(); ++i)
  {
    m_items[i]->fetchMore();
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

      std::vector<te::qt::widgets::AbstractTreeItem*> items = m_propertyGroup->getItems();

       for(size_t i = 0; i < items.size(); ++i)
      {
        PropertyItem* pItem = dynamic_cast<PropertyItem*>(items[i]);
        pItem->setSelected(m_selected);
      }
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

void te::vp::LayerItem::isSelected(bool selected)
{
  m_selected = selected;
}

bool te::vp::LayerItem::isSelected()
{
  return m_selected;
}

std::vector<te::dt::Property*> te::vp::LayerItem::getSelected()
{
  std::vector<te::qt::widgets::AbstractTreeItem*> propItems = m_propertyGroup->getItems();
  
  std::vector<te::dt::Property*> selected;

  for(size_t i = 0; i < propItems.size(); ++i)
  {
    PropertyItem* pitem = dynamic_cast<PropertyItem*>(propItems[i]);

    if(pitem->isSelected())
    {
      selected.push_back(pitem->getProperty());
    }
  }

  return selected;
}

const std::string te::vp::LayerItem::getItemType() const
{
  return "LAYER_ITEM";
}
