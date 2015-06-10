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
  \file terralib/geometry/WKTReader.h

  \brief A class that deserializes a geometry from a valid WKT.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_WKTREADER_H
#define __TERRALIB_GEOMETRY_INTERNAL_WKTREADER_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace gm
  {
// Forward declarations
    class Geometry;

    /*!
      \class WKTReader
      
      \brief A class that deserializes a Geometry from a valid WKT.

      \ingroup geometry

      \sa WKTWriter
    */
    class TEGEOMEXPORT WKTReader : public te::common::Static
    {
      public:

        /*!
          \brief It returns a valid Geometry from a given WKT.
 
          \param wkt A valid WKT Geometry.

          \return A Geometry created from reading the WKT.

          \exception Exception It will throw an exception if the given WKT is invvalid.

          \note The caller of this method will take the ownership of the returned Geometry.
        */
        static Geometry* read(const char* wkt);
    };

  } // namespace gm
}   // namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_WKTREADER_H

