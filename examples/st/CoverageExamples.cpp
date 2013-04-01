// Examples
#include "STExamples.h"

//TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/st.h>
#include <terralib/stloader.h>

// STL
#include <iostream>

void CoverageExamples()
{
  try
  {  
    te::st::Coverage* output = 0;
    
    std::string XMLFileName = TE_DATA_EXAMPLE_DIR"\\data\\geotif\\coverage_angra_metadata.xml";
  
    te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");

    te::stloader::DataLoader::loadCoverage(ds, XMLFileName, &output);

    double sedeAngraLat = -23.007; 
    double sedeAngraLong = -44.318;
    te::gm::Point p(sedeAngraLong, sedeAngraLat);
    
    std::vector<te::st::TimeSeries*> result; 
    output->timeSeries(p, result);

    PrintTimeSeries(result[0]);
  
    te::common::FreeContents(result);
    delete ds;
    delete output;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in CoverageExamples: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in CoverageExamples!" << std::endl;
  } 
}

