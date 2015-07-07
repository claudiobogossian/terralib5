/*  Copyright (C) 2015 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/rp/SegmenterRegionGrowingFunctions.cpp
  \brief Useful functions for Region Growing segmentation.
 */

#include "SegmenterRegionGrowingFunctions.h"

#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"

#include <map>

namespace te
{
  namespace rp
  {
    namespace rg
    {
      void exportSegs2Tif( const SegmentsIdsMatrixT& segmentsIds,
                           bool normto8bits,
                           const std::string& fileName )
      {
        std::map<std::string, std::string> rinfo;
        rinfo["SOURCE"] = fileName;
      
        const unsigned int linesNmb = segmentsIds.getLinesNumber();
        const unsigned int colsNmb = segmentsIds.getColumnsNumber();
 
        te::rst::Grid* gridPtr = new te::rst::Grid( colsNmb, linesNmb );

        std::vector< te::rst::BandProperty* > bandsProps;
        bandsProps.push_back( new te::rst::BandProperty( 0, 
                (normto8bits ? te::dt::UCHAR_TYPE : te::dt::UINT32_TYPE) ) );
      
        te::rst::Raster* rasterPtr = te::rst::RasterFactory::make( "GDAL", 
                         gridPtr, bandsProps, rinfo );
        TERP_TRUE_OR_THROW( rasterPtr, "Invalid pointer" );
      
        unsigned int col = 0;
        unsigned int line = 0;
      
        double offset = 0.0;
        double scale = 1.0;
      
        if( normto8bits ) {
          double minValue = DBL_MAX;
          double maxValue = -1.0 * DBL_MAX;
          double value = 0;
      
          for( line = 0 ; line < linesNmb ; ++line ) {
            for( col = 0 ; col < colsNmb ; ++col ) {
              value = (double)segmentsIds( line, col );
            
              if( value > maxValue ) maxValue = value;
              if( value < minValue ) minValue = value;
            }
          }
        
          offset = minValue;
          scale = 254.0 / ( maxValue - minValue );
        }
      
        double value = 0;
      
        for( line = 0 ; line < linesNmb ; ++line ) {
          for( col = 0 ; col < colsNmb ; ++col ) {
            value = ( ((double)segmentsIds( line, col )) - offset ) * scale;
            TERP_TRUE_OR_THROW( value <= 255.0, "Invalid value:" +
              boost::lexical_cast< std::string >( value ) );
          
            rasterPtr->setValue( col, line, value , 0 );
          }
        }
      
        delete rasterPtr;
      }
    }
  }
}
