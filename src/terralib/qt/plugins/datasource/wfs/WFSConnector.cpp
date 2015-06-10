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
  \file terralib/qt/plugins/datasource/wfs/WFSConnector.cpp

  \brief OGC Web Feature Service (WFS) connector implementation for the Qt data source widget.
*/

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "WFSConnector.h"
#include "WFSConnectorDialog.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt
#include <QFileDialog>
#include <QMessageBox>

te::qt::plugins::wfs::WFSConnector::WFSConnector(QWidget* parent, Qt::WindowFlags f)
  : te::qt::widgets::AbstractDataSourceConnector(parent, f)
{
}

te::qt::plugins::wfs::WFSConnector::~WFSConnector()
{
}

void te::qt::plugins::wfs::WFSConnector::connect(std::list<te::da::DataSourceInfoPtr>& datasources)
{
  std::auto_ptr<WFSConnectorDialog> cdialog(new WFSConnectorDialog(static_cast<QWidget*>(parent())));

  cdialog->exec();

  te::da::DataSourceInfoPtr ds = cdialog->getDataSource();

  if(ds.get() != 0)
  {
    te::da::DataSourceInfoManager::getInstance().add(ds);
    datasources.push_back(ds);

    te::da::DataSourcePtr driver = cdialog->getDriver();
    te::da::DataSourceManager::getInstance().insert(driver);
  }
}

void te::qt::plugins::wfs::WFSConnector::create(std::list<te::da::DataSourceInfoPtr>& datasources)
{
  QMessageBox::information(this,tr("TerraLib Qt Components"), tr("The WFS data access driver not support creation operation."));
}

void te::qt::plugins::wfs::WFSConnector::update(std::list<te::da::DataSourceInfoPtr>& datasources)
{
  for(std::list<te::da::DataSourceInfoPtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
  {
    if(it->get() == 0)
      continue;

    std::auto_ptr<WFSConnectorDialog> cdialog(new WFSConnectorDialog(static_cast<QWidget*>(parent())));

    cdialog->set(*it);

    int retval = cdialog->exec();

    if(retval == QDialog::Rejected)
      continue;

    // Don't forget to replace the driver
    te::da::DataSourcePtr driver = cdialog->getDriver();

    if(driver.get() != 0)
    {
      if(te::da::DataSourceManager::getInstance().find(driver->getId()) != 0)
        te::da::DataSourceManager::getInstance().detach(driver->getId());

      te::da::DataSourceManager::getInstance().insert(driver);
    }
  }
}

void te::qt::plugins::wfs::WFSConnector::remove(std::list<te::da::DataSourceInfoPtr>& datasources)
{
  for(std::list<te::da::DataSourceInfoPtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
  {
    if(it->get() == 0)
      continue;

    // First remove driver
    te::da::DataSourcePtr rds = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(rds.get())
    {
      te::da::DataSourceManager::getInstance().detach(rds);
      rds.reset();
    }

    // Then remove data source
    te::da::DataSourceInfoManager::getInstance().remove((*it)->getId());
  }
}

