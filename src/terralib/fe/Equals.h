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
  \file Equals.h

  \brief Binary spatial operator: Equals.
 */

#ifndef __TERRALIB_FE_INTERNAL_EQUALS_H
#define __TERRALIB_FE_INTERNAL_EQUALS_H

// TerraLib
#include "BinarySpatialOp.h"
#include "Globals.h"

namespace te
{
  namespace fe
  {
    /*!
      \class Equals
      
      \brief Binary spatial operator: Equals.

      \sa BinarySpatialOp
     */
    class TEFEEXPORT Equals : public BinarySpatialOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief Constructor.

          \param p The property name.
          \param g The geometric value.

          \note The Equals operator will take the ownership of the given arguments.
         */
        Equals(PropertyName* p, te::gm::Geometry* g) : BinarySpatialOp(Globals::sm_equals, p, g) {}

        /*!
          \brief Constructor.

          \param p The property name.
          \param e The envelope value.

          \note The Equals operator will take the ownership of the given arguments.
         */
        Equals(PropertyName* p, te::gml::Envelope* e) : BinarySpatialOp(Globals::sm_equals, p, e) {}

        /*! \brief Destructor. */
        ~Equals() {}

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_EQUALS_H

