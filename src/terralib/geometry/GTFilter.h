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
  \file terralib/geometry/GTFilter.h

  \brief 2D Geometric transformation outliers remotion filter.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GTFILTER_H
#define __TERRALIB_GEOMETRY_INTERNAL_GTFILTER_H

#include "Config.h"
#include "GTParameters.h"
#include "GeometricTransformation.h"

#include <memory>
#include <vector>

namespace te
{
  namespace gm
  {
    /*!
      \class GTFilter
      
      \brief 2D Geometric transformation tie-points filter (outliers remotion).
    */
    class TEGEOMEXPORT GTFilter
    {
      public:

        /*! \brief Destructor. */
        ~GTFilter();
        
        /*!
          \brief Apply a RANSAC based outliers remotion strategy.

          \param transfName Transformation name (see te::gm::GTFactory dictionary for reference).
          
          \param inputParams Input transformation parameters.
          
          \param maxDMapError The maximum allowed direct mapping error.
          
          \param maxIMapError The maximum allowed inverse mapping error.
          
          \param maxDMapRmse The maximum allowed direct mapping root mean square error.
          
          \param maxIMapRmse The maximum allowed inverse mapping root mean square error.
          
          \param outTransf The generated output transformation.
          
          \param tiePointsWeights Optional tie-points weights (non-zero, positive values).

          \return true if OK, false on errors.
          
          \note Reference: Martin A. Fischler and Robert C. Bolles, Random Sample Consensus: A Paradigm for Model Fitting with Applications to Image Analysis and Automated Cartography, Communications of the ACM  archive, Volume 24 , Issue 6  (June 1981).
        */
        static bool applyRansac(const std::string& transfName, const GTParameters& inputParams,
          const double maxDMapError, const double maxIMapError,
          const double maxDMapRmse, const double maxIMapRmse,
          std::auto_ptr< GeometricTransformation >& outTransf,
          const std::vector< double > tiePointsWeights = std::vector< double >() );
        
      private:
        
        /*!
          \brief RANSAC iterations counter type.
        */        
        typedef unsigned int RansacItCounterT;        
        
        /*! \brief Default constructor. */
        GTFilter();

        /*!
          \brief Copy constructor.

          \param rhs The input filter.
        */
        GTFilter(const GTFilter& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side filter.

          \return A reference for this.
        */
        GTFilter& operator=(const GTFilter& rhs);
        
        /*!
          \brief Returns the tie-points convex hull area (GTParameters::TiePoint::first).

          \param tiePoints Tie points.

          \return The tie-points convex hull area 
        */
        static double getPt1ConvexHullArea( 
          const std::vector< GTParameters::TiePoint >& tiePoints );

    };
  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRICTRANSFORMATION_H

