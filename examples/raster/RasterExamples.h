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
  \file RasterExamples.h

  \brief These routines show how to use the raster module and the GDAL data source module.
 */

#ifndef __TERRALIB_EXAMPLES_INTERNAL_RASTEREXAMPLES_H
#define __TERRALIB_EXAMPLES_INTERNAL_RASTEREXAMPLES_H

#include "Config.h"

/*! \brief It loads the data source drivers. */
void LoadModules();

/*! \brief A set of examples using GDAL/raster/dataaccess support. */
void ExemplifyDataAccess();

/*! \brief Test to open a shapefile and save some polygons in a raster file. */
void RasterizePolygonSet();

/*! \brief Test to open a complex image and split into real/imaginary rasters. */
void SplitComplexImage();

/*! \brief Creates an image crop with a providen envelope. */
void MakeRasterCrop();

/*! \brief Creates a new raster file with a new projection. */
void ReprojectRaster();

/*! \brief Creates a new raster based on raster arithmetic operations. */
void ArithmeticWithRaster();

/*! \brief Resample one Raster based on user defined parameters. */
void ResampleRaster();

/*! \brief An example to use the Grib driver. */
void GribExample();

/*! \brief Reproject one raster using TerraLib implementation. */
void ReprojectRasterUsingDriver();

/*! \brief Reproject one raster using GDAL implementation. */
void ReprojectRasterUsingFunction();

/*! \brief An example to use the memory driver. */
void RasterInMemory();

/*! \brief An example to test iterators over Bands, Windows, and Rasters. */
void ExemplifyIterators();

/*! \brief An example to use Grib and Geometry. */
void GribPolygonExample();

/*! \brief This example shows how to create an 1bit Raster in memory, and how to save an 1bit Raster in Tiff format. */
void Raster1Bit();

/*! \brief This example shows how to use the vectorization method. */
void VectorizeRaster();

#endif
