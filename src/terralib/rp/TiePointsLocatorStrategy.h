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
  \file terralib/rp/TiePointsLocatorStrategy.h
  \brief Tie-Pointsr locator strategy.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSTRATEGY_H

#include "Config.h"
#include "Matrix.h"
#include "TiePointsLocatorInputParameters.h"
#include "../geometry/GTParameters.h"
#include "../raster/Raster.h"
#include "../raster/Interpolator.h"

#include <vector>
#include <set>
#include <string>
#include <list>
#include <memory>

namespace te
{
  namespace rp
  {
    // Forwards
    class TiePointsLocator;
    
    /*!
      \class TiePointsLocatorStrategy
      \brief Tie-points locator strategy.
     */
    class TERPEXPORT TiePointsLocatorStrategy
    {
      friend class TiePointsLocator;
      
      public:
        
        virtual ~TiePointsLocatorStrategy();
        
        /*!
          \brief Returns a sub-sampled version of the given locator strategy specific input parameters.
          \param subSampleOptimizationRescaleFactor Sub-sampled optimization tie-points search rescale factor.
          \param inputSpecParams Input parameters (sub-sample factor 1).
          \param subSampledSpecParamsPtr Sub-sampled parameters.
         */
        virtual void getSubSampledSpecStrategyParams( 
          const double subSampleOptimizationRescaleFactor,
          const TiePointsLocatorStrategyParameters& inputSpecParams,
          std::auto_ptr< TiePointsLocatorStrategyParameters >& subSampledSpecParamsPtr ) const = 0;
          
        /*!
          \brief Returns a sub-sampled version of the given locator strategy specific input parameters.
          \param subSampleOptimizationRescaleFactor Sub-sampled optimization tie-points search rescale factor.
          \param inputSpecParams Input parameters (sub-sample factor 1).
          \param subSampledSpecParams Sub-sampled parameters.
         */
        virtual void getDefaultSpecStrategyParams( 
          std::auto_ptr< TiePointsLocatorStrategyParameters >& defaultSpecParamsPtr ) const = 0;          
        
      protected :
        
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
            
            InterestPointT& operator=( const InterestPointT& other )
            {
              m_x = other.m_x;
              m_y = other.m_y;
              m_feature1 = other.m_feature1;
              m_feature2 = other.m_feature2;
              m_feature3 = other.m_feature3;
              return *this;
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
          \brief Initialize the strategy.
          \param inputParameters Input parameters.
          \return true if OK, false on errors.
         */
        virtual bool initialize( 
          const te::rp::TiePointsLocatorInputParameters& inputParameters )  = 0;        
          
        /*!
          \brief Clear all internal allocated resources and go back to the initial not-initialized state.
         */
        virtual void reset() = 0;

        /*!
          \brief Returns a automatically calculated optimum maximum amount tie-points following the current parameters.
          \return Returns a automatically calculated optimum maximum amount tie-points following the current parameters.
         */
        virtual unsigned int getAutoMaxTiePointsNumber() const = 0;
        
        /*!
          \brief Try to find matched interest points.
          \param matchedInterestPoints The matched interest points.
          \param raster1ToRaster2TransfPtr A pointer to a transformation (estimation) direct mapping raster 1 indexed coords into raster 2 indexed coords, of an empty pointer if there is no transformation avaliable.
          \param raster1ToRaster2TransfDMapError The expected transformation error.
          \return true if OK, false on errors.
         */        
        virtual bool getMatchedInterestPoints( 
          te::gm::GeometricTransformation const * const raster1ToRaster2TransfPtr,
          const double raster1ToRaster2TransfDMapError,
          MatchedInterestPointsSetT& matchedInterestPoints ) = 0;

        TiePointsLocatorStrategy();
        
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
          
          \param desiredRescaleFactorX The desired Scale factor to be applied on the loaded data.
          
          \param desiredRescaleFactorY The desired Scale factor to be applied on the loaded data.
          
          \param rasterInterpMethod The interpolation used when loading the input raster.
          
          \param maxMemPercentUsage The maximum amount (percent) of memory to use for the loaded data [0,100].
          
          \param loadedRasterData The loaded raster data.
          
          \param loadedMaskRasterData The loaded mask raster data.
          
          \param desiredRescaleFactorX The real achieved Scale factor.
          
          \param desiredRescaleFactorY The real achieved Scale factor.

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
          const double desiredRescaleFactorX,
          const double desiredRescaleFactorY,
          const te::rst::Interpolator::Method rasterInterpMethod,
          const unsigned char maxMemPercentUsage, 
          std::vector< boost::shared_ptr< FloatsMatrix > >& loadedRasterData,
          UCharsMatrix& loadedMaskRasterData,
          double& achievedRescaleFactorX,
          double& achievedRescaleFactorY );
        
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
          \brief Moravec interest points locator.
          
          \param rasterData The loaded raster data.
          
          \param interestPoints The found raster 1 interest points (coords related to rasterData lines/cols).          

          \param tifFileName Tif file name.
        */             
        static void createTifFromMatrix( 
          const FloatsMatrix& rasterData,
          const InterestPointsSetT& interestPoints,
          const std::string& tifFileName );         
        
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
          \brief Check for duplicated interest points.
          \param x The duplicated tie-points X coord.
          \param y The duplicated tie-points Y coord.
          \return false if duplicated interest points were found.
        */             
        static bool checkForDuplicatedInterestPoints( const InterestPointsSetT& interestPoints,
          double& x, double& y );         
        
      private:
        
        TiePointsLocatorStrategy( const TiePointsLocatorStrategy& );
        
        const TiePointsLocatorStrategy& operator=( const TiePointsLocatorStrategy& );
    };

  } // end namespace rp
}   // end namespace te

#endif

