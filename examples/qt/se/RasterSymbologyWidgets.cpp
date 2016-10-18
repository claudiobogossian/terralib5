#include "SymbologyWidgets.h"

// TerraLib
#include "../../Config.h"
#include <terralib/qt/widgets/se/RasterSymbolizerDialog.h>
#include <terralib/raster.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/se.h>

// Qt
#include <QApplication>

void RasterSymbologyWidgets()
{
  int argc = 1;
  QApplication app(argc, 0);

  // set input raster name
  std::string rinfo ("File://");
  
  std::string data_dir = TERRALIB_DATA_DIR;
  
  //std::string filename(data_dir + '/shp/SP_cities.shp");
  
  std::string filename(data_dir + "/Nulos/nulos2.shp");  

  rinfo += data_dir + "/geotiff/cbers2b_rgb342_crop.tif";

// open input raster
  std::unique_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL", rinfo);
  ds->open();

  std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();
  std::auto_ptr<te::da::DataSetType> dt = tr->getDataSetType("cbers2b_rgb342_crop.tif");
  te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(dt->getProperties()[0]->clone());

  std::auto_ptr<te::da::DataSet> dataSet = tr->getDataSet("cbers2b_rgb342_crop.tif");

  std::size_t rpos = te::da::GetFirstPropertyPos(dataSet.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> raster = dataSet->getRaster(rpos);

  te::se::RasterSymbolizer* rs = 0;

  // Creates the rastersymbolizer dialog
  te::qt::widgets::RasterSymbolizerDialog dlg;

  dlg.setRasterProperty(raster.get(), rstp);

  if(dlg.exec() == QDialog::Accepted)
  {
    rs = static_cast<te::se::RasterSymbolizer*>(dlg.getRasterSymbolizer());
  }

}
