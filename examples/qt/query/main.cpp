/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file main.cpp

  \brief A list of examples for the TerraLib Widgets.
 */

// TerraLib
#include "../../Config.h"
#include "Config.h"
#include "LoadModules.h"
#include "QueryExamples.h"
#include <terralib/common.h>
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/query/Select.h>
#include <terralib/plugin.h>
#include <terralib/qt/widgets/table/DataSetTableView.h>
#include <terralib/qt/widgets/query/QueryBuilderWizard.h>
#include <terralib/qt/widgets/query/QueryLayerBuilderWizard.h>

// QT
#include <QApplication>
#include <QGridLayout>
#include <QIcon>
#include <QMessageBox>

// STL
#include <string>
#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  try
  {
    // initialize Terralib support
    TerraLib::getInstance().initialize();
    LoadModules();

    // Adjusting icons theme
    //  QString spaths = std::string(ICON_THEME_PATH).c_str(); juan
    // juan QStringList paths = spaths.split(";");             juan
    // juan QIcon::setThemeName(ICON_THEME);                   juan
    // juan QIcon::setThemeSearchPaths(paths);                 juan

    // Load datasource
    std::string fileName = TERRALIB_EXAMPLES_DATA_DIR;
                fileName+= "/shp/munic_2001.shp";

    //LoadShapeDataSource(fileName, "MunicShp2001");

    te::da::DataSourcePtr ds = LoadPGISDataSource("graphDb");

    //te::qt::widgets::QueryBuilderWizard w(0);
    te::qt::widgets::QueryLayerBuilderWizard w(0);

    w.setDataSource(ds);

    if(w.exec() == QDialog::Accepted)
    {
      te::da::Select s = w.getSelectQuery();

      te::da::DataSet* dataSet = GetDataSet(s, ds);

      //create dialog to show the result
      QDialog dlg;
      dlg.setFixedSize(600, 300);

      QGridLayout* layout = new QGridLayout(&dlg);

      te::qt::widgets::DataSetTableView tv(&dlg);

      layout->addWidget(&tv);
      tv.show();

      //show result
      tv.setDataSet(dataSet);

      dlg.exec();

      delete dataSet;
    }

  }
  catch(const std::exception& e)
  {
    std::string msg =  "An exception has occurried: ";
                msg += e.what();

    QMessageBox::warning(0, "Query Example", msg.c_str());

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::string msg =  "An unexpected exception has occurried!";

    QMessageBox::warning(0, "Query Example", msg.c_str());

    return EXIT_FAILURE;
  }

  te::plugin::PluginManager::getInstance().unloadAll();

// finalize Terralib support
  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
