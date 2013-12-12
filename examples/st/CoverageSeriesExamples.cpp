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
    //Load coverage series from a set of geotif files
    std::auto_ptr<te::st::CoverageSeries> cvs = LoadCoverageSeriesFromGeotif();

    //Print coverage series info
    PrintCoverageSeriesInfo(cvs.get());

    //Extract time series from coverage series 
    
    //Load the centroide of the Angra city -> from a shapefile with the Angra city
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/st/coverage/angra_city.shp" ; 
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("OGR"));
    ds->setConnectionInfo(connInfo); 
    ds->open();
    
    std::auto_ptr<te::da::DataSet> dSet = ds->getDataSet("angra_city"); 
    std::auto_ptr<te::gm::Geometry> geom; 
    
    if(dSet->moveNext())
    {
      std::size_t geomPos = te::da::GetFirstPropertyPos(dSet.get(), te::dt::GEOMETRY_TYPE);
      geom = dSet->getGeometry(geomPos);
      
      //Centroid operation is not implemented! 
      //We will use the center point of the MBR of the Angra geometry  
      const te::gm::Envelope* env = geom->getMBR(); 
      te::gm::Coord2D coord = env->getCenter();
      te::gm::Point point(coord.x, coord.y);

      std::auto_ptr<te::st::TimeSeries> result = cvs->getTimeSeries(point);

      //Print the result time series
      PrintTimeSeries(result.get());
    }
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

