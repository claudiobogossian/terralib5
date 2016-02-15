#include "RPExamples.h"

// TerraLib

#include <terralib/rp.h>
#include <terralib/raster.h>

// STL
#include <string>

void ArithmeticOperations()
{
  try
  {
    std::cout << "Example of arithmetic operations using bands of a Raster." << std::endl << std::endl;

// open input raster
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
    std::auto_ptr<te::rst::Raster> rin(te::rst::RasterFactory::open(rinfo)); 
  
// defining input parameters, the arithmetic operation will be 
// band / same band, to check if in the final result, the mean 
// will be 1.0
    te::rp::ArithmeticOperations::InputParameters inputParams;
    inputParams.m_arithmeticString = "R0:0 / R0:0";
    inputParams.m_normalize = false;
    inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = TERRALIB_DATA_DIR "/rasters/arithmetic_operations_result.tif";

// defining output parameters
    te::rp::ArithmeticOperations::OutputParameters outputParams;
    outputParams.m_rInfo = orinfo;
    outputParams.m_rType = "GDAL";

    te::rp::ArithmeticOperations algorithmInstance;

    if(!algorithmInstance.initialize(inputParams))
      throw;
    if(!algorithmInstance.execute(outputParams))
      throw;
    outputParams.reset();
    
// open output raster
    std::auto_ptr<te::rst::Raster> rout(te::rst::RasterFactory::open(orinfo));
    std::cout << rout->toString();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in ArithmeticOperations(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in ArithmeticOperations()!" << std::endl;
  }
}
