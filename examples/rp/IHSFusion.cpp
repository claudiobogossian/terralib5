#include "RPExamples.h"

// TerraLib
#include <terralib/rp/IHSFusion.h>
#include <terralib/raster/RasterFactory.h>

#include <iostream>

void IHSFusion()
{
  try
  {
    std::cout << "Example of how to apply the IHS Fusion using Raster Processing module." << std::endl << std::endl;
    
    // Openning the the input image
    
    std::map<std::string, std::string> auxRasterInfo1;    
    auxRasterInfo1["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
    te::rst::Raster* inputRasterPointer1 = te::rst::RasterFactory::open(
      auxRasterInfo1 );
    if( inputRasterPointer1 == 0 )
    {
      std::cout << "Raster 1 error." << std::endl;
      return;
    }
    
    std::map<std::string, std::string> auxRasterInfo2;    
    auxRasterInfo2["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
    te::rst::Raster* inputRasterPointer2 = te::rst::RasterFactory::open(
      auxRasterInfo2 );
    if( inputRasterPointer2 == 0 )
    {
      std::cout << "Raster 2 error." << std::endl;
      return;
    }    
    
    // Creating the algorithm parameters
  
    te::rp::IHSFusion::InputParameters algoInputParams;
    
    algoInputParams.m_lowResRasterPtr = inputRasterPointer1;
    algoInputParams.m_lowResRasterRedBandIndex = 0;
    algoInputParams.m_lowResRasterGreenBandIndex = 1;
    algoInputParams.m_lowResRasterBlueBandIndex = 2;
    algoInputParams.m_highResRasterPtr = inputRasterPointer2;
    algoInputParams.m_highResRasterBand = 0;

    te::rp::IHSFusion::OutputParameters algoOutputParams;
    
    algoOutputParams.m_rInfo["URI"] =  
      TERRALIB_DATA_DIR "/rasters/IHSFusionExample.tif";  
    algoOutputParams.m_rType = "GDAL";
    
    // Executing the algorithm
    
    te::rp::IHSFusion algorithmInstance;
    
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
    
    delete inputRasterPointer1;
    delete inputRasterPointer2;
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

