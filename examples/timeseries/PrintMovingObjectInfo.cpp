// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintMovingObjectInfo(std::vector<te::st::MovingObject*>& output)
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
    
    te::dt::DateTime* dt = mo->temporalExtent();
    if(dt != 0)
    {
      std::cout << "Temporal extension: " <<  dt->toString()  << std::endl;
      delete dt;
    }
    
    //std::cout << "Spatial extension: " <<  mo->getGeometry(j)->toString()  << std::endl << std::endl;     
  }
}

