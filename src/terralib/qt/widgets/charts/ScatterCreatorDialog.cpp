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
  \file terralib/qt/widgets/charts/ScatterCreatorDialog.cpp

  \brief AA dialog used to create a nem Scatter chart.
*/

//Terralib
#include "ui_scatterCreatorDialogForm.h"
#include "ChartStyleDialog.h"
#include "ChartDisplay.h"
#include "../../../dataaccess.h"
#include "../../../qt/widgets/charts/Utils.h"
#include "../../../datatype/Property.h"
#include <PointSymbolizerWidget.h>
#include "Scatter.h"
#include "ScatterChart.h"
#include "ScatterCreatorDialog.h"
#include "Symbol.h"

te::qt::widgets::ScatterCreatorDialog::ScatterCreatorDialog(te::da::DataSet* dataSet, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ScatterCreatorDialogForm),
    m_dataSet (dataSet)
{
    m_ui->setupUi(this);

  QString item;
  std::vector<te::dt::Property*>& properties = dataSet->getType()->getProperties();

  for (double i = 0; i < properties.size(); i++)
  {
    item = item.fromStdString(properties[i]->getName());
    m_ui->m_propertyXComboBox->addItem(item);
    m_ui->m_propertyYComboBox->addItem(item);
  }

// connect signal and slots
  connect(m_ui->m_stylePushButton, SIGNAL(clicked()), this, SLOT(onStylePushButtonClicked()));
  connect(m_ui->m_plotStylePushButton, SIGNAL(clicked()), this, SLOT(onPlotStylePushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::ScatterCreatorDialog::~ScatterCreatorDialog(){}

void te::qt::widgets::ScatterCreatorDialog::onStylePushButtonClicked()
{
    te::qt::widgets::ChartStyleDialog dlg(this, 0, "Scattter", m_ui->m_propertyXComboBox->currentText(), m_ui->m_propertyYComboBox->currentText());
    dlg.exec();
}

void te::qt::widgets::ScatterCreatorDialog::onPlotStylePushButtonClicked()
{
  QDialog dlg;
  dlg.setWindowTitle("Symbolizer Widgets Example");

  // Point Symbolizer Widget
  te::qt::widgets::PointSymbolizerWidget* pts = new te::qt::widgets::PointSymbolizerWidget(&dlg);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(&dlg);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(pts);

  dlg.exec();
//  m_symbol->setSymbolizer(1, static_cast<PointSymbolizerWidget*>(pts)->getSymbolizer());
}

void te::qt::widgets::ScatterCreatorDialog::onOkPushButtonClicked()
{
  //Acquiring the dataset Properties types
  m_type = m_dataSet->getType();

  QString aux = m_ui->m_propertyXComboBox->currentText();

  QString aux2 = m_ui->m_propertyYComboBox->currentText();

  std::string selectedPropertyX = aux.toStdString();
  std::string selectedPropertyY = aux2.toStdString();

  int selectedPropertyXIdx= m_type->getPropertyPosition(selectedPropertyX);
  int selectedPropertyYIdx= m_type->getPropertyPosition(selectedPropertyY);

  m_scatter = te::qt::widgets::createScatter(m_dataSet, selectedPropertyXIdx, selectedPropertyYIdx);

  m_scatterChart = new te::qt::widgets::ScatterChart(m_scatter);

  QDialog dlg(this);
  QGridLayout* lay = new QGridLayout(&dlg);
  dlg.setLayout(lay);

  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(&dlg);

  chartDisplay->setTitle("Scatter");

  lay->addWidget(chartDisplay);

  m_scatterChart->attach(chartDisplay);

  chartDisplay->show();

  chartDisplay->replot();

  this->close();
  dlg.exec();
}

void te::qt::widgets::ScatterCreatorDialog::onHelpPushButtonClicked(){}