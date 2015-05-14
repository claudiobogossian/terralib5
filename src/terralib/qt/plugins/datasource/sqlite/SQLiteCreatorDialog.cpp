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
  \file terralib/qt/widgets/connector/sqlite/SQLiteCreatorDialog.cpp

  \brief ....
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../widgets/Exception.h"
#include "ui_SQLiteCreatorDialogForm.h"
#include "SQLiteCreatorDialog.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

te::qt::plugins::sqlite::SQLiteCreatorDialog::SQLiteCreatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SQLiteCreatorDialogForm)
{
  m_ui->setupUi(this);

  connect(m_ui->m_applyPushButton, SIGNAL(pressed()), this, SLOT(applyPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_searchDatabaseToolButton, SIGNAL(pressed()), this, SLOT(searchDatabaseToolButtonPressed()));
}

te::qt::plugins::sqlite::SQLiteCreatorDialog::~SQLiteCreatorDialog()
{
}

const te::da::DataSourceInfoPtr& te::qt::plugins::sqlite::SQLiteCreatorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::sqlite::SQLiteCreatorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::sqlite::SQLiteCreatorDialog::applyPushButtonPressed()
{
  try
  {
    if(te::da::DataSourceFactory::find("SQLITE") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for SQLite data sources!"));

    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

    te::da::DataSource::create("SQLITE", dsInfo);

  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
    return;
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while opening SQLite database!"));
    return;
  }

  accept();
}

void te::qt::plugins::sqlite::SQLiteCreatorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::plugins::sqlite::SQLiteCreatorDialog::searchDatabaseToolButtonPressed()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save SQLite Database"), QString(""), tr("Database files (*.sqlite *.db);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

  if(fileName.isEmpty())
    return;

  m_ui->m_fileLineEdit->setText(fileName);
}

void te::qt::plugins::sqlite::SQLiteCreatorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
{
// clear input
  connInfo.clear();

  QString qstr = m_ui->m_fileLineEdit->text().trimmed();

  if(qstr.isEmpty())
    connInfo["SQLITE_FILE"] = ":memory:";
  else
    connInfo["SQLITE_FILE"] = qstr.toStdString();

  connInfo["SQLITE_CREATE_INTERMEDIATE_DIR"] = m_ui->m_allowIntermediateDirCheckBox->isChecked() ? "TRUE" : "FALSE";

  connInfo["SQLITE_CREATE_SPATIALITE_METADATA_TABLES"] = "TRUE";

  connInfo["SQLITE_HIDE_SPATIAL_METADATA_TABLES"] = "TRUE";
}
