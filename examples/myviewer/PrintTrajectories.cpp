// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintTrajectories(std::vector<te::st::Trajectory*>& output)
{
  for(unsigned int i=0; i<output.size(); ++i)
  {
    te::st::Trajectory* tj = output[i];
    if(tj == 0)
    {
      std::cout << "The " << i << "-th trajectory is NULL!" << std::endl;
      return;
    }

    std::cout << "Printing information about trajectories of moving object: " << tj->getMovingObject()->getId() << std::endl;
    std::cout << "============================================================" << std::endl;
    
    te::st::TrajectoryIterator it = tj->begin();  
    
    while(it != tj->end())
    {
      std::cout << "Date/Time: " <<  it.getDateTime()->toString() << std::endl;
      std::cout << "Geometry: " <<  it.getGeometry()->toString()  << std::endl << std::endl;     
      ++it;
    }
  }
}

