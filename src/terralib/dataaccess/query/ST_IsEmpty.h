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
  \file terralib/dataaccess/query/ST_IsEmpty.h

  \brief Spatial is empty operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_ISEMPTY_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_ISEMPTY_H

// TerraLib
#include "Function.h"
#include "FunctionNames.h"

namespace te
{

  // Forward declarations
  namespace gm { class Geometry; }

  namespace da
  {
    /*!
      \class ST_IsEmpty
      
      \brief Spatial is empty operator.

      This function is just a syntatic-suggar.

      \sa Function
    */
    class TEDATAACCESSEXPORT ST_IsEmpty : public Function
    {
      public:

        TE_DEFINE_VISITABLE
        
        /*!
          \brief Constructor.

          \param g    The geometry literal.
        */
        ST_IsEmpty(const te::gm::Geometry& g);

        /*!
          \brief Constructor.

          \param prop The geometry property name.
        */
        ST_IsEmpty(const te::da::PropertyName& prop);

        /*!
          \brief Constructor.

          \param g    The geometry literal.
          \param p    Precision.
        */
        ST_IsEmpty(const te::gm::Geometry& g, const bool& p);

        /*!
          \brief Constructor.

          \param prop The geometry property name.
          \param p    Precision.
        */
        ST_IsEmpty(const te::da::PropertyName& prop, const bool& p);

        /*!
          \brief Constructor.

          \param e    The expression.
          \param p    Precision.
        */
        ST_IsEmpty(const Expression& e);

        /*!
          \brief Constructor.

          \param e    The expression.
          \param p    Precision.
        */
        ST_IsEmpty(const Expression& e, const bool& p);

        /*! \brief Destructor. */
        ~ST_IsEmpty();

        /*! Assignment operator.  */
        ST_IsEmpty& operator=(const ST_IsEmpty& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_ISEMPTY_H

