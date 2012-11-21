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
  \file terralib/qt/widgets/connector/wcs/WCSConnectorDialog.cpp

  \brief ....
*/

// TerraLib
#include <terralib/common/Translator.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/qt/widgets/Exception.h>
#include <terralib/qt/widgets/datasource/core/DataSource.h>
#include "ui_WCSConnectorDialogForm.h"
#include "WCSConnectorDialog.h"

// Qt
#include <QtGui/QMessageBox>

namespace te_qt = te::qt::widgets;
namespace plg_wcs = qt_af::plugin::wcs;

plg_wcs::WCSConnectorDialog::WCSConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::WCSConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
}

plg_wcs::WCSConnectorDialog::~WCSConnectorDialog()
{
}

const te::qt::widgets::DataSourcePtr& plg_wcs::WCSConnectorDialog::getDataSource() const
{
  return m_datasource;
}

void plg_wcs::WCSConnectorDialog::set(const te_qt::DataSourcePtr& ds)
{
  m_datasource = ds;
}

void plg_wcs::WCSConnectorDialog::openPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void plg_wcs::WCSConnectorDialog::testPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void plg_wcs::WCSConnectorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

