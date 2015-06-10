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
  \file terralib/fe/Literal.h
  
  \brief This class can be used to represent literal values.
 */

#ifndef __TERRALIB_FE_INTERNAL_LITERAL_H
#define __TERRALIB_FE_INTERNAL_LITERAL_H

// TerraLib
#include "Expression.h"

// STL 
#include <cassert>
#include <string>

namespace te
{
  namespace fe
  {
    /*!
      \class Literal
      
      \brief This class can be used to represent literal values.

      A literal value is any part of a statement or
      expression that is to be used exactly as it is
      specified, rather than as a variable or other element.
      The Literal is used to encode literal scalar and geometric values.

      \ingroup fe

      \sa Expression, PropertyIsLike

      \todo In the XML schema it is marked as mixed, so this put another requirement to this class!
     */
    class TEFEEXPORT Literal : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new Literal.

          \param value The literal value.
         */
        Literal(const std::string& value);

        /*! \brief Destructor. */
        ~Literal();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the literal value.

          \return The literal value.
         */
        const std::string& getValue() const;

        /*!
          \brief It sets the literal value.

          \param v The literal value.
         */
        void setValue(const std::string& v);

        //@}

        /** @name Expression Re-implementation
         *  Methods re-implemented from Expression.
         */
        //@{

        Expression* clone() const;

        //@}

      private:

        std::string m_value;   //!< Literal expression. (Mandatory)
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_LITERAL_H
