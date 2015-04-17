// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintTrajectory(boost::ptr_vector<te::st::Trajectory>& input)
{
  for(std::size_t i=0; i<input.size(); ++i)
  {
    std::cout << "Printing observations of the trajectory " << i << " :" << std::endl;
    PrintTrajectory(&input[i]);
  }
}

void PrintTrajectory(te::st::Trajectory* tj)
{
  if(tj == 0)
  {
    std::cout << "Trajectory is NULL!" << std::endl;
    return;
  }

  PrintTrajectoryInfo(tj);

  std::cout << "Trajectory observations: " << std::endl;
  
  te::st::TrajectoryIterator it = tj->begin();  
  while(it != tj->end())
  {
    std::cout << "Date/Time: " <<  it.getTime()->toString() << std::endl;
    std::cout << "Geometry: " <<  it.getGeometry()->toString()  << std::endl << std::endl;     
    ++it;
  }
}

void PrintTrajectoryPatch(te::st::TrajectoryPatch& tjPatch)
{
  te::st::TrajectoryIterator it  = tjPatch.begin();
  te::st::TrajectoryIterator itEnd = tjPatch.end();
  while(it != itEnd)
  {
    std::cout << "Date and Time: " <<  it.getTime()->toString() << std::endl;
    std::cout << "Geometry: " <<  it.getGeometry()->toString()  << std::endl << std::endl;     
    ++it;
  }
}

void PrintTrajectoryInfo(boost::ptr_vector<te::st::Trajectory>& input)
{
  for(std::size_t i=0; i<input.size(); ++i)
  {
    std::cout << "Printing information about the trajectory " << i << " :" << std::endl;
    PrintTrajectoryInfo(&input[i]);
  }
}

void PrintTrajectoryInfo(te::st::Trajectory* tj)
{
  if(tj == 0)
  {
    std::cout << "Trajectory is NULL!" << std::endl;
    return;
  }

  std::cout << "Trajectory id: " << tj->getId() << std::endl;
  std::cout << "Trajectory spatial extent: " << tj->getSpatialExtent().getLowerLeftX() << std::endl;
  std::cout << "Trajectory temporal extent: " << tj->getTemporalExtent()->toString() << std::endl;
}

