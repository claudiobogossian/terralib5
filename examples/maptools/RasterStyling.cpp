#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/raster.h>
#include <terralib/se.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// STL
#include <cassert>
#include <iostream>
#include <vector>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QLabel>

te::map::RasterLayer* CreateLayer(const std::string& path)
{
  // Connection string to a shape file
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = path;

  // Creates and connects data source
  te::da::DataSource* dataSource = te::da::DataSourceFactory::make("GDAL");
  if(dataSource == 0)
    throw te::common::Exception("Sorry, I can not create the GDAL driver!");
  dataSource->open(connInfo);

  // Transactor and catalog
  te::da::DataSourceTransactor* transactor = dataSource->getTransactor();
  te::da::DataSourceCatalogLoader* catalogLoader = transactor->getCatalogLoader();
  catalogLoader->loadCatalog();

  // Gets the number of data set types that belongs to the data source
  std::vector<std::string*> datasets;
  transactor->getCatalogLoader()->getDataSets(datasets);
  assert(!datasets.empty());

  // Gets the first dataset
  std::string dataSetName(*datasets[0]);
  te::da::DataSetType* dt = catalogLoader->getDataSetType(dataSetName);
  te::da::DataSet* ds = transactor->getDataSet(dataSetName);
  te::rst::Raster* raster = ds->getRaster();

  // Box
  te::gm::Envelope* extent = raster->getExtent();

  // Creates a Layer
  te::map::RasterLayer* layer = new te::map::RasterLayer(te::common::Convert2String(0), dataSetName);
  layer->setDataSource(dataSource);
  layer->setDataSetName(dataSetName);
  layer->setExtent(extent);

  // Creates a Layer Renderer
  te::map::RasterLayerRenderer* r = new te::map::RasterLayerRenderer();
  layer->setRenderer(r);

  te::common::FreeContents(datasets);
  delete catalogLoader;
  delete transactor;

  return layer;
}

void Draw(te::map::RasterLayer* layer)
{
  te::gm::Envelope* extent = new te::gm::Envelope(*layer->getExtent());

  int srid = 4618; // LL SAD69

  extent->transform(layer->getRaster()->getSRID(), srid);

  double llx = extent->m_llx;
  double lly = extent->m_lly;
  double urx = extent->m_urx;
  double ury = extent->m_ury;

  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(512, 512);
  canvas->calcAspectRatio(llx, lly, urx, ury);
  canvas->setWindow(llx, lly, urx, ury);
  canvas->setBackgroundColor(te::color::RGBAColor(255, 255, 255, TE_OPAQUE));

  layer->draw(canvas, *extent, srid);

  QPixmap* pixmap = canvas->getPixmap();

  QDialog dialog;
  dialog.setFixedSize(pixmap->size());
  
  QLabel preview(&dialog);
  preview.setPixmap(*pixmap);
  
  dialog.exec();

  delete extent;
  delete canvas;
}

void DrawRasterStyledLayers()
{
  // If you want to use any Qt resource you must initialize a QApplication in some place!
  int argc = 0;
  QApplication app(argc, 0);

  try
  {
    // Creates a layer of raster
    te::map::RasterLayer* rasterLayer = CreateLayer(""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif");

    Draw(rasterLayer);

    delete rasterLayer;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Styling example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Styling example!" << std::endl;
  }
}
