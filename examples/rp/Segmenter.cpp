#include "RPExamples.h"

// TerraLib
#include <terralib/raster.h>
#include <terralib/dataaccess.h>
//#include <terralib/dataaccess/dataset/DataSetPersistence.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/geometry/GeometryProperty.h>
#include <terralib/memory/DataSet.h>
#include <terralib/memory/DataSetItem.h>
#include <terralib/rp/SegmenterRegionGrowingStrategy.h>
#include <terralib/rp/Segmenter.h>

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
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_segmentedMean.tif";

// define segmentation parameters

// input parameters
    te::rp::Segmenter::InputParameters algoInputParameters;
    algoInputParameters.m_inputRasterPtr = rin;
    algoInputParameters.m_inputRasterBands.push_back( 0 );
    algoInputParameters.m_inputRasterBands.push_back( 1 );
    algoInputParameters.m_inputRasterBands.push_back( 2 );

// link specific parameters with chosen implementation
// strategy specific parameters (m_minSegmentSize: size of the smallest segment to be created; m_segmentsSimilarityThreshold: similarity between neighboring segments to merge them or not)
    te::rp::SegmenterRegionGrowingStrategy::Parameters segparameters;
    segparameters.m_minSegmentSize = 50;
    segparameters.m_segmentsSimilarityThreshold = 0.3;
    segparameters.m_segmentFeatures = te::rp::SegmenterRegionGrowingStrategy::Parameters::MeanFeaturesType;

    algoInputParameters.m_strategyName = "RegionGrowing";
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
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_segmentedBaatz.tif";

// define segmentation parameters

// input parameters
    te::rp::Segmenter::InputParameters algoInputParameters;
    algoInputParameters.m_inputRasterPtr = rin;
    algoInputParameters.m_inputRasterBands.push_back( 0 );
    algoInputParameters.m_inputRasterBands.push_back( 1 );
    algoInputParameters.m_inputRasterBands.push_back( 2 );

// link specific parameters with chosen implementation
// strategy specific parameters (m_minSegmentSize: size of the smallest segment to be created; m_segmentsSimilarityThreshold: similarity between neighboring segments to merge them or not)
    te::rp::SegmenterRegionGrowingStrategy::Parameters segparameters;
    segparameters.m_minSegmentSize = 50;
    segparameters.m_segmentsSimilarityThreshold = 0.3;
    segparameters.m_segmentFeatures = te::rp::SegmenterRegionGrowingStrategy::Parameters::BaatzFeaturesType;
    segparameters.m_bandsWeights.resize( 3, 0.333333);
	  segparameters.m_bandsWeights[2] = 1 - segparameters.m_bandsWeights[0] - segparameters.m_bandsWeights[1];
    segparameters.m_colorWeight = 0.5;
    segparameters.m_compactnessWeight = 0.5;

    algoInputParameters.m_strategyName = "RegionGrowing";
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

