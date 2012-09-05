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
  \file terralib/rp/TiePointsLocator.h
  \brief TiePointsLocator locator.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATOR_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATOR_H

#include "Algorithm.h"
#include "Matrix.h"
#include "../raster/Raster.h"
#include "../geometry/GTParameters.h"
#include "../sam/rtree.h"

#include <vector>
#include <set>
#include <string>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocator
      \brief Tie points locator.
      \details Designed to find tie-points inside raster areas.
      \ingroup MatchingAlgorithms
     */
    class TERPEXPORT TiePointsLocator : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief TiePointsLocator input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            /*! \enum The strategy used to locate tie points.*/
            enum InteresPointsLocationStrategyType
            {
              InvalidStrategyT = 0, /*!< Invalid strategy. */
              MoravecStrategyT = 1, /*!<  Modified Moravec Interest Operator based image area matching. */
            };

            InteresPointsLocationStrategyType m_interesPointsLocationStrategy; //!< The strategy used to locate interest points (default:MoravecStrategyT).
            
            te::rst::Raster const* m_inRaster1Ptr; //!< Input raster 1.
            
            te::rst::Raster const* m_inMaskRaster1Ptr; //!< Optional one band input mask raster 1 (tie-points will not be generated inside mask image areas marked with zeroes).
            
            std::vector< unsigned int > m_inRaster1Bands; //!< Bands to be used from the input raster 1.
            
            unsigned int m_raster1TargetAreaLineStart; //!< The first line of the raster 1 target area to process (default:0 - The entire raster will be considered).
            
            unsigned int m_raster1TargetAreaColStart; //!< The first column of the raster 2 target area to process (default:0 - The entire raster will be considered).

            unsigned int m_raster1TargetAreaWidth; //!< The raster 1 target area width (default:0 - The entire raster will be considered).
            
            unsigned int m_raster1TargetAreaHeight; //!< The raster 1 target area height (default:0 - The entire raster will be considered).
            
            te::rst::Raster const* m_inRaster2Ptr; //!< Input raster 2.
            
            te::rst::Raster const* m_inMaskRaster2Ptr; //!< Optional one band input mask raster 2 (tie-points will not be generated inside mask image areas marked with zeroes).
            
            std::vector< unsigned int > m_inRaster2Bands; //!< Bands to be used from the input raster 2.
            
            unsigned int m_raster2TargetAreaLineStart; //!< The first line of the raster 2 target area to process (default:0 - The entire raster will be considered).
            
            unsigned int m_raster2TargetAreaColStart; //!< The first column of the raster 2 target area to process (default:0 - The entire raster will be considered).

            unsigned int m_raster2TargetAreaWidth; //!< The raster 2 target area width (default:0 - The entire raster will be considered).
            
            unsigned int m_raster2TargetAreaHeight; //!< The raster 2 target area height (default:0 - The entire raster will be considered).
            
            bool m_enableMultiThread; //!< Enable/Disable the use of multi-threads (default:true).
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            unsigned int m_maxTiePoints; //!< The maximum number of tie-points to generate (default=0).
            
            double m_pixelSizeXRelation; //!< The pixel resolution relation m_pixelSizeXRelation = raster1_pixel_res_x / raster2_pixel_res_x (default=1.0).
            
            double m_pixelSizeYRelation; //!< The pixel resolution relation m_pixelSizeYRelation = raster1_pixel_res_y / raster2_pixel_res_y (default=1.0).
            
            std::string m_geomTransfName; //!< The name of the geometric transformation used to ensure tie-points consistency (see each te::gm::GTFactory inherited classes to find each factory key/name, default:Affine).
            
            double m_geomTransfMaxError; //!< The maximum allowed transformation error (pixel units, default:1).
            
            unsigned int m_correlationWindowWidth; //!< The correlation window width used to correlate points between the images (minimum 3, default: 11).
            
            unsigned int m_moravecWindowWidth; //!< The Moravec window width used to locate canditate tie-points (minimum 3, default: 5 ).
            
            unsigned int m_maxR1ToR2Offset; //!< The maximum offset (pixels units) between a raster 1 point end the respective raster 2 point (default:0 - no offset restriction).
            
            bool m_enableGeometryFilter; //! < Enable/disable the geometry filter/outliers remotion (default:true).
            
            unsigned int m_GaussianFilterIterations; //!< The number of gaussian filter iterations (used to remove image noise, zero will disable the Gaussian Filter, default:1).
          
            InputParameters();
            
            InputParameters( const InputParameters& );
            
            ~InputParameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  InputParameters& operator=( const InputParameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };
        
        /*!
          \class OutputParameters
          \brief TiePointsLocator output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::vector< te::gm::GTParameters::TiePoint > m_tiePoints; //!< The generated tie-pionts (te::gm::GTParameters::TiePoint::first are raster 1 line/column indexes, te::gm::GTParameters::TiePoint::second are raster 2 line/column indexes).
            
            OutputParameters();
            
            OutputParameters( const OutputParameters& );
            
            ~OutputParameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  OutputParameters& operator=( const OutputParameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };        

        TiePointsLocator();
        
        ~TiePointsLocator();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        /*! Interest point type */
        class InterestPointT
        {
          public :
            unsigned int m_x; //!< Point X coord.

            unsigned int m_y; //!< Point Y coord.

            double m_featureValue; //!< Interest point feature value.
            
            InterestPointT() {};
            
            InterestPointT( const unsigned int& x, const unsigned int& y,
              const double& featureValue ) : m_x( x ), m_y( y ),
              m_featureValue( featureValue) {};
            
            ~InterestPointT() {};
            
            bool operator<( const InterestPointT& other ) const
            {
              return ( m_featureValue < other.m_featureValue );
            };
            
            const InterestPointT& operator=( const InterestPointT& other )
            {
              m_x = other.m_x;
              m_y = other.m_y;
              m_featureValue = other.m_featureValue;
              return other;
            };            
        };
        
        /*! Interest points container type 
        */
        typedef std::multiset< InterestPointT > InterestPointsContainerT;  
        
        /*! Matched Interest point type */
        class MatchedInterestPointsT
        {
          public :
            unsigned int m_x1; //!< Point X1 coord.

            unsigned int m_y1; //!< Point Y2 coord.
            
            unsigned int m_x2; //!< Point X1 coord.

            unsigned int m_y2; //!< Point Y2 coord.

            double m_featureValue; //!< Interest points feature value.
            
            MatchedInterestPointsT() {};
            
            MatchedInterestPointsT( const unsigned int& x1, const unsigned int& y1,
              const unsigned int& x2, const unsigned int& y2,
              const double& featureValue ) : m_x1( x1 ), m_y1( y1 ),
              m_x2( x2 ), m_y2( y2 ),
              m_featureValue( featureValue ) {};
            
            ~MatchedInterestPointsT() {};
            
            bool operator<( const MatchedInterestPointsT& other ) const
            {
              return ( m_featureValue < other.m_featureValue );
            };
            
            const MatchedInterestPointsT& operator=( const MatchedInterestPointsT& other )
            {
              m_x1 = other.m_x1;
              m_y1 = other.m_y1;
              m_x2 = other.m_x2;
              m_y2 = other.m_y2;
              m_featureValue = other.m_featureValue;
              return other;
            };            
        };        
        
        /*! Matched interest points container type 
        */
        typedef std::multiset< MatchedInterestPointsT > MatchedInterestPointsContainerT;         
        
        /*! 
          \brief The parameters passed to the moravecLocatorThreadEntry method.
        */     
        class MoravecLocatorThreadParams
        {
          public :
            
            /*!
            \brief Raster data container type.
            */          
            typedef Matrix< double > RasterDataContainerT;
            
            /*!
            \brief Mask raster data container type.
            */          
            typedef Matrix< unsigned char > MaskRasterDataContainerT;            
            
            bool* m_returnValuePtr; //! Thread return value pointer.
            
            unsigned int m_moravecWindowWidth; //!< The Moravec window width used to locate canditate tie-points (minimum 11, default: 11 ).
            
            unsigned int m_maxInterestPointsPerRasterLinesBlock; //!< The maximum number of points to find for each raster lines block.
            
            RasterDataContainerT const* m_rasterDataPtr; //!< The loaded raster data.
            
            MaskRasterDataContainerT const* m_maskRasterDataPtr; //!< The loaded mask raster data pointer (or zero if no mask is avaliable).
            
            InterestPointsContainerT* m_interestPointsPtr; //!< A pointer to a valid interest points container.
            
            boost::mutex* m_rastaDataAccessMutexPtr; //!< A pointer to a valid mutex to controle raster data access.
            
            boost::mutex* m_interestPointsAccessMutexPtr; //!< A pointer to a valid mutex to control the output interest points container access.
            
            unsigned int m_maxRasterLinesBlockMaxSize; //!< The maximum lines number of each raster block to process.
            
            unsigned int* m_nextRasterLinesBlockToProcessValuePtr; //!< A pointer to a valid counter to control the blocks processing sequence.
            
            MoravecLocatorThreadParams() {};
            
            ~MoravecLocatorThreadParams() {};
        };              
        
        /*! 
          \brief The parameters passed to the matchCorrelationEuclideanThreadEntry method.
        */     
        class CorrelationMatrixCalcThreadParams
        {
          public :
            
            Matrix< double > const* m_featuresSet1Ptr;
            
            Matrix< double > const* m_featuresSet2Ptr;
            
            InterestPointT const* m_interestPointsSet1Ptr;

            InterestPointT const* m_interestPointsSet2Ptr;            
            
            unsigned int* m_nextFeatureIdx1ToProcessPtr;
            
            Matrix< double >* m_corrMatrixPtr;
            
            boost::mutex* m_syncMutexPtr;
            
            unsigned int m_maxPt1ToPt2Distance; //!< Zero (disabled) or the maximum distance between a point from set 1 to a point from set 1 (points beyond this distance will not be correlated and will have zero as correlation value).
            
            te::sam::rtree::Index< unsigned int > const* m_interestPointsSet2RTreePtr; //!> A pointer to a RTree indexing interest point set points to their respective indexes.
            
            CorrelationMatrixCalcThreadParams() {};
            
            ~CorrelationMatrixCalcThreadParams() {};
        };        
        
        TiePointsLocator::InputParameters m_inputParameters; //!< TiePointsLocator input execution parameters.
