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
  \file terralib/rp/TiePointsLocatorSURFStrategy.h
  \brief Tie-Pointsr locator SURF strategy.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSURFSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSURFSTRATEGY_H

#include "TiePointsLocatorStrategy.h"
#include "TiePointsLocatorStrategyParameters.h"
#include "TiePointsLocatorStrategyFactory.h"

#include <boost/thread.hpp>

#include <cmath>

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocatorSURFStrategy
      \brief Tie-points locator SURF strategy.
     */
    class TERPEXPORT TiePointsLocatorSURFStrategy : public TiePointsLocatorStrategy
    {
      friend class TiePointsLocatorSURFStrategyFactory;
      
      public:
        
        /*!
          \class Parameters
          \brief TiePointsLocator SURF strategy parameters.
         */        
        class TERPEXPORT Parameters : public TiePointsLocatorStrategyParameters
        {
          public:
            
            unsigned int m_surfScalesNumber; //!< The number of sub-sampling scales to generate, when applicable (default:3, minimum:3).
            
            unsigned int m_surfOctavesNumber; //!< The number of octaves to generate, when applicable (default: 2, minimum:2).
            
            double m_surfMaxNormEuclideanDist; //!< The maximum acceptable euclidean distance when matching features (when applicable),  default:0.75, valid range: [0,1].
            
            Parameters();
            
            Parameters( const Parameters& );
            
            ~Parameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  Parameters& operator=( const Parameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };        
        
        ~TiePointsLocatorSURFStrategy();
        
        //overload
        void getSubSampledSpecStrategyParams( 
          const double subSampleOptimizationRescaleFactor,
          const TiePointsLocatorStrategyParameters& inputSpecParams,
          std::auto_ptr< TiePointsLocatorStrategyParameters >& subSampledSpecParamsPtr ) const;
          
        //overload
        void getDefaultSpecStrategyParams( 
          std::auto_ptr< TiePointsLocatorStrategyParameters >& defaultSpecParamsPtr ) const;          
        
      protected :
        
        /*! 
          \brief The parameters passed to the surfLocatorThreadEntry method.
        */     
        class SurfLocatorThreadParams
        {
          public :
            
            bool* m_returnValuePtr; //! Thread return value pointer.
            
            unsigned int m_scalesNumber; //!< The number of sub-samples scales to generate (minimum:3).
            
            unsigned int m_octavesNumber; //!< The number of octaves to generate (minimum:1).
            
            std::vector< InterestPointsSetT >* m_interestPointsSubSectorsPtr; //!< A pointer to a valid interest points container (one element by subsector)..
            
            unsigned int m_maxInterestPointsBySubSector; //!< The maximum number of interest points by sub-sector.
            
            unsigned int m_tiePointsSubSectorsSplitFactor; //!< The number of sectors along each direction.
            
            FloatsMatrix const* m_integralRasterDataPtr; //!< The integral image raster data.
            
            UCharsMatrix const* m_maskRasterDataPtr; //!< The loaded mask raster data pointer (or zero if no mask is avaliable).
            
            boost::mutex* m_rastaDataAccessMutexPtr; //!< A pointer to a valid mutex to controle raster data access.
            
            boost::mutex* m_interestPointsAccessMutexPtr; //!< A pointer to a valid mutex to control the output interest points container access.
            
            unsigned int m_processingBlocksNumber; //!< The raster data will be splitted into this number of blocks for processing.
            
            unsigned int* m_nextRasterLinesBlockToProcessValuePtr; //!< A pointer to a valid counter to control the blocks processing sequence.
            
            SurfLocatorThreadParams() {};
            
            ~SurfLocatorThreadParams() {};
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
            
            te::gm::GeometricTransformation const * m_raster1ToRaster2TransfPtr; //!< A pointer to a transformation direct mapping raster 1 indexed coords into raster 2 indexed coords, of an empty pointer if there is no transformation avaliable.
            
            double m_searchOptTreeSearchRadius; //!< Optimization tree search radius (pixels).
            
            ExecuteMatchingByEuclideanDistThreadEntryParams() {};
            
            ~ExecuteMatchingByEuclideanDistThreadEntryParams() {};
        };         
        
        bool m_isInitialized; //!< true if this instance is initialized.
        
        te::rp::TiePointsLocatorInputParameters m_inputParameters; //!< Input parameters
        
        TiePointsLocatorSURFStrategy();
            
        //overload
        bool initialize( 
          const te::rp::TiePointsLocatorInputParameters& inputParameters );
          
        //overload
        void reset();
        
        //overload
        bool getMatchedInterestPoints( 
          te::gm::GeometricTransformation const * const raster1ToRaster2TransfPtr,
          const double raster1ToRaster2TransfDMapError,
          MatchedInterestPointsSetT& matchedInterestPoints );
        
        //overload
        unsigned int getAutoMaxTiePointsNumber() const;        
        
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
          \brief SURF interest points locator.
          
          \param maxInterestPoints The maximum number of interest points to find.
          
          \param integralRasterData Integral image raster data.
          
          \param maskRasterDataPtr The loaded mask raster data pointer (or zero if no mask is avaliable).
          
          \param interestPoints The found interest points (coords related to rasterData lines/cols).          

          \return true if ok, false on errors.
          
          \note InterestPointT::m_feature1 will be sum of differences between the hessian matrix determinant each pixel and its neighborhoods (always a positive value).
          
          \note InterestPointT::m_feature2 will be used filter width (pixels).
          
          \note InterestPointT::m_feature3 will 1 if the laplacian sign is positive, or zero if negative.
        */             
        bool locateSurfInterestPoints( 
          const unsigned int maxInterestPoints,
          const FloatsMatrix& integralRasterData,
          UCharsMatrix const* maskRasterDataPtr,
          InterestPointsSetT& interestPoints ) const;   
          
        /*! 
          \brief Surf locator thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void locateSurfInterestPointsThreadEntry(SurfLocatorThreadParams* paramsPtr);          
        
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
          \brief Match each feature using eucliean distance.
          
          \param featuresSet1 Features set 1.
          
          \param featuresSet2 Features set 2.
          
          \param interestPointsSet1 The interest pionts set 1 (full raster 1 indexed coods reference)..
          
          \param interestPointsSet2 The interest pionts set 2 (full raster 1 indexed coods reference)..
          
          \param raster1ToRaster2TransfPtr A pointer to a transformation direct mapping raster 1 indexed coords into raster 2 indexed coords, of an empty pointer if there is no transformation avaliable.
          
          \param raster1ToRaster2TransfDMapError The expected transformation error.
          
          \param matchedPoints The matched points.
          
          \note Each matched point feature value ( MatchedInterestPoint::m_feature ) will be set to the distance value between both features.
        */          
        bool executeMatchingByEuclideanDist( 
          const FloatsMatrix& featuresSet1,
          const FloatsMatrix& featuresSet2,
          const InterestPointsSetT& interestPointsSet1,
          const InterestPointsSetT& interestPointsSet2,
          te::gm::GeometricTransformation const * const raster1ToRaster2TransfPtr,
          const double raster1ToRaster2TransfDMapError,
          MatchedInterestPointsSetT& matchedPoints ) const;           
          
        /*! 
          \brief Correlation/Euclidean match thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void executeMatchingByEuclideanDistThreadEntry(
          ExecuteMatchingByEuclideanDistThreadEntryParams* paramsPtr);    
        
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
    
    /*!
      \class TiePointsLocatorSURFStrategyFactory
      \brief SURF tie-points locator strategy factory.
      \note Factory key: SURF
     */
    class TERPEXPORT TiePointsLocatorSURFStrategyFactory : public 
      te::rp::TiePointsLocatorStrategyFactory
    {
      public:
        
        TiePointsLocatorSURFStrategyFactory();
        
        ~TiePointsLocatorSURFStrategyFactory();
   
        //overload
        te::rp::TiePointsLocatorStrategy* build();
        
    };      

  } // end namespace rp
}   // end namespace te

#endif

