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
  \file terralib/qt/widgets/charts/TrajectoryDialog.cpp

  \brief  A dialog used to define the basic parameters of a new TrajectoryLayer.
*/

//Terralib
#include "TrajectoryDialog.h"
#include "TemporalPropertiesWidget.h"
#include "TrajectoryPropertiesWidget.h"
#include "ui_TrajectoryDialogForm.h"
#include "ui_TemporalPropertiesWidgetForm.h"

te::qt::widgets::TrajectoryDialog::TrajectoryDialog(te::map::AbstractLayerPtr layer, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::TrajectoryDialogForm)
{
  m_ui->setupUi(this);

  // Time Properties Widgets
  te::da::DataSet* dataset = layer->getData().release();

  m_propertiesWidget.reset(new te::qt::widgets::TemporalPropertiesWidget(dataset, this));
  m_uniquePropWidget.reset(new te::qt::widgets::TrajectoryPropertiesWidget(dataset, this));

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_propertiesWidgetFrame);
  layout->addWidget(m_propertiesWidget.get());
  layout->addWidget(m_uniquePropWidget.get());
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

  //connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::TrajectoryDialog::~TrajectoryDialog()
{
}

void te::qt::widgets::TrajectoryDialog::onHelpPushButtonClicked(){}

void te::qt::widgets::TrajectoryDialog::onOkPushButtonClicked()
{

}
