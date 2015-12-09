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
\file terralib/qt/widgets/se/ScalePropertyItem.cpp

\brief A widget used to define the scale values.
*/

#include "../../../se/SvgParameter.h"
#include "../../../se/Utils.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "ScalePropertyItem.h"

// STL
#include <cassert>

te::qt::widgets::ScalePropertyItem::ScalePropertyItem(QtTreePropertyBrowser* pb, QColor c) 
  : te::qt::widgets::AbstractPropertyItem(pb, c),
  m_minScale(0.0),
  m_maxScale(std::numeric_limits<double>::infinity())
{
  //build property browser basic fill
  QtProperty* scaleProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Scale Denominator"));

  QStringList list;

  //min scale
  m_minScaleProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Minimum"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setDecimals(m_minScaleProperty, 4);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMinimum(m_minScaleProperty, 0.0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMaximum(m_minScaleProperty, std::numeric_limits<double>::infinity());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_minScaleProperty, 0.0);
  scaleProperty->addSubProperty(m_minScaleProperty);

  //max scale
  m_maxScaleProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Maximum"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setDecimals(m_maxScaleProperty, 4);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMinimum(m_maxScaleProperty, 0.0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMaximum(m_maxScaleProperty, std::numeric_limits<double>::infinity());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_maxScaleProperty, std::numeric_limits<double>::infinity());
  scaleProperty->addSubProperty(m_maxScaleProperty);

  addProperty(scaleProperty, tr("Scale Denominator"), QColor(250, 208, 190));
}

te::qt::widgets::ScalePropertyItem::~ScalePropertyItem()
{

}

void te::qt::widgets::ScalePropertyItem::setMinScaleDenominator(const double& value)
{
  m_minScale = value;

  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_minScaleProperty, value);
}

double te::qt::widgets::ScalePropertyItem::getMinScaleDenominator()
{
  return m_minScale;
}

void te::qt::widgets::ScalePropertyItem::setMaxScaleDenominator(const double& value)
{
  m_maxScale = value;

  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_maxScaleProperty, value);
}

double te::qt::widgets::ScalePropertyItem::getMaxScaleDenominator()
{
  return m_maxScale;
}

void te::qt::widgets::ScalePropertyItem::valueChanged(QtProperty *p, double value)
{
  if (p == m_minScaleProperty)
  {
    m_minScale = value;

    emit scaleMinChanged();
  }
  else if (p == m_maxScaleProperty)
  {
    m_maxScale = value;

    emit scaleMaxChanged();
  }
}
