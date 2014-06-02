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
  \file terralib/qt/widgets/datasource/selector/DataSourceSelectorWizardPage.cpp

  \brief ...
*/

// TerraLib
#include "DataSourceSelectorWidget.h"
#include "DataSourceSelectorWizardPage.h"
#include "ui_DataSourceSelectorWidgetForm.h"

// Qt
#include <QVBoxLayout>

te::qt::widgets::DataSourceSelectorWizardPage::DataSourceSelectorWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_selector.reset(new DataSourceSelectorWidget(this));

  QVBoxLayout* layout = new QVBoxLayout(this);

  layout->addWidget(m_selector.get());

  connect(m_selector->getForm()->m_datasourceListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(dataSourceSelectionChanged()));

  //configure page
  this->setTitle(tr("Data Source Selector"));
  this->setSubTitle(tr("Select the datasource to be explored."));
}

te::qt::widgets::DataSourceSelectorWizardPage::~DataSourceSelectorWizardPage()
{
}

bool te::qt::widgets::DataSourceSelectorWizardPage::isComplete() const
{
  return !m_selector->getForm()->m_datasourceListWidget->selectedItems().empty();
}

te::qt::widgets::DataSourceSelectorWidget* te::qt::widgets::DataSourceSelectorWizardPage::getSelectorWidget() const
{
  return m_selector.get();
}

void te::qt::widgets::DataSourceSelectorWizardPage::dataSourceSelectionChanged()
{
  emit completeChanged();
}
