// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintMovingObjectDistance(te::st::MovingObject* mo1, te::st::MovingObject* mo2)
{
  if(mo1 == 0)
  {
    std::cout << "The first moving object is NULL!" << std::endl;
    return;
  }

  if(mo2 == 0)
  {
    std::cout << "The second moving object is NULL!" << std::endl;
    return;
  }

  //First: associate a interpolation function to the second moving object
  te::st::AbstractInterpolator* interpolator = &(te::st::TemporalNearestNeigInterpolator::getInstance());
  
  mo2->setInterpolator(interpolator);

  //Second: calculate the distance
  te::st::TimeSeries* distance = mo1->distance(mo2);

  std::cout << "Printing the distance between two moving objects " << std::endl;
  std::cout << "============================================================" << std::endl;
    
  PrintTimeSeries(distance);

  delete distance;
}

