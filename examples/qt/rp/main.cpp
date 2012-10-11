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

#include "Config.h"
#include "LoadModules.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/gdal/Utils.h>
#include <terralib/geometry.h>
#include <terralib/qt/widgets/rp/ClassifierDialog.h>
#include <terralib/qt/widgets/rp/ContrastDialog.h>
#include <terralib/qt/widgets/rp/SegmenterDialog.h>
#include <terralib/raster/RasterFactory.h>

// QT
#include <QtGui/QApplication>

// STL
#include <exception>
#include <iostream>

void SegmenterDialogExample( int argc, char** argv )
{
  // open the input raster

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TE_DATA_EXAMPLE_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";

  std::auto_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(rinfo) );

  // Defining the output raster info

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_EXAMPLE_LOCALE
    "/data/rasters/terralib_example_qt_rp_SegmenterDialog.tif";

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

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TE_DATA_EXAMPLE_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";

  std::auto_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(rinfo) );

  // Defining the output raster info

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_EXAMPLE_LOCALE
    "/data/rasters/terralib_example_qt_rp_ContrastDialog.tif";

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
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

  std::auto_ptr<te::rst::Raster> inputRasterPointer(te::rst::RasterFactory::open(rinfo));

// define the output raster info
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_EXAMPLE_LOCALE"/data/rasters/terralib_example_qt_rp_ClassifierDialog.tif";

// execute the dialog
  QApplication app(argc, argv);

// open a previously segmentation (from SegmenterDialogExample)
  std::map<std::string, std::string> segmentedinfo;
  segmentedinfo["URI"] = TE_DATA_EXAMPLE_LOCALE"/data/rasters/terralib_example_qt_rp_SegmenterDialog.tif";
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

int main(int argc, char** argv)
{
  try
  {
    // initialize Terralib support
    TerraLib::getInstance().initialize();
    LoadModules();

    SegmenterDialogExample( argc, argv );
    ContrastDialogExample( argc, argv );
    ClassifierDialogExample( argc, argv );
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;

    return EXIT_FAILURE;
  }

// finalize Terralib support
  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
