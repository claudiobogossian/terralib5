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
  \file terralib/geometry/RSTGT.h

  \brief 2D Rotation/scale/translation (rigid body) Geometric transformation.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_RSTGT_H
#define __TERRALIB_GEOMETRY_INTERNAL_RSTGT_H

// TerraLib
#include "Config.h"
#include "GeometricTransformation.h"

// Boost
#include <boost/concept_check.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  namespace gm
  {
    /*!
      \class RSTGT

      \brief 2D Rotation/scale/translation(rigid body) Geometric transformation.

      \note The transformation parameters (inside GTParameters::m_directParameters) are disposed on the following form:

      \code
           u = a.x - b.y + c
           v = b.x + a.y + d

  -        | u | = | a -b c | * | x |
  -        | v |   | b  a d |   | y |
  -        | 1 |   | 0  0 1 |   | 1 |

           |u0| = |x0  -y0  1  0|.|a|
           |v0|   |y0   x0  0  1| |b|
           |u1|   |x1  -y12 1  0| |c|
           |v1|   |y1   y1  0  1| |d|
           |..|   |.............|
           |..|   |.............|

        GTParameters::m_directParameters = [ a b c d ]
      \endcode
    */
    class TEGEOMEXPORT RSTGT : public GeometricTransformation
    {
      public:
        
        /*! \brief Default constructor. */
        RSTGT();

        /*! \brief Destructor. */
        ~RSTGT();

        const std::string& getName() const;

        bool isValid( const GTParameters& params ) const;

        void directMap( const GTParameters& params, const double& pt1X, 
                        const double& pt1Y, double& pt2X, double& pt2Y ) const;

        void inverseMap( const GTParameters& params, const double& pt2X, 
                         const double& pt2Y, double& pt1X, double& pt1Y ) const;

        unsigned int getMinRequiredTiePoints() const;
        
        GeometricTransformation* clone() const;

      protected:
        
        // Variables used by method computeParameters
        mutable unsigned int m_computeParameters_index1;
        mutable unsigned int m_computeParameters_index2; 
        mutable unsigned int m_computeParameters_tpIdx;        
        mutable unsigned int m_computeParameters_tiepointsSize;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_A;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_L;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_At;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_N;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_U;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_N_inv;  
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_X;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_XExpanded;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_XExpandedInv;

        bool computeParameters( GTParameters& params ) const;
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_RSTGT_H

