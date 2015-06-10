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
  \file terralib/raster/Reprojection.h

  \brief It contains the algorithm to reproject raster data.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_REPROJECTION_H
#define __TERRALIB_RASTER_INTERNAL_REPROJECTION_H

// TerraLib
#include "Config.h"
#include "Interpolator.h"

// STL
#include <map>
#include <string>

namespace te
{
// Forward declarations
  namespace gm { class Envelope; }
  namespace srs { class Converter; }

  namespace rst
  {
    class Raster;

    /*!
      \brief Reprojects a raster to another SRS.

      \param rin       The input raster file. Do not pass a null pointer.
      \param srid      The target SRID for the reprojection.
      \param routinfo  The basic parameters necessary to create the reprojected raster.
      \param m         The method of interpolation to apply. \sa te::rst::Interpolator

      \return A pointer to the raster reprojected if success or a null pointer otherwise.

      \exception Exception This function might through an exception if the coordinate conversion fails.

      \note The caller will take the ownership of the returned pointer.
    */
    TERASTEREXPORT te::rst::Raster* Reproject(te::rst::Raster const * const rin, int srid, const std::map<std::string, std::string>& routinfo, int m = te::rst::NearestNeighbor);

    /*!
      \brief Reprojects a portion of a raster to another SRS.

      \param rin        The input raster file. Do not pass a null pointer.
      \param srid       The target SRID for the reprojection.
      \param llx        Lower-left X-coordinate of the portion to be reprojected (in the original SRS).
      \param lly        Lower-left Y-coordinate of the portion to be reprojected (in the original SRS).
      \param urx        Upper-Right X-coordinate of the portion to be reprojected (in the original SRS).
      \param ury        Upper-Right Y-coordinate of the portion to be reprojected (in the original SRS).
      \param routinfo   The basic parameters necessary to create the reprojected raster.
      \param m          The method of interpolation to apply. \sa te::rst::Interpolator

      \return A pointer to the raster reprojected if success or a null pointer otherwise.

      \exception Exception This function might through an exception if the coordinate conversion fails.

      \note The caller will take the ownership of the returned pointer.
    */
    TERASTEREXPORT te::rst::Raster* Reproject(te::rst::Raster const * const rin, int srid, double llx, double lly, double urx, double ury, const std::map<std::string, std::string>& routinfo, int m = te::rst::NearestNeighbor);

    /*!
      \brief Reprojects a portion of a raster to another SRS and maintaining a given resolution.

      \param rin        The input raster file. Do not pass a null pointer.
      \param srid       The target SRID for the reprojection.
      \param llx        Lower-left X-coordinate of the portion to be reprojected (in the original SRS).
      \param lly        Lower-left Y-coordinate of the portion to be reprojected (in the original SRS).
      \param urx        Upper-Right X-coordinate of the portion to be reprojected (in the original SRS).
      \param ury        Upper-Right Y-coordinate of the portion to be reprojected (in the original SRS).
      \param resx       The output x resolution (in units of the target SRS).
      \param resx       The output y resolution (in units of the target SRS).
      \param routinfo   The basic parameters necessary to create the reprojected raster.
      \param m          The method of interpolation to apply. \sa te::rst::Interpolator

      \return A pointer to the raster reprojected if success or a null pointer otherwise.

      \exception Exception This function might through an exception if the coordinate conversion fails.

      \note The caller will take the ownership of the returned pointer.
    */
    TERASTEREXPORT te::rst::Raster* Reproject(te::rst::Raster const * const rin, int srid, double llx, double lly, double urx, double ury, double resx, double resy, const std::map<std::string, std::string>& routinfo, int m = te::rst::NearestNeighbor);
  }
}

#endif // __TERRALIB_RASTER_INTERNAL_REPROJECTION_H
