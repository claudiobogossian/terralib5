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
  \file terralib/rp/SegmenterDummyStrategy.h
  \brief Dummy strategy (just for testing purposes).
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERDUMMYSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERDUMMYSTRATEGY_H

#include "SegmenterStrategyFactory.h"
#include "SegmenterStrategy.h"
#include "Config.h"


namespace te
{
  namespace rp
  {
    /*!
      \class SegmenterDummyStrategy
      \brief Dummy strategy (just for testing purposes).
     */
    class TERPEXPORT SegmenterDummyStrategy : public SegmenterStrategy
    {
      public:
        
        ~SegmenterDummyStrategy();
        
        SegmenterDummyStrategy();
        
        //overload
        bool initialize( 
          SegmenterStrategyParameters const* const strategyParams ) 
          throw( te::rp::Exception );
          
        //overload
        void reset();          
        
        //overload
        bool execute( 
          SegmenterIdsManager& segmenterIdsManager,
          const te::rp::SegmenterSegmentsBlock& block2ProcessInfo,
          const te::rst::Raster& inputRaster,
          const std::vector< unsigned int >& inputRasterBands,
          const std::vector< double >& inputRasterNoDataValues,
          const std::vector< double >& inputRasterBandMinValues,
          const std::vector< double >& inputRasterBandMaxValues,
          te::rst::Raster& outputRaster,
          const unsigned int outputRasterBand,
          const bool enableProgressInterface ) throw( te::rp::Exception );
        
        //overload         
        double getMemUsageEstimation( const unsigned int bandsToProcess,
          const unsigned int pixelsNumber ) const;
        
        //overload  
        unsigned int getOptimalBlocksOverlapSize() const;

        //overload
        bool shouldComputeMinMaxValues() const;

        //overload
        BlocksMergingMethod getBlocksMergingMethod() const;
    };
    
    /*!
      \class SegmenterDummyStrategyFactory
      \brief Raster dummy segmenter strategy factory.
      \note Factory key: RegionGrowing
     */
    class TERPEXPORT SegmenterDummyStrategyFactory : public 
      te::rp::SegmenterStrategyFactory
    {
      public:
        
        SegmenterDummyStrategyFactory();
        
        ~SegmenterDummyStrategyFactory();
   
        //overload
        te::rp::SegmenterStrategy* build();
        
    };    

  } // end namespace rp
}   // end namespace te

#endif

