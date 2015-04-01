// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/stmemory.h>
#include <terralib/st.h>
#include <terralib/datatype.h>

// Examples
#include "STExamples.h"

// STL
#include <iostream>

std::auto_ptr<te::st::CoverageSeries> LoadCoverageSeriesFromGeotif()
{
  try
  {     
    //Indicates the data source
    te::da::DataSourceInfo dsinfo;

    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TERRALIB_DATA_DIR"/st/coverage/geotif";
    dsinfo.setConnInfo(connInfo);
    dsinfo.setType("GDAL");

    //It creates a new Data Source and put it into the manager
    CreateDataSourceAndUpdateManager(dsinfo);

    //Indicates the raster file names and their associated times
	  te::st::RasterCoverageSeriesDataSetInfo cvsinfo;
	
    te::dt::TimeInstant* time1 = new te::dt::TimeInstant(te::dt::Date(2009,12,31), te::dt::TimeDuration(0,0,0));
    te::st::RasterCoverageDataSetInfo infoRaster1(dsinfo, "hidro_3_20091231000000.tif", "", time1);
    cvsinfo.push_back(infoRaster1);

    te::dt::TimeInstant* time2 = new te::dt::TimeInstant(te::dt::Date(2009,12,31), te::dt::TimeDuration(1,0,0));
    te::st::RasterCoverageDataSetInfo infoRaster2(dsinfo, "hidro_3_20091231010000.tif", "", time2);
    cvsinfo.push_back(infoRaster2);

    te::dt::TimeInstant* time3 = new te::dt::TimeInstant(te::dt::Date(2009,12,31), te::dt::TimeDuration(2,0,0));
    te::st::RasterCoverageDataSetInfo infoRaster3(dsinfo, "hidro_3_20091231020000.tif", "", time3);
    cvsinfo.push_back(infoRaster3);
	
	  //Use the STDataLoader to create a CoverageSeries
    return te::st::STDataLoader::getCoverageSeries(cvsinfo);
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in TrajectoryExamplesFromKML: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in TrajectoryExamplesFromKML!" << std::endl;
  }

  return std::auto_ptr<te::st::CoverageSeries>(0);
}

