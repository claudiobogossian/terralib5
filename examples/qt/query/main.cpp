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
  \file main.cpp

  \brief A list of examples for the TerraLib Widgets.
 */

// TerraLib
#include "../../Config.h"
#include "Config.h"
#include "LoadModules.h"
#include "QueryExamples.h"
#include <terralib/common.h>
#include <terralib/core/utils/Platform.h>
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/query/Select.h>

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
    QStringList ithemes = QIcon::themeSearchPaths();
    ithemes.push_back(te::core::FindInTerraLibPath("share/terralib/icons").c_str());
    QIcon::setThemeName("terralib");
    QIcon::setThemeSearchPaths(ithemes);

    // Load datasource
    std::string fileName = TERRALIB_DATA_DIR "/shp/munic_2001.shp";

    te::da::DataSourcePtr ds = LoadPGISDataSource("graphDb");

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
    std::string msg =  "An exception has occurred: ";
                msg += e.what();

    QMessageBox::warning(0, "Query Example", msg.c_str());

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::string msg =  "An unexpected exception has occurred!";

    QMessageBox::warning(0, "Query Example", msg.c_str());

    return EXIT_FAILURE;
  }

  te::plugin::PluginManager::getInstance().unloadAll();

// finalize Terralib support
  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
