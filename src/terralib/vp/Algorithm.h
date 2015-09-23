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
 \file Algorithm.h
 
 \brief Abstract algorithm.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_ALGORITHM_H
#define __TERRALIB_VP_INTERNAL_ALGORITHM_H

// Terralib
#include "../memory/DataSet.h"

#include "Config.h"
#include "AlgorithmParams.h"

// STL
#include <map>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT Algorithm
    {
    public:
      
      Algorithm();
      
      virtual ~Algorithm() {}
      
      virtual bool executeMemory(AlgorithmParams* mainParams, te::da::DataSet* teste) = 0;

      virtual bool executeQuery(AlgorithmParams* mainParams) = 0;

      virtual bool isSupportQuery(AlgorithmParams* mainParams) = 0;
    
    private:

      Algorithm(const Algorithm&);

      const Algorithm& operator=(const Algorithm&);
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_ALGORITHM_H