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
  \file terralib/rp/SegmenterRegionGrowingStrategy.h
  \brief Raster region growing segmenter strategy.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGSTRATEGY_H

#include "SegmenterStrategyFactory.h"
#include "SegmenterStrategy.h"
#include "SegmenterStrategyParameters.h"
#include "SegmenterSegmentsBlock.h"
#include "Matrix.h"
#include "Config.h"

#include <vector>
#include <list>

namespace te
{
  namespace rp
  {
    /*!
      \class SegmenterRegionGrowingStrategy
      \brief Raster region growing segmenter strategy.
     */
    class TERPEXPORT SegmenterRegionGrowingStrategy : public SegmenterStrategy
    {
      public:
        
        /*!
          \class Parameters
          \brief Segmenter Parameters
         */        
        class TERPEXPORT Parameters : public SegmenterStrategyParameters
        {
          public:
            
            /*! \enum SegmentFeaturesType Segment features types. */
            enum SegmentFeaturesType
            {
              MeanFeatureType = 0, /*!< The mean of segments pixel values will be used. */
            };             
            
            unsigned int m_minSegmentSize; //!< A positive minimum segment size (pixels number).
            
            double m_segmentsSimilarityThreshold; //!< Segments similarity treshold - Segments with similarity values below this value will be merged.
            
            SegmentFeaturesType m_segmentFeatures; //!< What segment features will be used on the segmentation process (default:MeanFeatureType).
            
            Parameters();
            
            ~Parameters();
            
            //overload 
            const AbstractParameters& operator=( const AbstractParameters& params );
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            AbstractParameters* clone() const;
        };        
        
        ~SegmenterRegionGrowingStrategy();
        
        SegmenterRegionGrowingStrategy();
        
        //overload
        bool initialize( 
          SegmenterStrategyParameters const* const strategyParams ) 
          throw( te::rp::Exception );
        
        //overload
        bool execute( 
          SegmenterIdsManager& segmenterIdsManager,
          const te::rst::Raster& inputRaster,
          const std::vector< unsigned int >& inputRasterBands,
          te::rst::Raster& outputRaster,
          const unsigned int outputRasterBand,
          const bool enableProgressInterface ) throw( te::rp::Exception );
        
        //overload         
        double getMemUsageFactor( const unsigned int inputRasterBandsNumber ) 
          const;
        
        //overload  
        unsigned int getOptimalBlocksOverlapSize() const;
        
      protected :
        
        /*!
          \brief Internal segments ids container.
         */          
        typedef Matrix< SegmenterSegmentsBlock::SegmentIdDataType >
          SegmentsIdsContainerT;
        
        /*!
          \class Segment
          \brief Segment base class
         */        
        class TERPEXPORT Segment
        {
          public:
            
            /*!
              \brief Segment ID.
            */             
            SegmenterSegmentsBlock::SegmentIdDataType m_id;
            
            /*!
              \brief Segment size (pixels number).
            */              
            unsigned int m_size;             
            
            /*!
              \brief Segment left X coordinate box over the label image.
            */              
            unsigned int m_xStart;

            /*!
              \brief Segment lower bound X coordinate box over the label image.
            */              
            unsigned int m_xBound;

            /*!
              \brief Segment upper Y coordinate box over the label image.
            */              
            unsigned int m_yStart;

            /*!
              \brief Segment lower bound Y coordinate box over the label image.
            */              
            unsigned int m_yBound;            
            
            /*!
              \brief Neighborhood segments.
            */             
            std::list< Segment* > m_neighborSegments;            
            
            virtual ~Segment();
            
            /*!
              \brief Returns a dissimilarity index between this and the
              other segment.
              \param otherSegment The other segment.
              \return A dissimilarity index between this and the
              other segment.
            */              
            virtual double getDissimilarityIndex( 
              Segment const * const otherSegment ) = 0;
              
            /*!
              \brief Merge specific segment features from both segments
              into this one.
              \param otherSegment The other segment.
            */                
            virtual void mergeFeatures( Segment const * const otherSegment ) = 0;
            
          protected :
            
            Segment();
            
          private :
          
            Segment( const Segment& );
            
            const Segment& operator=( const Segment& );
        };
        
