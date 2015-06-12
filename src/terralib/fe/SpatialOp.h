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
  \file SpatialOp.h
  
  \brief A spatial operator determines whether its geometric arguments satisfy the stated spatial relationship.
 */

#ifndef __TERRALIB_FE_INTERNAL_SPATIALOP_H
#define __TERRALIB_FE_INTERNAL_SPATIALOP_H

// TerraLib
#include "AbstractOp.h"

namespace te
{
  namespace fe
  {     
    /*!
      \class SpatialOp
      
      \brief A spatial operator determines whether its geometric arguments satisfy the stated spatial relationship.

      The operator evaluates to true if the spatial
      relationship is satisfied. Otherwise the operator
      evaluates to false.

      \ingroup fe

      \sa AbstractOp, BinarySpatialOp, BBOXOp, DistanceBuffer
     */
    class TEFEEXPORT SpatialOp : public AbstractOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes the SpatialOp.

          \param opName The operator name.

          \note The SpatialOp will not take the ownership of the given name.
         */
        SpatialOp(const char* opName = 0);

        /*! \brief Virtual destructor. */
        virtual ~SpatialOp();

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_SPATIALOP_H
