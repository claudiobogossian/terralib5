/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/rp/Utils.h

  \brief Utility functions for the raster processor module.
*/

#ifndef __TERRALIB_RP_INTERNAL_UTILS_H
#define __TERRALIB_RP_INTERNAL_UTILS_H

// TerraLib
#include "../raster.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace rp
  {
    /*! \brief Returns the maximun and minimum reflectance values of a given sensor/band. */
    std::pair<double, double> GetSpectralBandInfo(std::string bandName);

    /*! \brief Returns the minimum reflectance value of a given sensor/band. */
    double GetSpectralBandMin(std::string bandName);

    /*! \brief Returns the maximum reflectance value of a given sensor/band. */
    double GetSpectralBandMax(std::string bandName);

    /*! \brief Returns the maximun and minimum digital numbers of a given sensor/band. */
    std::pair<double, double> GetDigitalNumberBandInfo(std::string bandName);

    /*! \brief Returns the maximum digital number of a given sensor/band. */
    double GetDigitalNumberBandMax(std::string bandName);

    /*!
      \brief Normalizes one raster in a given interval.

      \param rst   The given raster.
      \param nmin  The new minimum value (default = 0.0).
      \param nmax  The new maximum value (default = 255.0).

      \return true if normalization occurs and false otherwise.
    */
    bool NormalizeRaster(te::rst::Raster& inraster, double nmin = 0.0, double nmax = 255.0);
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_UTILS_H
