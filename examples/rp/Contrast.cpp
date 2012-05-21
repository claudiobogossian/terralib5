#include "RPExamples.h"

// TerraLib
#include <terralib/gdal/Utils.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/dataset/DataSetPersistence.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/geometry/GeometryProperty.h>
#include <terralib/memory/DataSet.h>
#include <terralib/memory/DataSetItem.h>
#include <terralib/raster.h>
#include <terralib/raster/RasterSummaryManager.h>
#include <terralib/rp/Contrast.h>

// STL
#include <string>
#include <map>

void Contrast()
{
  try
  {
    std::cout << "Linear Contrast example using Raster Processing module." << std::endl << std::endl;

// open input raster
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

    bool executeok = false;
    bool initok = false;
    {
      std::cout << "Using Linear Contrast" << std::endl;

// create output raster for linear contrast
      std::map<std::string, std::string> orinfo;
      orinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop_linear_contrast.tif";

      te::rst::Grid* ogrid = new te::rst::Grid(*rin->getGrid());

      std::vector<te::rst::BandProperty*> obands;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
        obands.push_back(new te::rst::BandProperty(b, rin->getBandDataType(b), "band with linear contrast"));

      te::rst::Raster* rout = te::rst::RasterFactory::make(ogrid, obands, orinfo);

// create contrast algorithm parameters
      te::rp::Contrast::InputParameters contInputParameters;
      te::rp::Contrast::OutputParameters contOutputParameters;

      contInputParameters.m_type = te::rp::Contrast::InputParameters::LinearContrastT;
      contInputParameters.m_lCMinInput = 100;
      contInputParameters.m_lCMaxInput = 255;
      contInputParameters.m_inRasterPtr = rin;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
      {
        contInputParameters.m_inRasterBands.push_back(b);
        contOutputParameters.m_outRasterBands.push_back(b);
      }
      contOutputParameters.m_outRasterPtr = rout;

// execute the algorithm
      te::rp::Contrast continstance;

      initok = continstance.initialize(contInputParameters, contOutputParameters );

      if (initok)
        executeok = continstance.execute();

      if (!executeok)
        std::cout << "Problems in linear contrast." << std::endl;

      delete rout;
    }

    // clean up
    delete rin;

    if (executeok)
      std::cout << "Done!" << std::endl << std::endl;
    else
      std::cout << "Problems in contrast." << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Contrast(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Contrast()!" << std::endl;
  }
}

