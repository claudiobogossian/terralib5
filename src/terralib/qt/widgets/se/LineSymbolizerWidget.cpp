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
  \file terralib/qt/widgets/se/LineSymbolizerWidget.cpp

  \brief A widget used to build a line symbolizer element.
*/

// TerraLib
#include "../../../se/LineSymbolizer.h"
#include "BasicStrokeWidget.h"
#include "LineSymbolizerWidget.h"
#include "ui_LineSymbolizerWidgetForm.h"

// STL
#include <cassert>
#include <string>
#include <vector>

te::qt::widgets::LineSymbolizerWidget::LineSymbolizerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::LineSymbolizerWidgetForm),
    m_symb(new te::se::LineSymbolizer)
{
  m_ui->setupUi(this);

  // Allowed types
  m_ui->m_lkineSymbolizerTypeComboBox->addItem(tr("Basic Line"));

  // Stroke Widget
  m_strokeWidget = new te::qt::widgets::BasicStrokeWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_strokeGroupBox);
  layout->addWidget(m_strokeWidget);
  layout->setAlignment(Qt::AlignTop);

  // Setups initial line symbolizer
  m_symb->setStroke(m_strokeWidget->getStroke());

  // Signals & slots
  connect(m_strokeWidget, SIGNAL(strokeChanged()), SLOT(onStrokeChanged()));
  connect(m_ui->m_strokeGroupBox, SIGNAL(toggled(bool)), this, SLOT(onStrokeGroupBoxToggled(bool)));
}

te::qt::widgets::LineSymbolizerWidget::~LineSymbolizerWidget()
{
  delete m_symb;
}

void te::qt::widgets::LineSymbolizerWidget::setSymbolizer(const te::se::LineSymbolizer* symb)
{
  assert(symb);

  delete m_symb;

  m_symb = static_cast<te::se::LineSymbolizer*>(symb->clone());

  updateUi();
}

te::se::Symbolizer* te::qt::widgets::LineSymbolizerWidget::getSymbolizer() const
{
  return m_symb->clone();
}

void te::qt::widgets::LineSymbolizerWidget::updateUi()
{
  const te::se::Stroke* stroke = m_symb->getStroke();
  if(stroke)
  {
    m_strokeWidget->setStroke(stroke);
    m_ui->m_strokeGroupBox->setChecked(true);
  }
  else
    m_ui->m_strokeGroupBox->setChecked(false);
}

void te::qt::widgets::LineSymbolizerWidget::onStrokeChanged()
{
  m_symb->setStroke(m_strokeWidget->getStroke());
  emit symbolizerChanged();
}

void te::qt::widgets::LineSymbolizerWidget::onStrokeGroupBoxToggled(bool on)
{
  if(on == false)
    m_symb->setStroke(0);
  else
    m_symb->setStroke(m_strokeWidget->getStroke());

  emit symbolizerChanged();
}
