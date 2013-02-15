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
  \file terralib/rp/SegmenterStrategy.h
  \brief Raster segmenter strategy base class.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERSTRATEGY_H

#include "SegmenterIdsManager.h"
#include "SegmenterStrategyParameters.h"
#include "Config.h"
#include "Exception.h"

#include "../raster/Raster.h"

#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class SegmenterStrategy
      \brief Raster segmenter strategy base class.
     */
    class TERPEXPORT SegmenterStrategy
    {
      public:
        
        virtual ~SegmenterStrategy();
        
        /*!
          \brief Initialize the segmentation strategy.
          \param strategyParams A pointer to the user given specific segmentation
          strategy parameters ou NULL if no parameters are present.
          \return true if OK, false on errors.
         */
        virtual bool initialize( 
          SegmenterStrategyParameters const* const strategyParams ) 
          throw( te::rp::Exception ) = 0;        
          
        /*!
          \brief Clear all internal allocated resources and go back to the initial not-initialized state.
         */
        virtual void reset() = 0;
        
        /*!
          \brief Executes the segmentation strategy.
          \param segmenterIdsManager The segments IDs manager to be used when
          acquiring/releaseing unique segment IDs.
          \param inputRaster Input raster.
          \param inputRasterBands Input raster bands.
          \param outputRaster Output raster.
          \param outputRasterBand Output raster band.
          \param enableProgressInterface Enable the internal strategy to update
          the progress interface.
          \return true if OK, false on errors.
         */
        virtual bool execute( 
          SegmenterIdsManager& segmenterIdsManager,
          const te::rst::Raster& inputRaster,
          const std::vector< unsigned int >& inputRasterBands,
          te::rst::Raster& outputRaster,
          const unsigned int outputRasterBand,
          const bool enableProgressInterface ) throw( te::rp::Exception ) = 0;          
        
        /*!
            \brief Returns a memory use weight factor.
            \details This parameter is used by the segmenter to
            guess the sub-image blocks size.
            \param inputRasterBandsNumber The number of input raster bands that will 
            be processed at the strategy execution time.
            \return A positive memory use weight factor (greater than one).
        */          
        virtual double getMemUsageFactor( const unsigned int inputRasterBandsNumber ) 
          const = 0;  
        
        /*!
            \brief Returns a optimal blocks overlap size (number of border pixels 
            overlapped between blocks, under one direction).
            \note This value will be used in the case 
            where the image is splitted into blocks for segmentation.
            \return The blocks overlap size.
        */          
        virtual unsigned int getOptimalBlocksOverlapSize() const = 0;        

      protected:

        SegmenterStrategy();
        
      private:
        
        SegmenterStrategy( const SegmenterStrategy& );
        
        const SegmenterStrategy& operator=( const SegmenterStrategy& );
    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ALGORITHM_H

