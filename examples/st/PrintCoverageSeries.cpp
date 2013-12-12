// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintCoverageSeriesInfo(te::st::CoverageSeries* output)
{
  if(output == 0)
  {
    std::cout << "Coverage series is NULL!" << std::endl;
    return;
  }

  std::cout << "Coverage series information: " << std::endl;
  
  std::auto_ptr<te::dt::DateTimePeriod> period = output->getTemporalExtent();
  std::cout << "Temporal extent: " <<  period->toString() << std::endl;
  
  te::gm::Geometry* sextent = output->getSpatialExtent();
  if(sextent!=0)
    std::cout << "Spatial extent: " <<  sextent->toString() << std::endl;

  std::cout << "Type: " <<  output->getType() << std::endl;
  std::cout << "Size: " <<  output->size() << std::endl;
    
  std::cout << "Coverage series observations: " << std::endl;
  te::st::CoverageSeriesIterator it = output->begin();  
  while(it != output->end())
  {
    std::cout << "Date and time: " <<  it.getTime()->toString() << std::endl;    
    ++it;
  }
}













