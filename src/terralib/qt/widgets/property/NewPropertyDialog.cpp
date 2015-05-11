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
  \file terralib/qt/widgets/property/NewPropertyDialog.cpp

  \brief A class used to define the NewPropertyDialog class.
*/

// TerraLib
#include "../../../datatype/SimpleProperty.h"
#include "NewPropertyDialog.h"
#include "NewPropertyWidget.h"
#include "ui_NewPropertyDialogForm.h"
#include "ui_NewPropertyWidgetForm.h"

// Qt
#include <QMessageBox>
#include <QGridLayout>


te::qt::widgets::NewPropertyDialog::NewPropertyDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::NewPropertyDialogForm),
    m_propWidget(0)
{
  m_ui->setupUi(this);

  //build form
  m_propWidget = new te::qt::widgets::NewPropertyWidget(m_ui->m_widget);

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);

  layout->addWidget(m_propWidget);

  //connects
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
}

te::qt::widgets::NewPropertyDialog::~NewPropertyDialog()
{
}

void te::qt::widgets::NewPropertyDialog::setDataSourceId(std::string id)
{
  m_propWidget->setDataSourceId(id);
}

te::dt::SimpleProperty* te::qt::widgets::NewPropertyDialog::getProperty()
{
  return m_propWidget->getProperty();
}

void te::qt::widgets::NewPropertyDialog::onOkPushButtonClicked()
{
  //creates a defaul simple property
  if(m_propWidget->buildProperty() == false)
    return;

  accept();
}
