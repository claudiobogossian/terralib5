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
  \file terralib/dataaccess/query/ST_Relate.h

  \brief Spatial relation function.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_RELATE_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_RELATE_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace da
  {
// Forward declarations
    class LiteralString;

    /*!
      \class ST_Relate
      
      \brief Spatial relation function.

      This function is just a syntatic-suggar.

      \sa Function
    */
    class TEDATAACCESSEXPORT ST_Relate : public Function
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.
          
          \param g1 The first geometric argument.
          \param g2 The second geometric argument.
          \param intersectionMatrix A intersection matrix.

          \note The third parameter is optional. If omitted the function result is a intersection matrix otherwise the result is a boolean.

          \note The operator will take the ownership of the given arguments.
        */
        ST_Relate(Expression* g1, Expression* g2, LiteralString* intersectionMatrix = 0);

        /*!
          \brief Constructor.
          
          \param g1 The first geometric argument.
          \param g2 The second geometric argument.
          \param intersectionMatrix A intersection matrix.

          \note The third parameter is optional. If omitted the function result is a intersection matrix otherwise the result is a boolean.
        */
        ST_Relate(const Expression& g1, const Expression& g2, const std::string& intersectionMatrix = std::string(""));

        /*! \brief Copy constructor. */
        ST_Relate(const ST_Relate& rhs);

        /*! \brief Destructor. */
        ~ST_Relate() {}

        /*! Assignment operator.  */
        ST_Relate& operator=(const ST_Relate& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_RELATE_H

