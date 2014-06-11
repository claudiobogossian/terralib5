#include "RPExamples.h"

// TerraLib
#include <terralib/raster.h>
#include <terralib/rp/MixtureModel.h>
#include <terralib/rp/MixtureModelLinearStrategy.h>

// STL
#include <iostream>

void MixtureModel()
{
  try
  {
    std::cout << "Raster decomposition using Mixture Model module." << std::endl << std::endl;

// open input raster
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

    bool executeok = false;
    bool initok = false;
    {
      std::cout << "Using Linear Mixture Model" << std::endl;

// create output raster for linear mixture model
      std::map<std::string, std::string> orinfo;
      orinfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_linearMixtureModel.tif";

// create algorithm parameters
      te::rp::MixtureModel::InputParameters mmInputParameters;
      te::rp::MixtureModel::OutputParameters mmOutputParameters;

// defining input parameters
      mmInputParameters.m_inputRasterPtr = rin;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
        mmInputParameters.m_inputRasterBands.push_back(b);
      mmInputParameters.m_strategyName = "linear";

// link specific parameters with chosen implementation
      te::rp::MixtureModelLinearStrategy::Parameters lmmParameters;
      mmInputParameters.setMixtureModelStrategyParams(lmmParameters);

// defining endmembers
      mmInputParameters.m_components["clouds"].push_back(255.0);
      mmInputParameters.m_components["clouds"].push_back(184.875);
      mmInputParameters.m_components["clouds"].push_back(255.0);

      mmInputParameters.m_components["shadow"].push_back(42.075);
      mmInputParameters.m_components["shadow"].push_back(24.99);
      mmInputParameters.m_components["shadow"].push_back(49.98);

      mmInputParameters.m_components["vegetation"].push_back(99.96);
      mmInputParameters.m_components["vegetation"].push_back(64.005);
      mmInputParameters.m_components["vegetation"].push_back(154.02);

// defining sensor information
      mmInputParameters.m_inputSensorBands.push_back("CBERS2_CCD_3_RED");
      mmInputParameters.m_inputSensorBands.push_back("CBERS2_CCD_4_NIR");
      mmInputParameters.m_inputSensorBands.push_back("CBERS2_CCD_2_GREEN");

// defining output parameters
      mmOutputParameters.m_rInfo = orinfo;
      mmOutputParameters.m_rType = "GDAL";
      mmOutputParameters.m_normalizeOutput = true;
      mmOutputParameters.m_createErrorRaster = true;

// execute the algorithm
      te::rp::MixtureModel mmInstance;

      initok = mmInstance.initialize(mmInputParameters);

      if (initok)
        executeok = mmInstance.execute(mmOutputParameters);

      if (!executeok)
        std::cout << "Problems in linear mixture model." << std::endl;

      std::cout << mmOutputParameters.m_outputRasterPtr->toString() << std::endl;
    }

    // clean up
    delete rin;

    if (executeok)
      std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in MixtureModel(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in MixtureModel()!" << std::endl;
  }
}

