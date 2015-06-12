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
  \file terralib/qt/widgets/se/ContrastEnhancementWidget.cpp

  \brief A dialog used to build a ContrastEnhancement element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../se.h"
#include "ContrastEnhancementWidget.h"
#include "ui_ContrastEnhancementWidgetForm.h"

// Qt


// STL
#include <cassert>


te::qt::widgets::ContrastEnhancementWidget::ContrastEnhancementWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ContrastEnhancementWidgetForm),
    m_contrast(new te::se::ContrastEnhancement)
{
  m_ui->setupUi(this);

  initialize();

  // Signals & slots
  connect(m_ui->m_gammaDoubleSpinBox, SIGNAL(valueChanged(double)), SLOT(onGammaValueChanged(double)));
  connect(m_ui->m_typeComboBox, SIGNAL(activated(QString)), SLOT(onTypeChanged(QString)));
}

te::qt::widgets::ContrastEnhancementWidget::~ContrastEnhancementWidget()
{
  delete m_contrast;

  m_ceNames.clear();
}

void te::qt::widgets::ContrastEnhancementWidget::setContrastEnhancement(te::se::ContrastEnhancement* ce)
{
  assert(ce);

  delete m_contrast;

  m_contrast = ce->clone();

  updateUi();
}

te::se::ContrastEnhancement* te::qt::widgets::ContrastEnhancementWidget::getContrastEnhancement() const
{
  return m_contrast->clone();
}

void te::qt::widgets::ContrastEnhancementWidget::initialize()
{
  //set the contrast enhancement names
  m_ceNames.clear();

  m_ceNames.insert(std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::value_type
    (te::se::ContrastEnhancement::ENHANCEMENT_NORMALIZE, tr("Normalize")));
  m_ceNames.insert(std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::value_type
    (te::se::ContrastEnhancement::ENHANCEMENT_HISTOGRAM, tr("Histogram")));
  m_ceNames.insert(std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::value_type
    (te::se::ContrastEnhancement::ENHANCEMENT_NONE, tr("None")));

  //fill interface combo box
  std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::iterator it = m_ceNames.begin();

  while(it != m_ceNames.end())
  {
    m_ui->m_typeComboBox->addItem(it->second);

    ++it;
  }

  //set default values
  m_contrast->setContrastEnhancementType(te::se::ContrastEnhancement::ENHANCEMENT_NONE);
  m_contrast->setGammaValue(m_ui->m_gammaDoubleSpinBox->value());

  updateUi();
}

void te::qt::widgets::ContrastEnhancementWidget::updateUi()
{
  m_ui->m_typeComboBox->setCurrentIndex(m_ui->m_typeComboBox->findText(m_ceNames[m_contrast->getContrastEnhancementType()]));
  m_ui->m_gammaDoubleSpinBox->setValue(m_contrast->getGammaValue());
}

void te::qt::widgets::ContrastEnhancementWidget::onGammaValueChanged(double value)
{
  m_contrast->setGammaValue(value);

  emit contrastEnhancementChanged();
}

void te::qt::widgets::ContrastEnhancementWidget::onTypeChanged(QString value)
{
  std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::iterator it = m_ceNames.begin();

  while(it != m_ceNames.end())
  {
    if(it->second == value)
    {
      m_contrast->setContrastEnhancementType(it->first);
      break;
    }

    ++it;
  }

  emit contrastEnhancementChanged();
}

