#include "RasterExamples.h"

// TerraLib
#include <terralib/memory.h>
#include <terralib/raster.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/raster/RasterSummaryManager.h>

// STL
#include <iostream>
#include <map>

void RasterInMemory()
{
  try
  {
    std::cout << "This test uses the memory driver." << std::endl << std::endl;

    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::mem::Raster r;
    te::rst::Raster* memraster = te::rst::RasterFactory::open(std::string("MEM"), rinfo, te::common::RWAccess);

    const te::rst::RasterSummary* rs = te::rst::RasterSummaryManager::getInstance().get(memraster, te::rst::SUMMARY_ALL);

    std::cout << "band " << 0 << std::endl;
    std::cout << "   min val: " << *rs->at(0).m_minVal << std::endl;
    std::cout << "   max val: " << *rs->at(0).m_maxVal << std::endl;
    std::cout << "   std val: " << *rs->at(0).m_stdVal << std::endl;
    std::cout << "   mean val: " << *rs->at(0).m_meanVal << std::endl;

    std::cout << "   histogram:" << std::endl;
    std::cout << "   " << *rs->at(0).m_minVal << std::endl;

    std::map<double, unsigned>::iterator it = rs->at(0).m_histogramR->begin();
    while (it != rs->at(0).m_histogramR->end())
    {
      std::cout << "      ";

      for (unsigned i = 0; i < it->second; i+=200)
        std::cout << "*";

      std::cout << std::endl;

      ++it;
      ++it;
      ++it;
    }
    std::cout << "   " << *rs->at(0).m_maxVal << std::endl;

    delete memraster;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in RasterInMemory(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in RasterInMemory()!" << std::endl;
  }

}

