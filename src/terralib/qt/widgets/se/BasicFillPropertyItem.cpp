/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/BasicFillPropertyItem.cpp

  \brief A widget used to define the basic fill se object.
*/

#include "../../../color.h"
#include "../../../se/Fill.h"
#include "../../../se/Utils.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "BasicFillPropertyItem.h"

// STL
#include <cassert>

// QtPropertyBrowser


te::qt::widgets::BasicFillPropertyItem::BasicFillPropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c),
  m_fill(new te::se::Fill)
{
  //build property browser basic fill
  QtProperty* basicFillProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Basic Fill"));

  //color
  m_colorProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager->addProperty(tr("Color"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager->setValue(m_colorProperty, QColor(TE_SE_DEFAULT_FILL_BASIC_COLOR));
  basicFillProperty->addSubProperty(m_colorProperty);

  //opacity
  m_opacityProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->addProperty(tr("Opacity"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, 100);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMinimum(m_opacityProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMaximum(m_opacityProperty, 100);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setSingleStep(m_opacityProperty, 10);
  basicFillProperty->addSubProperty(m_opacityProperty);

  addProperty(basicFillProperty, tr("Basic Fill"), QColor(255, 255, 175));
}

te::qt::widgets::BasicFillPropertyItem::~BasicFillPropertyItem()
{
  delete m_fill;
}

void te::qt::widgets::BasicFillPropertyItem::valueChanged(QtProperty *p, int value)
{
  if(p == m_opacityProperty)
  {
    double opacity = value / 100.0;

    m_color.setAlpha(opacity * 255);
    updateUiFillColor();

    // Updating fill opacity
    m_fill->setOpacity(QString::number(opacity, 'g', 2).toStdString());
    emit fillChanged();
  }
}

void te::qt::widgets::BasicFillPropertyItem::valueChanged(QtProperty *p, const QColor &value)
{
  if(p == m_colorProperty)
  {
  // The new fill color
    m_color.setRgb(value.red(), value.green(), value.blue(), m_color.alpha());

    updateUiFillColor();

    // Updating fill color
    m_fill->setColor(m_color.name().toStdString());
    emit fillChanged();
  }
}

bool te::qt::widgets::BasicFillPropertyItem::setFill(const te::se::Fill* fill)
{
  assert(fill);

  // Verifying if this widget can deal with the given fill...
  const te::se::Graphic* g = fill->getGraphicFill();
  if(g != 0)
    return false;

  delete m_fill;

  m_fill = fill->clone();

  updateUi();

  return true;
}

te::se::Fill* te::qt::widgets::BasicFillPropertyItem::getFill() const
{
  return m_fill->clone();
}

void te::qt::widgets::BasicFillPropertyItem::updateUi()
{
  // Color
  te::color::RGBAColor rgba(TE_SE_DEFAULT_FILL_BASIC_COLOR, TE_OPAQUE);
  te::se::GetColor(m_fill, rgba);
  m_color = QColor(rgba.getRgba());
  m_color.setAlpha(rgba.getAlpha());

  te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager->setValue(m_colorProperty, m_color);
  

  // Opacity
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, m_color.alphaF() * 100);
}

void te::qt::widgets::BasicFillPropertyItem::updateUiFillColor()
{
  te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager->setValue(m_colorProperty, m_color);
}
