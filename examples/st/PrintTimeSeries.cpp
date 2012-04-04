// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintTimeSeries(te::st::TimeSeries* ts)
{
  if(ts == 0)
  {
    std::cout << "The time series is NULL!" << std::endl;
    return;
  }

  std::cout << "Printing the time series " << std::endl;
  std::cout << "============================================================" << std::endl;
    
  te::st::TimeSeriesIterator it = ts->begin();  
    
  while(it != ts->end())
  {
    std::cout << "Date/Time: " <<  it.getDateTime()->toString() << std::endl;
    std::cout << "Value: " <<  it.getObservedValue()->toString()  << std::endl << std::endl;     
    ++it;
  }
}

