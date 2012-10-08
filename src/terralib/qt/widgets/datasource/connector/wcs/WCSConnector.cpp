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
  \file terralib/qt/widgets/datasource/connector/wcs/WCSConnector.cpp

  \brief ....
*/

// TerraLib
#include "../../../../../dataaccess/datasource/DataSource.h"
#include "../../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../core/DataSourceManager.h"
#include "WCSConnector.h"
#include "WCSConnectorDialog.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

te::qt::widgets::WCSConnector::WCSConnector(QWidget* parent, Qt::WindowFlags f)
  : AbstractDataSourceConnector(parent, f)
{
}

te::qt::widgets::WCSConnector::~WCSConnector()
{
}

void te::qt::widgets::WCSConnector::create(std::list<DataSourcePtr>& datasources)
{
  std::auto_ptr<WCSConnectorDialog> cdialog(new WCSConnectorDialog(static_cast<QWidget*>(parent())));

  cdialog->exec();

  te::qt::widgets::DataSourcePtr ds = cdialog->getDataSource();

  if(ds.get() != 0)
  {
    DataSourceManager::getInstance().add(ds);
    datasources.push_back(ds);
  }
}

void te::qt::widgets::WCSConnector::update(std::list<DataSourcePtr>& /*datasources*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::widgets::WCSConnector::remove(std::list<DataSourcePtr>& /*datasources*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

