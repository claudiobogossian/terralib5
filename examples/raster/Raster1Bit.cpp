#include "RasterExamples.h"

// TerraLib
#include <terralib/gdal/Utils.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>

// STL
#include <iostream>
#include <string>

void Raster1Bit()
{
  try
  {
    std::cout << "This example shows how to create an 1bit Raster in memory, and how to save an 1bit Raster in Tiff format." << std::endl << std::endl;

// 1. create empty 1bit image in memory
    // define grid
    te::rst::Grid* grid = new te::rst::Grid(32, 32);

    // define band properties
    std::vector<te::rst::BandProperty*> bprops;
    bprops.push_back(new te::rst::BandProperty(0, te::dt::R1BIT_TYPE));

    // define tiles information (in this case, only one block used)
    bprops[0]->m_blkh = grid->getNumberOfRows();
    bprops[0]->m_blkw = grid->getNumberOfColumns();
    bprops[0]->m_nblocksx = 1;
    bprops[0]->m_nblocksy = 1;

    te::rst::Raster* mem1bitraster = te::rst::RasterFactory::make("MEM", grid, bprops, std::map<std::string, std::string>());

    // fill raster (only 0's and 1's are allowed)
    for (unsigned r = 0; r < mem1bitraster->getNumberOfRows(); r++)
      for (unsigned c = 0; c < mem1bitraster->getNumberOfColumns(); c++)
      {
        if ((c > 5 && c < 25) && (r > 10 && r < 20))
          mem1bitraster->setValue(c, r, 1);
        else
          mem1bitraster->setValue(c, r, 0);
      }

    // clean up
    delete mem1bitraster;

// 2. create tiff with 1bit using gdal, need to set NBIT=1
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/creating-1bit-raster.tif";
    rinfo["NBITS"] = "1";

    bprops.clear();
    bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE, "1bit band 0"));

    te::rst::Grid* tif1bitrastergrid = new te::rst::Grid(32, 32);

    te::rst::Raster* tif1bitraster = te::rst::RasterFactory::make("GDAL", tif1bitrastergrid, bprops, rinfo);

    // fill raster (only 0's and 1's are allowed)
    for (unsigned r = 0; r < tif1bitraster->getNumberOfRows(); r++)
      for (unsigned c = 0; c < tif1bitraster->getNumberOfColumns(); c++)
      {
        if ((c > 5 && c < 25) && (r > 10 && r < 20))
          tif1bitraster->setValue(c, r, 1);
        else
          tif1bitraster->setValue(c, r, 0);
      }

    // clean up
    delete tif1bitraster;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in Raster1Bit(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in Raster1Bit()!" << std::endl;
  }
}
