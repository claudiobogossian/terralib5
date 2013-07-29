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
  \file terralib/qt/widgets/slider/TimePropertiesDialog.h

  \brief A widget used to alow the user to control the visualization of temporal data
*/


//Terralib
#include "TimePropertiesDialog.h"
#include "ui_TimePropertiesDialogForm.h"

te::qt::widgets::TimePropertiesDialog::TimePropertiesDialog(QWidget* parent,  Qt::WindowFlags f, int minimum, int maximum)
  : QDialog(parent, f),
    m_ui(new Ui::TimePropertiesDialogForm)
{
    m_ui->setupUi(this);

  //Adjusting the slider's range
  m_ui->m_speedHorizontalSlider->setMinimum(minimum);
  m_ui->m_speedHorizontalSlider->setMaximum(maximum);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::TimePropertiesDialog::~TimePropertiesDialog()
{
}

int te::qt::widgets::TimePropertiesDialog::getValue()
{
  return m_ui->m_speedHorizontalSlider->value();
}

bool te::qt::widgets::TimePropertiesDialog::isLoopChecked()
{
  return m_ui->m_loopCheckBox->isChecked();
}

void te::qt::widgets::TimePropertiesDialog::onHelpPushButtonClicked()
{
}

void te::qt::widgets::TimePropertiesDialog::onOkPushButtonClicked()
{
  this->accept();
}