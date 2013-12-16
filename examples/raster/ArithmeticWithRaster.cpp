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
    sri["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_rgb342_crop.tif";
    srndvi["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_ndvi.tif";
    srndvin["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_ndvi_normalized.tif";
    std::string base_path = ""TE_DATA_EXAMPLE_DIR"/data/rasters/";
    srden["URI"] = "ndvi_den.tif"; // temporary raster dataset

// open input raster (band 0 = Red, band 1 = IRed, band 2 = Blue)
    te::rst::Raster* ri = te::rst::RasterFactory::open(sri);

// create temporary raster
    te::rst::Grid* gden = new te::rst::Grid(*ri->getGrid());
    std::vector<te::rst::BandProperty*> bdsden;
    bdsden.push_back(new te::rst::BandProperty(0, te::dt::FLOAT_TYPE, "ndvi den"));
    te::rst::RasterProperty* rpden = new te::rst::RasterProperty(gden, bdsden, srden);

    te::da::DataSetType* dstpden = new te::da::DataSetType(srden["URI"]);
    dstpden->add(rpden);

// access a raster datasource to create temporary raster
    std::map<std::string, std::string> connInfoRaster;
    connInfoRaster["URI"] = base_path;
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(connInfoRaster);
    ds->open();

    std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();

// create temporary raster using data set type persistence
    tr->createDataSet(dstpden, std::map<std::string, std::string> ());

// access the temporary data set
    std::auto_ptr<te::da::DataSet> dsden = tr->getDataSet(srden["URI"], te::common::FORWARDONLY, false, te::common::RWAccess);

    std::size_t rpos = te::da::GetFirstPropertyPos(dsden.get(), te::dt::RASTER_TYPE);
    te::gdal::Raster* rden = static_cast<te::gdal::Raster*> (dsden->getRaster(rpos).release());
    //te::gdal::Raster* rden = static_cast<te::gdal::Raster*> (dsden->getRaster());

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

// calculate ndvi = (IRed - Red) / (IRed + Red)
    *rndvi /= *rden;

// show ndvi status
    std::cout << rndvi->toString();

// create normalized ndvi [-1,1] -> [0,255] for visualization
    te::rst::Grid* gndvin = new te::rst::Grid(*ri->getGrid());
    std::vector<te::rst::BandProperty*> bdsndvin;
    bdsndvin.push_back(new te::rst::BandProperty(0, te::dt::CHAR_TYPE));
    te::rst::Raster* rndvin = te::rst::RasterFactory::make(gndvin, bdsndvin, srndvin);

    double ndvimin = rndvi->getBand(0)->getMinValue().real();
    double ndvimax = 255 / (rndvi->getBand(0)->getMaxValue().real() - ndvimin);
    for (unsigned r = 0; r < rndvi->getNumberOfRows(); r++)
      for (unsigned c = 0; c < rndvi->getNumberOfColumns(); c++)
      {
        rndvi->getValue(c, r, value, 0);
        value-=ndvimin;
        value*=ndvimax;
        rndvin->setValue(c, r, value, 0);
      }

// remove temporary raster using data set type persistence
    delete rden;
    tr->dropDataSet(base_path + srden["URI"]);

// clean up
    delete ri;
    delete rndvi;
    delete rndvin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in ArithmetichWithRaster(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in ArithmetichWithRaster()!" << std::endl;
  }
}
