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
#include <terralib/qt/widgets/datasource/connector/gdal/GDALConnectorDialog.h>
#include <terralib/qt/widgets/datasource/connector/ogr/OGRConnectorDialog.h>
#include <terralib/qt/widgets/datasource/connector/postgis/PostGISConnectorDialog.h>
#include <terralib/qt/widgets/datasource/selector/DataSourceSelectorDialog.h>

// Qt
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>

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
  
  m_openPostGIS = new QAction(tr("PostGIS"), this);
  m_openPostGIS->setCheckable(true);
  connect(m_openPostGIS, SIGNAL(triggered()), SLOT(onOpenPostGISTriggered()));

  m_openDataSourceSelector = new QAction(tr("DataSource Selector"), this);
  m_openDataSourceSelector->setCheckable(true);
  connect(m_openPostGIS, SIGNAL(triggered()), SLOT(onOpenDatasourceSelectorTriggered()));
  
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
  te::qt::widgets::PostGISConnectorDialog* pgisDialog = new te::qt::widgets::PostGISConnectorDialog(this);
  
  while (pgisDialog->exec() == QDialog::Accepted)
  {
    te::qt::widgets::DataSourcePtr dsP = pgisDialog->getDataSource();
    
    te::da::DataSourcePtr daDsP = pgisDialog->getDriver();
    
    //    QMessageBox msgBox;
    //    QString mess = QString("SRId: %1, Authority: %2").arg(srid.first).arg(srid.second.c_str()); 
    //    msgBox.setText(mess);
    //    msgBox.exec();
  }
}

void MainWindow::onOpenOGRTriggered()
{
  te::qt::widgets::OGRConnectorDialog* ogrDialog = new te::qt::widgets::OGRConnectorDialog();
  
  while (ogrDialog->exec() == QDialog::Accepted)
  {
    te::qt::widgets::DataSourcePtr dsP = ogrDialog->getDataSource();
    
    te::da::DataSourcePtr daDsP = ogrDialog->getDriver();
    
    //    QMessageBox msgBox;
    //    QString mess = QString("SRId: %1, Authority: %2").arg(srid.first).arg(srid.second.c_str()); 
    //    msgBox.setText(mess);
    //    msgBox.exec();
  }
}

void MainWindow::onOpenGDALTriggered()
{
  te::qt::widgets::GDALConnectorDialog* gdalDialog = new te::qt::widgets::GDALConnectorDialog();
  
  while (gdalDialog->exec() == QDialog::Accepted)
  {
    te::qt::widgets::DataSourcePtr dsP = gdalDialog->getDataSource();
    
    te::da::DataSourcePtr daDsP = gdalDialog->getDriver();
    
    //    QMessageBox msgBox;
    //    QString mess = QString("SRId: %1, Authority: %2").arg(srid.first).arg(srid.second.c_str()); 
    //    msgBox.setText(mess);
    //    msgBox.exec();
  }
}

void MainWindow::onOpenDataSourceSelectorTriggered()
{
  std::unique_ptr<te::qt::widgets::DataSourceSelectorDialog> selector(new te::qt::widgets::DataSourceSelectorDialog(this));

  selector->exec();
}
