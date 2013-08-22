#include "RPExamples.h"

// TerraLib
#include <terralib/rp/Skeleton.h>
#include <terralib/raster/RasterFactory.h>

#include <iostream>

void Skeleton()
{
  try
  {
    std::cout << "Example of how to apply the skeleton detection using Raster Processing module." << std::endl << std::endl;
    
    // Openning the the input image
    
    std::map<std::string, std::string> auxRasterInfo;    
    auxRasterInfo["URI"] = TE_DATA_EXAMPLE_DIR "/data/rasters/cbers_rgb342_crop1.tif";
    te::rst::Raster* inputRasterPointer = te::rst::RasterFactory::open(
      auxRasterInfo );
    if( inputRasterPointer == 0 )
    {
      std::cout << "Raster 1 error." << std::endl;
      return;
    }
    
    // Creating the algorithm parameters
  
    te::rp::Skeleton::InputParameters algoInputParams;
    
    algoInputParams.m_inputRasterPtr = inputRasterPointer;

    te::rp::Skeleton::OutputParameters algoOutputParams;
    
    algoOutputParams.m_rInfo["URI"] =  
      TE_DATA_EXAMPLE_DIR "/data/rasters/SkeletonExample.tif";  
    algoOutputParams.m_rType = "GDAL";
    
    // Executing the algorithm
    
    te::rp::Skeleton algorithmInstance;
    
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
    std::cout << std::endl << "An exception has occuried:" << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;
  }
}

