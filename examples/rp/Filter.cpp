#include "RPExamples.h"

// TerraLib
#include <terralib/rp/Filter.h>
#include <terralib/raster/RasterFactory.h>

#include <iostream>

void Filter()
{
  try
  {
    std::cout << "Example of how to apply a filter using Raster Processing module." << std::endl << std::endl;
    
    // Openning the the input image
    
    std::map<std::string, std::string> auxRasterInfo;    
    auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
    te::rst::Raster* inputRasterPointer = te::rst::RasterFactory::open(
      auxRasterInfo );
    if( inputRasterPointer == 0 )
    {
      std::cout << "Raster 1 error." << std::endl;
      return;
    }
    
    // Creating the algorithm parameters
    te::rp::Filter::InputParameters algoInputParams;

    algoInputParams.m_filterType = te::rp::Filter::InputParameters::RobertsFilterT;

    algoInputParams.m_inRasterPtr = inputRasterPointer;

    algoInputParams.m_inRasterBands.push_back( 0 );
    algoInputParams.m_inRasterBands.push_back( 1 );
    algoInputParams.m_inRasterBands.push_back( 2 );

    algoInputParams.m_iterationsNumber = 1;

    te::rp::Filter::OutputParameters algoOutputParams;

    algoOutputParams.m_rInfo["URI"] =
      TERRALIB_DATA_DIR "/rasters/FilterExample.tif";
    algoOutputParams.m_rType = "GDAL";

    // Executing the algorithm
    te::rp::Filter algorithmInstance;
    if( ! algorithmInstance.initialize( algoInputParams ) )
    {
      std::cout << "Algorithm initialization error." << std::endl;
      return;
    }

    if( ! algorithmInstance.execute( algoOutputParams ) )
    {
      std::cout << "Algorithm execution error." << std::endl;
      return;
    }
    else
    {
      std::cout << "Algorithm execution OK." << std::endl;
    }

    // clean up
    
    delete inputRasterPointer;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred:" << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred!" << std::endl;
  }
}

