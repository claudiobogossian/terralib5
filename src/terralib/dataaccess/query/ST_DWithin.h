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
  \file terralib/dataaccess/query/ST_DWithin.h

  \brief Spatial Distance Buffer Within operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_DWITHIN_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_DWITHIN_H

// TerraLib
#include "ST_DistanceBuffer.h"

namespace te
{
  namespace da
  {
    /*!
      \class ST_DWithin
      
      \brief Spatial Distance Buffer Within operator.

      This function is just a syntatic-suggar.

      \sa ST_DistanceBuffer
    */
    class TEDATAACCESSEXPORT ST_DWithin : public ST_DistanceBuffer
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
        ST_DWithin(Expression* e, te::gm::Geometry* g, te::common::Distance* d);

        /*!
          \brief Constructor.

          \param e The expression.
          \param g The geometry literal.
          \param d Distance.
        */
        ST_DWithin(const Expression& e, const te::gm::Geometry& g, const te::common::Distance& d);

        /*! \brief Copy constructor. */
        ST_DWithin(const ST_DWithin& rhs);
        
        /*! \brief Destructor. */
        ~ST_DWithin() {}

        /*! Assignment operator.  */
        ST_DWithin& operator=(const ST_DWithin& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_DWITHIN_H

