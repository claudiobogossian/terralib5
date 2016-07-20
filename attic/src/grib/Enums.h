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
  \file terralib/grib/Enums.h

  \brief General enumerations.
 */

#ifndef __TERRALIB_GRIB_INTERNAL_ENUMS_H
#define __TERRALIB_GRIB_INTERNAL_ENUMS_H

namespace te
{
  namespace grib
  {
    /*!
      \enum DataRepresentationType

      \brief Indicates the grib data representation type
    */
    enum DataRepresentationType
    {          
      LATLNG_REP = 0,      /*!< LatLong               */
      SATELLITE_REP = 90,  /*!< Satellite             */
      REDUCEDGG_REP = 4    /*!< Reduced gaussian grid */
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_ENUMS_H
