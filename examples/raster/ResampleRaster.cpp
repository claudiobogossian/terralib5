#include "RasterExamples.h"

// TerraLib
#include <terralib/raster.h>
#include <terralib/raster/Interpolator.h>
#include <terralib/raster/RasterFactory.h>

// STL
#include <cstdio>
#include <iostream>
#include <string>

void ResampleRaster()
{
  try
  {
    std::cout << "This test resample one raster based on user defined parameters." << std::endl << std::endl;

    std::map<std::string, std::string> rinfo;

// set input raster name
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

// open input raster
    te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

// set output raster names
    std::map<std::string, std::string> nnname;
    nnname["URI"] = ""TERRALIB_DATA_DIR"/rasters/resampled_nn_scale_plus2.tif";

    std::map<std::string, std::string> biliname;
    biliname["URI"] = ""TERRALIB_DATA_DIR"/rasters/resampled_bili_scale_minus2.tif";

    std::map<std::string, std::string> bicname;
    bicname["URI"] = ""TERRALIB_DATA_DIR"/rasters/resampled_bic_scale_minus3.tif";

// applies the interpolation
    te::rst::Raster* nnraster = inraster->resample(te::rst::NearestNeighbor, 2, nnname);
    delete nnraster;
    std::cout << "   Raster " << nnname["URI"] << " created!" << std::endl;

    te::rst::Raster* biliraster = inraster->resample(te::rst::Bilinear, -2, biliname);
    delete biliraster;
    std::cout << "   Raster " << biliname["URI"] << " created!" << std::endl;

    te::rst::Raster* bicraster = inraster->resample(te::rst::Bicubic, -3, bicname);
    delete bicraster;
    std::cout << "   Raster " << bicname["URI"] << " created!" << std::endl;

    delete inraster;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in MakeRasterBlock(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in MakeRasterBlock()!" << std::endl;
  }
}
