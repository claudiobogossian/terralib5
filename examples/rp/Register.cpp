#include "RPExamples.h"

// TerraLib
#include <terralib/rp/Register.h>
#include <terralib/raster/RasterFactory.h>

#include <iostream>

void Register()
{
  try
  {
    std::cout << "Example of how Register a TIF image using Raster Processing module." << std::endl << std::endl;
    
    // Openning the the input image
    
    std::map<std::string, std::string> auxRasterInfo;    
    auxRasterInfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
    te::rst::Raster* inputRasterPointer = te::rst::RasterFactory::open(
      auxRasterInfo );
    if( inputRasterPointer == 0 )
    {
      std::cout << "Raster 1 error." << std::endl;
      return;
    }
    
    // Creating the algorithm parameters
    
    te::rp::Register::InputParameters algoInputParams;
    
    algoInputParams.m_inputRasterPtr = inputRasterPointer;
    
    algoInputParams.m_inputRasterBands.push_back( 0 );
    algoInputParams.m_inputRasterBands.push_back( 1 );
    algoInputParams.m_inputRasterBands.push_back( 2 );
    
    te::gm::GTParameters::TiePoint tiePoint1;
    tiePoint1.first.x = 0.0;
    tiePoint1.first.y = 0.0;
    tiePoint1.second.x = 528980.0;
    tiePoint1.second.y = 7945280.0;
    algoInputParams.m_tiePoints.push_back( tiePoint1 );
    
    te::gm::GTParameters::TiePoint tiePoint2;
    tiePoint2.first.x = 874.0;
    tiePoint2.first.y = 0.0;
    tiePoint2.second.x = 546460.0;
    tiePoint2.second.y = 7945280.0;
    algoInputParams.m_tiePoints.push_back( tiePoint2 );
    
    te::gm::GTParameters::TiePoint tiePoint3;
    tiePoint3.first.x = 874.0;
    tiePoint3.first.y = 1008.0;
    tiePoint3.second.x = 546460.0;
    tiePoint3.second.y = 7925120.0;
    algoInputParams.m_tiePoints.push_back( tiePoint3 );

    te::gm::GTParameters::TiePoint tiePoint4;
    tiePoint4.first.x = 0.0;
    tiePoint4.first.y = 1008.0;
    tiePoint4.second.x = 528980.0;
    tiePoint4.second.y = 7925120.0;
    algoInputParams.m_tiePoints.push_back( tiePoint4 );
    
    algoInputParams.m_outputSRID = 29182;
    
    algoInputParams.m_outputResolutionX = 20.0;
    algoInputParams.m_outputResolutionY = 20.0;
    
    te::rp::Register::OutputParameters algoOutputParams;
    
    algoOutputParams.m_rInfo["URI"] =  
      TERRALIB_EXAMPLES_DATA_DIR "/GeoReferencedImagesRegisterExample.tif";  
    algoOutputParams.m_rType = "GDAL";
    
    // Executing the algorithm
    
    te::rp::Register algorithmInstance;
    
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

