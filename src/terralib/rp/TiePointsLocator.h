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
#include "../raster/Interpolator.h"
#include "../geometry/GeometricTransformation.h"
#include "../geometry/GTParameters.h"
#include "../sam/rtree.h"
#include "../common/progress/TaskProgress.h"

#include <vector>
#include <set>
#include <string>
#include <list>

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
      \ingroup rp_match
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
            
            /**
            * \name Global parameters
            */
            /**@{*/            
            
            /*! \enum The strategy used to locate tie points.*/
            enum InteresPointsLocationStrategyType
            {
              InvalidStrategyT = 0, /*!< Invalid strategy. */
              MoravecStrategyT = 1, /*!<  Modified Moravec Interest Operator based image area matching - Reference: Extration of GCP chips from GeoCover using Modified Moravec Interest Operator (MMIO) algorithm, The United States Geological Survey. */
              SurfStrategyT = 2 /*!<  SURF based image area matching - Reference: SURF: Speeded Up Robust Features, Herbert Bay. */
            };

            InteresPointsLocationStrategyType m_interesPointsLocationStrategy; //!< The strategy used to locate interest points (default:SurfStrategyT).
            
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
            
            unsigned int m_maxTiePoints; //!< The maximum number of tie-points to generate (default=0 - Automatically found).
            
            double m_pixelSizeXRelation; //!< The pixel resolution relation m_pixelSizeXRelation = raster1_pixel_res_x / raster2_pixel_res_x (default=1.0).
            
            double m_pixelSizeYRelation; //!< The pixel resolution relation m_pixelSizeYRelation = raster1_pixel_res_y / raster2_pixel_res_y (default=1.0).
            
            std::string m_geomTransfName; //!< The name of the geometric transformation used to ensure tie-points consistency (see each te::gm::GTFactory inherited classes to find each factory key/name, default:Affine).
            
            double m_geomTransfMaxError; //!< The maximum allowed transformation error (pixel units, default:2).
            
            unsigned int m_maxR1ToR2Offset; //!< The maximum offset (pixels units) between a raster 1 point end the respective raster 2 point (default:0 - no offset restriction).
            
            bool m_enableGeometryFilter; //!< Enable/disable the geometry filter/outliers remotion (default:true).
            
            double m_geometryFilterAssurance; //!< Geometry assurance (the error-free selection percent assurance) - valid range (0-1) - default:0.1 - Use 0-zero to let this number be automatically found.
            
            double m_rastersRescaleFactor; //!< Global rescale factor to apply to all input rasters (default:1, valid range: non-zero positive values).
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
            //@}
            
            /**
            * \name Moravec parameters
            */
            /**@{*/            
            
            unsigned int m_moravecCorrelationWindowWidth; //!< The correlation window width used to correlate points between the images (minimum 3, default: 11).
            
            unsigned int m_moravecWindowWidth; //!< The Moravec window width used to locate canditate tie-points (minimum 3, default: 5 ).
            
            unsigned int m_moravecGaussianFilterIterations; //!< The number of noise Gaussin iterations, when applicable (used to remove image noise, zero will disable the Gaussian Filter, default:1).
            
            double m_moravecMinAbsCorrelation; //!< The minimum acceptable absolute correlation value when matching features (when applicable),  default:0.5, valid range: [0,1].
            
            //@}
            
            /**
            * \name SURF parameters
            */
            /**@{*/
            
            unsigned int m_surfScalesNumber; //!< The number of sub-sampling scales to generate, when applicable (default:4, minimum:3).
            
            unsigned int m_surfOctavesNumber; //!< The number of octaves to generate, when applicable (default: 3, minimum:1).
            
            double m_surfMaxNormEuclideanDist; //!< The maximum acceptable euclidean distance when matching features (when applicable),  default:0.5, valid range: [0,1].
            
            //@}
            
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
            
            std::auto_ptr< te::gm::GeometricTransformation > m_transformationPtr; //!< The generated geometric transformation with the base mininum required tie-points set ( depending on the tie-points, a valid transformation may not exist, in this case, this pointer will be null).
            
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
        
        /*!
          \typedef FloatsMatrix
          \brief A matrix do store float values.
         */        
        typedef te::rp::Matrix< float > FloatsMatrix;      
        
        /*!
          \typedef DoublesMatrix
          \brief A matrix do store double values.
         */        
        typedef te::rp::Matrix< double > DoublesMatrix;
        
        /*!
          \typedef UCharsMatrix
          \brief A matrix do store unsigned char values.
         */    
        typedef te::rp::Matrix< unsigned char > UCharsMatrix;
        
        /*! Interest point type */
        class InterestPointT
        {
          public :
            unsigned int m_x; //!< Point X coord.

            unsigned int m_y; //!< Point Y coord.

            float m_feature1; //!< Interest point feature 1 value.
            
            float m_feature2; //!< Interest point feature 2 value.
            
            float m_feature3; //!< Interest point feature 3 value.
            
            InterestPointT() 
              : m_x( 0 ), m_y( 0 ), m_feature1( 0 ), m_feature2( 0 ),
                m_feature3( 0 ) 
            {
            };
            
            InterestPointT( const InterestPointT& other )
            {
              operator=( other );
            };
            
            ~InterestPointT() {};
            
            bool operator<( const InterestPointT& other ) const
            {
              return ( m_feature1 < other.m_feature1 );
            };
            
            const InterestPointT& operator=( const InterestPointT& other )
            {
              m_x = other.m_x;
              m_y = other.m_y;
              m_feature1 = other.m_feature1;
              m_feature2 = other.m_feature2;
              m_feature3 = other.m_feature3;
              return other;
            };            
        };
        
        /*! Interest points set container type 
        */
        typedef std::multiset< InterestPointT > InterestPointsSetT;  
        
        /*! Interest points list container type 
        */
        typedef std::list< InterestPointT > InterestPointsListT;
        
        /*! Matched Interest point type */
        class MatchedInterestPointsT
        {
          public :
            
            InterestPointT m_point1; //!< Interest point 1
            
            InterestPointT m_point2; //!< Interest point 2
            
            float m_feature; //!< Matched interest feature.
            
            MatchedInterestPointsT() {};
            
            MatchedInterestPointsT( const MatchedInterestPointsT& other )
            {
              operator=( other );
            };
            
            MatchedInterestPointsT( const InterestPointT& point1, 
              const InterestPointT& point2, const float& feature ) : 
              m_point1( point1 ), m_point2( point2 ),
              m_feature( feature ) {};
            
            ~MatchedInterestPointsT() {};
            
            bool operator<( const MatchedInterestPointsT& other ) const
            {
              return ( m_feature < other.m_feature );
            };
            
            const MatchedInterestPointsT& operator=( const MatchedInterestPointsT& other )
            {
              m_point1 = other.m_point1;
              m_point2 = other.m_point2;
              m_feature = other.m_feature;
              return other;
            };            
        };        
        
        /*! Matched interest points container type 
        */
        typedef std::multiset< MatchedInterestPointsT > MatchedInterestPointsSetT;         
        
        /*! 
          \brief The parameters passed to the moravecLocatorThreadEntry method.
        */     
        class MoravecLocatorThreadParams
        {
          public :
            
            bool* m_returnValuePtr; //! Thread return value pointer.
            
            unsigned int m_moravecWindowWidth; //!< The Moravec window width used to locate canditate tie-points (minimum 11, default: 11 ).
            
            unsigned int m_maxInterestPointsPerRasterLinesBlock; //!< The maximum number of points to find for each raster lines block.
            
            FloatsMatrix const* m_rasterDataPtr; //!< The loaded raster data.
            
            UCharsMatrix const* m_maskRasterDataPtr; //!< The loaded mask raster data pointer (or zero if no mask is avaliable).
            
            InterestPointsSetT* m_interestPointsPtr; //!< A pointer to a valid interest points container.
            
            boost::mutex* m_rastaDataAccessMutexPtr; //!< A pointer to a valid mutex to controle raster data access.
            
            boost::mutex* m_interestPointsAccessMutexPtr; //!< A pointer to a valid mutex to control the output interest points container access.
            
            unsigned int m_maxRasterLinesBlockMaxSize; //!< The maximum lines number of each raster block to process.
            
            unsigned int* m_nextRasterLinesBlockToProcessValuePtr; //!< A pointer to a valid counter to control the blocks processing sequence.
            
            MoravecLocatorThreadParams() {};
            
            ~MoravecLocatorThreadParams() {};
        };              
        
        
        /*! 
          \brief The parameters passed to the surfLocatorThreadEntry method.
        */     
        class SurfLocatorThreadParams
        {
          public :
            
            bool* m_returnValuePtr; //! Thread return value pointer.
            
            unsigned int m_scalesNumber; //!< The number of sub-samples scales to generate (minimum:3).
            
            unsigned int m_octavesNumber; //!< The number of octaves to generate (minimum:1).
            
            unsigned int m_maxInterestPointsPerRasterLinesBlock; //!< The maximum number of points to find for each raster lines block for each scale.
            
            FloatsMatrix const* m_integralRasterDataPtr; //!< The integral image raster data.
            
            UCharsMatrix const* m_maskRasterDataPtr; //!< The loaded mask raster data pointer (or zero if no mask is avaliable).
            
            InterestPointsSetT* m_interestPointsPtr; //!< A pointer to a valid interest points container.
            
            boost::mutex* m_rastaDataAccessMutexPtr; //!< A pointer to a valid mutex to controle raster data access.
            
            boost::mutex* m_interestPointsAccessMutexPtr; //!< A pointer to a valid mutex to control the output interest points container access.
            
            unsigned int m_maxRasterLinesBlockMaxSize; //!< The maximum lines number of each raster block to process.
            
            unsigned int* m_nextRasterLinesBlockToProcessValuePtr; //!< A pointer to a valid counter to control the blocks processing sequence.
            
            SurfLocatorThreadParams() {};
            
            ~SurfLocatorThreadParams() {};
        };         
        
        /*! 
          \brief The parameters passed to the matchCorrelationEuclideanThreadEntry method.
        */     
        class ExecuteMatchingByCorrelationThreadEntryParams
        {
          public :
            
            FloatsMatrix const* m_featuresSet1Ptr;
            
            FloatsMatrix const* m_featuresSet2Ptr;
            
            InterestPointT const* m_interestPointsSet1Ptr;

            InterestPointT const* m_interestPointsSet2Ptr;            
            
            unsigned int* m_nextFeatureIdx1ToProcessPtr;
            
            FloatsMatrix* m_corrMatrixPtr;
            
            boost::mutex* m_syncMutexPtr;
            
            unsigned int m_maxPt1ToPt2Distance; //!< Zero (disabled) or the maximum distance between a point from set 1 to a point from set 1 (points beyond this distance will not be correlated and will have zero as correlation value).
            
            ExecuteMatchingByCorrelationThreadEntryParams() {};
            
            ~ExecuteMatchingByCorrelationThreadEntryParams() {};
        };     
        
        /*! 
          \brief The parameters passed to the executeMatchingByEuclideanDistThreadEntry method.
        */     
        class ExecuteMatchingByEuclideanDistThreadEntryParams
        {
          public :
            
            FloatsMatrix const* m_featuresSet1Ptr;
            
            FloatsMatrix const* m_featuresSet2Ptr;
            
            InterestPointT const* m_interestPointsSet1Ptr;

            InterestPointT const* m_interestPointsSet2Ptr;            
            
            unsigned int* m_nextFeatureIdx1ToProcessPtr;
            
            FloatsMatrix* m_distMatrixPtr;
            
            boost::mutex* m_syncMutexPtr;
            
            unsigned int m_maxPt1ToPt2Distance; //!< Zero (disabled) or the maximum distance between a point from set 1 to a point from set 1 (points beyond this distance will not be correlated and will have zero as correlation value).
            
            ExecuteMatchingByEuclideanDistThreadEntryParams() {};
            
            ~ExecuteMatchingByEuclideanDistThreadEntryParams() {};
        };         
        
        TiePointsLocator::InputParameters m_inputParameters; //!< TiePointsLocator input execution parameters.
