// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintMovingObjects(std::vector<te::st::MovingObject*>& output)
{
  for(unsigned int i=0; i<output.size(); ++i)
  {
    te::st::MovingObject* mo = output[i];
    if(mo == 0)
    {
      std::cout << "The " << i << "-th moving object is NULL!" << std::endl;
      return;
    }

    std::cout << "Printing information about moving object id: " << mo->getId() << std::endl;
    std::cout << "============================================================" << std::endl;
    
    te::st::MovingObjectIterator it = mo->begin();  
    
    while(it != mo->end())
    {
      std::cout << "Date/Time: " <<  it.getDateTime()->toString() << std::endl;
      std::cout << "Geometry: " <<  it.getGeometry()->toString()  << std::endl << std::endl;     
      ++it;
    }
  }
}

