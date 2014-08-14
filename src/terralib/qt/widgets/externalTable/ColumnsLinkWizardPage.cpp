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

// TerraLib
#include "ui_ColumnsLinkWidgetForm.h"
#include "ColumnsLinkWidget.h"
#include "ColumnsLinkWizardPage.h"


te::qt::widgets::ColumnsLinkWizardPage::ColumnsLinkWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_widget.reset(new ColumnsLinkWidget(this));

  QGridLayout* layout = new QGridLayout(this);

  layout->addWidget(m_widget.get());

  //configure page
  this->setTitle(tr("Columns' link"));
  this->setSubTitle(tr("Select the columns that will be used to link the datasets"));

  //connect slots
  connect(m_widget.get(), SIGNAL(itemChanged()), this, SLOT(onItemChanged()));
}

te::qt::widgets::ColumnsLinkWizardPage::~ColumnsLinkWizardPage()
{
}

bool te::qt::widgets::ColumnsLinkWizardPage::isComplete() const
{
  return true;
}

te::qt::widgets::ColumnsLinkWidget* te::qt::widgets::ColumnsLinkWizardPage::getWidget() const
{
  return m_widget.get();
}

void te::qt::widgets::ColumnsLinkWizardPage::onItemChanged()
{
  emit completeChanged();
}