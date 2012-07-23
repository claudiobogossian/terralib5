#include "SymbologyWidgets.h"

// TerraLib
#include <terralib/qt/widgets/se/RasterSymbolizerDialog.h>
#include <terralib/raster.h>
#include <terralib/gdal/Platform.h>

// Qt
#include <QtGui/QApplication>

void RasterSymbologyWidgets()
{
  int argc = 1;
  QApplication app(argc, 0);

  // open raster
  te::gdal::Platform::initialize();

  // set input raster name
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = "D:/landsat.tif";

// open input raster
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

  // Creates the rastersymbolizer dialog
  te::qt::widgets::RasterSymbolizerDialog dlg;
  dlg.exec();

  te::gdal::Platform::finalize();
}
