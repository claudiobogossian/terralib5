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
  \file LogicOp.h
  
  \brief A logical operator can be used to combine one or more conditional expressions.
 */

#ifndef __TERRALIB_FE_INTERNAL_LOGICOP_H
#define __TERRALIB_FE_INTERNAL_LOGICOP_H

// TerraLib
#include "AbstractOp.h"

namespace te
{
  namespace fe
  {     
    /*!
      \class LogicOp
      
      \brief A logical operator can be used to combine one or more conditional expressions.

      The logical operator AND evaluates to true if
      all the combined expressions evaluate to true.
      The operator OR operator evaluates to true is
      any of the combined expressions evaluate to true.
      The NOT operator reverses the logical value of
      an expression.

      \ingroup fe

      \sa AbstractOp, BinaryLogicOp, UnaryLogicOp
     */
    class TEFEEXPORT LogicOp : public AbstractOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes the LogicOp.

          \param opName The operator name.

          \note The LogicOp will not take the ownership of the given name.
         */
        LogicOp(const char* opName = 0);

        /*! \brief Virtual destructor. */
        virtual ~LogicOp();

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_LOGICOP_H

