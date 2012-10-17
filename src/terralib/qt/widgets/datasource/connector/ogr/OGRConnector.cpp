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
  \file terralib/qt/widgets/datasource/connector/ogr/OGRConnector.cpp

  \brief ....
*/

// TerraLib
#include "../../../../../dataaccess/datasource/DataSource.h"
#include "../../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../core/DataSourceManager.h"
#include "OGRConnector.h"
#include "OGRConnectorDialog.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

te::qt::widgets::OGRConnector::OGRConnector(QWidget* parent, Qt::WindowFlags f)
  : AbstractDataSourceConnector(parent, f)
{
}

te::qt::widgets::OGRConnector::~OGRConnector()
{
}

void te::qt::widgets::OGRConnector::create(std::list<DataSourcePtr>& datasources)
{
  std::auto_ptr<OGRConnectorDialog> cdialog(new OGRConnectorDialog(static_cast<QWidget*>(parent())));

  int retval = cdialog->exec();

  if(retval == QDialog::Rejected)
    return;

  te::qt::widgets::DataSourcePtr ds = cdialog->getDataSource();

  if(ds.get() != 0)
  {
    DataSourceManager::getInstance().add(ds);
    datasources.push_back(ds);

    te::da::DataSourcePtr driver = cdialog->getDriver();

    te::da::DataSourceManager::getInstance().insert(driver->getId(),driver.get());
  }
}

void te::qt::widgets::OGRConnector::update(std::list<DataSourcePtr>& datasources)
{
  for(std::list<DataSourcePtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
  {
    if(it->get() == 0)
      continue;

    std::auto_ptr<OGRConnectorDialog> cdialog(new OGRConnectorDialog(static_cast<QWidget*>(parent())));

    cdialog->set(*it);

    int retval = cdialog->exec();

    if(retval == QDialog::Rejected)
      continue;

// don't forget to replace the driver
    te::da::DataSourcePtr driver = cdialog->getDriver();

    if(driver.get() != 0)
    {
      if(te::da::DataSourceManager::getInstance().find(driver->getId()) != 0)
        te::da::DataSourceManager::getInstance().detach(driver->getId());

      te::da::DataSourceManager::getInstance().insert(driver->getId(),driver.get());
    }
  }
}

void te::qt::widgets::OGRConnector::remove(std::list<DataSourcePtr>& datasources)
{
  for(std::list<DataSourcePtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
  {
    if(it->get() == 0)
      continue;

// first remove driver
    te::da::DataSource* rds = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(rds)
    {
      te::da::DataSourceManager::getInstance().detach(rds);
      //rds.reset();
    }

// then remove data source
    DataSourceManager::getInstance().remove((*it)->getId());
  }
}

