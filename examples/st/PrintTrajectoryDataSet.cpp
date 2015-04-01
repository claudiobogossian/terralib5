// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintTrajectoryDataSet(boost::ptr_vector<te::st::TrajectoryDataSet>& output)
{
  for(std::size_t i=0; i<output.size(); ++i)
  {
	  std::cout << "Printing observations of the trajectory data set" << i << " :" << std::endl;
    PrintTrajectoryDataSet(&output[i]);
  }
}

void PrintTrajectoryDataSet(te::st::TrajectoryDataSet* output)
{
  if(output == 0)
  {
    std::cout << "Trajectory Data Set is NULL!" << std::endl;
    return;
  }
  
  PrintTrajectoryDataSetInfo(output);
  
  std::cout << "Trajectory observations: " <<  std::endl;
  
  output->moveBeforeFirst();
  
  while(output->moveNext())
  {
    std::auto_ptr<te::dt::DateTime> time = output->getTime();
    std::auto_ptr<te::gm::Geometry> geom = output->getGeometry();
    
    std::cout << "Date and Time: " <<  time->toString() << std::endl;
    std::cout << "Geometry: " <<  geom->toString()  << std::endl << std::endl;
  }
}

void PrintTrajectoryDataSetInfo(boost::ptr_vector<te::st::TrajectoryDataSet>& output)
{
  for(std::size_t i=0; i<output.size(); ++i)
  {
	  std::cout << "Printing information about the trajectory data set " << i << " :" << std::endl;
    PrintTrajectoryDataSetInfo(&output[i]);
  }
}

void PrintTrajectoryDataSetInfo(te::st::TrajectoryDataSet* output)
{
  const te::dt::DateTimePeriod* textent = output->getTemporalExtent();
  std::cout << "Temporal Extent: " << textent->toString() << std::endl;

  //const te::gm::Envelope sextent = output->getSpatialExtent();
  //std::cout << "Spatial Extent: " << sextent.getLowerLeftX() << std::endl;
}

