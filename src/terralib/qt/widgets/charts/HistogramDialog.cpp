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
  \file terralib/qt/widgets/charts/HistogramDialog.cpp

  \brief A dialog used to define the basic parameters of a new histogram.
*/

//Terralib
#include "ChartDisplay.h"
#include "ChartDisplayWidget.h"
#include "ChartStyle.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "HistogramChart.h"
#include "HistogramDataWidget.h"
#include "HistogramDialog.h"
#include "HistogramStyle.h"
#include "ui_HistogramDialogForm.h"
#include "ui_HistogramDataWidgetForm.h"

//QT
#include <QtGui/QDialog>
#include <QtGui/QDockWidget>

te::qt::widgets::HistogramDialog::HistogramDialog(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::HistogramDialogForm)
{
  m_ui->setupUi(this);

  // Histogram data Widget
  m_histogramDataWidget = new te::qt::widgets::HistogramDataWidget(dataSet, dataType, this, 0);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_dataWidgetFrame);
  layout->addWidget(m_histogramDataWidget);
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::HistogramDialog::~HistogramDialog()
{
  delete m_histogramDataWidget;
}

te::qt::widgets::ChartDisplayWidget* te::qt::widgets::HistogramDialog::getDisplayWidget()
{
  return m_displayWidget;
}

void te::qt::widgets::HistogramDialog::onHelpPushButtonClicked(){}

void te::qt::widgets::HistogramDialog::onOkPushButtonClicked()
{
  te::qt::widgets::HistogramChart* chart = new te::qt::widgets::HistogramChart(m_histogramDataWidget->getHistogram());
  chart->setHistogramStyle(new te::qt::widgets::HistogramStyle());

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Histogram"));
  chartDisplay->getStyle()->setTitle(QString::fromStdString("Histogram"));
  chartDisplay->getStyle()->setAxisX(m_histogramDataWidget->getForm()->m_propertyComboBox->currentText());
  chartDisplay->getStyle()->setAxisY(QString::fromStdString("Frequency"));
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);
  chartDisplay->show();
  chartDisplay->replot();

  //Adjusting the chart widget
  m_displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::HISTOGRAM_CHART, chartDisplay, this->parentWidget());

  this->accept();


}
