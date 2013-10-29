// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintTrajectoryDataSet(te::st::TrajectoryDataSet* output)
{
  std::cout << "Printing information about trajectory: " << output->getId() << std::endl;
  std::cout << "============================================================" << std::endl;
  
  te::dt::DateTimePeriod* textent = output->getTemporalExtent();
  std::cout << "Temporal Extent: " << textent->toString() << std::endl;

  const te::gm::Envelope sextent = output->getSpatialExtent();
  //std::cout << "Spatial Extent: " << sextent.toString() << std::endl;
  
  std::cout << "Observations: " << std::endl;
  while(output->moveNext())
  {
    std::auto_ptr<te::dt::DateTime> time = output->getTime();
    std::auto_ptr<te::gm::Geometry> geom = output->getGeometry();
    
    std::cout << "Date and Time: " <<  time->toString() << std::endl;
    std::cout << "Geometry: " <<  geom->toString()  << std::endl << std::endl;
  }
}

