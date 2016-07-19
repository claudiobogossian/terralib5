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
  \file terralib/sqlite/EWKBReader.h
   
  \brief An utility class for reading a SpatiaLite EWKB geometry.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_EWKBREADER_H
#define __TERRALIB_SQLITE_INTERNAL_EWKBREADER_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"

// Forward declarations
#include "../geometry_fw.h"

#ifdef TE_ENABLE_SPATIALITE

namespace te
{
  namespace sqlite
  {
    class EWKBReader : public te::common::Static
    {
      public:

        static te::gm::Geometry* read(const unsigned char* ewkb);

        static te::gm::Geometry* readHex(const unsigned char* hewkb);

      private:

        static te::gm::Geometry* getGeometry(const unsigned char* ewkb,
                                             const unsigned char** endptr,
                                             unsigned char ewkbByteOrder,
                                             int srid,
                                             te::gm::Envelope* mbr);

        static te::gm::Point* getPoint(const unsigned char* ewkb,
                                       const unsigned char** endptr,
                                       unsigned char ewkbByteOrder,
                                       unsigned int gtype,
                                       int srid,
                                       te::gm::Envelope* mbr);

        static te::gm::LineString* getLineString(const unsigned char* ewkb,
                                                 const unsigned char** endptr,
                                                 unsigned char ewkbByteOrder,
                                                 unsigned int gtype,
                                                 int srid,
                                                 te::gm::Envelope* mbr);

        static te::gm::LinearRing* getLinearRing(const unsigned char* ewkb,
                                                 unsigned int gtype,
                                                 const unsigned char** endptr,
                                                 unsigned char ewkbByteOrder,
                                                 int srid);

        static te::gm::Polygon* getPolygon(const unsigned char* ewkb,
                                           const unsigned char** endptr,
                                           unsigned char ewkbByteOrder,
                                           unsigned int gtype,
                                           int srid,
                                           te::gm::Envelope* mbr);

        static te::gm::GeometryCollection* getGeometryCollection(const unsigned char* ewkb,
                                                                 const unsigned char** endptr,
                                                                 unsigned char ewkbByteOrder,
                                                                 unsigned int gtype,
                                                                 int srid,
                                                                 te::gm::Envelope* mbr);
    };

  } // end namespace sqlite
}   // end namespace te

#endif // TE_ENABLE_SPATIALITE

#endif  // __TERRALIB_SQLITE_INTERNAL_EWKBREADER_H

