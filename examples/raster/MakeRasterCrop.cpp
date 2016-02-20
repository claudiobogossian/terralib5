#include "RasterExamples.h"

// TerraLib
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/raster/RasterFactory.h>

// STL
#include <iostream>
#include <string>
#include <map>

void MakeRasterCrop()
{
  try
  {
    std::cout << "This test creates an image crop with a provided envelope." << std::endl << std::endl;

    std::map<std::string, std::string> irinfo;

    irinfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* inraster = te::rst::RasterFactory::open(irinfo);

// set envelope based on line/column
    te::gm::Coord2D cur = inraster->getGrid()->gridToGeo(299, 150);
    te::gm::Coord2D cll = inraster->getGrid()->gridToGeo(100, 299);
    te::gm::Envelope* blenv = new te::gm::Envelope(cll.x, cll.y, cur.x, cur.y);

    std::map<std::string, std::string> orinfo;

    orinfo["URI"] = TERRALIB_DATA_DIR "/rasters/raster-crop.tif";

    te::rst::Raster* outraster = inraster->trim(blenv, orinfo);

    delete inraster;
    delete outraster;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in MakeRasterCrop(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in MakeRasterCrop()!" << std::endl;
  }

}

