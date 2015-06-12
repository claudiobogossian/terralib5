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
  \file terralib/st/Enums.h

  \brief General enumerations for the st module.
*/

#ifndef __TERRALIB_ST_INTERNAL_ENUMS_H
#define __TERRALIB_ST_INTERNAL_ENUMS_H

namespace te
{
  namespace st
  {
    /*! \brief An enum for the types of spatiotemporal relation */
    enum SpatioTemporalRelation
    {
      ENTERS = 0, 
      EXITS = 1, 
      REACHES = 2, 
      LEAVES = 3
    };

    /*! \brief An enum for the types of coverage */
    enum CoverageType 
    {
      UNKNOWN = 0,
      POINT_COVERAGE = 1,  /*!< Used to identify a point coverage.               */
      RASTER_COVERAGE = 2  /*!< Used to identify a raster coverage.              */
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_ENUMS_H

