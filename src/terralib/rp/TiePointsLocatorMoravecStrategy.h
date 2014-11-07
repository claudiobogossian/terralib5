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
  \file terralib/rp/TiePointsLocatorMoravecStrategy.h
  \brief Tie-Pointsr locator Moravec strategy.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORMORAVECSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORMORAVECSTRATEGY_H

#include "TiePointsLocatorStrategy.h"
#include "../geometry/GeometricTransformation.h"

#include <boost/thread.hpp>

namespace te
{
  namespace rp
  {
    // Forwards
    class TiePointsLocator;
    
    /*!
      \class TiePointsLocatorMoravecStrategy
      \brief Tie-points locator Moravec strategy.
     */
    class TERPEXPORT TiePointsLocatorMoravecStrategy : public TiePointsLocatorStrategy
    {
      friend class TiePointsLocator;
      
      public:
        
        ~TiePointsLocatorMoravecStrategy();
        
      protected :
        
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
            
            te::gm::GeometricTransformation const * m_raster1ToRaster2TransfPtr; //!< A pointer to a transformation direct mapping raster 1 indexed coords into raster 2 indexed coords, of an empty pointer if there is no transformation avaliable.
            
            double m_searchOptTreeSearchRadius; //!< Optimization tree search radius (pixels).
            
            ExecuteMatchingByCorrelationThreadEntryParams() {};
            
            ~ExecuteMatchingByCorrelationThreadEntryParams() {};
        };         
        
        bool m_isInitialized; //!< true if this instance is initialized.
        
        te::rp::TiePointsLocatorInputParameters m_inputParameters; //!< Input parameters
        
        TiePointsLocatorMoravecStrategy();
            
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
          \brief Movavec locator thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void locateMoravecInterestPointsThreadEntry(MoravecLocatorThreadParams* paramsPtr);        
        
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
          \brief Match each feature using correlation.
          
          \param featuresSet1 Features set 1.
          
          \param featuresSet2 Features set 2.
          
          \param interestPointsSet1 The interest pionts set 1 (full raster 1 indexed coods reference).
          
          \param interestPointsSet2 The interest pionts set 2 (full raster 1 indexed coods reference).
          
          \param raster1ToRaster2TransfPtr A pointer to a transformation direct mapping raster 1 indexed coords into raster 2 indexed coords, of an empty pointer if there is no transformation avaliable.
          
          \param raster1ToRaster2TransfDMapError The expected transformation error.
          
          \param matchedPoints The matched points (full raster 1 indexed coods reference).
          
          \note Each matched point feature value ( MatchedInterestPoint::m_feature ) will be set to the absolute value of the correlation between then.
        */          
        bool executeMatchingByCorrelation( 
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
        static void executeMatchingByCorrelationThreadEntry(
          ExecuteMatchingByCorrelationThreadEntryParams* paramsPtr);        
    };

  } // end namespace rp
}   // end namespace te

#endif

