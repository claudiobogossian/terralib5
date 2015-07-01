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
  \file terralib/postgis/EWKBReader.h
   
  \brief An utility class for reading a PostGIS EWKB.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_EWKBREADER_H
#define __TERRALIB_POSTGIS_INTERNAL_EWKBREADER_H

// TerraLib
#include "../common/Static.h"
#include "../geometry/Enums.h"
#include "Config.h"

// Forward declarations
#include "../geometry_fw.h"

namespace te
{
  namespace pgis
  {
    /*!
      \class EWKBReader

      \brief An utility class for reading a PostGIS EWKB.
    */
    class TEPGISEXPORT EWKBReader : public te::common::Static
    {
      public:

        /*!
          \brief It returns a valid geometry from a given EWKB.
 
          \param ewkb A valid EWKB geometry.

          \return A geometry created from reading the EWKB. The caller of this method will take the ownership of the returned geometry.
        */
        static te::gm::Geometry* read(const char* ewkb);

        /*!
          \brief It returns a valid geometry from a given hex-encoded EWKB.
 
          \param hewkb An hex-encoded EWKB geometry.

          \return A geometry created from reading the HEWKB. The caller of this method will take the ownership of the returned geometry.
        */
        static te::gm::Geometry* readHex(const char* hewkb);

      private:

        static te::gm::Geometry* getGeometry(const char* ewkb, const char** endptr);

        static te::gm::Point* getPoint(const char* ewkb, const char** endptr);

        static te::gm::LineString* getLineString(const char* ewkb, const char** endptr);

        static te::gm::LinearRing* getLinearRing(const char* ewkb, te::gm::GeomType t, int srid, char wkbByteOrder, const char** endptr);

        static te::gm::Polygon* getPolygon(const char* ewkb, const char** endptr);

        static te::gm::GeometryCollection* getGeometryCollection(const char* ewkb, const char** endptr);
    };    

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_EWKBREADER_H
