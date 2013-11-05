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
  \file  terralib/qt/widgets/externalTable/DataPropertiesDialog.h

  \brief A dialog used to configure the properties of a new textual file based layer
*/

//Terralib
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "DataPropertiesDialog.h"
#include "DataPropertiesWidget.h"
#include "ui_DataPropertiesDialogForm.h"
#include "ui_DataPropertiesWidgetForm.h"


te::qt::widgets::DataPropertiesDialog::DataPropertiesDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::DataPropertiesDialogForm)
{
  m_ui->setupUi(this);

  // data properties Widget
  m_dataPropertiesWidget = new te::qt::widgets::DatapPropertiesWidget(this, 0);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_dataWidgetFrame);
  layout->addWidget(m_dataPropertiesWidget);
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::DataPropertiesDialog::~DataPropertiesDialog()
{
  delete m_dataPropertiesWidget;
}

void te::qt::widgets::DataPropertiesDialog::onHelpPushButtonClicked(){}

void te::qt::widgets::DataPropertiesDialog::onOkPushButtonClicked()
{
  m_datasetAdapter = m_dataPropertiesWidget->getAdapter();
  this->accept();
}

//te::map::DataSetAdapterLayerPtr te::qt::widgets::DataPropertiesDialog::getTextualLayer()
//{
//  return m_dataSetLayer;
//}