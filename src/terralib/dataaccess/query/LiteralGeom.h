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
  \file terralib/dataaccess/query/LiteralGeom.h

  \brief A class that models a literal for Geometry values.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_LITERALGEOM_H
#define __TERRALIB_DATAACCESS_INTERNAL_LITERALGEOM_H

// TerraLib
#include "Literal.h"

namespace te
{
// Forward declaratons
  namespace gm { class Geometry; }

  namespace da
  {
    /*!
      \class LiteralGeom
      
      \brief A class that models a literal for Geometry values.

      \sa Literal
    */
    class TEDATAACCESSEXPORT LiteralGeom : public Literal
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param g The geometry value.

          \note The LiteralGeom will take the ownership of the given geometry.
        */
        LiteralGeom(te::gm::Geometry* g);

        /*!
          \brief Constructor.

          \param g The geometry value.
        */
        LiteralGeom(const te::gm::Geometry& g);

        /*! \brief Copy constructor. */
        LiteralGeom(const LiteralGeom& rhs);

        /*! \brief Destructor. */
        ~LiteralGeom();

        /*! Assignment operator.  */
        LiteralGeom& operator=(const LiteralGeom& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_LITERALGEOM_H

