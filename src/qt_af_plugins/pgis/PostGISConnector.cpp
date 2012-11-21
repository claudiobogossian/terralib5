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
  \file terralib/qt/widgets/datasource/connector/postgis/PostGISConnector.cpp

  \brief ....
*/

//! TerraLib
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/qt/widgets/datasource/core/DataSourceManager.h>
#include "PostGISConnector.h"
#include "PostGISConnectorDialog.h"

//! Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

//! Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

namespace te_qt = te::qt::widgets;
namespace plg_pgis = qt_af::plugin::pgis;

plg_pgis::PostGISConnector::PostGISConnector(QWidget* parent, Qt::WindowFlags f)
  : AbstractDataSourceConnector(parent, f)
{
}

plg_pgis::PostGISConnector::~PostGISConnector()
{
}

void plg_pgis::PostGISConnector::create(std::list<te_qt::DataSourcePtr>& datasources)
{
  std::auto_ptr<PostGISConnectorDialog> cdialog(new PostGISConnectorDialog(static_cast<QWidget*>(parent())));

  int retval = cdialog->exec();

  if(retval == QDialog::Rejected)
    return;

  te_qt::DataSourcePtr ds = cdialog->getDataSource();

  if(ds.get() != 0)
  {
    te_qt::DataSourceManager::getInstance().add(ds);
    datasources.push_back(ds);

    te::da::DataSourcePtr driver = cdialog->getDriver();

    te::da::DataSourceManager::getInstance().insert(driver);
  }
}

void plg_pgis::PostGISConnector::update(std::list<te_qt::DataSourcePtr>& datasources)
{
  for(std::list<te_qt::DataSourcePtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
  {
    if(it->get() == 0)
      continue;

    std::auto_ptr<PostGISConnectorDialog> cdialog(new PostGISConnectorDialog(static_cast<QWidget*>(parent())));

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

      te::da::DataSourceManager::getInstance().insert(driver);
    }
  }
}

void plg_pgis::PostGISConnector::remove(std::list<te_qt::DataSourcePtr>& datasources)
{
  for(std::list<te_qt::DataSourcePtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
  {
    if(it->get() == 0)
      continue;

// first remove driver
    te::da::DataSourcePtr rds = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(rds.get())
    {
      te::da::DataSourceManager::getInstance().detach(rds);
      rds.reset();
    }

// then remove data source
    te_qt::DataSourceManager::getInstance().remove((*it)->getId());
  }
}

