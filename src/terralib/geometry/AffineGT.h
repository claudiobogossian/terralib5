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
  \file terralib/geometry/AffineGT.h
  \brief 2D Affine Geometric transformation base class.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_AFFINEGT_H
#define __TERRALIB_GEOMETRY_INTERNAL_AFFINEGT_H

#include "GeometricTransformation.h"
#include "GTFactory.h"
#include "Config.h"
#include <boost/concept_check.hpp>

namespace te
{
  namespace gm
  {
    /*!
      \class AffineGT
      
      \brief 2D Affine Geometric transformation base class.
      
      \note The transformation parameters (inside GTParameters::m_directParameters) are disposed on the following form:
      
      \code
| u | = | a b c | * | x |
| v |   | d e f |   | y |
        | 0 0 1 |
        
GTParameters::m_directParameters = [ a b c d e f ]
      \endcode
    */
    class TEGEOMEXPORT AffineGT : public GeometricTransformation
    {
      public:        
        
        /*!
          \brief Default constructor.
        */
        AffineGT();        

        /*! 
          \brief Virtual destructor. 
          */
        virtual ~AffineGT();
        
        //overload
        const std::string& getName() const;
        
        //overload
        bool isValid( const GTParameters& params ) const;
        
        //overload
        void directMap( const GTParameters& params, const Coord2D& pt1, 
          Coord2D& pt2 ) const;
          
        //overload
        void inverseMap( const GTParameters& params, const Coord2D& pt2, 
          Coord2D& pt1 ) const;
          
        //overload
        unsigned int getMinRequiredTiePoints() const;
        
        /*!
          \brief Returns the basic set of transform parameters given by the decomposition of a given affine transformation parameters as described above.
          
          \param transfParams Input affine transformation parameters.
        
          \param translationX X axis translation (combination of a squeeze and scaling).
          
          \param translationY Y axis translation (combination of a squeeze and scaling).
          
          \param scalingFactorX X axis scaling.
          
          \param scalingFactorX Y axis scaling.
          
          \param skew Skew.
          
          \param squeeze Aspect ratio changes.
          
          \param scaling Uniform scaling.
          
          \param rotation Rotation angle (radians).
          
          \return true if ok, false on errors.
        */       
        static bool decompose( const std::vector< double >& transfParams,
          double& translationX, double& translationY,
          double& scalingFactorX, double& scalingFactorY, double& skew,
          double& squeeze, double& scaling, double& rotation );         

      protected:
        
        //overload
        bool computeParameters( GTParameters& params ) const;

    };
    
    /*!
      \class GTAffineFactory

      \brief 2D Affine Geometric transformation factory.
    */
    class TEGEOMEXPORT GTAffineFactory : public GTFactory
    {
      public:

        ~GTAffineFactory();

        GTAffineFactory();
        
        GeometricTransformation* build();
    };    
  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRICTRANSFORMATION_H

