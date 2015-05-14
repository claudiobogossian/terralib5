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
  \file terralib/qt/widgets/se/PolygonSymbolizerWidget.cpp

  \brief A widget used to build a polygon symbolizer element.
*/

// TerraLib
#include "../../../se/PolygonSymbolizer.h"
#include "AbstractFillWidget.h"
#include "AbstractFillWidgetFactory.h"
#include "BasicStrokeWidget.h"
#include "PolygonSymbolizerWidget.h"
#include "ui_PolygonSymbolizerWidgetForm.h"

// Qt
#include <QStackedWidget>

// STL
#include <cassert>
#include <string>
#include <vector>

te::qt::widgets::PolygonSymbolizerWidget::PolygonSymbolizerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::PolygonSymbolizerWidgetForm),
    m_symb(new te::se::PolygonSymbolizer)
{
  m_ui->setupUi(this);

  m_ui->m_mainLayout->setAlignment(Qt::AlignTop);

  // Stack of Fill Widgets
  m_fillWidgets = new QStackedWidget(this);

  // Adjusting...
  QGridLayout* fillLayout = new QGridLayout(m_ui->m_fillGroupBox);
  fillLayout->addWidget(m_fillWidgets);
  fillLayout->setAlignment(Qt::AlignTop);

  // Gets registered fill widgets
  std::vector<std::string> keys;
  te::qt::widgets::AbstractFillWidgetFactory::RegisteredWidgets(keys);
  std::vector<std::string>::iterator it;
  for(it = keys.begin(); it != keys.end(); ++it)
  {
    te::qt::widgets::AbstractFillWidget* fw = te::qt::widgets::AbstractFillWidgetFactory::make(*it);
    m_ui->m_polygonSymbolizerTypeComboBox->addItem(fw->getFillType());
    connect(fw, SIGNAL(fillChanged()), SLOT(onFillChanged()));
    m_fillWidgets->addWidget(fw);
  }

  // Stroke Widget
  m_strokeWidget = new te::qt::widgets::BasicStrokeWidget(this);

  // Adjusting...
  QGridLayout* strokeLayout = new QGridLayout(m_ui->m_strokeGroupBox);
  strokeLayout->addWidget(m_strokeWidget);
  strokeLayout->setAlignment(Qt::AlignTop);

  // Setups initial polygon symbolizer
  te::qt::widgets::AbstractFillWidget* w = static_cast<te::qt::widgets::AbstractFillWidget*>(m_fillWidgets->currentWidget());
  m_symb->setFill(w->getFill());
  m_symb->setStroke(m_strokeWidget->getStroke());

  // Signals & slots
  connect(m_ui->m_polygonSymbolizerTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onPolygonSymbolizerTypeComboBoxCurrentIndexChanged(int)));
  connect(m_ui->m_fillGroupBox, SIGNAL(toggled(bool)), this, SLOT(onFillGroupBoxToggled(bool)));
  connect(m_strokeWidget, SIGNAL(strokeChanged()), SLOT(onStrokeChanged()));
  connect(m_ui->m_strokeGroupBox, SIGNAL(toggled(bool)), this, SLOT(onStrokeGroupBoxToggled(bool)));
}

te::qt::widgets::PolygonSymbolizerWidget::~PolygonSymbolizerWidget()
{
  delete m_symb;
}

void te::qt::widgets::PolygonSymbolizerWidget::setSymbolizer(const te::se::PolygonSymbolizer* symb)
{
  assert(symb);

  delete m_symb;

  m_symb = static_cast<te::se::PolygonSymbolizer*>(symb->clone());

  updateUi();
}

te::se::Symbolizer* te::qt::widgets::PolygonSymbolizerWidget::getSymbolizer() const
{
  return m_symb->clone();
}

void te::qt::widgets::PolygonSymbolizerWidget::updateUi()
{
  const te::se::Stroke* stroke = m_symb->getStroke();
  if(stroke)
  {
    m_strokeWidget->setStroke(stroke);
    m_ui->m_strokeGroupBox->setChecked(true);
  }
  else
    m_ui->m_strokeGroupBox->setChecked(false);

  const te::se::Fill* fill = m_symb->getFill();
  if(fill == 0)
  {
    m_ui->m_fillGroupBox->setChecked(false);
    return;
  }
  
  // Updating UI fill element
  int i;
  for(i = 0; i < m_fillWidgets->count(); ++i)
  {
    te::qt::widgets::AbstractFillWidget* fw = static_cast<te::qt::widgets::AbstractFillWidget*>(m_fillWidgets->widget(i));
    if(fw->setFill(fill))
      break;
  }

  if(i == m_fillWidgets->count())
    return; // TODO: Exception! The polygon symbolizer fill cannot be dealt by any registered graphic widgets.

  // Updating Ui
  m_ui->m_polygonSymbolizerTypeComboBox->setCurrentIndex(i);
  m_fillWidgets->setCurrentIndex(i);
}

void te::qt::widgets::PolygonSymbolizerWidget::onPolygonSymbolizerTypeComboBoxCurrentIndexChanged(int index)
{
  m_fillWidgets->setCurrentIndex(index);

  te::qt::widgets::AbstractFillWidget* w = static_cast<te::qt::widgets::AbstractFillWidget*>(m_fillWidgets->currentWidget());
  m_symb->setFill(w->getFill());

  emit symbolizerChanged();
}

void te::qt::widgets::PolygonSymbolizerWidget::onStrokeChanged()
{
  m_symb->setStroke(m_strokeWidget->getStroke());
  emit symbolizerChanged();
}

void te::qt::widgets::PolygonSymbolizerWidget::onStrokeGroupBoxToggled(bool on)
{
  if(on == false)
    m_symb->setStroke(0);
  else
    m_symb->setStroke(m_strokeWidget->getStroke());

  emit symbolizerChanged();
}

void te::qt::widgets::PolygonSymbolizerWidget::onFillChanged()
{
  te::qt::widgets::AbstractFillWidget* w = static_cast<te::qt::widgets::AbstractFillWidget*>(m_fillWidgets->currentWidget());
  m_symb->setFill(w->getFill());

  emit symbolizerChanged();
}

void te::qt::widgets::PolygonSymbolizerWidget::onFillGroupBoxToggled(bool on)
{
  if(on == false)
    m_symb->setFill(0);
  else
  {
    te::qt::widgets::AbstractFillWidget* w = static_cast<te::qt::widgets::AbstractFillWidget*>(m_fillWidgets->currentWidget());
    m_symb->setFill(w->getFill());
  }

  emit symbolizerChanged();
}
