// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/common.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void TrajectoryIntersection(te::st::Trajectory* tj, te::gm::Geometry* geom)
{
  if(tj == 0)
  {
    std::cout << "Trajectory is NULL!" << std::endl;
    return;
  }

  if(geom == 0)
  {
    std::cout << "Geometry is NULL!" << std::endl;
    return;
  }
    
  std::vector<te::st::TrajectoryPatch> result;
  
  //It returns the patches of the trajectory that intersects the given geometry
  tj->getPatches(*geom, te::gm::INTERSECTS, result); 

  //Print the returned patches
  for(std::size_t i = 0; i<result.size(); ++i)
  {
    std::cout << "Printing trajectory patch " << i << ":" << std::endl;
    PrintTrajectoryPatch(result[i]);
  }
}

