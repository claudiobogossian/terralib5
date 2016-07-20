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
\file terralib/qt/widgets/se/LinePlacementPropertyItem.cpp

\brief A widget used to define the LinePlacement se object.
*/


#include "../../../common/StringUtils.h"
#include "../../../se/SvgParameter.h"
#include "../../../se/LinePlacement.h"
#include "../../../se/Utils.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "LinePlacementPropertyItem.h"

// STL
#include <cassert>


te::qt::widgets::LinePlacementPropertyItem::LinePlacementPropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c),
m_linePlacement(new te::se::LinePlacement)
{
  //build property browser basic fill
  QtProperty* linePlacementProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Line Placement"));

  //offset
  m_perpendicularOffset = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Offset"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_perpendicularOffset, 10.);
  linePlacementProperty->addSubProperty(m_perpendicularOffset);

  //isAligned
  m_isAligned = te::qt::widgets::AbstractPropertyManager::getInstance().m_boolManager->addProperty(tr("Is Aligned"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_boolManager->setValue(m_isAligned, false);
  linePlacementProperty->addSubProperty(m_isAligned);

  addProperty(linePlacementProperty, tr("Line Placement"), QColor(241, 165, 61));

  m_update = true;
}

te::qt::widgets::LinePlacementPropertyItem::~LinePlacementPropertyItem()
{
  delete m_linePlacement;
}

void te::qt::widgets::LinePlacementPropertyItem::valueChanged(QtProperty* p, double value)
{
  if (!m_update)
    return;

  if (p == m_perpendicularOffset)
  {
    m_update = false;

    m_linePlacement->setPerpendicularOffset(new te::se::ParameterValue(QString::number(value).toStdString()));
    
    emit linePlacementChanged();
    m_update = true;
  }
}

void te::qt::widgets::LinePlacementPropertyItem::valueChanged(QtProperty* p, bool value)
{
  if (!m_update)
    return;

  if (p == m_isAligned)
  {
    m_update = false;

    m_linePlacement->setIsAligned(value);

    emit linePlacementChanged();
    m_update = true;
  }
}

void te::qt::widgets::LinePlacementPropertyItem::setLinePlacement(const te::se::LinePlacement* pp)
{
  assert(pp);

  // Verifying if this widget can deal with the given fill...
  delete m_linePlacement;

  m_linePlacement = dynamic_cast<te::se::LinePlacement*>(pp->clone());

  updateUi();

  m_update = true;
}

te::se::LinePlacement* te::qt::widgets::LinePlacementPropertyItem::getLinePlacement() const
{
  return dynamic_cast<te::se::LinePlacement*>(m_linePlacement->clone());
}

void te::qt::widgets::LinePlacementPropertyItem::resetLinePlacement()
{
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_perpendicularOffset, 10.);

  te::qt::widgets::AbstractPropertyManager::getInstance().m_boolManager->setValue(m_isAligned, false);
}

void te::qt::widgets::LinePlacementPropertyItem::updateUi()
{
  m_update = true;

  //update offset
  double offset = 10.;

  if (m_linePlacement->getPerpendicularOffset())
    offset = te::se::GetDouble(m_linePlacement->getPerpendicularOffset());

  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_perpendicularOffset, offset);

  //update isAligned
  bool isAligned = m_linePlacement->isAligned();
  
  te::qt::widgets::AbstractPropertyManager::getInstance().m_boolManager->setValue(m_isAligned, isAligned);

  m_update = false;
}

