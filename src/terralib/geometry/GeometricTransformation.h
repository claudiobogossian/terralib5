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
  \file terralib/geometry/GeometricTransformation.h

  \brief 2D Geometric transformation base class.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOMETRICTRANSFORMATION_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOMETRICTRANSFORMATION_H

// TerraLib
#include "Config.h"
#include "GTParameters.h"

// STL
#include <cassert>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace gm
  {
// Forward declaration
    class GTFilter;
    class GTParameters;

    struct Coord2D;

    /*!
      \class GeometricTransformation

      \brief 2D Geometric transformation base class.
    */
    class TEGEOMEXPORT GeometricTransformation : public boost::noncopyable
    {
      friend class GTFilter;
      
      public:

        /*! \brief Virtual destructor. */
        virtual ~GeometricTransformation();
        
        /*!
          \brief Returns the current transformation name.

          \return The current transformation name.
        */
        virtual const std::string& getName() const = 0;

        /*!
          \brief Verifies if the supplied parameters already has a valid transformation.

          \param params Transformation parameters.

          \return true if a transformation is already defined, false otherwise.
        */
        virtual bool isValid( const GTParameters& params ) const = 0;
        
        /*!
          \brief Tells if the current instance has a valid transformation.

          \return true if a transformation is already defined, false otherwise.
        */
        bool isValid() const
        {
          return isValid( m_internalParameters );
        };
        
        /*!
          \brief Initialize the current transformation following the new supplied parameters.

          \param newparams The new parameters.

          \return true if OK, false on errors.
        */
        bool initialize( const GTParameters& newParameters );
        
        /*!
          \brief Returns a reference of the current internal transformation parameters.
          
          \return A reference of the current internal transformation parameters.
        */
        const GTParameters& getParameters() const
        {
          return m_internalParameters;
        };

        /*!
          \brief Direct mapping ( from pt1 space into pt2 space ).

          \param params Transformation parameters.
          \param pt1 pt1 coordinate.
          \param pt2 pt2 coordinate.
        */
        virtual void directMap( const GTParameters& params, const Coord2D& pt1, Coord2D& pt2 ) const = 0;

        /*!
          \brief Direct mapping ( from pt1 space into pt2 space ).

          \param pt1 pt1 coordinate.
          \param pt2 pt2 coordinate.
        */
        void directMap( const Coord2D& pt1, Coord2D& pt2 ) const
        {
          assert( isValid( m_internalParameters ) );

          directMap( m_internalParameters, pt1, pt2 );
        };

        /*!
          \brief Inverse mapping ( from pt2 space into pt1 space ).

          \param params Transformation parameters.
          \param pt2 pt2 coordinate.
          \param pt1 pt1 coordinate.
        */
        virtual void inverseMap( const GTParameters& params, const Coord2D& pt2, Coord2D& pt1 ) const = 0;

        /*!
          \brief Inverse mapping ( from pt2 space into pt1 space ).

          \param pt2 pt2 coordinate.
          \param pt1 pt1 coordinate.
        */
        void inverseMap( const Coord2D& pt2, Coord2D& pt1 ) const
        {
          assert( isValid( m_internalParameters ) );

          inverseMap( m_internalParameters, pt2, pt1 );
        };

        /*!
          \brief Calculates maximum direct mapping error for the supplied parameters.

          \param params Transformation parameters.

          \return The maximum direct mapping error for the supplied parameters.
        */
        double getMaxDirectMappingError( const GTParameters& params ) const;
          
        /*!
          \brief Calculates the current transformation maximum direct mapping error.
          
          \return The current maximum direct mapping error.
        */
        double getMaxDirectMappingError() const
        {
          assert( isValid( m_internalParameters ) );
          
          return getMaxDirectMappingError( m_internalParameters );
        };
        
        /*!
          \brief Calculates maximum inverse mapping error for the supplied parameters.

          \param params Transformation parameters.

          \return The maximum inverse mapping error for the supplied parameters.
        */
        double getMaxInverseMappingError( const GTParameters& params ) const;
          
        /*!
          \brief Calculates the current transformation maximum inverse mapping error.
          
          \return The current maximum inverse mapping error.
        */
        double getMaxInverseMappingError() const
        {
          assert( isValid( m_internalParameters ) );
          
          return getMaxInverseMappingError( m_internalParameters );
        };
        
        /*!
          \brief Calculates root mean square direct mapping error for the supplied parameters.

          \param params Transformation parameters.

          \return The root mean square error.
        */
        double getDirectMapRMSE( const GTParameters& params ) const;

        /*!
          \brief Calculates root mean square direct mapping error.
          
          \return The root mean square error.
        */
        double getDirectMapRMSE() const
        {
          assert( isValid( m_internalParameters ) );

          return getDirectMapRMSE( m_internalParameters );
        };

        /*!
          \brief Calculates root mean square inverse mapping error for the supplied parameters.

          \param params Transformation parameters.

          \return The root mean square error.
        */
        double getInverseMapRMSE( const GTParameters& params ) const;        

        /*!
          \brief Calculates root mean square inverse mapping error.
          
          \return The root mean square error.
        */
        double getInverseMapRMSE() const
        {
          assert( isValid( m_internalParameters ) );

          return getInverseMapRMSE( m_internalParameters );
        };

        /*!
          \brief Calculates the direct mapping error for the supplied tie-point.

          \param tiePoint The tie-point.

          \param params Transformation parameters.

          \return The current maximum direct mapping error.
        */
        double getDirectMappingError( const GTParameters::TiePoint& tiePoint, const GTParameters& params ) const;

        /*!
          \brief Calculates the direct mapping error for the supplied tie-point.

          \param tiePoint The tie-point.

          \return The current direct mapping error.
        */
        double getDirectMappingError( const GTParameters::TiePoint& tiePoint ) const
        {
          assert( isValid( m_internalParameters ) );

          return getDirectMappingError( tiePoint, m_internalParameters );      
        };

        /*!
          \brief Calculates the inverse mapping error for the supplied tie-point.

          \param tiePoint The tie-point.

          \param params Transformation parameters.

          \return The current maximum inverse mapping error.
        */
        double getInverseMappingError( const GTParameters::TiePoint& tiePoint, const GTParameters& params ) const;

        /*!
          \brief Calculates the inverse mapping error for the supplied tie-point.

          \param tiePoint The tie-point.

          \return The current inverse mapping error.
        */
        double getInverseMappingError( const GTParameters::TiePoint& tiePoint ) const
        {
          assert( isValid( m_internalParameters ) );
        
          return getInverseMappingError( tiePoint, m_internalParameters );
        };

        /*!
          \brief Returns the minimum number of required tie-points for the current transformation.

          \return The minimum number of required tie-points for the current transformation.
        */
        virtual unsigned int getMinRequiredTiePoints() const = 0;

      protected:

        /*! \brief Default constructor. */
        GeometricTransformation();
        
        /*!
          \brief Calculate the transformation parameters following the new supplied tie-points.

          \param params Transformation parameters.

          \return true if OK, false on errors.
        */
        virtual bool computeParameters( GTParameters& params ) const = 0;        

      private:

        GTParameters m_internalParameters;  //!< The current internal parameters.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRICTRANSFORMATION_H

