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
  \file terralib/dataaccess/query/Literal.h

  \brief This class models a literal value.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_LITERAL_H
#define __TERRALIB_DATAACCESS_INTERNAL_LITERAL_H

// TerraLib
#include "Expression.h"

// STL
#include <memory>

namespace te
{
// Forward declaration
  namespace dt { class AbstractData; }

  namespace da
  {
    /*!
      \class Literal
      
      \brief This class models a literal value.

      \sa Expression, te::dt::AbstractData,
          LiteralGeom, LiteralByteArray,
          LiteralFloat, LiteralDouble,
          LiteralInt32, LiteralInt64,
          LiteralString, LiteralDateTime
    */
    class TEDATAACCESSEXPORT Literal : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param value The value to be assigned to the literal. The literal takes its ownership.
        */
        Literal(te::dt::AbstractData* value);

        /*!
          \brief Constructor.

          \param value The value to be assigned to the literal.
        */
        Literal(const te::dt::AbstractData& value);

        /*! \brief Copy constructor. */
        Literal(const Literal& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Literal();

        /*! Assignment operator.  */
        Literal& operator=(const Literal& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

        /*! \brief It returns the value associated to the literal. */
        te::dt::AbstractData* getValue() const;

        /*!
          \brief It sets the value associated to the literal.
          
          \param v The value to be assigned to the literal. The literal takes its ownership.
        */
        void setValue(te::dt::AbstractData* v);

      protected:

        std::auto_ptr<te::dt::AbstractData> m_value;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_LITERAL_H

