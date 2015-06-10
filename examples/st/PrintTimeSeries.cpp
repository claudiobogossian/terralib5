// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintTimeSeries(boost::ptr_vector<te::st::TimeSeries>& input)
{
  for(std::size_t i=0; i<input.size(); ++i)
  {
    std::cout << "Printing observations of the time series " << i << " :" << std::endl;
    PrintTimeSeries(&input[i]);
  }
}

void PrintTimeSeries(te::st::TimeSeries* ts)
{
  if(ts == 0)
  {
    std::cout << "Time series is NULL!" << std::endl;
    return;
  }

  PrintTimeSeriesInfo(ts);

  std::cout << "Time series observations: " << std::endl;
  
  te::st::TimeSeriesIterator it = ts->begin();  
  while(it != ts->end())
  {
    std::cout << "Date and time: " <<  it.getTime()->toString() << std::endl;
    std::cout << "Value: " <<  it.getValue()->toString()  << std::endl << std::endl;
    ++it;
  }
}

void PrintTimeSeriesInfo(boost::ptr_vector<te::st::TimeSeries>& input)
{
  for(std::size_t i=0; i<input.size(); ++i)
  {
    std::cout << "Printing information about the time series " << i << " :" << std::endl;
    PrintTimeSeriesInfo(&input[i]);
  }
}

void PrintTimeSeriesInfo(te::st::TimeSeries* ts)
{
  if(ts == 0)
  {
    std::cout << "Time series is NULL!" << std::endl;
    return;
  }
  
  std::cout << "Time series id: " << ts->getId() << std::endl;
  std::cout << "Time series temporal extent: " << ts->getTemporalExtent()->toString() << std::endl;

  double minValue = 0.;
  double maxValue = 0.;
  ts->getValueExtent(minValue, maxValue);
  std::cout << "Time series value extent: (" << minValue << "," << maxValue << ")" << std::endl;

  te::gm::Geometry* gm = ts->getLocation();
  if(gm!=0)
    std::cout << "Time series location: " << gm->toString() << std::endl;
}











