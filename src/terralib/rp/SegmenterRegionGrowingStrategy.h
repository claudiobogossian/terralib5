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
#include "SegmenterRegionGrowingSegment.h"
#include "SegmenterRegionGrowingSegmentsPool.h"
#include "SegmenterSegmentsBlock.h"
#include "Matrix.h"
#include "Config.h"

#include <vector>
#include <list>
#include <set>

namespace te
{
  namespace rp
  {
    /*!
      \class SegmenterRegionGrowingStrategy
      \brief Raster region growing segmenter strategy.
      \ingroup rp_seg
     */
    class TERPEXPORT SegmenterRegionGrowingStrategy : public SegmenterStrategy
    {
      public :
        
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
              InvalidFeaturesType, //!< Invalid features type.
              MeanFeaturesType, //!< The mean of segments pixel values will be used - Reference: S. A. Bins, L. M. G. Fonseca, G. J. Erthal e F. M. Ii, "Satellite Imagery segmentation: a region growing approach", VIII Simposio Brasileiro de Sensoriamento Remoto, Salvador, BA, 14-19 abril 1996.
              BaatzFeaturesType //!< The Baatz based features will be used - Reference: Baatz, M.; Schape, A. Multiresolution segmentation: an optimization approach for high quality multi-scale image segmentation. In: XII Angewandte Geographische Informationsverarbeitung, Wichmann-Verlag, Heidelberg, 2000.
            };             
            
            unsigned int m_minSegmentSize; //!< A positive minimum segment size (pixels number - default: 100).
            
            double m_segmentsSimilarityThreshold; //!< Segments similarity treshold - Use lower values to merge only those segments that are more similar - Higher values will allow more segments to be merged - valid values range: positive values - default: 0.03 ).
            
            SegmentFeaturesType m_segmentFeatures; //!< What segment features will be used on the segmentation process (default:InvalidFeaturesType).
            
            std::vector< double > m_bandsWeights; //!< The weight given to each band, when applicable (note: the bands weights sum must always be 1) or an empty vector indicating that all bands have the same weight.
            
            double m_colorWeight; //!< The weight given to the color component, deafult:0.9, valid range: [0,1].
            
            double m_compactnessWeight; //!< The weight given to the compactness component, deafult:0.5, valid range: [0,1].
            
            unsigned int m_segmentsSimIncreaseSteps; //!< The maximum number of steps to increment the similarity threshold value for the cases where no segment merge occurred - zero will disable segment similarity threshold increments - defaul: 2.
            
            Parameters();
            
            ~Parameters();
            
            //overload 
            const Parameters& operator=( const Parameters& params );
            
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
        
      protected :
        
        /*!
          \brief Internal segments ids matrix type definition.
         */          
        typedef Matrix< SegmenterSegmentsBlock::SegmentIdDataType >
          SegmentsIdsMatrixT;
        
        /*!
          \class Merger
          \brief Segments merger
         */        
        class TERPEXPORT Merger
        {
          public:
            
            virtual ~Merger();
            
            /*!
              \brief Returns a dimilarity index between this and the other segment.
              \param segment1Ptr A pointer to the first segment.
              \param segment2Ptr A pointer to the second segment.
              \param mergePreviewSegPtr A pointer to a valid segment where the merged features values will be stored (when aplicable).
              \return A similarity index between this and the other segment ( normalized between 0 and 1 ).
            */              
            virtual SegmenterRegionGrowingSegment::FeatureType getDissimilarity(
              SegmenterRegionGrowingSegment const * const segment1Ptr, 
              SegmenterRegionGrowingSegment const * const segment2Ptr, 
              SegmenterRegionGrowingSegment * const mergePreviewSegPtr ) const = 0;
              
            /*!
              \brief Merge specific segment features from both segments into the first segment.
              \param segment1Ptr The first segment.
              \param segment2Ptr A pointer to the second segment.
              \param mergePreviewSegPtr A pointer to a valid segment where the merged features values were be stored by calling getDissimilarityIndex (when aplicable).
            */                
            virtual void mergeFeatures( 
              SegmenterRegionGrowingSegment * const segment1Ptr, 
              SegmenterRegionGrowingSegment const * const segment2Ptr, 
              SegmenterRegionGrowingSegment const * const mergePreviewSegPtr ) const = 0;
              
