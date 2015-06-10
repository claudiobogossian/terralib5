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
  \file terralib/qt/widgets/se/BasicFillWidget.cpp

  \brief A widget used to build a basic fill element.
*/

// TerraLib
#include "../../../se/Fill.h"
#include "../../../se/Utils.h"
#include "../utils/ColorPickerToolButton.h"
#include "BasicFillWidget.h"
#include "ui_BasicFillWidgetForm.h"

// STL
#include <cassert>

te::qt::widgets::BasicFillWidget::BasicFillWidget(QWidget* parent, Qt::WindowFlags f)
  : AbstractFillWidget(parent, f),
    m_ui(new Ui::BasicFillWidgetForm),
    m_fill(new te::se::Fill)
{
  m_ui->setupUi(this);

  // Color Picker
  m_colorPicker = new te::qt::widgets::ColorPickerToolButton(this);
  m_colorPicker->setFixedSize(70, 24);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_colorPickerFrame);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(m_colorPicker);

  initialize();

  // Signals & slots
  connect(m_colorPicker, SIGNAL(colorChanged(const QColor&)), SLOT(onColorChanged(const QColor&)));
  connect(m_ui->m_fillOpacitySlider, SIGNAL(valueChanged(int)), SLOT(onFillOpacitySliderValueChanged(int)));
}

te::qt::widgets::BasicFillWidget::~BasicFillWidget()
{
  delete m_fill;
}

bool te::qt::widgets::BasicFillWidget::setFill(const te::se::Fill* fill)
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

te::se::Fill* te::qt::widgets::BasicFillWidget::getFill() const
{
  return m_fill->clone();
}

QString te::qt::widgets::BasicFillWidget::getFillType() const
{
  return tr("Basic Fill");
}

void te::qt::widgets::BasicFillWidget::initialize()
{
  // Default fill color
  m_color = QColor(TE_SE_DEFAULT_FILL_BASIC_COLOR);
  updateUiFillColor();
}

void te::qt::widgets::BasicFillWidget::updateUi()
{
  // Color
  te::color::RGBAColor rgba(TE_SE_DEFAULT_FILL_BASIC_COLOR, TE_OPAQUE);
  te::se::GetColor(m_fill, rgba);
  m_color = QColor(rgba.getRgba());
  m_color.setAlpha(rgba.getAlpha());
  updateUiFillColor();

  // Opacity
  m_ui->m_fillOpacitySlider->setValue(m_color.alphaF() * 100);
}

void te::qt::widgets::BasicFillWidget::updateUiFillColor()
{
  m_colorPicker->setColor(m_color);
}

void te::qt::widgets::BasicFillWidget::onColorChanged(const QColor& color)
{
  // The new fill color
  m_color.setRgb(color.red(), color.green(), color.blue(), m_color.alpha());

  updateUiFillColor();

  // Updating fill color
  m_fill->setColor(m_color.name().toStdString());
  emit fillChanged();
}

void te::qt::widgets::BasicFillWidget::onFillOpacitySliderValueChanged(int value)
{
  double opacity = value / 100.0;

  m_color.setAlpha(opacity * 255);
  updateUiFillColor();

  // Updating fill opacity
  m_fill->setOpacity(QString::number(opacity, 'g', 2).toStdString());
  emit fillChanged();
}
