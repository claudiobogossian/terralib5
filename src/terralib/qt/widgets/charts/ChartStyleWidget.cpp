/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/chartStyleWidget.cpp

  \brief A dialog created to customize the style parameters of a chart
*/

//Terralib
#include "ChartStyleWidget.h"
#include "ChartStyle.h"
#include "../utils/ColorPickerToolButton.h"
#include "ui_chartStyleWidgetForm.h"
#include "../../../qt/widgets/se/Utils.h"

te::qt::widgets::ChartStyleWidget::ChartStyleWidget(QWidget* parent, Qt::WindowFlags f, QString title, QString PropertyX, QString PropertyY)
  : QWidget(parent, f),
    m_ui(new Ui::chartStyleWidgetForm)
{
  m_ui->setupUi(this);
  m_ui->m_labelStylePushButton->setEnabled(false);
  m_ui->m_titleStylePushButton->setEnabled(false);
  m_ui->m_chartTitleLineEdit->setText(title);
  m_ui->m_labelXlLineEdit->setText(PropertyX);
  m_ui->m_labelYlLineEdit->setText(PropertyY);

  // Color Picker
  m_colorPicker = new te::qt::widgets::ColorPickerToolButton(this);
  m_colorPicker->setFixedSize(107, 24);

  // Adjusting the color picker
  QGridLayout* layout = new QGridLayout(m_ui->m_colorPickerFrame);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(m_colorPicker);

  m_chartStyle = new te::qt::widgets::ChartStyle();

  connect(m_ui->m_chartTitleLineEdit, SIGNAL(editingFinished()), this, SLOT(onTitleLineEditFinish()));
  connect(m_ui->m_gridCheckBox, SIGNAL(Toggled()), this, SLOT(onGridCheckBoxToggled()));
  connect(m_colorPicker, SIGNAL(colorChanged(const QColor&)), SLOT(onColorChanged(const QColor&)));
}

te::qt::widgets::ChartStyleWidget::~ChartStyleWidget(){}

void te::qt::widgets::ChartStyleWidget::onTitleStylePushButtonClicked(){}
void te::qt::widgets::ChartStyleWidget::onLabelStylePushButtonClicked(){}

void te::qt::widgets::ChartStyleWidget::onGridCheckBoxToggled()
{
  m_chartStyle->setGridChecked(m_ui->m_gridCheckBox->isChecked());
}

void te::qt::widgets::ChartStyleWidget::onColorChanged(const QColor& color)
{
  // The new fill color
  m_chartStyle->setColor(color);

  m_colorPicker->setColor(m_chartStyle->getColor());
}

void te::qt::widgets::ChartStyleWidget::onTitleLineEditFinish()
{
  m_chartStyle->setTitle(m_ui->m_chartTitleLineEdit->text());
}

te::qt::widgets::ChartStyle* te::qt::widgets::ChartStyleWidget::getStyle(te::qt::widgets::ChartStyle* initial, QWidget* parent, Qt::WindowFlags f, QString title, QString PropertyX, QString PropertyY)
{
  te::qt::widgets::ChartStyleWidget dlg(parent, f, title, PropertyX, PropertyY);

  dlg.setWindowTitle("Chart Style");
  
  if(initial)
    dlg.setStyle(initial);
  
  //if(dlg.exec() == QDialog::Accepted)
  //  return dlg.adjustStyle();

  return dlg.m_chartStyle;
}

void te::qt::widgets::ChartStyleWidget::setStyle(te::qt::widgets::ChartStyle* newStyle)
{
  m_chartStyle = newStyle;
}

te::qt::widgets::ChartStyle* te::qt::widgets::ChartStyleWidget::adjustStyle() 
{
  m_chartStyle->setTitle(m_ui->m_chartTitleLineEdit->text());
  m_chartStyle->setColor(m_colorPicker->getColor());
  m_chartStyle->setGridChecked(m_ui->m_gridCheckBox->isChecked());
  return m_chartStyle;
}