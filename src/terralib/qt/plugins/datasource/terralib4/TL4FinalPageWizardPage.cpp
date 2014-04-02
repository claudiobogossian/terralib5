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

  \brief A wizard page for selecting converted datasets to layers in TerraLib 5.
*/

// TerraLib
#include "../../../../common/StringUtils.h"
#include "../../../../terralib4/ThemeInfo.h"
#include "ui_TL4FinalPageWizardPageForm.h"
#include "TL4FinalPageWizardPage.h"

Q_DECLARE_METATYPE(::terralib4::ThemeInfo);

std::vector<std::string> getNames(const std::string& names)
{
  std::vector<std::string> final;

  std::vector<std::string> tokens;
  te::common::Tokenize(names, tokens, " | ");

  final.push_back(tokens[1]);
  final.push_back(tokens[3]);
  final.push_back(tokens[5]);

  return final;
}

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

void te::qt::plugins::terralib4::TL4FinalPageWizardPage::setDataSets(const std::vector<std::string>& datasets)
{
  m_ui->m_layersListWidget->clear();

  for(std::size_t i = 0; i < datasets.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(datasets[i].c_str(), m_ui->m_layersListWidget, 0);

    item->setCheckState(Qt::Checked);

    m_ui->m_layersListWidget->addItem(item);
  }
}

std::vector<std::string> te::qt::plugins::terralib4::TL4FinalPageWizardPage::getSelected()
{
  std::vector<std::string> checked;

  for(std::size_t i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    QListWidgetItem* item = m_ui->m_layersListWidget->item(i);

    if(item->type() != 0 || item->checkState() != Qt::Checked)
      continue;

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

bool te::qt::plugins::terralib4::TL4FinalPageWizardPage::parentLayerIsSeleted(std::string layerName)
{
  for(std::size_t i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    if(m_ui->m_layersListWidget->item(i)->text() == layerName.c_str())
      return true;
  }

  return false;
}

void te::qt::plugins::terralib4::TL4FinalPageWizardPage::setThemes(const std::vector<::terralib4::ThemeInfo>& themes)
{
  for(std::size_t i = 0; i < themes.size(); ++i)
  {
    ::terralib4::ThemeInfo theme = themes[i];

    if(!parentLayerIsSeleted(theme.m_layerName))
      continue;

    std::string names = "Layer: " + theme.m_layerName + " | View: " + theme.m_viewName + " | Theme: " + theme.m_name;

    QListWidgetItem* item = new QListWidgetItem(QIcon::fromTheme("tl4-theme"), names.c_str(), m_ui->m_layersListWidget, 1);

    item->setData(Qt::UserRole, QVariant::fromValue(theme));

    item->setCheckState(Qt::Checked);

    m_ui->m_layersListWidget->addItem(item);
  }
}

std::vector<::terralib4::ThemeInfo> te::qt::plugins::terralib4::TL4FinalPageWizardPage::getSelectedThemes()
{
  std::vector<::terralib4::ThemeInfo> themes;

  for(std::size_t i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    QListWidgetItem* item = m_ui->m_layersListWidget->item(i);

    if(item->type() != 1 || item->checkState() != Qt::Checked)
      continue;

    std::vector<std::string> names = getNames(m_ui->m_layersListWidget->item(i)->text().toStdString());

    ::terralib4::ThemeInfo theme;
    theme.m_layerName = names[0];
    theme.m_viewName = names[1];
    theme.m_name = names[2];

    themes.push_back(theme);
  }

  return themes;
}