            /*!
              \brief Update the internal state.
              \param actSegsListHeadPtr A pointer the the active segments list head.
            */    
            virtual void update( SegmenterRegionGrowingSegment* const actSegsListHeadPtr ) = 0;

            /*!
              \brief Return the required segments features vector size (numer of elements).
              \return Return the required segments features vector size (numer of elements).
            */                
            virtual unsigned int getSegmentFeaturesSize() const = 0;
            
          protected :
            
            Merger();
            
          private :
          
            Merger( const Merger& ) {};
            
            const Merger& operator=( const Merger& other ) { return other; };
        };        
        
        /*!
          \class MeanMerger
          \brief Mean based Segments merger
         */        
        class TERPEXPORT MeanMerger : public SegmenterRegionGrowingStrategy::Merger
        {
          public:
            
            MeanMerger( const unsigned int featuresNumber );
            
            ~MeanMerger();
            
            //overload        
            SegmenterRegionGrowingSegment::FeatureType getDissimilarity(
              SegmenterRegionGrowingSegment const * const segment1Ptr, 
              SegmenterRegionGrowingSegment const * const segment2Ptr, 
              SegmenterRegionGrowingSegment * const mergePreviewSegPtr ) const;
              
            //overload                
            void mergeFeatures( 
              SegmenterRegionGrowingSegment * const segment1Ptr, 
              SegmenterRegionGrowingSegment const * const segment2Ptr, 
              SegmenterRegionGrowingSegment const * const mergePreviewSegPtr ) const;
              
            //overload
            void update( SegmenterRegionGrowingSegment* const ) {};  
            
            //overload
            inline unsigned int getSegmentFeaturesSize() const { return m_featuresNumber; };
            
          protected :
            
            unsigned int m_featuresNumber; //!< The number of features (bands).
            SegmenterRegionGrowingSegment::FeatureType m_dissimilarityNormFactor;
            
            // variables used by the method getDissimilarity
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_dissValue;
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_diffValue; 
            mutable unsigned int m_getDissimilarity_meansIdx;
        };        
        
        /*!
          \class BaatzMerger
          \brief Baatz based Segments merger
         */        
        class TERPEXPORT BaatzMerger : public SegmenterRegionGrowingStrategy::Merger
        {
          public:
            
            /*!
              \brief Default constructor.
              \param bandsWeights A reference to an external valid structure where each bands weight are stored.
              \param segmentsIds //!< A reference to an external valid structure where all segments IDs are stored.
              \param segmentsMatrix //!< A reference to an external valid segments matrix.
              \param colorWeight //!< The weight given to the color component, deafult:0.5, valid range: [0,1].
              \param compactnessWeight //!< The weight given to the compactness component, deafult:0.5, valid range: [0,1].
            */
            BaatzMerger( const double& colorWeight, const double& compactnessWeight,
              const std::vector< double >& bandsWeights,
              const SegmentsIdsMatrixT& segmentsIds );
            
            ~BaatzMerger();
            
            //overload        
            SegmenterRegionGrowingSegment::FeatureType getDissimilarity(
              SegmenterRegionGrowingSegment const * const segment1Ptr, 
              SegmenterRegionGrowingSegment const * const segment2Ptr, 
              SegmenterRegionGrowingSegment * const mergePreviewSegPtr ) const;
              
            //overload                
            void mergeFeatures( 
              SegmenterRegionGrowingSegment * const segment1Ptr, 
              SegmenterRegionGrowingSegment const * const segment2Ptr, 
              SegmenterRegionGrowingSegment const * const mergePreviewSegPtr ) const;
              
            //overload
            void update( SegmenterRegionGrowingSegment* const actSegsListHeadPtr );
            
            //overload
            inline unsigned int getSegmentFeaturesSize() const { return 3 + ( 3 * m_bandsNumber ); };            
              
          protected :

            const SegmentsIdsMatrixT& m_segmentsIds; //!< A reference to an external valid structure where each all segments IDs are stored.
            
