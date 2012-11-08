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
    rinfo["URI"] = TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";
    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

    bool executeok = false;
    bool initok = false;
    {
      std::cout << "Using Linear Mixture Model" << std::endl;

// create output raster for linear mixture model
      std::map<std::string, std::string> orinfo;
      orinfo["URI"] = TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop_linearMixtureModel.tif";

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
      mmInputParameters.m_components["clouds"].push_back(1.0);
      mmInputParameters.m_components["clouds"].push_back(0.725);
      mmInputParameters.m_components["clouds"].push_back(1.0);

      mmInputParameters.m_components["vegetation"].push_back(0.392);
      mmInputParameters.m_components["vegetation"].push_back(0.251);
      mmInputParameters.m_components["vegetation"].push_back(0.604);

      mmInputParameters.m_components["shadow"].push_back(0.165);
      mmInputParameters.m_components["shadow"].push_back(0.098);
      mmInputParameters.m_components["shadow"].push_back(0.196);


/*
  componentList.insertComponent(0, "nuvem");
  componentList.insertPixel(0, 0, 1.000);
  componentList.insertPixel(0, 1, 0.725);
  componentList.insertPixel(0, 2, 1.000);

  componentList.insertComponent(1, "vegetacao");
  componentList.insertPixel(1, 0, 0.392);
  componentList.insertPixel(1, 1, 0.251);
  componentList.insertPixel(1, 2, 0.604);

  componentList.insertComponent(2, "sombra");
  componentList.insertPixel(2, 0, 0.165);
  componentList.insertPixel(2, 1, 0.098);
  componentList.insertPixel(2, 2, 0.196);

  TePDIMixModelSpectralBandList spectralBandList;
  spectralBandList.insertSpectralBand(TePDIMixModelSpectralBand(250, 2, 0.450000, 0.520000, "CBERS2_CCD_BLUE"));
  spectralBandList.insertSpectralBand(TePDIMixModelSpectralBand(250, 3, 0.520000, 0.590000, "CBERS2_CCD_GREEN"));
  spectralBandList.insertSpectralBand(TePDIMixModelSpectralBand(250, 4, 0.630000, 0.690000, "CBERS2_CCD_RED"));

*/

// defining output parameters
      mmOutputParameters.m_rInfo = orinfo;
      mmOutputParameters.m_rType = "GDAL";

// execute the algorithm
      te::rp::MixtureModel mmInstance;

      initok = mmInstance.initialize(mmInputParameters);

      if (initok)
        executeok = mmInstance.execute(mmOutputParameters);

      if (!executeok)
        std::cout << "Problems in linear mixture model." << std::endl;

    }

    // clean up
    delete rin;

    if (executeok)
      std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in MixtureModel(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in MixtureModel()!" << std::endl;
  }
}

