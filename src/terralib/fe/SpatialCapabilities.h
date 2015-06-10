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
  \file SpatialCapabilities.h
  
  \brief Spatial capabilities include the ability to filter spatial data.
 */

#ifndef __TERRALIB_FE_INTERNAL_SPATIALCAPABILITIES_H
#define __TERRALIB_FE_INTERNAL_SPATIALCAPABILITIES_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace fe
  {
// Forward declarations
    class GeometryOperands;
    class SpatialOperators;

    /*!
      \class SpatialCapabilities
      
      \brief Spatial capabilities include the ability to filter spatial data.

      A service that supports spatial filtering shall
      include a spatial capabilities section in the capabilities
      document that it returns. Spatial capabilities include
      the ability to filter spatial data of specified geometry
      types based on the definition of a bounding box (BBOX) as well
      as the ability to process the spatial operators defined by
      CQL and the Simple Features for SQL specification:
      <ul>
      <li>Equals</li>
      <li>Disjoint</li>
      <li>Touches</li>
      <li>Within</li>
      <li>Overlaps</li>
      <li>Crosses</li>
      <li>Intersects</li>
      <li>Contains</li>
      <li>DWithin</li>
      <li>Beyond</li>
      </ul>

      \sa FilterCapabilities, GeometryOperands, SpatialOperators
     */
    class TEFEEXPORT SpatialCapabilities
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new SpatialCapabilities. */
        SpatialCapabilities();

        /*! \brief Destructor. */
        ~SpatialCapabilities();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the geometry operands.

          \param gOps The geometry operands.

          \note It will take the ownership of the geometry operands.
                So, don't release the geometry operands resources.
         */
        void setGeometryOperands(GeometryOperands* gOps);

        /*!
          \brief It returns the geometry operands.

          \return The geometry operands.
         */
        const GeometryOperands* getGeometryOperands() const;

        /*!
          \brief It sets the spatial operators.

          \param sOps The spatial operators.

          \note It will take the ownership of the spatial operators.
                So, don't release the arithmetic operators resources.
         */
        void setSpatialOperators(SpatialOperators* sOps);

        /*!
          \brief It returns the spatial operators.

          \return The spatial operators.
         */
        const SpatialOperators* getSpatialOperators() const;

        //@}

      private:

        GeometryOperands* m_geometryOperands;  //!< Mandatory.
        SpatialOperators* m_spatialOperators;  //!< Mandatory.
    };   

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_SPATIALCAPABILITIES_H
