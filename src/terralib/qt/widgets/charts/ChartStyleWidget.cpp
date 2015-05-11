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
  \file terralib/qt/widgets/charts/chartStyleWidget.cpp

  \brief A widget created to customize the style parameters of a chart
*/

//Terralib
#include "../../../qt/widgets/se/Utils.h"
#include "../utils/ColorPickerToolButton.h"
#include "ChartStyle.h"
#include "ChartStyleWidget.h"
#include "ui_ChartStyleWidgetForm.h"

//QT
 #include <QFontDialog>

te::qt::widgets::ChartStyleWidget::ChartStyleWidget(QWidget* parent, Qt::WindowFlags f, QString title, QString PropertyX, QString PropertyY)
  : QWidget(parent, f),
    m_ui(new Ui::chartStyleWidgetForm)
{
  m_ui->setupUi(this);
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
  connect(m_ui->m_labelXlLineEdit, SIGNAL(editingFinished()), this, SLOT(onlabelXEditFinish()));
  connect(m_ui->m_labelYlLineEdit, SIGNAL(editingFinished()), this, SLOT(onlabelYEditFinish()));
  connect(m_ui->m_labelStylePushButton, SIGNAL(clicked()), this, SLOT(onLabelStylePushButtonClicked()));
  connect(m_ui->m_titleStylePushButton, SIGNAL(clicked()), this, SLOT(onTitleStylePushButtonClicked()));
  connect(m_ui->m_gridCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onGridCheckBoxToggled(int)));
  connect(m_colorPicker, SIGNAL(colorChanged(const QColor&)), SLOT(onColorChanged(const QColor&)));
}

te::qt::widgets::ChartStyleWidget::~ChartStyleWidget(){}

void te::qt::widgets::ChartStyleWidget::onLabelStylePushButtonClicked()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, m_chartStyle->getAxisFont(), this);
  if (ok)
  {
    m_chartStyle->setAxisFont(font);
  }
}

void te::qt::widgets::ChartStyleWidget::onTitleStylePushButtonClicked()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, m_chartStyle->getTitleFont(), this);
  if (ok)
  {
    m_chartStyle->setTitleFont(font);
  }
}

void te::qt::widgets::ChartStyleWidget::onGridCheckBoxToggled(int state)
{
  m_chartStyle->setGridChecked(state);
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

void te::qt::widgets::ChartStyleWidget::onlabelXEditFinish()
{
  m_chartStyle->setAxisX(m_ui->m_labelXlLineEdit->text());
}

void te::qt::widgets::ChartStyleWidget::onlabelYEditFinish()
{
  m_chartStyle->setAxisY(m_ui->m_labelYlLineEdit->text());
}
te::qt::widgets::ChartStyle* te::qt::widgets::ChartStyleWidget::getStyle()
{
  return m_chartStyle;
}

void te::qt::widgets::ChartStyleWidget::setStyle(te::qt::widgets::ChartStyle* newStyle)
{
  m_chartStyle = newStyle;
  m_ui->m_chartTitleLineEdit->setText(m_chartStyle->getTitle());
  m_ui->m_labelXlLineEdit->setText(m_chartStyle->getAxisX());
  m_ui->m_labelYlLineEdit->setText(m_chartStyle->getAxisY());
  m_colorPicker->setColor(m_chartStyle->getColor());
  m_ui->m_gridCheckBox->setChecked(m_chartStyle->getGridChecked());
}

