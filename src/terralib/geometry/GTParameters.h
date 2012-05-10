/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/GTParameters.h
  
  \brief 2D Geometric transformation parameters.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GTPARAMETERS_H
#define __TERRALIB_GEOMETRY_INTERNAL_GTPARAMETERS_H

#include "../common/AbstractParameters.h"
#include "GTModelParameters.h"
#include "Coord2D.h"
#include "Config.h"

#include <string>
#include <vector>
#include <utility>

namespace te
{
  namespace gm
  {
    /*!
      \class GTParameters
      
      \brief 2D Geometric transformation parameters.
    */
    class TEGEOMEXPORT GTParameters : public te::common::AbstractParameters
    {
      public:      
        
        /*!
          \typedef std::pair< Coord2D, Coord2D > TiePoint
          \brief Tie point type definition.
          */        
        typedef std::pair< Coord2D, Coord2D > TiePoint;
        
        /*!
          \brief Tie points.
          */        
        std::vector< TiePoint > m_tiePoints;
        
        /*!
          \brief Transformation model numeric parameters.
          */        
        std::vector< double > m_modelParameters;         
        
        GTParameters();
        
        ~GTParameters();
        
        // overload
        te::common::AbstractParameters* clone() const;
        
        // overload
        void reset() throw( te::common::Exception );
        
        // overload
        const AbstractParameters& operator=( const AbstractParameters& params );

    };
  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GTPARAMETERS_H

