/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file raster_h.h

  \brief This file contains include headers for the Raster module.
 */

#ifndef __TERRALIB_RASTER_INTERNAL_RASTER_H_H
#define __TERRALIB_RASTER_INTERNAL_RASTER_H_H

// TerraLib
#include "raster/Band.h"
#include "raster/BandIterator.h"
#include "raster/BandIteratorWindow.h"
#include "raster/BandProperty.h"
#include "raster/BandSummary.h"
#include "raster/BlockUtils.h"
#include "raster/BandProperty.h"
#include "raster/Config.h"
#include "raster/Exception.h"
#include "raster/Grid.h"
#include "raster/Interpolator.h"
#include "raster/PositionIterator.h"
#include "raster/ProxyRaster.h"
#include "raster/Raster.h"
#include "raster/RasterFactory.h"
#include "raster/RasterIterator.h"
#include "raster/RasterProperty.h"
#include "raster/RasterSummary.h"
#include "raster/RasterSummaryManager.h"
#include "raster/RasterSynchronizer.h"
#include "raster/Reprojection.h"
#include "raster/SynchronizedBand.h"
#include "raster/SynchronizedBandBlocksManager.h"
#include "raster/SynchronizedRaster.h"
#include "raster/TileIndexer.h"
#include "raster/Utils.h"
#include "raster/Vectorizer.h"
#include "raster/VectorizerPolygonStructure.h"

/*!
  \defgroup rst Raster

  \brief This module can be used to manage geographical data with raster representation.

    The TerraLib Raster module provides the base classes foundation for handling geographical data such as remote sensing imagery data.
    The raster data structure can be viewed as a matrix of cells or pixels associated with spatial locations forming a regular grid.
    By design each cell can be viewed, individually, as a multi-dimensional data.

    This module defines only the abstract classes and therefore implementations must be provided.
    We call these implementations: raster drivers. TerraLib provides raster drivers on top of other libraries (e.g. GDAL), DBMS (e.g. PostGIS Raster)
    and an In-Memory implementation.

    The main classes/concepts in this module are listed here. The namespace associated to the Raster module is te::rst.
    To know more about it, see the te::rst namespace documentation.
*/

namespace te
{
  /*!
    \brief Namespace for the Raster module of TerraLib.
  */
  namespace rst
  {
  } // end namespace rst
}   // end namespa

#endif  // __TERRALIB_RASTER_INTERNAL_RASTER_H_H

