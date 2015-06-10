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
  \file terralib/qt/plugins/terralib4/TL4ThemeSelectionWizardPage.cpp

  \brief A wizard page for selecting converted datasets to layers in TerraLib 5.
*/

// TerraLib
#include "../../../../common/StringUtils.h"
#include "../../../../terralib4/ThemeInfo.h"
#include "ui_TL4ThemeSelectionWizardPageForm.h"
#include "TL4ThemeSelectionWizardPage.h"

Q_DECLARE_METATYPE(::terralib4::ThemeInfo);



te::qt::plugins::terralib4::TL4ThemeSelectionWizardPage::TL4ThemeSelectionWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::TL4ThemeSelectionWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

  connect(m_ui->m_selectAllPushButton, SIGNAL(clicked()), this, SLOT(onSelectAllPushButtonClicked()));
  connect(m_ui->m_deselectAllPushButton, SIGNAL(clicked()), this, SLOT(onDeselectAllPushButtonClicked()));
}

te::qt::plugins::terralib4::TL4ThemeSelectionWizardPage::~TL4ThemeSelectionWizardPage()
{
}

void te::qt::plugins::terralib4::TL4ThemeSelectionWizardPage::onSelectAllPushButtonClicked()
{
  for(int i = 0; i < m_ui->m_themeListWidget->count(); ++i)
  {
    Qt::CheckState state = m_ui->m_themeListWidget->item(i)->checkState();

    if(state != Qt::Checked)
      m_ui->m_themeListWidget->item(i)->setCheckState(Qt::Checked);
  }
}

void te::qt::plugins::terralib4::TL4ThemeSelectionWizardPage::onDeselectAllPushButtonClicked()
{
  for(int i = 0; i < m_ui->m_themeListWidget->count(); ++i)
  {
    Qt::CheckState state = m_ui->m_themeListWidget->item(i)->checkState();

    if(state == Qt::Checked)
      m_ui->m_themeListWidget->item(i)->setCheckState(Qt::Unchecked);
  }
}

void te::qt::plugins::terralib4::TL4ThemeSelectionWizardPage::setThemes(const std::vector<::terralib4::ThemeInfo>& themes)
{
  for(std::size_t i = 0; i < themes.size(); ++i)
  {
    ::terralib4::ThemeInfo theme = themes[i];

    std::string names = "Layer: " + theme.m_layerName + " | View: " + theme.m_viewName + " | Theme: " + theme.m_name;

    QListWidgetItem* item = new QListWidgetItem(QIcon::fromTheme("tl4-theme"), QString::fromLatin1(names.c_str()), m_ui->m_themeListWidget, 1);

    item->setData(Qt::UserRole, QVariant::fromValue(theme));

    item->setCheckState(Qt::Checked);

    m_ui->m_themeListWidget->addItem(item);
  }
}

std::vector<::terralib4::ThemeInfo> te::qt::plugins::terralib4::TL4ThemeSelectionWizardPage::getThemes()
{
  std::vector<::terralib4::ThemeInfo> themes;

  for(int i = 0; i < m_ui->m_themeListWidget->count(); ++i)
  {
    QListWidgetItem* item = m_ui->m_themeListWidget->item(i);

    if(item->type() != 1 || item->checkState() != Qt::Checked)
      continue;

    std::string aux = m_ui->m_themeListWidget->item(i)->text().toLatin1();

    std::vector<std::string> names = getNames(aux);

    ::terralib4::ThemeInfo theme;
    theme.m_layerName = names[0];
    theme.m_viewName = names[1];
    theme.m_name = names[2];

    themes.push_back(theme);
  }

  return themes;
}

std::vector<std::string> te::qt::plugins::terralib4::TL4ThemeSelectionWizardPage::getNames(const std::string& names)
{
  std::vector<std::string> final;

  std::vector<std::string> tokens;
  te::common::Tokenize(names, tokens, " | ");

  final.push_back(tokens[1]);
  final.push_back(tokens[3]);
  final.push_back(tokens[5]);

  return final;
}