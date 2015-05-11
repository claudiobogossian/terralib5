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
  \file terralib/raster/RasterSummary.h
 
  \brief RasterSummary is just a typedef of a boost::ptr_vector.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_RASTERSUMMARY_H
#define __TERRALIB_RASTER_INTERNAL_RASTERSUMMARY_H

// TerraLib
#include "BandSummary.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace rst
  {
    /*!
      \brief RasterSummary is just a typedef of a boost::ptr_vector.

      It is a vector of BandSummary, one summary for each band in a raster.
    */
    typedef boost::ptr_vector<BandSummary> RasterSummary;

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_RASTERSUMMARY_H
