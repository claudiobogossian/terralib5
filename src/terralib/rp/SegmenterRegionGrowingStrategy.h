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
              BaatzFeaturesType, //!< The Baatz based features will be used - Reference: Baatz, M.; Schape, A. Multiresolution segmentation: an optimization approach for high quality multi-scale image segmentation. In: XII Angewandte Geographische Informationsverarbeitung, Wichmann-Verlag, Heidelberg, 2000.
            };             
            
            unsigned int m_minSegmentSize; //!< A positive minimum segment size (pixels number - default: 1).
            
            double m_segmentsSimilarityThreshold; //!< Segments similarity treshold - Segments with similarity values below this value will be merged; valid values range: [ 0, 1 ]; default:0.5.
            
            SegmentFeaturesType m_segmentFeatures; //!< What segment features will be used on the segmentation process (default:MeanFeatureType).
            
            std::vector< double > m_bandsWeights; //!< The weight given to each band, when applicable (note: the bands weights sum must always be 1) or an empty vector indicating that all bands have the same weight.
            
            double m_colorWeight; //!< The weight given to the color component, deafult:0.5, valid range: [0,1].
            
            double m_compactnessWeight; //!< The weight given to the compactness component, deafult:0.5, valid range: [0,1].
            
            unsigned int m_segmentsSimIncreaseSteps; //!< The maximum number of steps to increment the similarity value for the cases where no segment merge occurred - defaul: 10.
            
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
          \class SegmentFeatures
          \brief Segment features base class
         */        
        class TERPEXPORT SegmentFeatures
        {
          public:
            
            /*!
              \brief Segment ID.
            */             
            SegmenterSegmentsBlock::SegmentIdDataType m_id;            
            
            /*!
              \brief Segment area (pixels number).
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
            
            SegmentFeatures() {};
            
            virtual ~SegmentFeatures() {};
            
            /*!
              \brief Creat a clone of this object.
              \return A clone of this object.
              \note The caller of this method must take the ownership of the returned object and delete it when necessary.
            */                          
            virtual SegmentFeatures* clone() const = 0;

            /*!
              \brief Copy the other instance state into this one.
              \param otherPtr The other instance ponter.
            */
            virtual void copy( SegmenterRegionGrowingStrategy::SegmentFeatures const * const otherPtr ) = 0;
            
          protected :
            
            const SegmenterRegionGrowingStrategy::SegmentFeatures& operator=(
              const SegmenterRegionGrowingStrategy::SegmentFeatures& other );
            
          private :
            
            SegmentFeatures( const SegmenterRegionGrowingStrategy::SegmentFeatures& ) {};
        };
        
        /*!
          \class Segment
          \brief Segment base class
         */        
        class TERPEXPORT Segment
        {
          public:
            
            /*!
              \brief Neighborhood segments.
            */             
            std::list< Segment* > m_neighborSegments;             
            
            virtual ~Segment() {};
            
            /*!
              \brief Returns the current segment internal features
              \return Returns the current segment internal features.
            */              
            virtual SegmenterRegionGrowingStrategy::SegmentFeatures* getFeatures() = 0;
            
            /*!
              \brief Returns the current segment internal features
              \return Returns the current segment internal features.
            */              
            virtual SegmenterRegionGrowingStrategy::SegmentFeatures const* getFeatures() const = 0;
            
          protected :
            
            Segment() {};
            
          private :
          
            Segment( const Segment& ) {};
            
            const Segment& operator=( const Segment& other ) { return other; };
        };
        
        /*!
          \class MeanBasedSegment
          \brief A segment based on pixel mean values
         */        
        class TERPEXPORT MeanBasedSegment : public Segment
        {
          public:
            
            /*!
              \class SegmentFeatures
              \brief Mean based segment features.
            */              
            class TERPEXPORT SegmentFeatures : public SegmenterRegionGrowingStrategy::SegmentFeatures
            {
              public :
                
                std::vector< double > m_means; //!< Segment mean values (for each band), normalized between 0 and 1.
                
                SegmentFeatures() {};
                
                ~SegmentFeatures() {};
                
                //overload
                SegmenterRegionGrowingStrategy::SegmentFeatures* clone() const;
                
                //overload
                void copy( SegmenterRegionGrowingStrategy::SegmentFeatures const * const otherPtr );
                
              private :
                
                SegmentFeatures( const SegmentFeatures& ) {};
                
                const SegmentFeatures& operator=( const SegmentFeatures& other ) { return other; };
            };
            
            MeanBasedSegment::SegmentFeatures m_features;
            
            MeanBasedSegment() {};
            
            ~MeanBasedSegment() {};
            
            //overload
            inline SegmenterRegionGrowingStrategy::SegmentFeatures* getFeatures()
            {
              return &m_features;
            };
            
            //overload
            inline SegmenterRegionGrowingStrategy::SegmentFeatures const* getFeatures() const
            {
              return &m_features;
            };            
        };
        
        /*!
          \class BaatzBasedSegment
          \brief A segment based on Baatz features values
         */        
        class TERPEXPORT BaatzBasedSegment : public Segment
        {
          public:
            
            /*!
              \class SegmentFeatures
              \brief Baatz based segment features.
            */              
            class TERPEXPORT SegmentFeatures : public SegmenterRegionGrowingStrategy::SegmentFeatures
            {
              public :
                
                std::vector< double > m_sums; //!< Segment sum of segment pixel velues.
                
                std::vector< double > m_squaresSum; //!< The sum of squares of pixel values (for each band).
                
                std::vector< double > m_stdDev; //!< Standard deviation of segment pixel velues.
                
                unsigned int m_edgeLength; //!< Segment edge length.
                
                double m_compactness; //!< Compactness of the current segment.
                
                double m_smoothness; //!< Smoothness of the current segment.
                
                SegmentFeatures() {};
                
                ~SegmentFeatures() {};
                
                //overload
                SegmenterRegionGrowingStrategy::SegmentFeatures* clone() const;
                
                //overload
                void copy( SegmenterRegionGrowingStrategy::SegmentFeatures const * const otherPtr );
                
              private :
                
                SegmentFeatures( const SegmentFeatures& ) {};
                
                const SegmentFeatures& operator=( const SegmentFeatures& other ) { return other; };                
            };  
            
            BaatzBasedSegment::SegmentFeatures m_features;
            
            BaatzBasedSegment() {};
            
            ~BaatzBasedSegment() {};
            
            //overload
            inline SegmenterRegionGrowingStrategy::SegmentFeatures* getFeatures()
            {
              return &m_features;
            };
            
            //overload
            inline SegmenterRegionGrowingStrategy::SegmentFeatures const* getFeatures() const
            {
              return &m_features;
            };                        
        };                   
        
        /*!
          \brief Internal segments container.
          \note All segment objects will be deleted at this instance destruction time.
         */        
        class TERPEXPORT SegmentsContainer : public std::map< 
          SegmenterSegmentsBlock::SegmentIdDataType, Segment* >
        {
          public :
            
            SegmentsContainer() {};
            
            ~SegmentsContainer();
            
            /*!
              \brief Delete all segment objects and clears the map.
              */              
            void deleteSegments();
        };        
        
        /*!
          \class Merger
          \brief Segments merger
         */        
        class TERPEXPORT Merger
        {
          public:
            
            virtual ~Merger() {};
            
            /*!
              \brief Returns a dissimilarity index between this and the
              other segment (normalized between 0 and 1).
              \param segment1Ptr A pointer to the first segment.
              \param segment2Ptr A pointer to the second segment.
              \param mergedFeatures A pointer to a valid segment features instance where the merged features values will be stored.
              \return A dissimilarity index between this and the
              other segment ( normalized between 0 and 1 ).
            */              
            virtual double getDissimilarityIndex(
              SegmenterRegionGrowingStrategy::Segment const * const segment1Ptr, 
              SegmenterRegionGrowingStrategy::Segment const * const segment2Ptr, 
              SegmenterRegionGrowingStrategy::SegmentFeatures* mergedFeatures ) const = 0;
              
            /*!
              \brief Merge specific segment features from both segments into the first segment.
              \param segment1Ptr The first segment.
              \param segment2Ptr A pointer to the second segment.
              \param mergedFeatures A pointer to a valid segment features instance where the merged features values are stored.
            */                
            virtual void mergeFeatures( SegmenterRegionGrowingStrategy::Segment * const segment1Ptr, 
              Segment const * const segment2Ptr, 
              SegmenterRegionGrowingStrategy::SegmentFeatures const * const mergedFeatures ) const = 0;
            
          protected :
            
            Merger() {};
            
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
            
            MeanMerger();
            
            ~MeanMerger();
            
            //overload        
            double getDissimilarityIndex(
              SegmenterRegionGrowingStrategy::Segment const * const segmen1tPtr, 
              SegmenterRegionGrowingStrategy::Segment const * const segmen2tPtr, 
              SegmenterRegionGrowingStrategy::SegmentFeatures* mergedFeatures ) const;
              
            //overload                
            void mergeFeatures( SegmenterRegionGrowingStrategy::Segment * const segmen1tPtr, 
              Segment const * const segmen2tPtr, 
              SegmenterRegionGrowingStrategy::SegmentFeatures const * const mergedFeatures ) const;
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
              \param segmentsIds //!< A reference to an external valid structure where each all segments IDs are stored.
              \param colorWeight //!< The weight given to the color component, deafult:0.5, valid range: [0,1].
              \param compactnessWeight //!< The weight given to the compactness component, deafult:0.5, valid range: [0,1].
            */
            BaatzMerger( const double& colorWeight, const double& compactnessWeight,
              const std::vector< double >& bandsWeights,
              const SegmentsIdsContainerT& segmentsIds );
            
            ~BaatzMerger();
            
            //overload        
            double getDissimilarityIndex(
              SegmenterRegionGrowingStrategy::Segment const * const segmen1tPtr, 
              SegmenterRegionGrowingStrategy::Segment const * const segmen2tPtr, 
              SegmenterRegionGrowingStrategy::SegmentFeatures* mergedFeatures ) const;
              
            //overload                
            void mergeFeatures( SegmenterRegionGrowingStrategy::Segment * const segmen1tPtr, 
              Segment const * const segmen2tPtr, 
              SegmenterRegionGrowingStrategy::SegmentFeatures const * const mergedFeatures ) const;
              
          protected :
            
            double m_colorWeight; //!< The weight given to the color component, deafult:0.5, valid range: [0,1].
            
            double m_compactnessWeight; //!< The weight given to the compactness component, deafult:0.5, valid range: [0,1].
            
            std::vector< double > m_bandsWeights; //!< A vector where each bands weight are stored.
            
            const SegmentsIdsContainerT& m_segmentsIds; //!< A reference to an external valid structure where each all segments IDs are stored.
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
          \param merger The merger instance to use.
          \param segments The output segments container.
          \return The number of merged segments.
        */           
        unsigned int mergeSegments( 
          const double similarityThreshold,
          SegmenterIdsManager& segmenterIdsManager,
          SegmentsIdsContainerT& segmentsIds,
          const Merger& merger,
          SegmentsContainer& segments );
          
        /*!
          \brief Merge only small segments to their closest segment.
          \param minSegmentSize The minimum segment size (pixels)
          \param similarityThreshold The minimum similarity value used
          when deciding when to merge two segments.
          \param segmenterIdsManager A segments ids manager to acquire
          unique segments ids.
          \param segmentsIds The output segment ids container.
          \param merger The merger instance to use.
          \param segments The output segments container.
          \return The number of merged segments.
        */           
        unsigned int mergeSmallSegments( 
          const unsigned int minSegmentSize,
          SegmenterIdsManager& segmenterIdsManager,
          SegmentsIdsContainerT& segmentsIds,
          const Merger& merger,
          SegmentsContainer& segments );          
          
        /*!
          \brief Export the segments IDs to a tif file.
          \param segmentsIds The output segment ids container.
          \param normto8bits If true, a 8 bits file will be generated.
          \param fileName The output tif file name.
        */           
        void exportSegs2Tif( const SegmentsIdsContainerT& segmentsIds,
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
          \return Returns the count of points from region 1 (with ID1) touching the region 2 (with ID2).
        */            
        static unsigned int getTouchingEdgeLength( const SegmentsIdsContainerT& segsIds,
          const unsigned int& xStart, const unsigned int& yStart,
          const unsigned int& xBound, const unsigned int& yBound,
          const SegmenterSegmentsBlock::SegmentIdDataType& id1,
          const SegmenterSegmentsBlock::SegmentIdDataType& id2 );
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

