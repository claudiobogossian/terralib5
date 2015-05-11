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
  \file terralib/qt/widgets/query/LayerAttributesWizardPage.cpp

  \brief This file has the LayerAttributesWizardPage class.
*/

// TerraLib
#include "LayerAttributesWizardPage.h"
#include "ui_LayerAttributesWidgetForm.h"

te::qt::widgets::LayerAttributesWizardPage::LayerAttributesWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::LayerAttributesWidgetForm)
{
  m_ui->setupUi(this);

  //configure page
  this->setTitle(tr("Layer Attributes"));
  this->setSubTitle(tr("Used to define all layer attributes."));

  connect(m_ui->m_layerNameLineEdit, SIGNAL(textChanged(QString)), SLOT(layerNameChanged(QString)));
}

te::qt::widgets::LayerAttributesWizardPage::~LayerAttributesWizardPage()
{
}

bool te::qt::widgets::LayerAttributesWizardPage::isComplete() const
{
  if(m_ui->m_layerNameLineEdit->text().isEmpty())
    return false;

  return true;
}

std::string te::qt::widgets::LayerAttributesWizardPage::getLayerName()
{
  std::string name = "";

  if(m_ui->m_layerNameLineEdit->text().isEmpty() == false)
  {
    name = m_ui->m_layerNameLineEdit->text().toStdString();
  }

  return name;
}

void te::qt::widgets::LayerAttributesWizardPage::layerNameChanged(QString)
{
  emit completeChanged();
}
