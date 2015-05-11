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
  \file UnaryLogicOp.h
  
  \brief A logical operator that can be used to combine one conditional expressions.
 */

#ifndef __TERRALIB_FE_INTERNAL_UNARYLOGICOP_H
#define __TERRALIB_FE_INTERNAL_UNARYLOGICOP_H

// TerraLib
#include "LogicOp.h"

namespace te
{
  namespace fe
  { 
// Forard declarations
    class AbstractOp;

    /*!
      \class UnaryLogicOp
      
      \brief A logical operator that can be used to combine one conditional expressions.

      \sa LogicOp, AbstractOp, BinaryLogicOp
     */
    class TEFEEXPORT UnaryLogicOp : public LogicOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new UnaryLogicOp.

          \param opName The unary logic operator name.

          \note The LogicOp will not take the ownership of the given name.
         */
        UnaryLogicOp(const char* opName = 0);

        /*!
          \brief It initializes a new UnaryLogicOp.

          \param opName The unary logic operator name.
          \param o      The operand.

          \note The LogicOp will not take the ownership of the given name.
         */
        UnaryLogicOp(const char* opName, AbstractOp* o);

        /*! \brief Virtual destructor. */
        virtual ~UnaryLogicOp();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the operand.

          \param o The operand.

          \note It will take the ownership of the operand.
         */
        void setOp(AbstractOp* o);

        /*!
          \brief It returns the operand.

          \return The operand.
         */
        AbstractOp* getOp() const;

        //@}

      protected:

        AbstractOp* m_op;    //!< Mandatory.
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_UNARYLOGICOP_H
