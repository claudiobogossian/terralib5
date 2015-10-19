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
  \file terralib/qt/widgets/se/BasicFillPropertyItem.cpp

  \brief A widget used to define the basic fill se object.
*/

#include "../../../color.h"
#include "../../../raster/Utils.h"
#include "../../../se/Fill.h"
#include "../../../se/SvgParameter.h"
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
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, 255);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMinimum(m_opacityProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMaximum(m_opacityProperty, 255);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setSingleStep(m_opacityProperty, 10);
  basicFillProperty->addSubProperty(m_opacityProperty);

  addProperty(basicFillProperty, tr("Basic Fill"), QColor(255, 255, 175));

  m_update = true;
}

te::qt::widgets::BasicFillPropertyItem::~BasicFillPropertyItem()
{
  delete m_fill;
}

void te::qt::widgets::BasicFillPropertyItem::valueChanged(QtProperty *p, int value)
{
  if(!m_update)
    return;

  if(p == m_opacityProperty && m_update)
  {
    m_update = false;
    
    m_color.setAlpha(value);

    double opacity = value / 255.;

    // Updating fill opacity
    m_fill->setOpacity(QString::number(opacity, 'g', 2).toStdString());

    updateUi();

    emit fillChanged();
    m_update = true;
  }
}

void te::qt::widgets::BasicFillPropertyItem::valueChanged(QtProperty *p, const QColor &value)
{
  if(!m_update)
    return;

  if(p == m_colorProperty)
  {
    m_update = false;
  // The new fill color
    m_color.setRgb(value.red(), value.green(), value.blue(), value.alpha());

    double alpha = value.alpha() / 255.;

    // Updating fill color
    m_fill->setColor(m_color.name().toStdString());
    m_fill->setOpacity(QString::number(alpha, 'g', 2).toStdString());

    updateUi();

    emit fillChanged();
    m_update = true;
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
  QColor c = QColor(rgba.getRgba());

  int alpha = 255;
  
  if (m_fill->getOpacity())
    alpha = te::rst::Round(te::se::GetDouble(m_fill->getOpacity()) * 255.);

  c.setAlpha(alpha);

  te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager->setValue(m_colorProperty, c);

  // Opacity
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, alpha);
}

