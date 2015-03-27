/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file AggregationMemory.h
   
  \brief 
 
  \ingroup vp
*/

#ifndef __TERRALIB_VP_INTERNAL_INTERSECTION_MEMORY_H
#define __TERRALIB_VP_INTERNAL_INTERSECTION_MEMORY_H

//Terralib
#include "../common/Exception.h"
#include "../datatype/Property.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "../sam.h"
#include "../statistics/core/Enums.h"

#include "IntersectionOp.h"
#include "Config.h"
#include "Enums.h"
#include "Exception.h"


// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT IntersectionMemory : public IntersectionOp
    {
    
    public:
      
      IntersectionMemory();
      
      ~IntersectionMemory();
      
      bool run() throw(te::common::Exception);
      
    private:

      struct IntersectionMember
      {
        te::da::DataSetType* dt;
        te::da::DataSet* ds;
        std::vector<te::dt::Property*> props;
      };

      typedef te::sam::rtree::Index<size_t, 8>* DataSetRTree;


      std::pair<te::da::DataSetType*, te::da::DataSet*> pairwiseIntersection(std::string newName, 
                                                                            IntersectionMember firstMember, 
                                                                            IntersectionMember secondMember,
                                                                            std::size_t outputSRID);

      te::da::DataSetType* createDataSetType(std::string newName, 
                                            te::da::DataSetType* firstDt,
                                            std::vector<te::dt::Property*> firstProps, 
                                            te::da::DataSetType* secondDt,
                                            std::vector<te::dt::Property*> secondProps);

    }; // end class
  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_INTERSECTION_MEMORY_H

