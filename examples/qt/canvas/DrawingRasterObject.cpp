#include "CanvasExamples.h"

// TerraLib
#include "../../Config.h"
#include <terralib/raster.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// Qt
#include <QApplication>

void DrawingRasterObject()
{
  int argc = 1;
  
  //create application
  QApplication app(argc, 0);
 
  // set input raster name
  std::string data_dir = TERRALIB_EXAMPLES_DATA_DIR;
  
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = data_dir + "/rasters/cbers2b_rgb342_crop.tif";

  // open input raster
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
  ds->setConnectionInfo(rinfo);
  ds->open();

  std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();

  std::auto_ptr<te::da::DataSet> dataSet = tr->getDataSet("cbers2b_rgb342_crop.tif");
  std::size_t rpos = te::da::GetFirstPropertyPos(dataSet.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> raster = dataSet->getRaster(rpos);

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
  canvas.drawImage(0,0, canvas.getWidth(), canvas.getHeight(), raster.get(), 0, 0, raster->getNumberOfColumns(), raster->getNumberOfRows());

  // save a PNG to disk
  canvas.save("raster.png", te::map::PNG);
}