//        TiePointsLocator::OutputParameters* m_outputParametersPtr; //!< TiePointsLocator input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        /*!
          \brief Load rasters data.
          
          \param rasterPtr Input raster pointer.
          
          \param rasterBands Input raster bands.
          
          \param maskRasterPtr The related input mask raster pointer (or zero, if no mask raster is avaliable).
          
          \param maskRasterBand The input mask raster band to use.
          
          \param rasterTargetAreaLineStart The raster target area initial line.
          
          \param rasterTargetAreaColStart The raster target area initial column.
          
          \param rasterTargetAreaWidth The raster target area width.
          
          \param rasterTargetAreaHeight The raster target area height.
          
          \param rescaleFactorX Scale factor to be applied on the loaded data.
          
          \param rescaleFactorY Scale factor to be applied on the loaded data.
          
          \param loadedRasterData The loaded raster data.
          
          \param loadedMaskRasterData The loaded mask raster data.

          \return true if ok, false on errors.
        */             
        bool loadRasterData( 
          te::rst::Raster const* rasterPtr,
          const std::vector< unsigned int >& rasterBands,
          te::rst::Raster const* maskRasterPtr,
          const unsigned int maskRasterBand,
          const unsigned int rasterTargetAreaLineStart,
          const unsigned int rasterTargetAreaColStart,
          const unsigned int rasterTargetAreaWidth,
          const unsigned int rasterTargetAreaHeight,
          const double rescaleFactorX,
          const double rescaleFactorY,
          std::vector< boost::shared_ptr< Matrix< double > > >& loadedRasterData,
          Matrix< unsigned char >& loadedMaskRasterData );
          
        /*!
          \brief Moravec interest points locator.
          
          \param rasterData The loaded raster data.
          
          \param maskRasterDataPtr The loaded mask raster data pointer (or zero if no mask is avaliable).
          
          \param moravecWindowWidth Moravec window width.
          
          \param maxInterestPoints The maximum number of interest points to find over raster 1.
          
          \param enableMultiThread Enable/disable multi-thread.
          
          \param interestPoints The found interest points (coords related to rasterData lines/cols).          

          \return true if ok, false on errors.
        */             
        static bool locateMoravecInterestPoints( 
          const Matrix< double >& rasterData,
          Matrix< unsigned char > const* maskRasterDataPtr,
          const unsigned int moravecWindowWidth,
          const unsigned int maxInterestPoints,
          const unsigned int enableMultiThread,
          InterestPointsContainerT& interestPoints );
          
        /*! 
          \brief Movavec locator thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void moravecLocatorThreadEntry(MoravecLocatorThreadParams* paramsPtr);
        
        /*! 
          \brief RoolUp a buffer of lines.
          
          \param bufferPtr Buffer pointer.
          
          \param bufferLinesNumber Buffer lines number.
        */         
        template< typename BufferElementT >
        static void roolUpBuffer( BufferElementT** bufferPtr, 
          const unsigned int& bufferLinesNumber )
        {
          BufferElementT* auxLinePtr = bufferPtr[ 0 ];
          
          for( unsigned int idx = bufferLinesNumber - 1 ; idx  ; --idx )
          {
            bufferPtr[ idx - 1 ] = bufferPtr[ idx ];            
          };
          
          bufferPtr[ bufferLinesNumber - 1 ] = auxLinePtr;
        };
        
        /*!
          \brief Moravec interest points locator.
          
          \param rasterData The loaded raster data.
          
          \param interestPoints The found raster 1 interest points (coords related to rasterData lines/cols).          

          \param tifFileName Tif file name.
        */             
        static void createTifFromMatrix( 
          const Matrix< double >& rasterData,
          const InterestPointsContainerT& interestPoints,
          const std::string& tifFileName );
          
        /*!
          \brief Gaussian Filter.
          
          \param inputData The input data.
          
          \param outputData The output data.
          
          \param iterationsNumber The number of filter iterations.

          \return true if ok, false on errors.
        */             
        static bool applyGaussianFilter( 
          const Matrix< double >& inputData,
          Matrix< double >& outputData,
          const unsigned int iterationsNumber );
          
        /*!
          \brief Generate a correlation features matrix for the given interes points.
          
          \param interestPoints The interest points (coords related to rasterData lines/cols).
          
          \param correlationWindowWidth The correlation window width used to correlate points between the images.
          
          \param rasterData The loaded raster data.
          
          \param normalize Normalize features values by subtracting its mean and dividing by its standard deviation.
          
          \param features The generated features matrix (one feature per line, one feature per interes point).
          
          \param validInteresPoints The valid interest pionts related to each feature inside the features matrix (some interest points may be invalid and are removed).
          
          \note Interest points outside the valid raster area will have all features with zero values.

          \return true if ok, false on errors.
        */             
        static bool generateCorrelationFeatures( 
          const InterestPointsContainerT& interestPoints,
          const unsigned int correlationWindowWidth,
          const Matrix< double >& rasterData,
          const bool normalize,
          Matrix< double >& features,
          InterestPointsContainerT& validInteresPoints );
          
        /*!
          \brief Save the generated features to tif files.
          
          \param features The features to be saved.
          
          \param validInteresPoints The interest pionts related to each feature inside the features matrix.
          
          \param fileNameStart The output file name beginning.
        */          
        static void features2Tiff( 
          const Matrix< double >& features,
          const InterestPointsContainerT& interestPoints,
          const std::string& fileNameBeginning );
          
        /*!
          \brief Match each feature using correlation and eucliean distance.
          
          \param featuresSet1 Features set 1.
          
          \param featuresSet2 Features set 2.
          
          \param interestPointsSet1 The interest pionts set 1.
          
          \param interestPointsSet2 The interest pionts set 2.
          
          \param maxPt1ToPt2Distance Zero (disabled) or the maximum distance between a point from set 1 to a point from set 1 (points beyond this distance will not be correlated and will have zero as correlation value).
          
          \param enableMultiThread Enable/disable the use of threads.
          
          \param matchedPoints The matched points.
        */          
        static bool matchCorrelationEuclidean( 
          const Matrix< double >& featuresSet1,
          const Matrix< double >& featuresSet2,
          const InterestPointsContainerT& interestPointsSet1,
          const InterestPointsContainerT& interestPointsSet2,
          const unsigned int maxPt1ToPt2Distance,
          const unsigned int enableMultiThread,
          MatchedInterestPointsContainerT& matchedPoints );
          
        /*! 
          \brief Correlation/Euclidean match thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void correlationMatrixCalcThreadEntry(
          CorrelationMatrixCalcThreadParams* paramsPtr);          
    };

  } // end namespace rp
}   // end namespace te

#endif

