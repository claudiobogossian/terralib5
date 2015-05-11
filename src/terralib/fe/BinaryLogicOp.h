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
  \file terralib/fe/BinaryLogicOp.h
  
  \brief A logical operator can be used to combine two or more conditional expressions.  
 */

#ifndef __TERRALIB_FE_INTERNAL_BINARYLOGICOP_H
#define __TERRALIB_FE_INTERNAL_BINARYLOGICOP_H

// TerraLib
#include "LogicOp.h"

// STL
#include <vector>

namespace te
{
  namespace fe
  {
// Forward decloarations
    class AbstractOp;

    /*!
      \class BinaryLogicOp
      
      \brief A logical operator can be used to combine two or more conditional expressions.

      The following operators are defined in the Filter
      Encoding specification:
      <ul>
      <li>And</li>
      <li>Or</li>
      </ul>

      \ingroup fe

      \sa LogicOp, UnaryLogicOp      
     */
    class TEFEEXPORT BinaryLogicOp : public LogicOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new BinaryLogicOp.

          \param opName The BinaryLogicOp operator name.

          \note The BinaryLogicOp will not take the ownership of the given name.
         */
        BinaryLogicOp(const char* opName = 0);

        /*!
          \brief It initializes a new BinaryLogicOp.

          \param opName The BinaryLogicOp operator name.
          \param first  The first operand.
          \param second The second operand.

          \note The BinaryLogicOp will not take the ownership of the given name.

          \note The BinaryLogicOp object will take the ownership of the operands.
         */
        BinaryLogicOp(const char* opName,
                      AbstractOp* first,
                      AbstractOp* second);

        /*! \brief Virtual destructor. */
        virtual ~BinaryLogicOp();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the number of operands.

          \return The number of operands.
         */
        std::size_t size() const;

        /*!
          \brief It adds the operand to the operand list.
         
          \param o The operand to be added.

          \note The BinaryLogicOp object will take the ownership of the operand.
         */
        void add(AbstractOp* o);

        /*!
          \brief It returns a specified operand.

          \param i The index of desired operand.

          \return A specified operand.

          \note The method will not check the index range.
         */
        AbstractOp* operator[](std::size_t i) const;

        /*!
          \brief It returns a specified operand.

          \param i The index of desired operand.

          \return A specified operand.

          \note The method will not check the index range.
         */
        AbstractOp* getOp(std::size_t i) const;

        //@}

      protected:

        std::vector<AbstractOp*> m_ops;   //!< Mandatory at least two.
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_BINARYLOGICOP_H
