#include "RPExamples.h"

// TerraLib
#include <terralib/raster.h>
#include <terralib/rp/Filter.h>
/*
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
#include <terralib/rp/Filter.h>

// STL
#include <string>
#include <map>
*/
// STL
#include <iostream>

void Filter()
{
  try
  {
    std::cout << "Filter example using Raster Processing module." << std::endl << std::endl;

// open input raster
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

    bool executeok = false;
    bool initok = false;
    {
// create output raster for sobel filter in band 0
      std::map<std::string, std::string> orinfo;
      orinfo["URI"] = TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_rgb342_crop_sobel_filter.tif";

// create filter algorithm parameters
      te::rp::Filter::InputParameters filterInputParameters;
      filterInputParameters.m_type = te::rp::Filter::InputParameters::MedianFilterT;
      filterInputParameters.m_windowH = 5;
      filterInputParameters.m_windowW = 5;
      filterInputParameters.m_enableProgress = true;
      filterInputParameters.m_inRasterPtr = rin;
      filterInputParameters.m_inRasterBands.push_back(0);
      
      filterInputParameters.m_inRasterBands.push_back(1);
      filterInputParameters.m_inRasterBands.push_back(2);

      te::rp::Filter::OutputParameters filterOutputParameters;
      filterOutputParameters.m_createdOutRasterInfo = orinfo;
      filterOutputParameters.m_createdOutRasterDSType = "GDAL";
      filterOutputParameters.m_normalizeOutput = false;

// execute the algorithm
      te::rp::Filter filterInstance;
      initok = filterInstance.initialize(filterInputParameters);
      if (initok)
        executeok = filterInstance.execute(filterOutputParameters);

      if (!executeok)
        std::cout << "Problems in Filter." << std::endl;
    }

// clean up
    delete rin;

    if (executeok)
      std::cout << "Done!" << std::endl << std::endl;
    else
      std::cout << "Problems in Filter." << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Filter(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Filter()!" << std::endl;
  }
}

