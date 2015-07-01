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
  \file raster_fw.h
   
  \brief Forward declarations for the Raster module.
 */

#ifndef __TERRALIB_RASTER_INTERNAL_RASTER_FW_H
#define __TERRALIB_RASTER_INTERNAL_RASTER_FW_H

// Forward declarations
namespace te
{
  namespace rst
  {
    class Band;
		class BandProperty;
    class Grid;
    class Raster;
    class RasterFactory;
	  class RasterProperty;
    class SynchronizedBand;
    class SynchronizedBandBlocksManager;
    class SynchronizedRaster;
    class Platform;

  } // end namespace rst
}   // end namespace te

#endif // __TERRALIB_RASTER_INTERNAL_RASTER_FW_H

