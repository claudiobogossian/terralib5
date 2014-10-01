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
  \file terralib/qt/widgets/charts/ObservationDialog.cpp

  \brief  A dialog used to define the basic parameters of a new ObservationLayer.
*/

//Terralib
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "../utils/DoubleListWidget.h"
#include "ObservationDialog.h"
#include "TemporalPropertiesWidget.h"
#include "ui_DoubleListWidgetForm.h"
#include "ui_ObservationDialogForm.h"
#include "ui_TemporalPropertiesWidgetForm.h"

te::qt::widgets::ObservationDialog::ObservationDialog(te::map::AbstractLayerPtr layer, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ObservationDialogForm)
{
  m_ui->setupUi(this);

  // Time Properties Widget
  te::da::DataSet* dataset = layer->getData().release();
  m_propertiesWidget.reset(new te::qt::widgets::TemporalPropertiesWidget(dataset, this));

  //Observed Properties
  std::vector<std::string> properties;

  // Creating the properties list. DATETIME_TYPE properties can not be included.
  for (std::size_t i = 0; i < dataset->getNumProperties(); i++)
  {
    if(dataset->getPropertyDataType(i) != te::dt::DATETIME_TYPE)
    {
      properties.push_back(dataset->getPropertyName(i));
    }
  }

  m_observedPropWidget.reset(new DoubleListWidget(this));
  m_observedPropWidget->getForm()->m_leftItemsLabel->setText(tr(""));
  m_observedPropWidget->getForm()->m_rightItemsLabel->setText(tr(""));
  m_observedPropWidget->setInputValues(properties);

  // Adjusting the properties' widgets
  QGridLayout* propLayout = new QGridLayout(m_ui->m_propertiesWidgetFrame);
  QVBoxLayout *vboxLayout = new QVBoxLayout;

  propLayout->addWidget(m_propertiesWidget.get());
  vboxLayout->addWidget(m_observedPropWidget.get());
  m_ui->m_uniquePropertiesGroupBox->setLayout(vboxLayout);
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

  //connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::ObservationDialog::~ObservationDialog()
{
}

void te::qt::widgets::ObservationDialog::onHelpPushButtonClicked(){}

void te::qt::widgets::ObservationDialog::onOkPushButtonClicked()
{

}
