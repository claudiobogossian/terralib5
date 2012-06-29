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
    
  te::st::TimeSeriesIterator it = distance->begin();  
    
  while(it != distance->end())
  {
    std::cout << "Date/Time: " <<  it.getDateTime()->toString() << std::endl;
    std::cout << "Value: " <<  it.getObservedValue()->toString()  << std::endl << std::endl;     
    ++it;
  }

  delete distance;
}


void plotMovingObjectDistance(te::st::MovingObject* mo1, te::st::MovingObject* mo2)
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
   
  te::st::TimeSeriesIterator it = distance->begin();  

  std::vector<std::pair<te::dt::DateTime*, double> > values;
  while(it != distance->end())
  {
    te::dt::DateTime* date = it.getDateTime();
    te::dt::AbstractData* value = it.getObservedValue();
    double dv = atof(value->toString().c_str());
    std::pair<te::dt::DateTime*, double>  val = std::pair<te::dt::DateTime*, double>(date, dv);
    values.push_back(val);
    ++it;
  }


  delete distance;
}

