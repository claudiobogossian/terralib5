/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/plugins/terralib4/TL4FinalPageWizardPage.cpp

  \brief A Qt dialog for ....
*/

// TerraLib
#include "ui_TL4FinalPageWizardPageForm.h"
#include "TL4FinalPageWizardPage.h"

te::qt::plugins::terralib4::TL4FinalPageWizardPage::TL4FinalPageWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::TL4FinalPageWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

  connect(m_ui->m_selectAllPushButton, SIGNAL(clicked()), this, SLOT(onSelectAllPushButtonClicked()));
  connect(m_ui->m_deselectAllPushButton, SIGNAL(clicked()), this, SLOT(onDeselectAllPushButtonClicked()));
}

te::qt::plugins::terralib4::TL4FinalPageWizardPage::~TL4FinalPageWizardPage()
{
}

void te::qt::plugins::terralib4::TL4FinalPageWizardPage::setDataSets(std::vector<std::string> datasets)
{
  m_ui->m_layersListWidget->clear();

  for(std::size_t i = 0; i < datasets.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(datasets[i].c_str(), m_ui->m_layersListWidget);
    item->setCheckState(Qt::Checked);
    m_ui->m_layersListWidget->addItem(item);
  }
}

std::vector<std::string> te::qt::plugins::terralib4::TL4FinalPageWizardPage::getSelected()
{
  std::vector<std::string> checked;
  for(std::size_t i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    if(m_ui->m_layersListWidget->item(i)->checkState() == Qt::Checked)
      checked.push_back(m_ui->m_layersListWidget->item(i)->text().toStdString());
  }

  return checked;
}

void te::qt::plugins::terralib4::TL4FinalPageWizardPage::onSelectAllPushButtonClicked()
{
  for(std::size_t i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    Qt::CheckState state = m_ui->m_layersListWidget->item(i)->checkState();
    if(state != Qt::Checked)
      m_ui->m_layersListWidget->item(i)->setCheckState(Qt::Checked);
  }
}

void te::qt::plugins::terralib4::TL4FinalPageWizardPage::onDeselectAllPushButtonClicked()
{
  for(std::size_t i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    Qt::CheckState state = m_ui->m_layersListWidget->item(i)->checkState();
    if(state == Qt::Checked)
      m_ui->m_layersListWidget->item(i)->setCheckState(Qt::Unchecked);
  }
}