#include "RasterExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/raster.h>
#include <terralib/geometry.h>
#include <terralib/raster/RasterFactory.h>

// STL
#include <complex>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void SplitComplexImage()
{
  try
  {
    std::cout << "This is a test to open a complex image and split into real/imaginary rasters." << std::endl << std::endl;

    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/te_complex.tif";

    te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

    std::map<std::string, std::string> realinfo;
    realinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/real.tif";

    std::map<std::string, std::string> imaginfo;
    imaginfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/imag.tif";

    te::rst::Grid* realgrid = new te::rst::Grid(*inraster->getGrid());

    te::rst::Grid* imaggrid = new te::rst::Grid(*inraster->getGrid());

    std::vector<te::rst::BandProperty*> realbprops;

    std::vector<te::rst::BandProperty*> imagbprops;

    realbprops.push_back(new te::rst::BandProperty(0, te::dt::CHAR_TYPE, "real band"));

    imagbprops.push_back(new te::rst::BandProperty(0, te::dt::CHAR_TYPE, "imag band"));

    te::rst::Raster* realraster = te::rst::RasterFactory::make(realgrid, realbprops, realinfo);

    te::rst::Raster* imagraster = te::rst::RasterFactory::make(imaggrid, imagbprops, imaginfo);

    std::complex<double> value;
    for (unsigned r = 0; r < inraster->getNumberOfRows(); r++)
      for (unsigned c = 0; c < inraster->getNumberOfColumns(); c++)
      {
        inraster->getValue(c, r, value);
        realraster->setValue(c, r, value.real());
        imagraster->setValue(c, r, value.imag());
      }

    delete inraster;
    delete realraster;
    delete imagraster;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in SplitComplexImage(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in SplitComplexImage()!" << std::endl;
  }
}