//        TiePointsLocator::OutputParameters* m_outputParametersPtr; //!< TiePointsLocator input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        
        /*!
          \brief Executes the Moravec algorithm using the supplied parameters.
          
          \param raster1XRescFact The X axis rescale factor to be aplied into raster 1.
          
          \param raster1YRescFact The Y axis rescale factor to be aplied into raster 1.

          \param raster2XRescFact The X axis rescale factor to be aplied into raster 2.
          
          \param raster2YRescFact The Y axis rescale factor to be aplied into raster 2.
          
          \param progressPtr A pointer to the progress instance or a NULL pointer if it is not used.
          
          \param raster1Data The raster 1 loaded data.
          
          \param maskRaster1Data The mask raster 1 loaded data.

          \param outParamsPtr Output parameters pointer.
          
          \param tiePointsWeights Ouptut tie-points weights.
          
          \return true if OK, false on errors.
         */        
        bool executeMoravec( 
          const double raster1XRescFact, 
          const double raster1YRescFact,
          const double raster2XRescFact,
          const double raster2YRescFact,
          te::common::TaskProgress* progressPtr,
          TiePointsLocator::OutputParameters* outParamsPtr,
          std::vector< double >& tiePointsWeights )
          throw( te::rp::Exception );
          
        /*!
          \brief Executes the SURF algorithm using the supplied parameters.
          
          \param raster1XRescFact The X axis rescale factor to be aplied into raster 1.
          
          \param raster1YRescFact The Y axis rescale factor to be aplied into raster 1.

          \param raster2XRescFact The X axis rescale factor to be aplied into raster 2.
          
          \param raster2YRescFact The Y axis rescale factor to be aplied into raster 2.
          
          \param progressPtr A pointer to the progress instance or a NULL pointer if it is not used.
          
          \param raster1Data The raster 1 loaded data.
          
          \param maskRaster1Data The mask raster 1 loaded data.

          \param outParamsPtr Output parameters pointer.
          
          \param tiePointsWeights Ouptut tie-points weights.
          
          \return true if OK, false on errors.
         */        
        bool executeSurf( 
          const double raster1XRescFact, 
          const double raster1YRescFact,
          const double raster2XRescFact,
          const double raster2YRescFact,
          te::common::TaskProgress* progressPtr,
          TiePointsLocator::OutputParameters* outParamsPtr,
          std::vector< double >& tiePointsWeights )
          throw( te::rp::Exception );        
        
        /*!
          \brief Load rasters data (normalized between 0 and 1).
          
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
          
          \param rasterInterpMethod The interpolation used when loading the input raster.
          
          \param maxMemPercentUsage The maximum amount (percent) of memory to use for the loaded data [0,100].
          
          \param loadedRasterData The loaded raster data.
          
          \param loadedMaskRasterData The loaded mask raster data.

          \return true if ok, false on errors.
        */             
        static bool loadRasterData( 
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
          const te::rst::Interpolator::Method rasterInterpMethod,
          const unsigned char maxMemPercentUsage, 
          std::vector< boost::shared_ptr< FloatsMatrix > >& loadedRasterData,
          UCharsMatrix& loadedMaskRasterData );
          
        /*!
          \brief Moravec interest points locator.
          
          \param rasterData The loaded raster data.
          
          \param maskRasterDataPtr The loaded mask raster data pointer (or zero if no mask is avaliable).
          
          \param moravecWindowWidth Moravec window width.
          
          \param maxInterestPoints The maximum number of interest points to find over raster 1.
          
          \param enableMultiThread Enable/disable multi-thread.
          
          \param interestPoints The found interest points (coords related to rasterData lines/cols).          
          
          \note InterestPointT::m_feature1 will be sum of differences between the Moravec filter response of each pixel and its neighborhoods (always a positive value).

          \return true if ok, false on errors.
        */             
        static bool locateMoravecInterestPoints( 
          const FloatsMatrix& rasterData,
          UCharsMatrix const* maskRasterDataPtr,
          const unsigned int moravecWindowWidth,
          const unsigned int maxInterestPoints,
          const unsigned int enableMultiThread,
          InterestPointsSetT& interestPoints );
          
        /*!
          \brief SURF interest points locator.
          
          \param integralRasterData Integral image raster data.
          
          \param maskRasterDataPtr The loaded mask raster data pointer (or zero if no mask is avaliable).
          
          \param interestPoints The found interest points (coords related to rasterData lines/cols).          

          \return true if ok, false on errors.
          
          \note InterestPointT::m_feature1 will be sum of differences between the hessian matrix determinant each pixel and its neighborhoods (always a positive value).
          
          \note InterestPointT::m_feature2 will be used filter width (pixels).
          
          \note InterestPointT::m_feature3 will 1 if the laplacian sign is positive, or zero if negative.
        */             
        bool locateSurfInterestPoints( 
          const FloatsMatrix& integralRasterData,
          UCharsMatrix const* maskRasterDataPtr,
          InterestPointsSetT& interestPoints ) const;          
          
        /*! 
          \brief Movavec locator thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void locateMoravecInterestPointsThreadEntry(MoravecLocatorThreadParams* paramsPtr);
        
        /*! 
          \brief Surf locator thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void locateSurfInterestPointsThreadEntry(SurfLocatorThreadParams* paramsPtr);        
        
        /*! 
          \brief RoolUp a buffer of lines.
          
          \param bufferPtr Buffer pointer.
          
          \param bufferLinesNumber Buffer lines number.
        */       
        template< typename BufferElementT >
        static void roolUpBuffer( BufferElementT** bufferPtr, 
          const unsigned int& bufferLinesNumber )
        {
          assert( bufferPtr );
          assert( bufferLinesNumber );
          
          unsigned int idx = 0;
          BufferElementT* auxLinePtr = bufferPtr[ 0 ];
          const unsigned int lastLineIdx = bufferLinesNumber - 1;
          
          for( idx = 0 ; idx < lastLineIdx ; ++idx )
          {
            bufferPtr[ idx ] = bufferPtr[ idx + 1 ];
          }
          
          bufferPtr[ lastLineIdx ] = auxLinePtr;
        }
        
        /*! 
          \brief Fill a buffer with zeroes.
          
          \param bufferPtr Buffer pointer.
          
          \param bufferLinesNumber Buffer lines number.
          
          \param bufferColsNumber Buffer columns number.
        */         
        template< typename BufferElementT >
        static void zeroFillBuffer( BufferElementT** bufferPtr, 
          const unsigned int& bufferLinesNumber, const unsigned int& 
          bufferColsNumber )
        {
          BufferElementT* linePtr = 0;
          unsigned int col = 0;
          for( unsigned int line = 0 ; line < bufferLinesNumber ; ++line )
          {
            linePtr = bufferPtr[ line ];
            for( col = 0 ; col < bufferColsNumber  ; ++col )
            {
              linePtr[ col ] = 0;
            }
          }
        }     
        
        /*!
          \brief Moravec interest points locator.
          
          \param rasterData The loaded raster data.
          
          \param interestPoints The found raster 1 interest points (coords related to rasterData lines/cols).          

          \param tifFileName Tif file name.
        */             
        static void createTifFromMatrix( 
          const DoublesMatrix& rasterData,
          const InterestPointsSetT& interestPoints,
          const std::string& tifFileName );
          
        /*!
          \brief Gaussian Filter.
          
          \param inputData The input data.
          
          \param outputData The output data.
          
          \param iterationsNumber The number of filter iterations.

          \return true if ok, false on errors.
        */             
        static bool applyGaussianFilter( 
          const DoublesMatrix& inputData,
          DoublesMatrix& outputData,
          const unsigned int iterationsNumber );
          
        /*!
          \brief Mean Filter.
          
          \param inputData The input data.
          
          \param outputData The output data.
          
          \param iterationsNumber The number of filter iterations.

          \return true if ok, false on errors.
        */             
        static bool applyMeanFilter( 
          const FloatsMatrix& inputData,
          FloatsMatrix& outputData,
          const unsigned int iterationsNumber );     
          
        /*!
          \brief Create an integral image.
          
          \param inputData The input data.
          
          \param outputData The output data.
          
          \note The entry of an integral image IΣ(x) at a location x = (x, y) represents the sum of all pixels in the input image I of a rectangular region formed by the point x and the origi.

          \return true if ok, false on errors.
        */             
        static bool createIntegralImage( 
          const FloatsMatrix& inputData,
          FloatsMatrix& outputData );           
          
        /*!
          \brief Generate correlation features ( normalized - unit vector ) matrix for the given interes points.
          
          \param interestPoints The interest points (coords related to rasterData lines/cols).
          
          \param correlationWindowWidth The correlation window width used to correlate points between the images.
          
          \param rasterData The loaded raster data.
          
          \param features The generated features matrix (one feature per line, one feature per interes point).
          
          \param validInteresPoints The valid interest pionts related to each feature inside the features matrix (some interest points may be invalid and are removed).

          \return true if ok, false on errors.
        */             
        static bool generateCorrelationFeatures( 
          const InterestPointsSetT& interestPoints,
          const unsigned int correlationWindowWidth,
          const FloatsMatrix& rasterData,
          FloatsMatrix& features,
          InterestPointsSetT& validInteresPoints );
          
        /*!
          \brief Generate a Surf features matrix for the given interes points.
          
          \param interestPoints The interest points (coords related to rasterData lines/cols).
          
          \param integralRasterData The integral raster data.
          
          \param validInterestPoints The valid interest points.
          
          \param features The generated features matrix (one feature per line, one feature per interest point).
          
          \return true if ok, false on errors.
        */             
        static bool generateSurfFeatures( 
          const InterestPointsSetT& interestPoints,
          const FloatsMatrix& integralRasterData,
          InterestPointsSetT& validInterestPoints,
          FloatsMatrix& features );          
          
        /*!
          \brief Save the generated features to tif files.
          
          \param features The features to be saved.
          
          \param validInteresPoints The interest pionts related to each feature inside the features matrix.
          
          \param fileNameStart The output file name beginning.
        */          
        static void features2Tiff( 
          const DoublesMatrix& features,
          const InterestPointsSetT& interestPoints,
          const std::string& fileNameBeginning );
          
        /*!
          \brief Match each feature using correlation.
          
          \param featuresSet1 Features set 1.
          
          \param featuresSet2 Features set 2.
          
          \param interestPointsSet1 The interest pionts set 1.
          
          \param interestPointsSet2 The interest pionts set 2.
          
          \param maxPt1ToPt2PixelDistance Zero (disabled) or the maximum distance (pixels) between a point from set 1 to a point from set 1 (points beyond this distance will not be correlated and will have zero as correlation value).
          
          \param enableMultiThread Enable/disable the use of threads.
          
          \param minAllowedAbsCorrelation The minimum acceptable absolute correlation value when matching features (when applicable).
          
          \param matchedPoints The matched points.
          
          \note Each matched point feature value ( MatchedInterestPoint::m_feature ) will be set to the absolute value of the correlation between then.
        */          
        static bool executeMatchingByCorrelation( 
          const FloatsMatrix& featuresSet1,
          const FloatsMatrix& featuresSet2,
          const InterestPointsSetT& interestPointsSet1,
          const InterestPointsSetT& interestPointsSet2,
          const unsigned int maxPt1ToPt2PixelDistance,
          const unsigned int enableMultiThread,
          const double minAllowedAbsCorrelation,
          MatchedInterestPointsSetT& matchedPoints );
          
        /*! 
          \brief Correlation/Euclidean match thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void executeMatchingByCorrelationThreadEntry(
          ExecuteMatchingByCorrelationThreadEntryParams* paramsPtr);
          
        /*!
          \brief Match each feature using eucliean distance.
          
          \param featuresSet1 Features set 1.
          
          \param featuresSet2 Features set 2.
          
          \param interestPointsSet1 The interest pionts set 1.
          
          \param interestPointsSet2 The interest pionts set 2.
          
          \param maxPt1ToPt2PixelDistance Zero (disabled) or the maximum distance (pixels) between a point from set 1 to a point from set 1 (points beyond this distance will not be correlated and will have zero as correlation value).
          
          \param maxEuclideanDist //!< The maximum acceptable euclidean distance when matching features.
          
          \param enableMultiThread Enable/disable the use of threads.
          
          \param matchedPoints The matched points.
          
          \note Each matched point feature value ( MatchedInterestPoint::m_feature ) will be set to the inverse normalized distance in the range (0,1].
        */          
        static bool executeMatchingByEuclideanDist( 
          const FloatsMatrix& featuresSet1,
          const FloatsMatrix& featuresSet2,
          const InterestPointsSetT& interestPointsSet1,
          const InterestPointsSetT& interestPointsSet2,
          const unsigned int maxPt1ToPt2PixelDistance,
          const double maxEuclideanDist,
          const unsigned int enableMultiThread,
          MatchedInterestPointsSetT& matchedPoints ); 
          
          
        /*! 
          \brief Correlation/Euclidean match thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void executeMatchingByEuclideanDistThreadEntry(
          ExecuteMatchingByEuclideanDistThreadEntryParams* paramsPtr);          
          
        /*! 
          \brief Print the given matrix to std::out.
          
          \param matrix The given matrix.
        */
        template < typename ElementT >
        static void printMatrix( const te::rp::Matrix< ElementT >& matrix )
        {
          std::cout << std::endl;
          
          for( unsigned int line = 0 ; line < matrix.getLinesNumber() ; ++line )
          {
            std::cout << std::endl << "[";
            
            for( unsigned int col = 0 ; col < matrix.getColumnsNumber() ; ++col )
            {
              std::cout << " ";
              std::cout << matrix( line, col );
            }
            
            std::cout << "]";
          }
          
          std::cout << std::endl;
        }        
        
        /*! 
          \brief Print the given buffer to std::out.
          
          \param buffer Buffer pointer.
          
          \param nLines Number of lines.
          
          \param nCols Number of columns.
        */
        static void printBuffer( double** buffer, const unsigned int nLines,
          const unsigned int nCols );
          
        /*! 
          \brief Return a sum of all pixels inside a box over the given integral image buffer.
          
          \param bufferPtr Buffer pointer.
          
          \param upperLeftX Box upper left X.
          
          \param upperLeftY Box upper left Y.
          
          \param lowerRightX Box lower right X.
          
          \param lowerRightY Box lower right X.
        */          
        template< typename BufferType >
        inline static float getIntegralBoxSum( BufferType& buffer, 
          const unsigned int& upperLeftX, const unsigned int& upperLeftY, 
          const unsigned int& lowerRightX, const unsigned int& lowerRightY )
        {
          return buffer[ lowerRightY ][ lowerRightX ]
            - ( ( upperLeftY ) ? buffer[ upperLeftY - 1 ][ lowerRightX ] : 0 )
            - ( ( upperLeftX ) ? buffer[ lowerRightY ][ upperLeftX - 1 ] : 0 )
            + ( ( ( upperLeftX != 0 ) && ( upperLeftY != 0 ) ) ? buffer[ upperLeftY - 1 ][ upperLeftX - 1 ] : 0 );
        }
        
        /*! 
          \brief Return a SURF box filter Dxx derivative centered over the given position from the given integral image buffer.
          
          \param bufferPtr Buffer pointer.
          
          \param centerX Center X.
          
          \param centerY Center Y.
          
          \param lobeWidth Filter lobe width.
          
          \param lobeRadius Filter lobe radius.
        */         
        inline static float getSurfDyyDerivative( float** bufferPtr, 
          const unsigned int& centerX,  const unsigned int& centerY, 
           const unsigned int& lobeWidth,  const unsigned int& lobeRadius )
        {
          return 
            getIntegralBoxSum(
              bufferPtr,
              ( centerX + 1 - lobeWidth ),
              ( centerY - lobeWidth - lobeRadius ),
              ( centerX + lobeWidth - 1 ),
              ( centerY + lobeWidth + lobeRadius ) )
            - ( 3.0f *
            getIntegralBoxSum(
              bufferPtr,
              ( centerX + 1 - lobeWidth ),
              ( centerY - lobeRadius ),
              ( centerX + lobeWidth - 1 ),
              ( centerY + lobeRadius ) ) );
        };
        
        /*! 
          \brief Return a SURF box filter Dyy derivative centered over the given position from the given integral image buffer.
          
          \param bufferPtr Buffer pointer.
          
          \param centerX Center X.
          
          \param centerY Center Y.
          
          \param lobeWidth Filter lobe width.
          
          \param lobeRadius Filter lobe radius.
        */        
        inline static float getSurfDxxDerivative( float** bufferPtr, 
          const unsigned int& centerX,  const unsigned int& centerY, 
           const unsigned int& lobeWidth,  const unsigned int& lobeRadius )
        {
          return 
            getIntegralBoxSum(
              bufferPtr,
              ( centerX - lobeWidth - lobeRadius ),
              ( centerY + 1 - lobeWidth ),
              ( centerX + lobeWidth + lobeRadius ),
              ( centerY + lobeWidth - 1 ) )
            - 3.0f *
            getIntegralBoxSum(
              bufferPtr,
              ( centerX - lobeRadius ),
              ( centerY + 1 - lobeWidth ),
              ( centerX + lobeRadius ),
              ( centerY + lobeWidth - 1 ) );
        };
        
        /*! 
          \brief Return a SURF box filter Dxy derivative centered over the given position from the given integral image buffer.
          
          \param bufferPtr Buffer pointer.
          
          \param centerX Center X.
          
          \param centerY Center Y.
          
          \param lobeWidth Filter lobe width.
        */ 
        inline static float getSurfDxyDerivative( float** bufferPtr, 
          const unsigned int& centerX,  const unsigned int& centerY, 
           const unsigned int& lobeWidth )
        {
          return
            // upper-left filter lobe
            getIntegralBoxSum(
              bufferPtr,
              ( centerX - lobeWidth ),
              ( centerY - lobeWidth ),
              ( centerX - 1 ),
              ( centerY - 1 ) )
            +
            // lower-right filter lobe
            getIntegralBoxSum(
              bufferPtr,
              ( centerX + 1 ),
              ( centerY + 1 ),
              ( centerX + lobeWidth ),
              ( centerY + lobeWidth ) )
            -
            // upper-right filter lobe
            getIntegralBoxSum(
              bufferPtr,
              ( centerX + 1 ),
              ( centerY - lobeWidth ),
              ( centerX + lobeWidth ),
              ( centerY - 1 ) )
            -
            // lower-left filter lobe
            getIntegralBoxSum(
              bufferPtr,
              ( centerX - lobeWidth ),
              ( centerY + 1 ),
              ( centerX - 1 ),
              ( centerY + lobeWidth ) );
        };
        
        /*! 
          \brief Return a Haar X intesity vector for the window centered at the given point.
          
          \param buffer Integral image buffer.
          
          \param centerX Center X.
          
          \param centerY Center Y.
          
          \param radius Window radius.
        */         
        template< typename BufferType >
        inline static float getHaarXVectorIntensity( BufferType& buffer, 
          const unsigned int& centerX,  const unsigned int& centerY, 
           const unsigned int& radius )
        {
          return
            getIntegralBoxSum(
              buffer,
              ( centerX + 1 ),
              ( centerY - radius ),
              ( centerX + radius ),
              ( centerY + radius ) )
            -
            getIntegralBoxSum(
              buffer,
              ( centerX - radius ),
              ( centerY - radius ),
              ( centerX - 1 ),
              ( centerY + radius ) );
        }
        
        /*! 
          \brief Return a Haar Y intesity vector for the window centered at the given point.
          
          \param buffer Integral image buffer.
          
          \param centerX Center X.
          
          \param centerY Center Y.
          
          \param radius Window radius.
        */         
        template< typename BufferType >
        inline static float getHaarYVectorIntensity( BufferType& buffer, 
          const unsigned int& centerX,  const unsigned int& centerY, 
           const unsigned int& radius )
        {
          return
            getIntegralBoxSum(
              buffer,
              ( centerX - radius ),
              ( centerY - radius ),
              ( centerX + radius ),
              ( centerY - 1 ) )
            -
            getIntegralBoxSum(
              buffer,
              ( centerX - radius ),
              ( centerY + 1 ),
              ( centerX + radius ),
              ( centerY + radius ) );
        }
        
        /*! 
          \brief Return the surf octave filter step size (width).
          
          \param octaveIndex Octave index (starting from zero).
        */           
        inline static unsigned int getSurfOctaveFilterStepSize( const unsigned int& octaveIndex )
        {
          return (unsigned int)std::pow( 2.0, (double)(octaveIndex + 1) );
        };

        /*! 
          \brief Return the surf octave base filter size (width).
          
          \param octaveIndex Octave index (starting from zero).
        */
        inline static unsigned int getSurfOctaveBaseFilterSize( const unsigned int& octaveIndex )
        {
          return  3 + ( 3 * getSurfOctaveFilterStepSize( octaveIndex ) );
        };
        
        /*! 
          \brief Return the surf octave filter size (width).
          
          \param octaveIndex Octave index (starting from zero).
          
          \param scaleIndex Scale index (starting from zero).
        */        
        inline static unsigned int getSurfFilterSize( const unsigned int& octaveIndex,
          const unsigned int& scaleIndex )
        {
          return  getSurfOctaveBaseFilterSize( octaveIndex ) + 
            ( 3 * ( getSurfOctaveFilterStepSize( octaveIndex ) * scaleIndex ) );
        };                

    };

  } // end namespace rp
}   // end namespace te

#endif

