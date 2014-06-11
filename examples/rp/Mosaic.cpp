#include "RPExamples.h"

// TerraLib
#include <terralib/rp/GeoMosaic.h>
#include <terralib/raster/RasterFactory.h>

// STL
#include <vector>
#include <map>
#include <string>

void Mosaic()
{
  try
  {
    std::cout << "Mosaic of georeferenced images example using Raster Processing module." << std::endl << std::endl;
    
    std::map<std::string, std::string> auxRasterInfo;
    
    // Openning the the input images
    
    auxRasterInfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
    te::rst::Raster* inputRaster1Pointer = te::rst::RasterFactory::open(
      auxRasterInfo );
    if( inputRaster1Pointer == 0 )
    {
      std::cout << "Raster 1 error." << std::endl;
      return;
    }
    
    auxRasterInfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
    te::rst::Raster* inputRaster2Pointer =  te::rst::RasterFactory::open(
      auxRasterInfo );
    if( inputRaster2Pointer == 0 )
    {
      std::cout << "Raster 2 error." << std::endl;
      return;
    }
    
    auxRasterInfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR "/rasters/cbers_rgb342_crop3_EPSG_22522.tif";
    te::rst::Raster* inputRaster3Pointer ( te::rst::RasterFactory::open(
      auxRasterInfo ) );
    if( inputRaster3Pointer == 0 )
    {
      std::cout << "Raster 3 error." << std::endl;
      return;
    }
    
    // Creating the algorithm parameters
    
    te::rp::GeoMosaic::InputParameters algoInputParams;
    
    std::vector< const te::rst::Raster* > rasters;
    rasters.push_back( inputRaster1Pointer );
    rasters.push_back( inputRaster2Pointer );
    rasters.push_back( inputRaster3Pointer );
    te::rp::FeederConstRasterVector feeder( rasters );
    algoInputParams.m_feederRasterPtr = &feeder;
        
    // Input raster 1 bands to mosaic
    std::vector< unsigned int > bands;
    bands.push_back( 0 );
    bands.push_back( 1 );
    bands.push_back( 2 );
    algoInputParams.m_inputRastersBands.push_back( bands );
    
    // Input raster 2 bands to mosaic
    bands[ 0 ] = 1;
    bands[ 1 ] = 2;
    bands[ 2 ] = 0;
    algoInputParams.m_inputRastersBands.push_back( bands );
    
    // Input raster 3 bands to mosaic
    bands[ 2 ] = 2;
    bands[ 0 ] = 1;
    bands[ 1 ] = 0;
    algoInputParams.m_inputRastersBands.push_back( bands );
    
    te::rp::GeoMosaic::OutputParameters algoOutputParams;
    
    // The output raster info
    algoOutputParams.m_rInfo["URI"] =  
      TERRALIB_EXAMPLES_DATA_DIR "/rasters/GeoReferencedImagesMosaicExample.tif";  
    algoOutputParams.m_rType = "GDAL";
    
    // Executing the algorithm
    
    te::rp::GeoMosaic algorithmInstance;
    
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
      std::cout << "Algorithm execution OK." << std::endl << std::endl;
    }

    // clean up
    
    delete inputRaster1Pointer;
    delete inputRaster2Pointer;
    delete inputRaster3Pointer;
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

