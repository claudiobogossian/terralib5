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
  \file terralib/qt/widgets/se/ShadedReliefWidget.cpp

  \brief A dialog used to build a ShadedRelief element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../se/ShadedRelief.h"
#include "ShadedReliefWidget.h"
#include "ui_ShadedReliefWidgetForm.h"

// Qt


// STL
#include <cassert>


te::qt::widgets::ShadedReliefWidget::ShadedReliefWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ShadedReliefWidgetForm),
    m_sr(new te::se::ShadedRelief)
{
  m_ui->setupUi(this);

  initialize();

  // Signals & slots
  connect(m_ui->m_brightnessCheckBox, SIGNAL(toggled(bool)), SLOT(onBrightnessToggled(bool)));
  connect(m_ui->m_reliefFactorDoubleSpinBox, SIGNAL(valueChanged(double)), SLOT(onFactorChanged(double)));
}

te::qt::widgets::ShadedReliefWidget::~ShadedReliefWidget()
{
  delete m_sr;
}

void te::qt::widgets::ShadedReliefWidget::setShadedRelief(const te::se::ShadedRelief* sr) 
{
  assert(sr);

  delete m_sr;

  m_sr = sr->clone();

  updateUi();
}

te::se::ShadedRelief* te::qt::widgets::ShadedReliefWidget::getShadedRelief() const
{
  return m_sr->clone();
}

void te::qt::widgets::ShadedReliefWidget::initialize()
{
  m_sr->setBrightnessOnly(m_ui->m_brightnessCheckBox->isChecked());
  m_sr->setReliefFactor(m_ui->m_reliefFactorDoubleSpinBox->value());
}

void te::qt::widgets::ShadedReliefWidget::updateUi()
{
  m_ui->m_brightnessCheckBox->setChecked(m_sr->isBrightnessOnly());
  m_ui->m_reliefFactorDoubleSpinBox->setValue(m_sr->getReliefFactor());
}

void te::qt::widgets::ShadedReliefWidget::onBrightnessToggled(bool flag)
{
  m_sr->setBrightnessOnly(flag);
}

void te::qt::widgets::ShadedReliefWidget::onFactorChanged(double value)
{
  m_sr->setReliefFactor(value);
}
