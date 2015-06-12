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
  \file PropertyIsLessThanOrEqualTo.h

  \brief It models the inequality operator less than or equal to (<=).
 */

#ifndef __TERRALIB_FE_INTERNAL_PROPERTYISLESSTHANOREQUALTO_H
#define __TERRALIB_FE_INTERNAL_PROPERTYISLESSTHANOREQUALTO_H

// TerraLib
#include "BinaryComparisonOp.h"
#include "Globals.h"

namespace te
{
  namespace fe
  {
    /*!
      \class PropertyIsLessThanOrEqualTo
      
      \brief It models the inequality operator less than or equal to (<=).

      \ingroup fe

      \sa BinaryComparisonOp
     */
    class TEFEEXPORT PropertyIsLessThanOrEqualTo : public BinaryComparisonOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief Constructor.

          \param arg1 The first argument.
          \param arg2 The second argument.

          \note The PropertyIsLessThanOrEqualTo operator will take the ownership of the given arguments.
         */
        PropertyIsLessThanOrEqualTo(Expression* arg1, Expression* arg2) : BinaryComparisonOp(Globals::sm_propertyIsLessThanOrEqualTo, arg1, arg2) {}

        /*! \brief Destructor. */
        ~PropertyIsLessThanOrEqualTo() {}

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_PROPERTYISLESSTHANOREQUALTO_H

