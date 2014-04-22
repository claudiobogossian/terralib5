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
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/gdal/Utils.h>
#include <terralib/geometry.h>
//#include <terralib/maptools/RasterLayerRenderer.h> // * Under revision *
#include <terralib/plugin.h>
#include <terralib/qt/widgets/rp/ClassifierDialog.h>
#include <terralib/qt/widgets/rp/ContrastDialog.h>
#include <terralib/qt/widgets/rp/MixtureModelDialog.h>
#include <terralib/qt/widgets/rp/SegmenterDialog.h>
#include <terralib/qt/widgets/rp/TiePointsLocatorDialog.h>
#include <terralib/raster/RasterFactory.h>

// QT
#include <QApplication>

// STL
#include <exception>
#include <iostream>

void TiePointsLocatorDialogExample( int argc, char** argv )
{
  // * Under revision *

  // open the input rasters

  //const std::string dataSet1Name( "cbers2b_rgb342_crop.tif" );
  //std::map<std::string, std::string> rinfo1;
  //rinfo1["URI"] = TE_DATA_EXAMPLE_DIR "/data/rasters";
  //std::auto_ptr< te::da::DataSource > ds1( te::da::DataSourceFactory::make("GDAL") );
  //ds1->open(rinfo1);
  //if( ! ds1->isOpened() )
  //{
  //  std::cout << std::endl << "Data source openning error";
  //  return;
  //}
  //std::auto_ptr< te::map::RasterLayer > raster1Layer(
  //  new te::map::RasterLayer(dataSet1Name, dataSet1Name) );
  //raster1Layer->setDataSource(ds1.get());
  //raster1Layer->setDataSetName(dataSet1Name);
  //if( ! raster1Layer->isValid() )
  //{
  //  std::cout << std::endl << "Invalid layer";
  //  return;
  //}
  //te::map::RasterLayerRenderer* renderer1 = new te::map::RasterLayerRenderer();
  //raster1Layer->setRenderer( renderer1 );
  //raster1Layer->setVisibility(te::map::VISIBLE);

  //const std::string dataSet2Name( "cbers2b_rgb342_crop.tif" );
  //std::map<std::string, std::string> rinfo2;
  //rinfo2["URI"] = TE_DATA_EXAMPLE_DIR "/data/rasters";
  //std::auto_ptr< te::da::DataSource > ds2( te::da::DataSourceFactory::make("GDAL") );
  //ds2->open(rinfo2);
  //if( ! ds2->isOpened() )
  //{
  //  std::cout << std::endl << "Data source openning error";
  //  return;
  //}
  //std::auto_ptr< te::map::RasterLayer > raster2Layer(
  //  new te::map::RasterLayer(dataSet2Name, dataSet2Name) );
  //raster2Layer->setDataSource(ds2.get());
  //raster2Layer->setDataSetName(dataSet2Name);
  //if( ! raster2Layer->isValid() )
  //{
  //  std::cout << std::endl << "Invalid layer";
  //  return;
  //}
  //te::map::RasterLayerRenderer* renderer2 = new te::map::RasterLayerRenderer();
  //raster2Layer->setRenderer( renderer2 );
  //raster2Layer->setVisibility(te::map::VISIBLE);

  //// Executing the dialog

  //QApplication app(argc, argv);

  //te::qt::widgets::TiePointsLocatorDialog dialogInstance( raster1Layer.get(),
  //  raster2Layer.get(), 0, 0 );

  //dialogInstance.exec();

  //dialogInstance.hide();

  //// Getting the result

  //std::vector< te::gm::GTParameters::TiePoint > tiePoints;
  //dialogInstance.getTiePoints( tiePoints );
}

void SegmenterDialogExample( int argc, char** argv )
{
  // open the input raster
  
  std::string data_dir = TERRALIB_EXAMPLES_DATA_DIR;
  
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = data_dir +  "/rasters/cbers2b_rgb342_crop.tif";

  std::auto_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(rinfo) );

  // Defining the output raster info

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = data_dir + "/rasters/terralib_example_qt_rp_SegmenterDialog.tif";

  // Executing the dialog

  QApplication app(argc, argv);

  te::qt::widgets::SegmenterDialog dialogInstance( inputRasterPointer.get(),
    "GDAL", outputRasterInfo, 0, 0 );

  dialogInstance.exec();

  dialogInstance.hide();

  // Getting the result

  boost::shared_ptr< te::rst::Raster > outputRasterPtr;
  dialogInstance.getOutputRaster( outputRasterPtr );
}

