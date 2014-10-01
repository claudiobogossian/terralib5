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
  \file terralib/qt/widgets/se/WellKnownMarkPropertyItem.cpp

  \brief A widget used to define the mark se properties.
*/

// TerraLib
#include "../../../se/Mark.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "BasicFillPropertyItem.h"
#include "BasicStrokePropertyItem.h"
#include "WellKnownMarkPropertyItem.h"

// STL
#include <cassert>


te::qt::widgets::WellKnownMarkPropertyItem::WellKnownMarkPropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c) ,
  m_mark(new te::se::Mark), m_setLocalMark(false)
{
  //build property browser basic fill
  QtProperty* markProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("WellKnown Mark"));

  //color
  m_typeProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->addProperty(tr("Type"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumNames(m_typeProperty, getTypeNames());
  markProperty->addSubProperty(m_typeProperty);

  addProperty(markProperty, tr("Mark"), QColor(255, 175, 175));
  
  m_bf = new te::qt::widgets::BasicFillPropertyItem(m_propertyBrowser, QColor(255, 175, 175));
  m_bs = new te::qt::widgets::BasicStrokePropertyItem(m_propertyBrowser, QColor(255, 175, 175));

  connect(m_bf, SIGNAL(fillChanged()), SLOT(onFillChanged()));
  connect(m_bs, SIGNAL(strokeChanged()), SLOT(onStrokeChanged()));

  // Setups initial mark
  m_mark->setWellKnownName(new std::string(m_supportedMarks[0]));
  m_mark->setFill(m_bf->getFill());
  m_mark->setStroke(m_bs->getStroke());

}

te::qt::widgets::WellKnownMarkPropertyItem::~WellKnownMarkPropertyItem()
{
}

void te::qt::widgets::WellKnownMarkPropertyItem::setMark(const te::se::Mark* mark)
{
  assert(mark);

  delete m_mark;

  m_mark = mark->clone();

  m_setLocalMark = true;

  updateUi();

  m_setLocalMark = false;
}

te::se::Mark* te::qt::widgets::WellKnownMarkPropertyItem::getMark() const
{
  return m_mark->clone();
}

void te::qt::widgets::WellKnownMarkPropertyItem::updateUi()
{
  const std::string* name = m_mark->getWellKnownName();
  assert(name); // TODO: Exception?

  std::map<int, std::string>::iterator it = m_supportedMarksMap.begin();

  while(it != m_supportedMarksMap.end())
  {
    if(it->second == *name)
    {
      te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(m_typeProperty, it->first);
      break;
    }
    ++it;
  }

  const te::se::Stroke* stroke = m_mark->getStroke();
  if(stroke)
    m_bs->setStroke(stroke);

  const te::se::Fill* fill = m_mark->getFill();
  if(fill)
    m_bf->setFill(fill);
}

void te::qt::widgets::WellKnownMarkPropertyItem::valueChanged(QtProperty* p, int value)
{
  if(p == m_typeProperty)
  {
    m_mark->setWellKnownName(new std::string(m_supportedMarksMap[value]));

    if(!m_setLocalMark)
      emit markChanged();
  }
}

QStringList te::qt::widgets::WellKnownMarkPropertyItem::getTypeNames()
{
  QStringList l;

  te::map::MarkRendererManager::getInstance().getAllSupportedMarks(m_supportedMarks);

  for(std::size_t i = 0; i < m_supportedMarks.size(); ++i)
  {
    l << m_supportedMarks[i].c_str();
    
    m_supportedMarksMap.insert(std::map<int, std::string>::value_type(i, m_supportedMarks[i]));
  }

  return l;
}

void te::qt::widgets::WellKnownMarkPropertyItem::onStrokeChanged()
{
  m_mark->setStroke(m_bs->getStroke());

  if(!m_setLocalMark)
    emit markChanged();
}

void te::qt::widgets::WellKnownMarkPropertyItem::onFillChanged()
{
  m_mark->setFill(m_bf->getFill());

  if(!m_setLocalMark)
    emit markChanged();
}
