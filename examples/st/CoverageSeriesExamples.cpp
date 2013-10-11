// Examples
#include "STExamples.h"

//TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/datatype.h>
#include <terralib/st.h>

// STL
#include <iostream>

void CoverageSeriesExamples()
{
  try
  {  
    //Indicates the data source
    te::da::DataSourceInfo dsinfo;
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/geotif/geotif" ; 
    dsinfo.setConnInfo(connInfo);
    dsinfo.setType("GDAL");
    
    //Indicates the raster file names and their associated times
	  te::st::RasterCoverageSeriesDataSetInfo cvsinfo;
	
    te::dt::TimeInstant* time1 = new te::dt::TimeInstant(te::dt::Date(2009,12,31), te::dt::TimeDuration(0,0,0));
    te::st::RasterCoverageDataSetInfo infoRaster1(dsinfo, "hidro_3_20091231000000.tif", 0, time1);
    cvsinfo.push_back(infoRaster1);

    te::dt::TimeInstant* time2 = new te::dt::TimeInstant(te::dt::Date(2009,12,31), te::dt::TimeDuration(1,0,0));
    te::st::RasterCoverageDataSetInfo infoRaster2(dsinfo, "hidro_3_20091231010000.tif", 0, time2);
    cvsinfo.push_back(infoRaster2);

    te::dt::TimeInstant* time3 = new te::dt::TimeInstant(te::dt::Date(2009,12,31), te::dt::TimeDuration(2,0,0));
    te::st::RasterCoverageDataSetInfo infoRaster3(dsinfo, "hidro_3_20091231020000.tif", 0, time3);
    cvsinfo.push_back(infoRaster3);
	
	  //Use the STDataLoader to create a CoverageSeries
    std::auto_ptr<te::st::CoverageSeries> cvs = te::st::STDataLoader::getCoverageSeries(cvsinfo);
    
    //Generate a time series
    double sedeAngraLat = -23.007; 
    double sedeAngraLong = -44.318;
    te::gm::Point sedeAngra(sedeAngraLong, sedeAngraLat);

    std::auto_ptr<te::st::TimeSeries> ts = cvs->getTimeSeries(sedeAngra);

    //Print the result time series
    PrintTimeSeries(ts.get());
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

