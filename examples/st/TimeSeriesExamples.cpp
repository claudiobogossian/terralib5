// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void TimeSeriesExamples()
{
  try
  {
    //Output container
    boost::ptr_vector<te::st::TimeSeries> output;
    
    //Load the Time Series from a PostGIS database
    LoadTimeSeriesDataSetFromPGIS(output);

    //Print the Time Series data set info: spatial and temporal extent
    PrintTimeSeries(output);
  }
  
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in TimeSeriesExamples: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in TimeSeriesExamples!" << std::endl;
  } 
}

