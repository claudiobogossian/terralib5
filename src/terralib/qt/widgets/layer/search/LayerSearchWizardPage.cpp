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
  \file terralib/qt/widgets/layer/search/LayerSearchWizardPage.cpp

  \brief This file defines the LayerSearchWizardPage class.
*/

// TerraLib
#include "ui_LayerSearchWidgetForm.h"
#include "LayerSearchWidget.h"
#include "LayerSearchWizardPage.h"

// Qt
#include <QGridLayout>

te::qt::widgets::LayerSearchWizardPage::LayerSearchWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_widget.reset(new LayerSearchWidget(this));

  QGridLayout* layout = new QGridLayout(this);

  layout->addWidget(m_widget.get());

//connect
  connect(m_widget->getForm()->m_treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));

//configure page
  this->setTitle(tr("Layer Search"));
  this->setSubTitle(tr("Allows selection of layers using filters for selection."));
}

te::qt::widgets::LayerSearchWizardPage::~LayerSearchWizardPage()
{
}

bool te::qt::widgets::LayerSearchWizardPage::isComplete() const
{
  if(m_widget->getForm()->m_treeWidget->selectedItems().empty())
    return false;

  return true;
}

te::qt::widgets::LayerSearchWidget* te::qt::widgets::LayerSearchWizardPage::getSearchWidget() const
{
  return m_widget.get();
}

void te::qt::widgets::LayerSearchWizardPage::onItemSelectionChanged()
{
  emit completeChanged();
}
