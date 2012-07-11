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
#include <terralib/qt/widgets/charts/ChartDisplay.h>

// Qt
#include <QtGui/QApplication>

// STL
#include <exception>
#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
// initialize Terralib support
  TerraLib::getInstance().initialize();

  try
  {
    //======== load plugin ogr
    te::plugin::PluginInfo info;
    info.m_type = "dll";
    info.m_name = "OGR DataSource Driver";
    info.m_description = "This data source driver supports...";
    info.m_mainFile = "terralib_ogr_d.dll";
    te::plugin::PluginManager::getInstance().loadPlugin(info);
    //========
    
    //pegar um data set
    std::string ogrInfo("connection_string=C:/Users/andre.oliveira/Funcate/Projetos/Solutions/examples/data/data-charts");

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
    
	  //criar um scatter do data set
    std::string renda = "RENDA_FAM";
    std::string anosest = "ANOS_EST";
    te::da::DataSetType* type = dataset->getType();
    int rendaIdx = type->getPropertyPosition(renda);
    int anosestIdx = type->getPropertyPosition(anosest);
    
    int argc = 1;
    QApplication app(argc, 0);
    QString title("Testing Chart Widgets");
    
	  te::qt::widgets::Scatter* scatter = te::qt::widgets::createScatter(dataset, rendaIdx, anosestIdx );

    te::qt::widgets::ScatterChart* scatterChart = new te::qt::widgets::ScatterChart(scatter);

    te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay();
    chartDisplay->show();

    scatterChart->attach(chartDisplay);
    chartDisplay->show();

    chartDisplay->replot();

    int ret = app.exec();

	  //delete pointers
    //delete chartDisplay; // quando attacha não precisa deletar!!!! => ele cai!
    delete scatterChart;
    delete scatter;
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
  TerraLib::getInstance().finalize();

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
