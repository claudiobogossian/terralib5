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
  \file \file MainWindow.cpp

  \brief A simple main window to show example of TerraLib Qt Datasource components.
*/

// Example
#include "MainWindow.h"

// TerraLib
#include <terralib/qt/plugins/datasource/gdal/GDALConnectorDialog.h>
#include <terralib/qt/plugins/datasource/ogr/OGRConnectorDialog.h>
#include <terralib/qt/plugins/datasource/pgis/PostGISConnectorDialog.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/postgis/Utils.h>
#include <terralib/gdal/Utils.h>
#include <terralib/ogr/Utils.h>
// Qt
#include <QAction>
#include <QActionGroup>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>

// STL
#include <cassert>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f)
  : QMainWindow(parent, f)
{
  // Creates the tool bar
  m_toolBar = addToolBar("Datasources");

  // Creates the status bar
  statusBar();

  // Setups the tool actions
  setupActions();

  // Adjusting
  setWindowTitle(tr("TerraLib Qt DataSource Example"));
  setMinimumSize(60, 60);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupActions()
{
  m_openGDAL = new QAction(tr("GDAL"), this);
  m_openGDAL->setCheckable(true);
  connect(m_openGDAL, SIGNAL(triggered()), SLOT(onOpenGDALTriggered()));

  m_openOGR = new QAction(tr("OGR"), this);
  m_openOGR->setCheckable(true);
  connect(m_openOGR, SIGNAL(triggered()), SLOT(onOpenOGRTriggered()));
  
  m_openPostGIS = new QAction(tr("POSTGIS"), this);
  m_openPostGIS->setCheckable(true);
  connect(m_openPostGIS, SIGNAL(triggered()), SLOT(onOpenPostGISTriggered()));

  
  m_toolBar->addAction(m_openGDAL);
  m_toolBar->addAction(m_openOGR);
  m_toolBar->addAction(m_openPostGIS);
  
  QActionGroup* toolsGroup = new QActionGroup(this);
  toolsGroup->addAction(m_openGDAL);
  toolsGroup->addAction(m_openOGR); 
  toolsGroup->addAction(m_openPostGIS);
}


void MainWindow::onOpenPostGISTriggered()
{
  te::qt::plugins::pgis::PostGISConnectorDialog* pgisDialog = new te::qt::plugins::pgis::PostGISConnectorDialog(this);

  if (pgisDialog->exec() == QDialog::Accepted)
  {
    std::string connstr = te::pgis::MakeConnectionStr(pgisDialog->getDriver()->getConnectionInfo());
    QMessageBox msgBox;
    QString mess = QString("DS Connection string: %1").arg(connstr.c_str());

    msgBox.setText(mess);
    msgBox.exec();

  }
}

void MainWindow::onOpenOGRTriggered()
{
  te::qt::plugins::ogr::OGRConnectorDialog* ogrDialog = new te::qt::plugins::ogr::OGRConnectorDialog();
  
  if (ogrDialog->exec() == QDialog::Accepted)
  {
    std::string conStr = te::ogr::GetOGRConnectionInfo(ogrDialog->getDriver()->getConnectionInfo());
    QMessageBox msgBox;
    QString mess = QString("DS Connection string: %1").arg(conStr.c_str()); 
    msgBox.setText(mess);
    msgBox.exec();
  }
}

void MainWindow::onOpenGDALTriggered()
{
  te::qt::plugins::gdal::GDALConnectorDialog* gdalDialog = new te::qt::plugins::gdal::GDALConnectorDialog();
  
  if (gdalDialog->exec() == QDialog::Accepted)
  {
    std::string connstr = te::gdal::GetGDALConnectionInfo(gdalDialog->getDriver()->getConnectionInfo());
    QMessageBox msgBox;
    QString mess = QString("DS Connection string: %1").arg(connstr.c_str()); 
    msgBox.setText(mess);
    msgBox.exec();
  }
}
