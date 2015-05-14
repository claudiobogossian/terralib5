/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/AbstractPropertyItem.cpp

  \brief A widget used to define the main property items
         that can be used to describe a se object.
*/

#include "AbstractPropertyItem.h"

#include "AbstractPropertyManager.h"



te::qt::widgets::AbstractPropertyItem::AbstractPropertyItem(QtTreePropertyBrowser* pb, QColor c) : 
  m_propertyBrowser(pb), m_colorParent(c)
{
  te::qt::widgets::AbstractPropertyManager::getInstance().setFactories(m_propertyBrowser);

  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(valueChanged(QtProperty *, int)));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(valueChanged(QtProperty *, int)));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager, SIGNAL(valueChanged(QtProperty *, double)), this, SLOT(valueChanged(QtProperty *, double)));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)), this, SLOT(valueChanged(QtProperty *, const QString &)));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager, SIGNAL(valueChanged(QtProperty *, const QColor &)), this, SLOT(valueChanged(QtProperty *, const QColor &)));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_fontManager, SIGNAL(valueChanged(QtProperty *, const QFont &)), this, SLOT(valueChanged(QtProperty *, const QFont &)));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager, SIGNAL(valueChanged(QtProperty *, const QSize &)), this, SLOT(valueChanged(QtProperty *, const QSize &)));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(valueChanged(QtProperty *, int)));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager, SIGNAL(valueChanged(QtProperty *, const QPointF &)), this, SLOT(valueChanged(QtProperty *, const QPointF &)));
}

te::qt::widgets::AbstractPropertyItem::~AbstractPropertyItem()
{
}

void te::qt::widgets::AbstractPropertyItem::addProperty(QtProperty* p, const QString &id, QColor c)
{
  m_propertyToId[p] = id;
  m_idToProperty[id] = p;
  QtBrowserItem* item = m_propertyBrowser->addProperty(p);

  if(m_colorParent.isValid())
    m_propertyBrowser->setBackgroundColor(item, m_colorParent);
  else
    m_propertyBrowser->setBackgroundColor(item, c);

  m_propertyBrowser->setExpanded(item, true);

  setExpandedStatus(item, false);

}

void te::qt::widgets::AbstractPropertyItem::setExpandedStatus(QtBrowserItem* item, bool status)
{
  QList<QtBrowserItem*> list = item->children();

  for(int i = 0; i < list.size(); ++i)
  {
    m_propertyBrowser->setExpanded(list[i], false);

    setExpandedStatus(list[i], false);
  }
}

void te::qt::widgets::AbstractPropertyItem::valueChanged(QtProperty *p, int value)
{
}

void te::qt::widgets::AbstractPropertyItem::valueChanged(QtProperty *p, double value)
{
}

void te::qt::widgets::AbstractPropertyItem::valueChanged(QtProperty *p, const QString &value)
{
}

void te::qt::widgets::AbstractPropertyItem::valueChanged(QtProperty *p, const QColor &value)
{
}

void te::qt::widgets::AbstractPropertyItem::valueChanged(QtProperty *p, const QFont &value)
{
}

void te::qt::widgets::AbstractPropertyItem::valueChanged(QtProperty *p, const QSize &value)
{
}

void te::qt::widgets::AbstractPropertyItem::valueChanged(QtProperty *p, const QPointF &value)
{
}