            unsigned int m_bandsNumber; //!< The number of features (bands).
            
            SegmenterRegionGrowingSegment::FeatureType m_allSegsCompactnessOffset; //!< The offsets applied to normalize the compactness value.
            
            SegmenterRegionGrowingSegment::FeatureType m_allSegsCompactnessGain; //!< The gains applied to normalize the compactness value.
            
            SegmenterRegionGrowingSegment::FeatureType m_allSegsSmoothnessOffset; //!< The offsets applied to normalize the smoothness value.
            
            SegmenterRegionGrowingSegment::FeatureType m_allSegsSmoothnessGain; //!< The gains applied to normalize the smoothness value.            
            
            SegmenterRegionGrowingSegment::FeatureType m_allSegsStdDevOffset; //!< The offsets applied to normalize the standard deviation value.
            
            SegmenterRegionGrowingSegment::FeatureType m_allSegsStdDevGain; //!< The gains applied to normalize the standard deviation value.            
            
            SegmenterRegionGrowingSegment::FeatureType m_colorWeight; //!< The weight given to the color component, deafult:0.5, valid range: [0,1].
            
            SegmenterRegionGrowingSegment::FeatureType m_compactnessWeight; //!< The weight given to the compactness component, deafult:0.5, valid range: [0,1].
            
            std::vector< SegmenterRegionGrowingSegment::FeatureType > m_bandsWeights; //!< A vector where each bands weight are stored.
            
            // Variables used by the method getDissimilarity            
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_sizeUnionD;            
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_sizeSeg1D;
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_sizeSeg2D;
            mutable unsigned int m_getDissimilarity_touchingEdgeLength1;
            mutable unsigned int m_getDissimilarity_touchingEdgeLength2;               
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_hCompact; 
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_hSmooth;
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_hForm;
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_hColor;
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_sumUnion;
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_squaresSumUnion;
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_meanUnion;
            mutable SegmenterRegionGrowingSegment::FeatureType m_getDissimilarity_stdDevUnion;
            mutable unsigned int m_getDissimilarity_sumsIdx;
            
            // Variables used by the method update
            mutable SegmenterRegionGrowingSegment::FeatureType m_update_compactnessMin;
            mutable SegmenterRegionGrowingSegment::FeatureType m_update_compactnessMax;
            mutable SegmenterRegionGrowingSegment::FeatureType m_update_smoothnessMin;
            mutable SegmenterRegionGrowingSegment::FeatureType m_update_smoothnessMax;              
            mutable SegmenterRegionGrowingSegment::FeatureType m_update_stdDevMin;
            mutable SegmenterRegionGrowingSegment::FeatureType m_update_stdDevMax;              
            mutable SegmenterRegionGrowingSegment::FeatureType* m_update_featuresPtr;
            mutable SegmenterRegionGrowingSegment* m_update_currentActSegPtr;
            mutable unsigned int m_update_band;
            mutable SegmenterRegionGrowingSegment::FeatureType const* m_update_stdDevPtr;
        };          
        
        /*!
          \brief true if this instance is initialized.
        */        
        bool m_isInitialized;

        /*!
          \brief Internal execution parameters.
        */        
        SegmenterRegionGrowingStrategy::Parameters m_parameters;
        
        /*! \brief A pool of segments that can be reused on each strategy execution. */
        SegmenterRegionGrowingSegmentsPool m_segmentsPool;
        
        /*! \brief A internal segments IDs matrix that can be reused  on each strategy execution. */
        SegmentsIdsMatrixT m_segmentsIdsMatrix;
        
        /*!
          \brief Initialize the segment objects container and the segment IDs container.
          \param segmenterIdsManager A segments ids manager to acquire unique segments ids.
          \param block2ProcessInfo Info about the block to process.
          \param inputRaster The input raster.
          \param inputRasterBands Input raster bands to use.
          \param inputRasterNoDataValues A vector of values to be used as input raster no-data values.
          \param inputRasterBandMinValues The minimum value present on each band.
          \param inputRasterBandMinValues The maximum value present on each band.          
          \param actSegsListHeadPtr A pointer the the active segments list head.
          \return true if OK, false on errors.
        */        
        bool initializeSegments( SegmenterIdsManager& segmenterIdsManager,
          const te::rp::SegmenterSegmentsBlock& block2ProcessInfo, 
          const te::rst::Raster& inputRaster,
          const std::vector< unsigned int >& inputRasterBands,
          const std::vector< double >& inputRasterNoDataValues,
          const std::vector< double >& inputRasterBandMinValues,
          const std::vector< double >& inputRasterBandMaxValues,
          SegmenterRegionGrowingSegment** actSegsListHeadPtr );
          
