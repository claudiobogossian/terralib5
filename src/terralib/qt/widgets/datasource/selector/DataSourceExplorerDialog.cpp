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
  \file terralib/qt/widgets/connector/DataSourceExplorerDialog.cpp

  \brief A dialog for connecting to a data source.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../Exception.h"
#include "DataSourceExplorerDialog.h"
#include "DataSourceSelectorWidget.h"
#include "ui_DataSourceExplorerDialogForm.h"
#include "ui_DataSourceSelectorWidgetForm.h"

// Qt
#include <QMessageBox>
#include <QVBoxLayout>

te::qt::widgets::DataSourceExplorerDialog::DataSourceExplorerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::DataSourceExplorerDialogForm)
{
// add controls
  m_ui->setupUi(this);

  m_selectorWidget.reset(new DataSourceSelectorWidget(this, f));

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(m_selectorWidget.get(), 1);
  layout->addWidget(m_ui->layoutWidget, 0);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(pressed()), this, SLOT(okPushButtonPressed()));
  connect(m_selectorWidget->getForm()->m_datasourceListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(okPushButtonPressed()));

  m_ui->m_helpPushButton->setPageReference("widgets/datasource_explorer/datasource_explorer.html");
}

te::qt::widgets::DataSourceExplorerDialog::~DataSourceExplorerDialog()
{
}

void te::qt::widgets::DataSourceExplorerDialog::setDataSourceToUse(const QString& dsType)
{
  m_selectorWidget.get()->setDataSource(dsType);
}

const std::list<te::da::DataSourceInfoPtr>& te::qt::widgets::DataSourceExplorerDialog::getSelecteds() const
{
  return m_selecteds;
}

te::qt::widgets::DataSourceSelectorWidget* te::qt::widgets::DataSourceExplorerDialog::getSelectorWidget() const
{
  return m_selectorWidget.get();
}

void te::qt::widgets::DataSourceExplorerDialog::okPushButtonPressed()
{
  try
  {
    m_selecteds = m_selectorWidget->getSelecteds();

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