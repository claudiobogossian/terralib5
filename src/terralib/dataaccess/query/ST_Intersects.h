/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/query/ST_Intersects.h

  \brief Spatial intersects operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_INTERSECTS_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_INTERSECTS_H

// TerraLib
#include "BinaryFunction.h"
#include "FunctionNames.h"

namespace te
{
  namespace da
  {
    /*!
      \class ST_Intersects
      
      \brief Spatial intersects operator.

      This function is just a syntatic-suggar.

      \sa BinaryFunction
    */
    class TEDATAACCESSEXPORT ST_Intersects : public BinaryFunction
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param arg1 The first argument.
          \param arg2 The second argument.

          \note The operator will take the ownership of the given arguments.
        */
        ST_Intersects(Expression* arg1, Expression* arg2) : BinaryFunction(FunctionNames::sm_ST_Intersects, arg1, arg2) {}

        /*!
          \brief Constructor.

          \param arg1 The first argument.
          \param arg2 The second argument.
        */
        ST_Intersects(const Expression& arg1, const Expression& arg2) : BinaryFunction(FunctionNames::sm_ST_Intersects, arg1, arg2) {}

        /*! \brief Copy constructor. */
        ST_Intersects(const ST_Intersects& rhs);

        /*! \brief Destructor. */
        ~ST_Intersects() {}

        /*! Assignment operator.  */
        ST_Intersects& operator=(const ST_Intersects& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_INTERSECTS_H

