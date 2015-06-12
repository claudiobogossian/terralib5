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
  \file terralib/geometry/GeometryConverters.h

  \brief A set of function that convert a Geometry type to other types and vice-versa.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYCONVERTERS_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYCONVERTERS_H

// TerraLib
#include "Exception.h"

namespace te
{
// Forward declarations
  namespace dt
  {
    class AbstractData;
  }

  namespace gm
  {
    /** @name Geometry Methods
      *  Methods related to Geometry conversion.
      */
    //@{

    /*!
      \brief It converts a Geometry data value to a ByteArray data value.

      \param d The input data value.

      \return A new data value converted to ByteArray type. The caller will take the ownership of the returned data.

      \note The ByteArray data value will contains the Geometry encoded using WKB format.

      \exception It throws an exception if the input abstract data is not a Geometry type.
    */
    te::dt::AbstractData* GeometryToByteArrayConverter(te::dt::AbstractData* d) throw(Exception);

    /*!
      \brief It converts a Geometry data value to a String data value.

      \param d The input data value.

      \return A new data value converted to String type. The caller will take the ownership of the returned data.

      \note The String data value will contains the Geometry encoded using WKT format.

      \exception It throws an exception if the input abstract data is not a Geometry type.
    */
    te::dt::AbstractData* GeometryToStringConverter(te::dt::AbstractData* d) throw(Exception);

    /*!
      \brief It converts a ByteArray data value to a Geometry data value.

      \param d The input data value.

      \return A new data value converted to Geometry type. The caller will take the ownership of the returned data.

      \note This method consider that the ByteArray data value contains a geometry encoded using WKB format.

      \exception It throws an exception if the input abstract data is not a ByteArray type.
      \exception It throws an exception if the conversion can be not done.
    */
    te::dt::AbstractData* ByteArrayToGeometryConverter(te::dt::AbstractData* d);

    /*!
      \brief It converts a String data value to a Geometry data value.

      \param d The input data value.

      \return A new data value converted to Geometry type. The caller will take the ownership of the returned data.

      \note This method consider that the String data value contains a geometry encoded using WKT format.

      \exception It throws an exception if the input abstract data is not a String type.
      \exception It throws an exception if the conversion can be not done.
    */
    te::dt::AbstractData* StringToGeometryConverter(te::dt::AbstractData* d);

    //@}

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYCONVERTERS_H
