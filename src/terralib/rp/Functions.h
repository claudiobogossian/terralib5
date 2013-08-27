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
  \file terralib/rp/Functions.h
  \brief Raster Processing functions.
  \ingroup RPModule
 */

#ifndef __TERRALIB_RP_INTERNAL_FUNCTIONS_H
#define __TERRALIB_RP_INTERNAL_FUNCTIONS_H

#include "Config.h"

#include "../dataaccess/datasource/DataSource.h"

// Boost
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>

// STL
#include <vector>

namespace te
{
  namespace rst
  {
    class BandProperty;
    class Grid;
  };

  namespace rp
  {
    class RasterHandler;

    /*!
      \brief Create a new raster into the givem data source.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param outDataSetName The data set name to give to the created data set.
      \param outDataSource The data source to use when creating the output data set.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
     */
    bool CreateNewRaster( const te::rst::Grid& rasterGrid,
      const std::vector< te::rst::BandProperty* >& bandsProperties,
      const std::string& outDataSetName,
      te::da::DataSource& outDataSource,
      RasterHandler& outRasterHandler );


    /*!
      \brief Create a new raster into a new memory datasource.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
     */
    bool CreateNewMemRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      RasterHandler& outRasterHandler );

    /*!
      \brief Create a new geotiff raster.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param fileName The output tif file name.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
     */
    bool CreateNewGeotifRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      const std::string& fileName,
      RasterHandler& outRasterHandler );

    /*!
      \brief Returns the real data type range (all values that can be represented by the given data type).
      \param dataType The data type.
      \param min The minimum value.
      \param max The maximum value.
      \note The types are listed in terralib/datatype/DataTypes.h
    */
    void GetDataTypeRange( const int dataType, double& min, double& max );

    /*!
      \brief Convert vector elements.
      \param inputVector Input vector.
      \param inputVectorDataType Input vector data type.
      \param inputVectorSize The numer of input vector elements.
      \param outputVector A pré-allocated output vector.
      \note The types are listed in terralib/datatype/DataTypes.h
    */
    void Convert2DoublesVector( void* inputVector, const int inputVectorDataType,
      unsigned int inputVectorSize, double* outputVector );

    /*!
      \brief Convert a doubles vector.
      \param inputVector Input vector.
      \param inputVectorSize The numer of input vector elements.
      \param inputVectorDataType Input vector data type.
      \param outputVector A pré-allocated output vector.
      \note The types are listed in terralib/datatype/DataTypes.h
    */
    void ConvertDoublesVector( double* inputVector,
      unsigned int inputVectorSize, const int outputVectorDataType,
      void* outputVector );

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_FUNCTIONS_H

