// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/common.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/st.h>

// STL
#include <iostream>

void PrintMovingObjectIntersection(te::st::MovingObject* mo, te::gm::Geometry* geom)
{
  if(mo == 0)
  {
    std::cout << "The moving object is NULL!" << std::endl;
    return;
  }

  if(geom == 0)
  {
    std::cout << "The geometry is NULL!" << std::endl;
    return;
  }

  std::vector<te::st::Trajectory*> result;
  mo->intersection(geom, result);
  
  PrintTrajectories(result);

  te::common::FreeContents(result);
}

