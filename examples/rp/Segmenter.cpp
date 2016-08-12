#include "RPExamples.h"

// TerraLib
#include <terralib/raster.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/memory.h>
#include <terralib/rp.h>

// STL
#include <string>
#include <map>

void Segmenter()
{
  // -------------------------------------------------------------------------------------------------------------
  // Region growing segmenter (with mean features) example

  try
  {
    std::cout << "Region growing Segmentation example using Raster Processing module." << std::endl << std::endl;

// open input raster
    std::string data_dir = TERRALIB_DATA_DIR;
    std::string aux("");
    std::cout << "Inform your input image (ENTER to accept default \'" << (data_dir + "/geotiff/cbers2b_rgb342_crop.tif") << "\'): ";
    std::getline (std::cin, aux);
    std::map<std::string, std::string> rinfo;
    if (!aux.empty())
      rinfo["URI"] = aux;
    else
      rinfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::cout << "Inform your output image (ENTER to accept default \'" << (data_dir + "/geotiff/cbers2b_rgb342_crop_segmentedMean.tif") << "\'): ";
    std::getline (std::cin, aux);
    std::map<std::string, std::string> orinfo;
    if (!aux.empty())
      orinfo["URI"] = aux;
    else
      orinfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop_segmentedMean.tif";

// define segmentation parameters

// input parameters
    te::rp::Segmenter::InputParameters algoInputParameters;
    algoInputParameters.m_inputRasterPtr = rin;
    algoInputParameters.m_inputRasterBands.push_back( 0 );
    algoInputParameters.m_inputRasterBands.push_back( 1 );
    algoInputParameters.m_inputRasterBands.push_back( 2 );

// link specific parameters with chosen implementation
// strategy specific parameters (m_minSegmentSize: size of the smallest segment to be created; m_segmentsSimilarityThreshold: similarity between neighboring segments to merge them or not)
    te::rp::SegmenterRegionGrowingMeanStrategy::Parameters segparameters;
    segparameters.m_minSegmentSize = 50;
    segparameters.m_segmentsSimilarityThreshold = 0.3;

    algoInputParameters.m_strategyName = "RegionGrowingMean";
    algoInputParameters.setSegStrategyParams(segparameters);

// output parameters
// the output can be a previously created raster (in this case, rout)

    te::rp::Segmenter::OutputParameters algoOutputParameters;
    algoOutputParameters.m_rInfo = orinfo;
    algoOutputParameters.m_rType = "GDAL";

// execute the algorithm
    te::rp::Segmenter seginstance;

    if( !seginstance.initialize( algoInputParameters ) ) throw;
    if( !seginstance.execute( algoOutputParameters ) ) throw;

// clean up
    delete rin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in Segmenter(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in Segmenter()!" << std::endl;
  }
  
  // -------------------------------------------------------------------------------------------------------------
  // Region growing segmenter (with Baatz features) example
  
  try
  {
    std::cout << "Region growing Segmentation (Baatz features) example using Raster Processing module." << std::endl << std::endl;

    // open input raster
    std::string data_dir = TERRALIB_DATA_DIR;
    std::string aux("");
    std::cout << "Inform your input image (ENTER to accept default \'" << (data_dir + "/geotiff/cbers2b_rgb342_crop.tif") << "\'): ";
    std::getline (std::cin, aux);
    std::map<std::string, std::string> rinfo;
    if (!aux.empty())
      rinfo["URI"] = aux;
    else
      rinfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";
    
    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);
    
    // create output raster info
    std::cout << "Inform your output image (ENTER to accept default \'" << (data_dir + "/geotiff/cbers2b_rgb342_crop_segmentedBaatz.tif") << "\'): ";
    std::getline (std::cin, aux);
    std::map<std::string, std::string> orinfo;
    if (!aux.empty())
      orinfo["URI"] = aux;
    else
      orinfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop_segmentedBaatz.tif";

// define segmentation parameters

// input parameters
    te::rp::Segmenter::InputParameters algoInputParameters;
    algoInputParameters.m_inputRasterPtr = rin;
    algoInputParameters.m_inputRasterBands.push_back( 0 );
    algoInputParameters.m_inputRasterBands.push_back( 1 );
    algoInputParameters.m_inputRasterBands.push_back( 2 );

// link specific parameters with chosen implementation
// strategy specific parameters (m_minSegmentSize: size of the smallest segment to be created; m_segmentsSimilarityThreshold: similarity between neighboring segments to merge them or not)
    te::rp::SegmenterRegionGrowingBaatzStrategy::Parameters segparameters;
    segparameters.m_minSegmentSize = 50;
    segparameters.m_segmentsSimilarityThreshold = 0.3;
    segparameters.m_bandsWeights.resize( 3, 0.333333);
	  segparameters.m_bandsWeights[2] = 1 - segparameters.m_bandsWeights[0] - segparameters.m_bandsWeights[1];
    segparameters.m_colorWeight = 0.5;
    segparameters.m_compactnessWeight = 0.5;

    algoInputParameters.m_strategyName = "RegionGrowingBaatz";
    algoInputParameters.setSegStrategyParams(segparameters);

// output parameters
// the output can be a previously created raster (in this case, rout)

    te::rp::Segmenter::OutputParameters algoOutputParameters;
    algoOutputParameters.m_rInfo = orinfo;
    algoOutputParameters.m_rType = "GDAL";

// execute the algorithm
    te::rp::Segmenter seginstance;

    if( ! seginstance.initialize(algoInputParameters) ) throw;
    if( ! seginstance.execute( algoOutputParameters ) ) throw;

// clean up
    delete rin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in Segmenter(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in Segmenter()!" << std::endl;
  }  
  
}

