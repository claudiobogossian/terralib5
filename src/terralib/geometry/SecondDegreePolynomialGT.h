/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/SecondDegreePolynomialGT.h

  \brief Second Degree Polynomial Geometric transformation.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_SECONDDEGREEPOLYNOMIALGT_H
#define __TERRALIB_GEOMETRY_INTERNAL_SECONDDEGREEPOLYNOMIALGT_H

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
      \class SecondDegreePolynomialGT
      
      \brief Second Degree Polynomial Geometric transformation.
    */
    class TEGEOMEXPORT SecondDegreePolynomialGT : public GeometricTransformation
    {
      public:
        
        /*! \brief Default constructor. */
        SecondDegreePolynomialGT();

        /*! \brief Destructor. */
        ~SecondDegreePolynomialGT();

        const std::string& getName() const;

        bool isValid( const GTParameters& params ) const;

        void directMap( const GTParameters& params, const double& pt1X, 
                        const double& pt1Y, double& pt2X, double& pt2Y ) const;

        void inverseMap( const GTParameters& params, const double& pt2X, 
                         const double& pt2Y, double& pt1X, double& pt1Y ) const;

        unsigned int getMinRequiredTiePoints() const;
        
        GeometricTransformation* clone() const;

      protected:
        
        // Variables used by the method computeParameters
        mutable unsigned int m_computeParameters_tiepointsSize;
        mutable unsigned int m_computeParameters_tpIdx;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_W;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_WI;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_X;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_XI;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_Y;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_YI;        
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_PinvW;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_PinvWI;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_A;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_AI;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_B;
        mutable boost::numeric::ublas::matrix< double > m_computeParameters_BI;        

        bool computeParameters( GTParameters& params ) const;
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_SECONDDEGREEPOLYNOMIALGT_H

