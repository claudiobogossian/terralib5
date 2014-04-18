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
#include "../../Config.h"
#include <terralib/common/PlatformUtils.h>
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
#include <terralib/qt/widgets/charts/ChartDisplayWidget.h>
#include <terralib/qt/widgets/charts/ChartStyle.h>

// Qt
#include <QApplication>

// STL
#include <exception>
#include <iostream>

void LoadOGRModule()
{
  try
  {
    te::plugin::PluginInfo* info;
  
    std::string plugin_path = te::common::FindInTerraLibPath("share/terralib/plugins");

    info = te::plugin::GetInstalledPlugin(plugin_path + "/te.da.ogr.teplg");
    te::plugin::PluginManager::getInstance().add(info); 

    te::plugin::PluginManager::getInstance().loadAll();
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load data source OGR driver: unknow exception!" << std::endl;
  }
}

void generateHistogram(te::da::DataSet* dataset, te::da::DataSourceTransactor* transactor)
{
  //Getting the Column that will be used to populate the chart
  std::string renda = "RENDA_FAM";

  int rendaIdx= te::da::GetPropertyPos(dataset, renda);

  std::auto_ptr<te::da::DataSetType> dt =  transactor->getDataSetType("mapa_distritos_sp");

  //Creating the histogram and it's chart with the given dataset
  te::qt::widgets::Histogram* histogram = te::qt::widgets::createHistogram(dataset, dt.get(), rendaIdx, 10);
  te::qt::widgets::HistogramChart* chart = new te::qt::widgets::HistogramChart(histogram);

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(QString::fromStdString("Histogram"));
  chartStyle->setAxisX(QString::fromStdString(renda));
  chartStyle->setAxisY(QString::fromStdString("Frequency"));

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Histogram"), chartStyle);
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);

  //Adjusting the chart widget, once it's closed all the other pointers will be deleted. Check the charts documentation for further notes on pointer ownership
  te::qt::widgets::ChartDisplayWidget* displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::HISTOGRAM_CHART, chartDisplay);
  displayWidget->show();
  displayWidget->setWindowTitle("Histogram");
  displayWidget->setAttribute(Qt::WA_DeleteOnClose, true);
}

void generateScatter(te::da::DataSet* dataset, te::da::DataSourceTransactor* transactor)
{
    //Getting the Column that will be used to populate the chart
  std::string renda = "RENDA_FAM";
  std::string anosest = "ANOS_EST";

  int rendaIdx= te::da::GetPropertyPos(dataset, renda);
  int anosestIdx= te::da::GetPropertyPos(dataset, anosest);

  std::auto_ptr<te::da::DataSetType> dt =  transactor->getDataSetType("mapa_distritos_sp");

  //Creating the scatter and it's chart with the given dataset
  te::qt::widgets::Scatter* scatter = te::qt::widgets::createScatter(dataset, dt.get(), rendaIdx, anosestIdx);
  te::qt::widgets::ScatterChart* chart = new te::qt::widgets::ScatterChart(scatter);

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(QString::fromStdString("Scatter"));
  chartStyle->setAxisX(QString::fromStdString(renda));
  chartStyle->setAxisY(QString::fromStdString(anosest));

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Scatter"), chartStyle);
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);

  //Adjusting the chart widget, once it's closed all the other pointers will be deleted. Check the charts documentation for further notes on pointer ownership
  te::qt::widgets::ChartDisplayWidget* displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::SCATTER_CHART, chartDisplay);
  displayWidget->show();
  displayWidget->setWindowTitle("Scatter");
  displayWidget->setAttribute(Qt::WA_DeleteOnClose, true);
}

int main(int /*argc*/, char** /*argv*/)
{
// initialize Terralib support
  TerraLib::getInstance().initialize();

  try
  {
    LoadOGRModule();
    
    // Get a dataset
    std::map<std::string, std::string> connInfo;
  
    std::string data_dir = TERRALIB_EXAMPLES_DATA_DIR;
    connInfo["URI"] = data_dir + "/shp";
  
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  
    ds->setConnectionInfo(connInfo);
    ds->open();
    
    std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();

    std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet("mapa_distritos_sp");
    if(dataset.get()==0)
    {
       return 0;
    }

    int argc = 1;
    QApplication app(argc, 0);
    QString title("Testing Chart Widgets");

    generateHistogram(dataset.get(), transactor.get());
    generateScatter(dataset.get(), transactor.get());

    int ret;
    ret = app.exec();
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
