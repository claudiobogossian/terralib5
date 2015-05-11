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
  \file terralib/fe/Not.h

  \brief Boolean NOT operator.
 */

#ifndef __TERRALIB_FE_INTERNAL_NOT_H
#define __TERRALIB_FE_INTERNAL_NOT_H

// TerraLib
#include "Globals.h"
#include "UnaryLogicOp.h"

namespace te
{
  namespace fe
  {
    /*!
      \class Not
      
      \brief Boolean NOT operator.

      \sa Function
     */
    class TEFEEXPORT Not : public UnaryLogicOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief Constructor.

          \param arg The single function argument.

          \note The Not function will take the ownership of the given argument.
         */
        Not(AbstractOp* arg) : UnaryLogicOp(Globals::sm_not, arg) {}

        /*! \brief Destructor. */
        ~Not() {}

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_NOT_H

