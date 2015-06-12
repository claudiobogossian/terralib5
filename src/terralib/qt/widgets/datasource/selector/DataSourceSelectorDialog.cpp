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
  \file terralib/qt/widgets/connector/DataSourceSelectorDialog.cpp

  \brief A dialog for connecting to a data source.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../Exception.h"
#include "DataSourceSelectorDialog.h"
#include "DataSourceSelectorWidget.h"
#include "ui_DataSourceSelectorDialogForm.h"
#include "ui_DataSourceSelectorWidgetForm.h"

// Qt
#include <QMessageBox>
#include <QVBoxLayout>

te::qt::widgets::DataSourceSelectorDialog::DataSourceSelectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::DataSourceSelectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

  m_selectorWidget.reset(new DataSourceSelectorWidget(this, f));

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(m_selectorWidget.get(), 1);
  layout->addWidget(m_ui->layoutWidget, 0);

// connect signal and slots
  connect(m_ui->m_selectPushButton, SIGNAL(pressed()), this, SLOT(selectPushButtonPressed()));
  connect(m_selectorWidget->getForm()->m_datasourceListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(checkButtonsState()));
  connect(m_selectorWidget->getForm()->m_datasourceListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectPushButtonPressed()));

  m_ui->m_helpPushButton->setPageReference("widgets/datasource_selector/datasource_selector.html");
}

te::qt::widgets::DataSourceSelectorDialog::~DataSourceSelectorDialog()
{
}

void te::qt::widgets::DataSourceSelectorDialog::setDataSourceToUse(const QString& dsType)
{
  m_selectorWidget.get()->setDataSource(dsType);
}

const std::list<te::da::DataSourceInfoPtr>& te::qt::widgets::DataSourceSelectorDialog::getSelecteds() const
{
  return m_selecteds;
}

te::qt::widgets::DataSourceSelectorWidget* te::qt::widgets::DataSourceSelectorDialog::getSelectorWidget() const
{
  return m_selectorWidget.get();
}

void te::qt::widgets::DataSourceSelectorDialog::selectPushButtonPressed()
{
  try
  {
    m_selecteds = m_selectorWidget->getSelecteds();

    if(m_selecteds.empty())
      throw Exception(TE_TR("There is no selected data sources!"));

    accept();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));

    m_selecteds.clear();
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while selecting a data source!"));

    m_selecteds.clear();
  }
}

void te::qt::widgets::DataSourceSelectorDialog::checkButtonsState()
{
  if(m_selectorWidget.get() == 0)
    return;

  bool hasSelectedItems = !m_selectorWidget->getForm()->m_datasourceListWidget->selectedItems().empty();

  m_ui->m_selectPushButton->setEnabled(hasSelectedItems);
}

