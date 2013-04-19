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
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "HistogramChart.h"
#include "HistogramDataWidget.h"
#include "HistogramDialog.h"
#include "ui_histogramDialogForm.h"

//QT
#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>

te::qt::widgets::HistogramDialog::HistogramDialog(te::da::DataSet* dataSet, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::HistogramDialogForm)
{
  m_ui->setupUi(this);

  m_chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Histogram"));

  // Histogram data Widget
  m_histogramDataWidget = new te::qt::widgets::HistogramDataWidget(dataSet, this, 0);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_dataWidgetFrame);
  layout->addWidget(m_histogramDataWidget);
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::HistogramDialog::~HistogramDialog(){}

void te::qt::widgets::HistogramDialog::onHelpPushButtonClicked(){}

void te::qt::widgets::HistogramDialog::onOkPushButtonClicked()
{
  te::qt::widgets::Histogram* histogram = m_histogramDataWidget->getHistogram();
  m_histogramChart = new te::qt::widgets::HistogramChart(histogram);

  QDialog dlg(this);
  QGridLayout* lay = new QGridLayout(&dlg);
  dlg.setLayout(lay);

  m_chartDisplay->setParent(&dlg);

  lay->addWidget(m_chartDisplay);

  m_histogramChart->attach(m_chartDisplay);

  m_chartDisplay->show();
  m_chartDisplay->replot();

  this->close();
  dlg.exec();
}