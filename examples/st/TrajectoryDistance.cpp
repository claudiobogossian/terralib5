// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void TrajectoryDistance(te::st::Trajectory* tj1, te::st::Trajectory* tj2)
{
  if(tj1 == 0)
  {
    std::cout << "The first trajectory is NULL!" << std::endl;
    return;
  }

  if(tj2 == 0)
  {
    std::cout << "The second trajectory is NULL!" << std::endl;
    return;
  }

  //Second: calculate the distance
  std::auto_ptr<te::st::TimeSeries> distance = tj1->getDistance(*tj2);

  std::cout << "Printing the distance between two trajectories " << std::endl;
  std::cout << "============================================================" << std::endl;
    
  PrintTimeSeries(distance.get());
}

