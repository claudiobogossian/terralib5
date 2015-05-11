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
  \file terralib/dataaccess/query/ST_Buffer.h

  \brief Spatial buffer operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ST_BUFFER_H
#define __TERRALIB_DATAACCESS_INTERNAL_ST_BUFFER_H

// TerraLib
#include "Function.h"

// STL
#include <memory>

namespace te
{
  // Forward declarations
  namespace gm { class Geometry; }

  namespace da
  {
    /*!
      \class ST_Buffer
      
      \brief Spatial Buffer operator.

      This function is just a syntatic-suggar.

      \sa Function
    */
    class TEDATAACCESSEXPORT ST_Buffer : public Function
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param g    The geometry literal.
          \param d    Distance.
        */
        ST_Buffer(const te::gm::Geometry& g, const double& d);

        /*!
          \brief Constructor.

          \param prop The geometry property name.
          \param d    Distance.
        */
        ST_Buffer(const te::da::PropertyName& prop, const double& d);

        /*!
          \brief Constructor.

          \param g    The geometry literal.
          \param attName    The property name that will be used to calculate buffer.
        */
        ST_Buffer(const te::gm::Geometry& g, const PropertyName& propDist);

        /*!
          \brief Constructor.

          \param prop The geometry property name.
          \param attName    The property name that will be used to calculate buffer.
        */
        ST_Buffer(const te::da::PropertyName& propGeom, const PropertyName& propDist);

        /*!
          \brief Constructor.

          \param e    The expression.
        */
        ST_Buffer(const Expression& e);

        /*! \brief Destructor. */
        ~ST_Buffer();

        /*! Assignment operator.  */
        ST_Buffer& operator=(const ST_Buffer& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ST_BUFFER_H

