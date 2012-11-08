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
  \file terralib/qt/widgets/connector/wfs/WFSConnectorDialog.cpp

  \brief ....
*/

// TerraLib
#include "../../../../../common/Translator.h"
#include "../../../../../dataaccess/datasource/DataSource.h"
#include "../../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../Exception.h"
#include "../../core/DataSource.h"
#include "ui_WFSConnectorDialogForm.h"
#include "WFSConnectorDialog.h"

// Qt
#include <QtGui/QMessageBox>

te::qt::widgets::WFSConnectorDialog::WFSConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::WFSConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
}

te::qt::widgets::WFSConnectorDialog::~WFSConnectorDialog()
{
}

const te::qt::widgets::DataSourcePtr& te::qt::widgets::WFSConnectorDialog::getDataSource() const
{
  return m_datasource;
}

void te::qt::widgets::WFSConnectorDialog::set(const DataSourcePtr& ds)
{
  m_datasource = ds;
}

void te::qt::widgets::WFSConnectorDialog::openPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::widgets::WFSConnectorDialog::testPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::widgets::WFSConnectorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

