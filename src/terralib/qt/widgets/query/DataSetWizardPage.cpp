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
  \file terralib/qt/widgets/query/DataSetWizardPage.cpp

  \brief This file has the DataSetWizardPage class.
*/

// TerraLib
#include "ui_DataSetWidgetForm.h"
#include "DataSetWidget.h"
#include "DataSetWizardPage.h"

// Qt
#include <QGridLayout>


te::qt::widgets::DataSetWizardPage::DataSetWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_widget.reset(new DataSetWidget(this));

  QGridLayout* layout = new QGridLayout(this);

  layout->addWidget(m_widget.get());

  //configure page
  this->setTitle(tr("Data Set Selection"));
  this->setSubTitle(tr("Select the datasets to be searched."));

  //connect slots
  connect(m_widget.get(), SIGNAL(itemChanged()), this, SLOT(onItemChanged()));
}

te::qt::widgets::DataSetWizardPage::~DataSetWizardPage()
{
}

bool te::qt::widgets::DataSetWizardPage::isComplete() const
{
  int numRows = m_widget->getForm()->m_dataSetTableWidget->rowCount();

  if(numRows == 0)
    return false;

  return true;
}

te::qt::widgets::DataSetWidget* te::qt::widgets::DataSetWizardPage::getWidget() const
{
  return m_widget.get();
}

void te::qt::widgets::DataSetWizardPage::onItemChanged()
{
  emit completeChanged();
}