        /*!
          \brief Merge closest segments.
          \param disimilarityThreshold The maximum similarity value allowed when deciding when to merge two segments.
          \param maxSegSizeThreshold Segments with sizes smaller then this value will allways be merged with the closest segment (disimilarityThreshold will be ignored).
          \param segmenterIdsManager A segments ids manager to acquire unique segments ids.
          \param merger The merger instance to use.
          \param enablelocalMutualBestFitting If enabled, a merge only occurs between two segments if the minimum dissimilarity criteria is best fulfilled mutually.
          \param auxSeg1Ptr A pointer to a valid auxiliar segment that will be used by this method.
          \param auxSeg2Ptr A pointer to a valid auxiliar segment that will be used by this method.
          \param auxSeg3Ptr A pointer to a valid auxiliar segment that will be used by this method.
          \param minFoundDissimilarity The minimum dissimilarity value found.
          \param maxFoundDissimilarity The maximum dissimilarity value found.
          \param totalMergesNumber The total number of merges.
          \param mergeIterationCounter A reference to a iteration number counter (this variable will be only incremented, never zeroed. It never must be reset. ).
          \param actSegsListHeadPtr A pointer the the active segments list head.
        */           
        void mergeSegments( 
          const SegmenterRegionGrowingSegment::FeatureType disimilarityThreshold,
          const unsigned int maxSegSizeThreshold,
          SegmenterIdsManager& segmenterIdsManager,
          Merger& merger,
          const bool enablelocalMutualBestFitting,
          SegmenterRegionGrowingSegment* auxSeg1Ptr,
          SegmenterRegionGrowingSegment* auxSeg2Ptr,
          SegmenterRegionGrowingSegment* auxSeg3Ptr,
          SegmenterRegionGrowingSegment::FeatureType& minFoundDissimilarity,
          SegmenterRegionGrowingSegment::FeatureType& maxFoundDissimilarity,
          unsigned int& totalMergesNumber,
          SegmenterRegionGrowingSegment::IterationCounterType& globalMergeIterationsCounter,
          SegmenterRegionGrowingSegment** const actSegsListHeadPtrPtr );
          
        /*!
          \brief Export the segments IDs to a tif file.
          \param segmentsIds The output segment ids container.
          \param normto8bits If true, a 8 bits file will be generated.
          \param fileName The output tif file name.
        */           
        void exportSegs2Tif( const SegmentsIdsMatrixT& segmentsIds,
          bool normto8bits, const std::string& fileName );
          
        /*!
          \brief Returns the count of points from region 1 (with ID1) touching the region 2 (with ID2).
          \param segsIds The segment ids container.
          \param xStart The upper left X of the bounding box surrounding both regions.
          \param yStart The upper left Y of the bounding box surrounding both regions.
          \param xBound The lower right X bound of the bounding box surrounding both regions.
          \param yBound The lower right Y bound of the bounding box surrounding both regions.
          \param id1 Region 1 ID.
          \param id2 Region 2 ID.
          \param edgeLength1 The touching edge length for the region 1.
          \param edgeLength2 The touching edge length for the region 2.
          \return Returns the count of points from region 1 (with ID1) touching the region 2 (with ID2).
        */            
        static void getTouchingEdgeLength( const SegmentsIdsMatrixT& segsIds,
          const unsigned int& xStart, const unsigned int& yStart,
          const unsigned int& xBound, const unsigned int& yBound,
          const SegmenterSegmentsBlock::SegmentIdDataType& id1,
          const SegmenterSegmentsBlock::SegmentIdDataType& id2,
          unsigned int& edgeLength1,
          unsigned int& edgeLength2 );
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

