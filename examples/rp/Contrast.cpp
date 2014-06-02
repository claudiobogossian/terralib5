#include "RPExamples.h"

// TerraLib
#include <terralib/gdal/Utils.h>
#include <terralib/dataaccess.h>
//#include <terralib/dataaccess/dataset/DataSetPersistence.h>
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
#include <iostream>

void Contrast()
{
  try
  {
    std::cout << "Linear Contrast example using Raster Processing module." << std::endl << std::endl;

// open input raster
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

    bool executeok = false;
    bool initok = false;
    {
      std::cout << "Using Linear Contrast" << std::endl;

// create output raster for linear contrast
      std::map<std::string, std::string> orinfo;
      orinfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR"/rasters/cbers2b_rgb342_crop_linear_contrast.tif";

// create contrast algorithm parameters
      te::rp::Contrast::InputParameters contInputParameters;
      te::rp::Contrast::OutputParameters contOutputParameters;

      contInputParameters.m_type = te::rp::Contrast::InputParameters::LinearContrastT;
      contInputParameters.m_lCMinInput.resize( rin->getNumberOfBands(), 100 );
      contInputParameters.m_lCMaxInput.resize( rin->getNumberOfBands(), 255 );
      contInputParameters.m_inRasterPtr = rin;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
      {
        contInputParameters.m_inRasterBands.push_back(b);
      }
      contOutputParameters.m_createdOutRasterInfo = orinfo;
      contOutputParameters.m_createdOutRasterDSType = "GDAL";

// execute the algorithm
      te::rp::Contrast continstance;

      initok = continstance.initialize(contInputParameters );

      if (initok)
        executeok = continstance.execute( contOutputParameters );

      if (!executeok)
        std::cout << "Problems in linear contrast." << std::endl;
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

