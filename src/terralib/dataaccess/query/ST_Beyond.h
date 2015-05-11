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
  \file terralib/dataaccess/query/ST_Beyond.h

  \brief Spatial Distance Buffer Beyond operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_BEYOND_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_BEYOND_H

// TerraLib
#include "ST_DistanceBuffer.h"

namespace te
{
  namespace da
  {
    /*!
      \class ST_Beyond
      
      \brief Spatial Distance Buffer Beyond operator.

      This function is just a syntatic-suggar.

      \sa ST_DistanceBuffer
    */
    class TEDATAACCESSEXPORT ST_Beyond : public ST_DistanceBuffer
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param e The expression.
          \param g The geometry literal.
          \param d Distance.

          \note The operator will take the ownership of the given arguments.
        */
        ST_Beyond(Expression* e, te::gm::Geometry* g, te::common::Distance* d);

        /*!
          \brief Constructor.

          \param e The expression.
          \param g The geometry literal.
          \param d Distance.
        */
        ST_Beyond(const Expression& e, const te::gm::Geometry& g, const te::common::Distance& d);

        /*! \brief Copy constructor. */
        ST_Beyond(const ST_Beyond& rhs);

        /*! \brief Destructor. */
        ~ST_Beyond() { }

        /*! Assignment operator.  */
        ST_Beyond& operator=(const ST_Beyond& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_BEYOND_H

