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
  \file terralib/qt/widgets/charts/TimeSeriesDialog.cpp

  \brief A dialog used to define the basic parameters of a new timeSeries.
*/

//Terralib
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "ChartDisplay.h"
#include "ChartDisplayWidget.h"
#include "ChartStyle.h"
#include "TimeSeriesChart.h"
#include "TimeSeriesDataWidget.h"
#include "TimeSeriesDialog.h"
#include "TimeSeriesStyle.h"
#include "ui_ChartDialogForm.h"
#include "ui_TimeSeriesDataWidgetForm.h"

//QT
#include <QDockWidget>

te::qt::widgets::TimeSeriesDialog::TimeSeriesDialog(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ChartDialogForm)
{
  m_ui->setupUi(this);

  // TimeSeries data Widget
  m_timeSeriesDataWidget = new te::qt::widgets::TimeSeriesDataWidget(dataSet, dataType, this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_dataWidgetFrame);
  layout->addWidget(m_timeSeriesDataWidget);
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

  m_ui->m_helpPushButton->setPageReference("widgets/charts/timeSeries/timeSeries.html");
}

te::qt::widgets::TimeSeriesDialog::~TimeSeriesDialog()
{
  delete m_timeSeriesDataWidget;
}

te::qt::widgets::ChartDisplayWidget* te::qt::widgets::TimeSeriesDialog::getDisplayWidget()
{
  return m_displayWidget;
}

void te::qt::widgets::TimeSeriesDialog::onOkPushButtonClicked()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  te::qt::widgets::TimeSeriesChart* chart = new te::qt::widgets::TimeSeriesChart(m_timeSeriesDataWidget->getTimeSeries());

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(QString::fromStdString("TimeSeries"));

  chartStyle->setAxisX(m_timeSeriesDataWidget->getForm()->m_timeComboBox->currentText());
  chartStyle->setAxisY(m_timeSeriesDataWidget->getForm()->m_valueComboBox->currentText());

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("TimeSeries"), chartStyle);
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);
  chartDisplay->show();
  chartDisplay->replot();

  //Adjusting the chart widget
  m_displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::TIMESERIES_CHART, chartDisplay, this->parentWidget());

  QApplication::restoreOverrideCursor();
  this->accept();
}
