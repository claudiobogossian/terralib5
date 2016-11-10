/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file GeometricMemory.h
   
  \brief 
 
  \ingroup vp
*/

#ifndef __TERRALIB_VP_INTERNAL_GEOMETRIC_OP_MEMORY_H
#define __TERRALIB_VP_INTERNAL_GEOMETRIC_OP_MEMORY_H

//Terralib
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"

#include "GeometricOp.h"
#include "Exception.h"

namespace te
{
  namespace vp
  {
    class TEVPEXPORT GeometricOpMemory : public GeometricOp
    {
    
    public:
      
      GeometricOpMemory();
      
      ~GeometricOpMemory();
      
      bool run() throw(te::common::Exception);

    protected:

      te::mem::DataSet* SetAllObjects(te::da::DataSetType* dsType,
                                      std::vector<int> tabVec,
                                      std::vector<int> geoVec);

      te::mem::DataSet* SetAggregObj( te::da::DataSetType* dsType,
                                      std::vector<int> tabVec,
                                      std::vector<int> geoVec);

      te::mem::DataSet* SetAggregByAttribute( te::da::DataSetType* dsType,
                                              std::vector<int> tabVec,
                                              std::vector<int> geoVec);

      double CalculateTabularOp(int tabOperation, 
                                te::gm::Geometry* geom);

    }; // end class
  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_GEOMETRIC_OP_MEMORY_H

