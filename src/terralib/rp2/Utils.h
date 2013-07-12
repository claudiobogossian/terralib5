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
#include "Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace rp
  {
    /*! \brief Returns a vector os with band's names. */
    TERPEXPORT std::vector<std::string> GetBandNames();

    /*! \brief Returns the maximun and minimum reflectance values of a given sensor/band. */
    TERPEXPORT std::pair<double, double> GetSpectralBandInfo(std::string bandName);

    /*! \brief Returns the minimum reflectance value of a given sensor/band. */
    TERPEXPORT double GetSpectralBandMin(std::string bandName);

    /*! \brief Returns the maximum reflectance value of a given sensor/band. */
    TERPEXPORT double GetSpectralBandMax(std::string bandName);

    /*! \brief Returns the maximun and minimum digital numbers of a given sensor/band. */
    TERPEXPORT std::pair<double, double> GetDigitalNumberBandInfo(std::string bandName);

    /*! \brief Returns the maximum digital number of a given sensor/band. */
    TERPEXPORT double GetDigitalNumberBandMax(std::string bandName);

    /*!
      \brief Normalizes one raster in a given interval.

      \param inputRaster     The given raster.
      \param nmin            The new minimum value (default = 0.0).
      \param nmax            The new maximum value (default = 255.0).

      \return true if normalization occurs and false otherwise.
    */
    TERPEXPORT bool NormalizeRaster(te::rst::Raster& inputRaster, double nmin = 0.0, double nmax = 255.0);

    /*!
      \brief Creates a vector of random positions (points) inside the raster.

      \param inputRaster     The given raster.
      \param numberOfPoints  The number of random positions to be created (default = 1000).

      \return A vector of random positions (points).
    */
    TERPEXPORT std::vector<te::gm::Point*> GetRandomPointsInRaster(const te::rst::Raster& inputRaster, unsigned int numberOfPoints = 1000);

    /*!
      \brief Computes euclidean distance between two double vectors.

      \param v1    The first vector.
      \param v2    The second vector.

      \return The euclidean distance between v1 and v2.
    */
    TERPEXPORT double GetEuclideanDistance(std::vector<double> v1, std::vector<double> v2);

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_UTILS_H
