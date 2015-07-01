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
  \file SpatialOperator.h
  
  \brief Spatial operator.
 */

#ifndef __TERRALIB_FE_INTERNAL_SPATIALOPERATOR_H
#define __TERRALIB_FE_INTERNAL_SPATIALOPERATOR_H

// TerraLib
#include "../common/Comparators.h"
#include "Config.h"

// STL
#include <set>

namespace te
{
  namespace fe
  {
// Forward declarations
    class GeometryOperands;

    /*!
      \class SpatialOperator
      
      \brief Spatial operator.

      Spatial operator name can be one of the following names:
      <ul>
      <li>BBOX</li>
      <li>Equals</li>
      <li>Disjoint</li>
      <li>Intersects</li>
      <li>Touches</li>
      <li>Crosses</li>
      <li>Within</li>
      <li>Contains</li>
      <li>Overlaps</li>
      <li>Beyond</li>
      <li>DWithin</li>
      </ul>

      \ingroup fe

      \sa SpatialOperators, GeometryOperands
     */
    class TEFEEXPORT SpatialOperator
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new SpatialOperator.

          \param The spatial operator name.

          \note The opName must be a value from the list of valid spatial operators.
         */
        SpatialOperator(const char* opName);

        /*! \brief Destructor. */
        ~SpatialOperator();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the operator name.

          \return The operator name.
         */
        const char* getOpName() const;

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
          \brief It checks if operator 'o' is a valid operator. It returns a pointer
                 to the found operator or NULL otherwise.

          \param o The operator name we are looking for.
         */
        static const char* findOperator(const char* o);

        /*! \brief It loads the valid spatial operator list. */
        static void loadValidOperatorList();

        /*! \brief It clears the valid operator list. */
        static void clearValidOperatorList();

        //@}

      private:

        const char* m_name;                   //!< Spatial operator name.
        GeometryOperands* m_geometryOperands; //!< Optional.          

        static std::set<const char*, te::common::LessCmp<const char*> >  sm_validSpatialOperators; //!< The list of valid spatial operators.
    };

    

  } // end namespace fe
}     // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_SPATIALOPERATOR_H
