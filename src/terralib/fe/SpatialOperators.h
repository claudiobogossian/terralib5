/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file SpatialOperators.h
  
  \brief Spatial operators.  
 */

#ifndef __TERRALIB_FE_INTERNAL_SPATIALOPERATORS_H
#define __TERRALIB_FE_INTERNAL_SPATIALOPERATORS_H

// TerraLib
#include "Config.h"

// STL
#include <vector>

namespace te
{
  namespace fe
  {
// Forward declarations
    class SpatialOperator;

    /*!
      \class SpatialOperators
      
      \brief Spatial operators.

      \sa SpatialCapabilities, SpatialOperator
     */
    class TEFEEXPORT SpatialOperators
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new SpatialOperators. */
        SpatialOperators();

        /*! \brief Destructor. */
        ~SpatialOperators();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the number of operators.

          \return The number of available operators.
         */
        size_t size() const;

        /*!
          \brief It adds the spatial operator to the list.
         
          \param sOp The spatial operator to be added to the list.

          \note The SpatialOperators object will take the ownership of the operator. So, you must not free it.
         */
        void push_back(SpatialOperator* sOp);

        /*!
          \brief It returns a specified operator.

          \param i The index of desired operator.

          \return A specified operator.

          \note You must not clear the returned operator.

          \note The method will not check the index range.
         */
        const SpatialOperator* operator[](size_t i) const;

        //@}

      private:

        std::vector<SpatialOperator*> m_spatialOperators;   //!< Mandatory at least one.
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_SPATIALOPERATORS_H
