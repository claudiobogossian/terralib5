#include "RasterExamples.h"

#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/raster/Reprojection.h>

#include <iostream>
#include <string>

void ReprojectRasterUsingDriver()
{
  try
  {
    std::cout << "Reprojecting raster data using the raster driver internal capacity." << std::endl << std::endl;

    // 0 - Set input data: a RS image
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
    te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

    // 1 - The entire image.
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_LL.tif";
    te::rst::Raster* outraster = inraster->transform(4326, rinfo);
    if (outraster)
    {
      std::cout << "End. Check the reprojected raster file: cbers2b_rgb342_crop_LL.tif." << std::endl << std::endl;
      delete outraster;
    }
    else
    {
      std::cout << "Reprojection failed!" << std::endl << std::endl;
      return;
    }

    // 2- To a given region.
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_LL_P.tif";
    te::rst::Raster* outraster2 = inraster->transform(4326, 773113, 7365893, 776145, 7368469, rinfo);
    if (outraster2)
    {
      std::cout << "End. Check the reprojected raster file: cbers2b_rgb342_crop_LL_P.tif." << std::endl << std::endl;
      delete outraster2;
    }
    else
    {
      std::cout << "Reprojection failed!" << std::endl << std::endl;
      return;
    }

    // 3- To a given region and resolution.
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_LL_PR.tif";
    te::rst::Raster* outraster3 = inraster->transform(4326, 773113, 7365893, 776145, 7368469, 0.0003, 0.0003, rinfo);
    if (outraster3)
    {
      std::cout << "End. Check the reprojected raster file: cbers2b_rgb342_crop_LL_PR.tif." << std::endl << std::endl;
      delete outraster3;
    }
    else
    {
      std::cout << "Reprojection to cbers2b_rgb342_crop_LL_PR failed!" << std::endl << std::endl;
      return;
    }
    delete inraster;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in ReprojectRasterUsingDriver(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in ReprojectRasterUsingDriver()!" << std::endl;
  }
}

void ReprojectRasterUsingFunction()
{
  try
  {
    std::cout << "Reprojecting raster data using TerraLib's own algorithm." << std::endl << std::endl;

    // 0 - Set input data: a RS image
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
    te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

    // 1 - The entire image.
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_LL_ALG.tif";
    te::rst::Raster* outraster = te::rst::Reproject(inraster, 4326, rinfo);
    if (outraster)
    {
      std::cout << "End. Check the reprojected raster file: cbers2b_rgb342_crop_LL_ALG.tif." << std::endl << std::endl;
      delete outraster;
    }
    else
    {
      std::cout << "Reprojection to cbers2b_rgb342_crop_LL_ALG failed!" << std::endl << std::endl;
      return;
    }

    // 2- To a given region.
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_LL_P_ALG.tif";
    te::rst::Raster* outraster2 = te::rst::Reproject(inraster, 4326, 773113, 7365893, 776145, 7368469, rinfo);
    if (outraster2)
    {
      std::cout << "End. Check the reprojected raster file: cbers2b_rgb342_crop_LL_P_ALG.tif." << std::endl << std::endl;
      delete outraster2;
    }
    else
    {
      std::cout << "Reprojection to cbers2b_rgb342_crop_LL_P_ALG failed !" << std::endl << std::endl;
      return;
    }

    // 3- To a given region and resolution.
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_LL_PR_ALG.tif";
    te::rst::Raster* outraster3 = te::rst::Reproject(inraster, 4326, 773113, 7365893, 776145, 7368469, 0.0006, 0.0006, rinfo);
    if (outraster3)
    {
      std::cout << "End. Check the reprojected raster file: cbers2b_rgb342_crop_LL_PR_ALG.tif." << std::endl << std::endl;
      delete outraster3;
    }
    else
    {
      std::cout << "Reprojection to cbers2b_rgb342_crop_LL_PR_ALG failed!" << std::endl << std::endl;
      return;
    }
    delete inraster;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in ReprojectRasterUsingFunction(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in ReprojectRasterUsingFunction()!" << std::endl;
  }
}
