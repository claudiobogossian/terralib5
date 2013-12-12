// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void TrajectoryExamples()
{
  try
  {
	  //Output container
    boost::ptr_vector<te::st::TrajectoryDataSet> output;
    
	  //Load the trajectories from a KML Data Source
	  LoadTrajectoryDataSetFromKML(output);
	
	  //Print trajectory data set info: spatial and temporal extent
	  PrintTrajectoryDataSetInfo(output);
		
	  //Get the trajectories from the trajectory data sets to execute other operations 
	  boost::ptr_vector<te::st::Trajectory> trajectories;
	  for(std::size_t i=0; i<output.size(); ++i)
	  {
		  te::st::TrajectoryDataSet& ds = output[i];
      ds.moveBeforeFirst();
		  std::auto_ptr<te::st::Trajectory> tj = ds.getTrajectory();
		  trajectories.push_back(tj);
	  }
	
	  //Calculate and print the distance between the two first trajectories
	  if(trajectories.size()>1)
		  TrajectoryDistance(&trajectories[0], &trajectories[1]);
		
	  //Calculate the intersection between the first trajectory and a geometry
	  //Load the geometry from a shapefile
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/st/trajectory/t41_region.shp" ; 
    
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("OGR"));
    ds->setConnectionInfo(connInfo); 
    ds->open();
    
    std::auto_ptr<te::da::DataSet> dSet = ds->getDataSet("t41_region"); 
    std::auto_ptr<te::gm::Geometry> geom; 
    
    if(dSet->moveNext())
    {
      std::size_t geomPos = te::da::GetFirstPropertyPos(dSet.get(), te::dt::GEOMETRY_TYPE);
      geom = dSet->getGeometry(geomPos);

      //Calculate the intersection and print the result!
      TrajectoryIntersection(&trajectories[1], geom.get());
    }
  }
  
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in TrajectoryExamples: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in TrajectoryExamples!" << std::endl;
  } 
}