        /*!
          \brief Internal segments container.
          \note All segment objects will be deleted at this instance destruction time.
         */        
        class TERPEXPORT SegmentsContainer : public std::map< 
          SegmenterSegmentsBlock::SegmentIdDataType, Segment* >
        {
          public :
            
            SegmentsContainer();
            
            ~SegmentsContainer();
            
            /*!
              \brief Delete all segment objects and clears the map.
              */              
            void deleteSegments();
        };
        
        /*!
          \class MeanBasedSegment
          \brief A segment based on pixel mean values
         */        
        class TERPEXPORT MeanBasedSegment : public Segment
        {
          public:
            
            /*!
              \brief Segment mean values (for each band).
              \param otherSegment The other segment.
            */              
            std::vector< double > m_means;
            
            MeanBasedSegment();
            
            ~MeanBasedSegment();
            
            //overload
            double getDissimilarityIndex( Segment const * const otherSegment );
            
            //overload
            void mergeFeatures( Segment const * const otherSegment );
            
          protected :
            
            // Variables used by getDissimilarityIndex method
            MeanBasedSegment* getDissimilarityIndex_otherCastPtr;
            unsigned int getDissimilarityIndex_meansIdx;
            unsigned int getDissimilarityIndex_meansSize;
            double getDissimilarityIndex_diffValue;
            double getDissimilarityIndex_dissValue;
            
            // Variables used by mergeFeatures method
            MeanBasedSegment* mergeFeatures_otherCastPtr;
            unsigned int mergeFeatures_meansIdx;
            unsigned int mergeFeatures_meansSize;
        };                
        
        /*!
          \brief true if this instance is initialized.
        */        
        bool m_isInitialized;

        /*!
          \brief Internal execution parameters.
        */        
        SegmenterRegionGrowingStrategy::Parameters m_parameters;
        
        /*!
          \brief Initialize the segment objects container and the segment 
          IDs container.
          \param segmenterIdsManager A segments ids manager to acquire
          unique segments ids.
          \param inputRaster The input raster.
          \param inputRasterBands Input raster bands to use.
          \param segmentsIds The output segment ids container.
          \param segments The output segments container.
          \return true if OK, false on errors.
        */        
        bool initializeSegments( SegmenterIdsManager& segmenterIdsManager,
          const te::rst::Raster& inputRaster,
          const std::vector< unsigned int >& inputRasterBands,
          SegmentsIdsContainerT& segmentsIds,
          SegmentsContainer& segments );
          
        /*!
          \brief Merge closest segments.
          \param similarityThreshold The minimum similarity value used
          when deciding when to merge two segments.
          \param segmenterIdsManager A segments ids manager to acquire
          unique segments ids.
          \param segmentsIds The output segment ids container.
          \param segments The output segments container.
          \return The number of merged segments.
        */           
        unsigned int mergeSegments( 
          const double similarityThreshold,
          SegmenterIdsManager& segmenterIdsManager,
          SegmentsIdsContainerT& segmentsIds,
          SegmentsContainer& segments );
          
        /*!
          \brief Merge only small segments to their closest segment.
          \param minSegmentSize The minimum segment size (pixels)
          \param similarityThreshold The minimum similarity value used
          when deciding when to merge two segments.
          \param segmenterIdsManager A segments ids manager to acquire
          unique segments ids.
          \param segmentsIds The output segment ids container.
          \param segments The output segments container.
          \return The number of merged segments.
        */           
        unsigned int mergeSmallSegments( 
          const unsigned int minSegmentSize,
          SegmenterIdsManager& segmenterIdsManager,
          SegmentsIdsContainerT& segmentsIds,
          SegmentsContainer& segments );          
          
        /*!
          \brief Export the segments IDs to a tif file.
          \param segmentsIds The output segment ids container.
          \param normto8bits If true, a 8 bits file will be generated.
          \param fileName The output tif file name.
        */           
        void exportSegs2Tif( const SegmentsIdsContainerT& segmentsIds,
          bool normto8bits, const std::string& fileName );
    };
    
    /*!
      \class SegmenterRegionGrowingStrategyFactory
      \brief Raster region growing segmenter strategy factory.
      \note Factory key: RegionGrowing
     */
    class TERPEXPORT SegmenterRegionGrowingStrategyFactory : public 
      te::rp::SegmenterStrategyFactory
    {
      public:
        
        SegmenterRegionGrowingStrategyFactory();
        
        ~SegmenterRegionGrowingStrategyFactory();
   
        //overload
        te::rp::SegmenterStrategy* build();
        
    };    

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ALGORITHM_H

