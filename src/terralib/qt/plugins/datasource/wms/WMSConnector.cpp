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
  \file terralib/qt/widgets/datasource/connector/wms/WMSConnector.cpp

  \brief ....
*/

// TerraLib
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/qt/widgets/datasource/core/DataSourceManager.h>
#include "WMSConnector.h"
#include "WMSConnectorDialog.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

namespace te_qt = te::qt::widgets;
namespace plg_wms = qt_af::plugin::wms;

plg_wms::WMSConnector::WMSConnector(QWidget* parent, Qt::WindowFlags f)
  : te_qt::AbstractDataSourceConnector(parent, f)
{
}

plg_wms::WMSConnector::~WMSConnector()
{
}

void plg_wms::WMSConnector::create(std::list<te_qt::DataSourcePtr>& datasources)
{
  std::auto_ptr<WMSConnectorDialog> cdialog(new WMSConnectorDialog(static_cast<QWidget*>(parent())));

  cdialog->exec();

  te::qt::widgets::DataSourcePtr ds = cdialog->getDataSource();

  if(ds.get() != 0)
  {
    te_qt::DataSourceManager::getInstance().add(ds);
    datasources.push_back(ds);
  }
}

void plg_wms::WMSConnector::update(std::list<te_qt::DataSourcePtr>& /*datasources*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void plg_wms::WMSConnector::remove(std::list<te_qt::DataSourcePtr>& /*datasources*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

