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
  \file terralib/geometry/ThirdDegreePolynomialGT.h

  \brief Third Degree Polynomial Geometric transformation.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_THIRDDEGREEPOLYNOMIALGT_H
#define __TERRALIB_GEOMETRY_INTERNAL_THIRDDEGREEPOLYNOMIALGT_H

// TerraLib
#include "Config.h"
#include "GeometricTransformation.h"
#include "GTFactory.h"

// Boost
#include <boost/concept_check.hpp>

namespace te
{
  namespace gm
  {
    /*!
      \class ThirdDegreePolynomialGT
      
      \brief Third Degree Polynomial Geometric transformation.
    */
    class TEGEOMEXPORT ThirdDegreePolynomialGT : public GeometricTransformation
    {
      public:
        
        /*! \brief Default constructor. */
        ThirdDegreePolynomialGT();

        /*! \brief Virtual destructor. */
        virtual ~ThirdDegreePolynomialGT();

        const std::string& getName() const;

        bool isValid( const GTParameters& params ) const;

        void directMap( const GTParameters& params, const Coord2D& pt1, 
          Coord2D& pt2 ) const;
          
        void inverseMap( const GTParameters& params, const Coord2D& pt2, 
          Coord2D& pt1 ) const;

        unsigned int getMinRequiredTiePoints() const;
        
        GeometricTransformation* clone() const;

      protected:

        bool computeParameters( GTParameters& params ) const;
    };

    /*!
      \class ThirdDegreePolynomialGTFactory

      \brief Third Degree Polynomial Geometric transformation factory.
    */
    class TEGEOMEXPORT ThirdDegreePolynomialGTFactory : public GTFactory
    {
      public:

        ~ThirdDegreePolynomialGTFactory();

        ThirdDegreePolynomialGTFactory();
        
        GeometricTransformation* build();
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_SECONDDEGREEPOLYNOMIALGT_H

