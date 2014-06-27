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
  \file terralib/qt/widgets/utils/HorizontalSliderWidget.cpp

  \brief ...
*/

// TerraLib
#include "../../../common/Translator.h"
#include "ui_HorizontalSliderWidgetForm.h"
#include "HorizontalSliderWidget.h"

// Qt
#include <QPushButton>



te::qt::widgets::HorizontalSliderWidget::HorizontalSliderWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::HorizontalSliderWidgetForm)
{
  m_ui->setupUi(this);

// default values
  m_minValue = 0;
  m_maxValue = 99;
  m_defaultValue = 0;

// connect signals and slots
  connect(m_ui->m_defaultPushButton, SIGNAL(released()), this, SLOT(onDefaultButtonPressed()));
  connect(m_ui->m_slider, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
  connect(m_ui->m_slider, SIGNAL(sliderReleased()), this, SLOT(onSliderReleased()));
}

te::qt::widgets::HorizontalSliderWidget::~HorizontalSliderWidget()
{
}

Ui::HorizontalSliderWidgetForm* te::qt::widgets::HorizontalSliderWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::HorizontalSliderWidget::setTitle(QString value)
{
  m_ui->m_groupBox->setTitle(value);
}

void te::qt::widgets::HorizontalSliderWidget::setMinMaxValues(int min, int max)
{
  m_minValue = min;
  m_ui->m_slider->setMinimum(m_minValue);

  m_maxValue = max;
  m_ui->m_slider->setMaximum(m_maxValue);
}

void te::qt::widgets::HorizontalSliderWidget::setDefaultValue(int value)
{
  m_defaultValue = value;

  m_ui->m_sliderLabel->setNum(m_defaultValue);

  m_ui->m_slider->setValue(m_defaultValue);
}

void te::qt::widgets::HorizontalSliderWidget::setCurrentValue(int value)
{
  m_ui->m_sliderLabel->setNum(value);

  m_ui->m_slider->setValue(value);
}

int te::qt::widgets::HorizontalSliderWidget::getValue()
{
  return m_ui->m_slider->value();
}

void te::qt::widgets::HorizontalSliderWidget::setDefaultButtonVisible(bool visible)
{
  m_ui->m_defaultPushButton->setVisible(visible);
}

void te::qt::widgets::HorizontalSliderWidget::setSliderLabelVisible(bool visible)
{
  m_ui->m_sliderLabel->setVisible(visible);
}

void te::qt::widgets::HorizontalSliderWidget::onDefaultButtonPressed()
{
  m_ui->m_slider->setValue(m_defaultValue);
  
  onValueChanged(m_defaultValue);

  onSliderReleased();
}

void te::qt::widgets::HorizontalSliderWidget::onValueChanged(int value)
{
  m_ui->m_sliderLabel->setNum(value);

  emit sliderValueChanged(value);
}

void te::qt::widgets::HorizontalSliderWidget::onSliderReleased()
{
  emit sliderReleased();
}
