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
  \file terralib/dataaccess/query/ST_NumGeometries.h

  \brief Spatial number geometries operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_NUMGEOMETRIES_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_NUMGEOMETRIES_H

// TerraLib
#include "FunctionNames.h"
#include "UnaryFunction.h"

namespace te
{
  namespace da
  {
    /*!
      \class ST_NumGeometries
      
      \brief ST_NumGeometries statistical function.

      This function is just a syntatic-suggar.

      \sa UnaryFunction
    */
    class TEDATAACCESSEXPORT ST_NumGeometries : public UnaryFunction
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param arg The single function argument.

          \note The ST_NumGeometries function will take the ownership of the given argument.
        */
        ST_NumGeometries(Expression* arg) : UnaryFunction(FunctionNames::sm_ST_NumGeometries, arg) {}

        /*!
          \brief Constructor.

          \param arg The single function argument.
        */
        ST_NumGeometries(const Expression& arg) : UnaryFunction(FunctionNames::sm_ST_NumGeometries, arg) {}

        /*! \brief Copy constructor. */
        ST_NumGeometries(const ST_NumGeometries& rhs);

        /*! \brief Destructor. */
        ~ST_NumGeometries() {}

        /*! Assignment operator.  */
        ST_NumGeometries& operator=(const ST_NumGeometries& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_NUMGEOMETRIES_H

