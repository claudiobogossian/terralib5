/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file ST_Transform.h

  \brief Spatial reference system transformation function.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_TRANSFORM_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_TRANSFORM_H

// TerraLib
#include "Function.h"
#include "FunctionNames.h"

namespace te
{
  namespace da
  {
    /*!
      \class terralib/dataaccess/query/ST_Transform
      
      \brief Spatial reference system transformation function.

      This function is just a syntatic-suggar.

      \sa Function
    */
    class TEDATAACCESSEXPORT ST_Transform : public Function
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param arg  The first argument must evaluate to a geometry property or literal.
          \param srid The second argument must be a srid.

          \note The operator will take the ownership of the given arguments.
        */
        ST_Transform(Expression* arg, int srid);

        /*!
          \brief Constructor.

          \param arg  The first argument must evaluate to a geometry property or literal.
          \param srid The second argument must be a srid.
        */
        ST_Transform(const Expression& arg, int srid);

        /*! \brief Copy constructor. */
        ST_Transform(const ST_Transform& rhs);

        /*! \brief Destructor. */
        ~ST_Transform() {}

        /*! Assignment operator.  */
        ST_Transform& operator=(const ST_Transform& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_TRANSFORM_H

