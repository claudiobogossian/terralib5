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
  \file terralib/qt/widgets/charts/HistogramDialog.cpp

  \brief A dialog used to define the basic parameters of a new histogram.
*/

//Terralib
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "ChartDisplay.h"
#include "ChartDisplayWidget.h"
#include "ChartStyle.h"
#include "HistogramChart.h"
#include "HistogramDataWidget.h"
#include "HistogramDialog.h"
#include "HistogramStyle.h"
#include "ui_ChartDialogForm.h"
#include "ui_HistogramDataWidgetForm.h"

//QT
#include <QDockWidget>

te::qt::widgets::HistogramDialog::HistogramDialog(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ChartDialogForm)
{
  m_ui->setupUi(this);

  // Histogram data Widget
  m_histogramDataWidget = new te::qt::widgets::HistogramDataWidget(dataSet, dataType, this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_dataWidgetFrame);
  layout->addWidget(m_histogramDataWidget);
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

  m_ui->m_helpPushButton->setPageReference("widgets/charts/histogram/histogram.html");
}

te::qt::widgets::HistogramDialog::~HistogramDialog()
{
  delete m_histogramDataWidget;
}

te::qt::widgets::ChartDisplayWidget* te::qt::widgets::HistogramDialog::getDisplayWidget()
{
  return m_displayWidget;
}

void te::qt::widgets::HistogramDialog::onOkPushButtonClicked()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  te::qt::widgets::HistogramChart* chart = new te::qt::widgets::HistogramChart(m_histogramDataWidget->getHistogram());

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(QString::fromStdString("Histogram"));

  if(m_histogramDataWidget->getForm()->m_summaryComboBox->currentText() != "None")
    chartStyle->setAxisX(QString::fromStdString(m_histogramDataWidget->getSummaryFunction()) + ": " + m_histogramDataWidget->getForm()->m_propertyComboBox->currentText());
  else
    chartStyle->setAxisX(m_histogramDataWidget->getForm()->m_propertyComboBox->currentText());

  chartStyle->setAxisY(QString::fromStdString("Frequency"));

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Histogram"), chartStyle);
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);
  chartDisplay->show();
  chartDisplay->replot();

  //Adjusting the chart widget
  m_displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::HISTOGRAM_CHART, chartDisplay, this->parentWidget());

  QApplication::restoreOverrideCursor();
  this->accept();
}
