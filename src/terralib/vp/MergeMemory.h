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
  \file MergeMemory.h
   
  \brief 
 
  \ingroup vp
*/

#ifndef __TERRALIB_VP_INTERNAL_MERGE_MEMORY_H
#define __TERRALIB_VP_INTERNAL_MERGE_MEMORY_H

//Terralib
#include "MergeOp.h"
#include "Config.h"


// STL
#include <memory>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT MergeMemory : public MergeOp
    {
    
    public:
      
      MergeMemory();
      
      ~MergeMemory();
      
      bool run() throw( te::common::Exception );

    private:

      std::auto_ptr<te::da::DataSetType> buildOutDataSetType();

    }; // end class
  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_MERGE_MEMORY_H

