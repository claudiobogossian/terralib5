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

  \brief Examples about chart widgets
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/plugin.h>

//Utils
#include <terralib/qt/widgets/charts/Utils.h>

// Qt Chart
#include <terralib/qt/widgets/charts/Scatter.h>
#include <terralib/qt/widgets/charts/ScatterChart.h>
#include <terralib/qt/widgets/charts/Histogram.h>
#include <terralib/qt/widgets/charts/HistogramChart.h>
#include <terralib/qt/widgets/charts/ChartDisplay.h>

// Qt
#include <QtGui/QApplication>

// STL
#include <exception>
#include <iostream>

void LoadOGRModule()
{
  try
  {
    te::plugin::PluginInfo info; 
    info.m_name = "te.da.ogr";
    info.m_displayName = "OGR DataSource Driver";
    info.m_description = "This data source driver supports spatial data managed by OGR";
    info.m_engine = "C++";
    info.m_folder = TE_PLUGINS_PATH;

    std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_ogr");

    info.m_resources.push_back(rsc);

    te::plugin::PluginManager::getInstance().load(info);
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load data source OGR driver: unknow exception!" << std::endl;
  }
}

int main(int /*argc*/, char** /*argv*/)
{
// initialize Terralib support
  TerraLib::getInstance().initialize();

  try
  {
    LoadOGRModule();
    
    //pegar um data set
    std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_LOCALE"/data/shp");

    te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
    ds->setConnectionStr(ogrInfo);
    ds->open();
    
    te::da::DataSourceTransactor* transactor = ds->getTransactor();

    te::da::DataSet* dataset = transactor->getDataSet("mapa_distritos_sp");
    if(dataset==0)
    {
       delete dataset;
       delete transactor;
       delete ds;
       return 0;
    }
    
    //Acquiring the dataset Properties types
    te::da::DataSetType* type = dataset->getType();

    //Getting the Columns that will be used to populate the graph

    std::string renda = "RENDA_FAM";
    std::string anosest = "ANOS_EST";

    int rendaIdx= type->getPropertyPosition(renda);
    int anosestIdx= type->getPropertyPosition(anosest);

    int argc = 1;
    QApplication app(argc, 0);
    QString title("Testing Chart Widgets");

    te::qt::widgets::Histogram* histogram = te::qt::widgets::createHistogram(dataset, rendaIdx, 10);
    te::qt::widgets::HistogramChart* histogramChart = new te::qt::widgets::HistogramChart(histogram);

    te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay();

    histogramChart->attach(chartDisplay);

    chartDisplay->show();

    chartDisplay->replot();

//    te::qt::widgets::Scatter* scatter = te::qt::widgets::createScatter(dataset, rendaIdx, anosestIdx );

//    te::qt::widgets::ScatterChart* scatterChart = new te::qt::widgets::ScatterChart(scatter);

//     te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay();
//     chartDisplay->show();

//     scatterChart->attach(chartDisplay);
//     chartDisplay->show();

//     chartDisplay->replot();

    int ret = app.exec();

	  //delete pointers

    //delete scatterChart;
    //delete scatter;
    delete histogramChart;
    delete histogram;
    delete dataset;
    delete transactor;
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried: " << e.what() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

// finalize Terralib support
  te::plugin::PluginManager::getInstance().unloadAll();
  TerraLib::getInstance().finalize();

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
