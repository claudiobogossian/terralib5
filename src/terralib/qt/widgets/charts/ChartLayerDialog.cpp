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
  \file terralib/qt/widgets/charts/ChartLayerDialog.cpp

  \brief A dialog used to build a chart.
*/

// TerraLib
#include "ChartLayerDialog.h"
#include "ChartLayerWidget.h"
#include "ui_ChartLayerDialogForm.h"

te::qt::widgets::ChartLayerDialog::ChartLayerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ChartLayerDialogForm),
    m_chartWidget(0)
{
  m_ui->setupUi(this);
  
  // Fill Widget
  m_chartWidget = new te::qt::widgets::ChartLayerWidget(m_ui->m_widget);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(m_chartWidget);

  //connect
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onPushButtonClicked()));
  m_ui->m_helpPushButton->setPageReference("widgets/charts/pie_bar/pie_bar.html");

}

te::qt::widgets::ChartLayerDialog::~ChartLayerDialog()
{
}

void te::qt::widgets::ChartLayerDialog::setLayer(te::map::AbstractLayerPtr layer)
{
  std::auto_ptr<te::map::LayerSchema> dsType(layer->getSchema());

  m_chartWidget->setLayer(layer);
}

void te::qt::widgets::ChartLayerDialog::setChart(te::map::Chart* chart)
{
  m_chartWidget->setChart(chart);
}

void te::qt::widgets::ChartLayerDialog::onPushButtonClicked()
{
  bool res = m_chartWidget->buildChart();

  if(res)
    accept();
}
