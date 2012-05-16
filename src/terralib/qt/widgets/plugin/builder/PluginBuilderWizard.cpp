/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/plugin/builder/PluginBuilderWizard.cpp

  \brief A Qt dialog that allows users to create new plugins based on TerraLib framework.
*/

// TerraLib
#include "../../utils/ui/DoubleListWidgetForm.h"
#include "../../utils/DoubleListWidget.h"
#include "ui/PluginBuilderWizardForm.h"
#include "PluginBuilderWizard.h"

// Qt
#include <QtGui/QVBoxLayout>

te::qt::widgets::PluginBuilderWizard::PluginBuilderWizard(QWidget* parent)
  : QWizard(parent),
    m_ui(new Ui::PluginBuilderWizardForm)
{
  m_ui->setupUi(this);

  setButtonText(QWizard::CustomButton1, tr("&Settings"));

// plugin dependencies
  m_pluginDependencies.reset(new DoubleListWidget(m_ui->m_pluginDependenciesPage));
  m_pluginDependencies->getForm()->m_leftItemsLabel->setText(tr("Available Plugins"));
  m_pluginDependencies->getForm()->m_rightItemsLabel->setText(tr("Required Plugins"));
  m_ui->m_pluginDependenciesGridLayout->addWidget(m_pluginDependencies.get());

// category dependencies
  m_categoryDependencies.reset(new DoubleListWidget(m_ui->m_categoryDependenciesPage));
  m_categoryDependencies->getForm()->m_leftItemsLabel->setText(tr("Available plugin categories"));
  m_categoryDependencies->getForm()->m_rightItemsLabel->setText(tr("Required categories for your plugin"));
  m_ui->m_categoryDependenciesGridLayout->addWidget(m_categoryDependencies.get());

// module dependencies
  m_moduleDependencies.reset(new DoubleListWidget(m_ui->m_moduleDependenciesPage));
  m_moduleDependencies->getForm()->m_leftItemsLabel->setText(tr("Available modules"));
  m_moduleDependencies->getForm()->m_rightItemsLabel->setText(tr("Required modules for your plugin"));
  m_ui->m_moduleDependenciesGridLayout->addWidget(m_moduleDependencies.get());
}

te::qt::widgets::PluginBuilderWizard::~PluginBuilderWizard()
{
}