void ContrastDialogExample( int argc, char** argv )
{
  // open the input raster
  
  std::string data_dir = TERRALIB_EXAMPLES_DATA_DIR;

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = data_dir +  "/rasters/cbers2b_rgb342_crop.tif";

  std::auto_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(rinfo) );

  // Defining the output raster info

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = data_dir + "/rasters/terralib_example_qt_rp_ContrastDialog.tif";

  // Executing the dialog

  QApplication app(argc, argv);

  te::qt::widgets::ContrastDialog dialogInstance( inputRasterPointer.get(),
    "GDAL", outputRasterInfo, 0, 0 );

  dialogInstance.exec();

  dialogInstance.hide();

  // Getting the result

  boost::shared_ptr< te::rst::Raster > outputRasterPtr;
  dialogInstance.getOutputRaster( outputRasterPtr );
}

void ClassifierDialogExample(int argc, char** argv)
{
// open the input raster
  std::string data_dir = TERRALIB_EXAMPLES_DATA_DIR;

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = data_dir + "/rasters/cbers2b_rgb342_crop.tif";

  std::auto_ptr<te::rst::Raster> inputRasterPointer(te::rst::RasterFactory::open(rinfo));

// define the output raster info
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = data_dir + "/rasters/terralib_example_qt_rp_ClassifierDialog.tif";

// execute the dialog
  QApplication app(argc, argv);

// open a previously segmentation (from SegmenterDialogExample)
  std::map<std::string, std::string> segmentedinfo;
  segmentedinfo["URI"] = data_dir + "/rasters/terralib_example_qt_rp_SegmenterDialog.tif";
  std::auto_ptr<te::rst::Raster> segmentedraster(te::rst::RasterFactory::open(segmentedinfo));

// vectorize the segmentation
  std::vector<te::gm::Geometry*> geometries;
  te::gdal::Vectorize(((te::gdal::Raster*) segmentedraster.get())->getGDALDataset()->GetRasterBand(1), geometries);
  std::vector<te::gm::Polygon*> inputPolygons;
  for (unsigned i = 0; i < geometries.size(); i++)
    inputPolygons.push_back(static_cast<te::gm::Polygon*> (geometries[i]));

// create GUI dialog
  te::qt::widgets::ClassifierDialog dialogInstance(inputRasterPointer.get(), inputPolygons, "GDAL", outputRasterInfo, 0, 0);

  dialogInstance.exec();

  dialogInstance.hide();

// get the result
  boost::shared_ptr< te::rst::Raster > outputRasterPtr;

  dialogInstance.getOutputRaster(outputRasterPtr);
}

void MixtureModelDialogExample(int argc, char** argv)
{
// open the input raster
  std::string data_dir = TERRALIB_EXAMPLES_DATA_DIR;

  const std::string dsname("cbers2b_rgb342_crop.tif");
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = data_dir + "/rasters/cbers2b_rgb342_crop.tif";
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
 
  ds->open();
  if(!ds->isOpened())
  {
    std::cout << std::endl << "Data source openning error";
    return;
  }

 // * Under revision *
 /* std::auto_ptr<te::map::RasterLayer> rasterLayer(new te::map::RasterLayer(dsname, dsname) );
  rasterLayer->setDataSource(ds.get());
  rasterLayer->setDataSetName(dsname);
  if(!rasterLayer->isValid())
  {
    std::cout << std::endl << "Invalid layer";
    return;
  }

  te::map::RasterLayerRenderer* renderer = new te::map::RasterLayerRenderer();
  rasterLayer->setRenderer(renderer);
  rasterLayer->setVisibility(te::map::VISIBLE);*/

// define the output raster info
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = data_dir + "/rasters/terralib_example_qt_rp_MixtureModelDialog.tif";

// execute the dialog
  QApplication app(argc, argv);

  // * Under revision *
// create GUI dialog
  //te::qt::widgets::MixtureModelDialog dialogInstance(rasterLayer.get(), "GDAL", outputRasterInfo, 0, 0);

  //dialogInstance.exec();

  //dialogInstance.hide();

// get the result
  boost::shared_ptr< te::rst::Raster > outputRasterPtr;

  //dialogInstance.getOutputRaster(outputRasterPtr);
}

int main(int argc, char** argv)
{
  try
  {
    // initialize Terralib support
    TerraLib::getInstance().initialize();
    LoadModules();

    TiePointsLocatorDialogExample( argc, argv );
    SegmenterDialogExample( argc, argv );
    ContrastDialogExample( argc, argv );
    ClassifierDialogExample( argc, argv );
    MixtureModelDialogExample( argc, argv );
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried!" << std::endl;

    return EXIT_FAILURE;
  }

  te::plugin::PluginManager::getInstance().unloadAll();

// finalize Terralib support
  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
