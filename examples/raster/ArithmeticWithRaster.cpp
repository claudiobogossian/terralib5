#include "RasterExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/gdal.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/raster/RasterFactory.h>

// STL
#include <complex>
#include <iostream>
#include <string>
#include <vector>

// GDAL
#include <gdal_priv.h>

void ArithmeticWithRaster()
{
  try
  {
    std::cout << "This is a test to create a new raster (NDVI) based on raster arithmetic operations." << std::endl << std::endl;

// set raster names
    std::map<std::string, std::string> sri, srndvi, srndvin, srden;
    sri["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
    srndvi["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_ndvi.tif";
    srndvin["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_ndvi_normalized.tif";
    std::string base_path = ""TERRALIB_DATA_DIR"/rasters/";

// open input raster (band 0 = Red, band 1 = IRed, band 2 = Blue)
    te::rst::Raster* ri = te::rst::RasterFactory::open(sri);

// create temporary raster using in memory driver
    te::rst::Grid* gden = new te::rst::Grid(*ri->getGrid());
    std::vector<te::rst::BandProperty*> bdsden;
    bdsden.push_back(new te::rst::BandProperty(0, te::dt::FLOAT_TYPE, "ndvi den"));
    bdsden[0]->m_blkh = gden->getNumberOfRows();
    bdsden[0]->m_blkw = gden->getNumberOfColumns();
    bdsden[0]->m_nblocksx = 1;
    bdsden[0]->m_nblocksy = 1;
    std::map<std::string, std::string> deninfo;
    deninfo["FORCE_MEM_DRIVER"] = "TRUE";
    te::rst::Raster* rden = te::rst::RasterFactory::make(gden, bdsden, deninfo);

// access a raster datasource to create temporary raster
    std::map<std::string, std::string> connInfoRaster;
    connInfoRaster["URI"] = base_path;
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(connInfoRaster);
    ds->open();
    std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();

// create ndvi raster
    te::rst::Grid* gndvi = new te::rst::Grid(*ri->getGrid());
    std::vector<te::rst::BandProperty*> bdsndvi;
    bdsndvi.push_back(new te::rst::BandProperty(0, te::dt::FLOAT_TYPE));
    te::rst::Raster* rndvi = te::rst::RasterFactory::make(gndvi, bdsndvi, srndvi);

// get GDAL band pointers
    te::gdal::Band* ndviBand = static_cast<te::gdal::Band*> (rndvi->getBand(0));
    te::gdal::Band* redBand = static_cast<te::gdal::Band*> (ri->getBand(0));
    te::gdal::Band* iredBand = static_cast<te::gdal::Band*> (ri->getBand(1));
    te::gdal::Band* ndenBand = static_cast<te::gdal::Band*> (rden->getBand(0));

// calculate vegetation index (IRed - Red)
    std::complex<double> value;
    for (unsigned r = 0; r < rndvi->getNumberOfRows(); r++)
      for (unsigned c = 0; c < rndvi->getNumberOfColumns(); c++)
      {
        iredBand->getValue(c, r, value);
        ndviBand->setValue(c, r, value);
      }
    *ndviBand -= *redBand;

// derive denominator (den = IRed + Red)
    for (unsigned r = 0; r < rndvi->getNumberOfRows(); r++)
      for (unsigned c = 0; c < rndvi->getNumberOfColumns(); c++)
      {
        iredBand->getValue(c, r, value);
        ndenBand->setValue(c, r, value);
      }
    *ndenBand += *redBand;

// calculate ndvi [-1, +1] = (IRed - Red) / (IRed + Red)
    *rndvi /= *rden;

// show ndvi status
    std::cout << "NDVI created:" << std::endl;
    std::cout << rndvi->toString();

// create normalized ndvi [-1, +1] -> [0,255] for visualization
    te::rst::Grid* gndvin = new te::rst::Grid(*ri->getGrid());
    std::vector<te::rst::BandProperty*> bdsndvin;
    bdsndvin.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE));
    te::rst::Raster* rndvin = te::rst::RasterFactory::make(gndvin, bdsndvin, srndvin);
    te::rst::Copy(*rndvi, *rndvin);
    std::complex<double> ndvimin = rndvi->getBand(0)->getMinValue();
    std::complex<double> ndvimax = 255.0 / (rndvi->getBand(0)->getMaxValue() - ndvimin);

    *rndvin -= ndvimin;
    *rndvin *= ndvimax;

// show normalized ndvi status
    std::cout << "Normalized NDVI created:" << std::endl;
    std::cout << rndvin->toString();

// remove temporary raster using data set type persistence
    delete rden;

// clean up
    delete ri;
    delete rndvi;
    delete rndvin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in ArithmetichWithRaster(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in ArithmetichWithRaster()!" << std::endl;
  }
}
