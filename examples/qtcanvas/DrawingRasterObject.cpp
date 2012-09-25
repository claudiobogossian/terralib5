#include "QtCanvasExamples.h"

// TerraLib
#include <terralib/raster.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// Qt
#include <QtGui/QApplication>



void DrawingRasterObject()
{
  int argc = 1;
  
  //crate application
  QApplication app(argc, 0);
 
    // set input raster name
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

  // open input raster
  te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
  ds->open(rinfo);

  te::da::DataSourceTransactor* tr = ds->getTransactor();
  te::da::DataSetType* dt = tr->getCatalogLoader()->getDataSetType("cbers2b_rgb342_crop.tif");
  te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(dt->getProperties()[0]->clone());

  te::da::DataSet* dataSet = tr->getDataSet("cbers2b_rgb342_crop.tif");
  te::rst::Raster* raster = dataSet->getRaster();

  const te::gm::Envelope* extent = raster->getExtent();

  double llx = extent->m_llx;
  double lly = extent->m_lly;
  double urx = extent->m_urx;
  double ury = extent->m_ury;

  // create the canvas and adjust the world-device transformation parameters
  te::qt::widgets::Canvas canvas(800, 600);

  
  canvas.setWindow(llx,lly,urx,ury);
  canvas.calcAspectRatio(llx,lly,urx,ury);

  //draw imamge
  canvas.drawImage(0,0, canvas.getWidth(), canvas.getHeight(), raster, 0, 0, raster->getNumberOfColumns(), raster->getNumberOfRows());

  // save a PNG to disk
  canvas.save("raster.png", te::map::PNG);
}
