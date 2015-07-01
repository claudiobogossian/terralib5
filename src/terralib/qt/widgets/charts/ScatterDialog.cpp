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
  \file terralib/qt/widgets/charts/ScatterCreatorDialog.cpp

  \brief A widget used to define the basic parameters of a new Scatter chart.
*/

//Terralib

#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "../../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../../se/Graphic.h"
#include "../se/Symbol.h"
#include "ChartDisplay.h"
#include "ChartDisplayWidget.h"
#include "ChartStyle.h"
#include "Scatter.h"
#include "ScatterChart.h"
#include "ScatterDataWidget.h"
#include "ScatterDialog.h"
#include "ScatterStyle.h"
#include "ui_ChartDialogForm.h"
#include "ui_ScatterDataWidgetForm.h"

//QT
#include <QDockWidget>

//QWT
#include <qwt_symbol.h>

te::qt::widgets::ScatterDialog::ScatterDialog(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent,  Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ChartDialogForm)
{
    m_ui->setupUi(this);

  // Scatter data Widget
  m_scatterDataWidget = new te::qt::widgets::ScatterDataWidget(dataSet, dataType, this, 0);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_dataWidgetFrame);
  layout->addWidget(m_scatterDataWidget);
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

  m_ui->m_helpPushButton->setPageReference("widgets/charts/scatter/scatter.html");

}

te::qt::widgets::ScatterDialog::~ScatterDialog()
{
  delete m_scatterDataWidget;
}

te::qt::widgets::ChartDisplayWidget* te::qt::widgets::ScatterDialog::getDisplayWidget()
{
  return m_displayWidget;
}

void te::qt::widgets::ScatterDialog::onOkPushButtonClicked()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  te::qt::widgets::ScatterChart* chart = new te::qt::widgets::ScatterChart(m_scatterDataWidget->getScatter());

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(QString::fromStdString("Scatter"));

  if(m_scatterDataWidget->getForm()->m_summaryComboBox->currentText() != "None")
    chartStyle->setAxisX(m_scatterDataWidget->getForm()->m_summaryComboBox->currentText() + ": " + m_scatterDataWidget->getForm()->m_propertyXComboBox->currentText());
  else
    chartStyle->setAxisX(m_scatterDataWidget->getForm()->m_propertyXComboBox->currentText());

  chartStyle->setAxisY(m_scatterDataWidget->getForm()->m_propertyYComboBox->currentText());

  //Adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Scatter"), chartStyle);
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);
  chartDisplay->show();
  chartDisplay->replot();

  //Adjusting the chart widget
  m_displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::SCATTER_CHART, chartDisplay, this->parentWidget());

  QApplication::restoreOverrideCursor();
  this->accept();
}
