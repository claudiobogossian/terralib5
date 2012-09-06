/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/GTFilter.cpp

  \brief 2D Geometric transformation outliers remotion filter.
*/

// TerraLib
#include "GTFilter.h"
#include "GTFactory.h"
#include "MultiPoint.h"
#include "Point.h"
#include "Surface.h"

#include <limits>
#include <cmath>
#include <cstdlib>

te::gm::GTFilter::~GTFilter()
{
}

te::gm::GTFilter::GTFilter()
{
}

te::gm::GTFilter::GTFilter( const GTFilter& )
{
}

te::gm::GTFilter& te::gm::GTFilter::operator=( const GTFilter& )
{
  return *this;
}

bool te::gm::GTFilter::applyRansac(const std::string& transfName, 
  const GTParameters& inputParams, const double maxDMapError, 
  const double maxIMapError, const double maxDMapRmse, const double maxIMapRmse,
  std::auto_ptr< GeometricTransformation >& outTransf,
  const std::vector< double > tiePointsWeights )
{
  if( maxDMapError < 0 ) return false;
  if( maxIMapError < 0 ) return false;
  if( maxDMapRmse < 0 ) return false;
  if( maxIMapRmse < 0 ) return false;
  
  outTransf.reset( te::gm::GTFactory::make( transfName ) );
  if( outTransf.get() == 0 ) return false;
  
  // initializend the random numbers generator
  
  srand( rand() );
        
  // Checking the number of required tie-points
    
  const unsigned int reqTPsNmb = outTransf->getMinRequiredTiePoints();
  const unsigned int inputTPNmb = (unsigned int)inputParams.m_tiePoints.size();
  
  if( inputTPNmb < reqTPsNmb )
  {
    return false;  
  }
  else
  {
    // generating the tie-points accumulated probabilities map
    // with positive values up to RAND_MAX + 1
    std::map< double, GTParameters::TiePoint > tpsMap;
    {
      if( tiePointsWeights.size() > 0 )
      {
        if( tiePointsWeights.size() != inputParams.m_tiePoints.size () )
          return false;        
        
        // finding normalization factors

        double originalWSum = 0.0;
        unsigned int tpIdx = 0;

        for( tpIdx = 0 ; tpIdx < inputTPNmb ; ++tpIdx )
        {
          if( tiePointsWeights[ tpIdx ] <= 0.0 ) return false;
          
          originalWSum += tiePointsWeights[ tpIdx ];
        }

        if( originalWSum <= 0.0 ) return false;

        // map fill 
        
        double newWSum = 0.0;
        double newW = 0.0;
        
        for( tpIdx = 0 ; tpIdx < inputTPNmb ; ++tpIdx )
        {
          newW = tiePointsWeights[ tpIdx ];
          newW /= originalWSum;
          newW *= ((double)RAND_MAX) + 1.0;

          newWSum += newW;

          tpsMap[ newWSum ] = inputParams.m_tiePoints[ tpIdx ];
        }
      }
      else
      {
        double wSum = 0;
        const double increment = ( ((double)RAND_MAX) + 1.0 ) / 
          ((double)inputParams.m_tiePoints.size());
        
        for( unsigned int tpIdx = 0 ; tpIdx < inputTPNmb ; ++tpIdx )
        {
          wSum += increment;
          tpsMap[ wSum ] = inputParams.m_tiePoints[ tpIdx ];
        }
      }
    }
    
    // The Maximum Number of iterations
    
    const RansacItCounterT maxIterations = std::numeric_limits< RansacItCounterT >::max();
      
    // This is the maximum number of allowed consecutive invalid
    // consensus sets (no transformation generated)
    const RansacItCounterT maxConsecutiveInvalidBaseSets = ( 
      ((RansacItCounterT)inputTPNmb) * 
      ((RansacItCounterT)reqTPsNmb) * ((RansacItCounterT)reqTPsNmb) * 
      ((RansacItCounterT)reqTPsNmb) );
      
    // This is the minimum number of consecutive valid
    // consensus sets (with the same number of tie-points)
    const RansacItCounterT minConsecutiveValidBaseSets = 
      ( ((RansacItCounterT)inputTPNmb) * ((RansacItCounterT)reqTPsNmb) );
      
    // If the difference between consecutive valid sets RMSE drops
    // below this value the process is stoped.
    const double consecValidBaseSetsDRMSEThr = std::min( maxDMapError / 2.0,
      maxDMapRmse );
    const double consecValidBaseSetsIRMSEThr = std::min( maxIMapError / 2.0,
      maxIMapRmse );
    
    GTParameters bestParams;

    double bestParamsDRMSE = maxDMapRmse;
    double bestParamsIRMSE = maxIMapRmse;
    double bestParamsConvexHullArea = -1.0;

    GTParameters consensusSetParams;
    
    double consensusSetMaxDMapErr = 0;
    double consensusSetMaxIMapErr = 0;
    double consensusSetDRMSE = 0;
    double consensusSetIRMSE = 0;
    double consensusSetIRMSEDiff = 0; // diff against bestParamsIRMSE
    double consensusSetDRMSEDiff = 0; // diff against bestParamsDRMSE
    unsigned int consensusSetSize = 0;
    double consensusSetConvexHullArea = 0.0;
    std::vector< Coord2D > consensusSetPt1ConvexHullPoits;
    
    double tiePointDMapErr = 0;
    double tiePointIMapErr = 0;    
    
    std::map< double, GTParameters::TiePoint >::const_iterator tpsMapIt;
    unsigned int inputTpIdx = 0;
      
    RansacItCounterT selectedTpsCounter = 0;
    std::vector< GTParameters::TiePoint* > selectedTpsPtrsVec;
    selectedTpsPtrsVec.resize( reqTPsNmb, 0 );
    unsigned int selectedTpsPtrsVecIdx = 0;
    bool selectedTpsNotSelectedBefore = false;
      
    // RANSAC main loop
    
    RansacItCounterT consecutiveInvalidBaseSetsLeft = 
      maxConsecutiveInvalidBaseSets;  
    RansacItCounterT iterationsLeft = maxIterations;       
    RansacItCounterT consecutiveValidBaseSets = 0;
      
    while( iterationsLeft )
    {
      // Trying to find a valid base consensus set
      // with the minimum number of required tie-points
      // Random selecting n distinc tpoints from the original set

      consensusSetParams.reset();;
      selectedTpsCounter = 0;

      while( selectedTpsCounter < reqTPsNmb )
      {
        tpsMapIt = tpsMap.upper_bound( (double)rand() );
        assert( tpsMapIt != tpsMap.end() );
          
        // Checking if this TP was already selected before
        
        selectedTpsNotSelectedBefore = true;
        
        for( selectedTpsPtrsVecIdx = 0 ; selectedTpsPtrsVecIdx <
          selectedTpsCounter ; ++selectedTpsPtrsVecIdx )
        {
          if( selectedTpsPtrsVec[ selectedTpsPtrsVecIdx ] ==
            &(tpsMapIt->second) )
          {
            selectedTpsNotSelectedBefore = false;
            break;
          }
        }
        
        if( selectedTpsNotSelectedBefore )
        {
          consensusSetParams.m_tiePoints.push_back( tpsMapIt->second );
        }
        
        ++selectedTpsCounter;
      }

      /* Trying to generate a valid base consensus transformation with the 
      selected points */      
      
      if( outTransf->computeParameters( consensusSetParams ) ) 
      {
//        consecutiveInvalidBaseSetsLeft = maxConsecutiveInvalidBaseSets;
        
        // finding those tie-points in agreement with the generated
        // consensus basic transformation

        consensusSetParams.m_tiePoints.clear();
        consensusSetDRMSE = 0;
        consensusSetIRMSE = 0;
        consensusSetMaxDMapErr = 0;
        consensusSetMaxIMapErr = 0;

        for( inputTpIdx = 0 ; inputTpIdx < inputTPNmb ; ++inputTpIdx )
        {
          const GTParameters::TiePoint& curTP = inputParams.m_tiePoints[ inputTpIdx ];
          
          tiePointDMapErr = outTransf->getDirectMappingError( curTP, consensusSetParams );
          tiePointIMapErr = outTransf->getInverseMappingError( curTP, consensusSetParams );

          if( ( tiePointDMapErr < maxDMapError ) && 
            ( tiePointIMapErr < maxIMapError ) )
          {
            consensusSetParams.m_tiePoints.push_back( curTP );
            consensusSetDRMSE += ( tiePointDMapErr * tiePointDMapErr );
            consensusSetIRMSE += ( tiePointIMapErr * tiePointIMapErr );
            if( tiePointDMapErr > consensusSetMaxDMapErr )
              consensusSetMaxDMapErr = tiePointDMapErr;
            if( tiePointIMapErr > consensusSetMaxIMapErr )
              consensusSetMaxIMapErr = tiePointIMapErr;              
          }
        }
        
        consensusSetSize = (unsigned int)consensusSetParams.m_tiePoints.size();
        
        consensusSetDRMSE = sqrt( consensusSetDRMSE );
        consensusSetIRMSE = sqrt( consensusSetIRMSE );
        
        consensusSetIRMSEDiff = std::abs( bestParamsIRMSE - consensusSetIRMSE );
        consensusSetDRMSEDiff = std::abs( bestParamsDRMSE - consensusSetDRMSE );

        /* Is this an acceptable consensus set ?? */      
        
        if( ( consensusSetDRMSE < maxDMapRmse ) &&
            ( consensusSetIRMSE < maxIMapRmse ) &&
            ( consensusSetMaxDMapErr < maxDMapError ) &&
            ( consensusSetMaxIMapErr < maxIMapError ) )
        {
          // Is this consensus set better than the current better one ??
          // (by using the number of tie-points as parameter
          // since we are interested in consensus sets with
          // the higher number of tie-points
        
          if( consensusSetSize > bestParams.m_tiePoints.size() )
          {
            bestParams = consensusSetParams;
            bestParamsDRMSE = consensusSetDRMSE;
            bestParamsIRMSE = consensusSetIRMSE;
            bestParamsConvexHullArea = -1.0;
            
            consecutiveValidBaseSets = 0;
            
            consecutiveInvalidBaseSetsLeft = maxConsecutiveInvalidBaseSets;
          }
          else if( consensusSetSize == bestParams.m_tiePoints.size() )
          {
            // Calculating the best consensus set convexhull area , if needed
            
            if( bestParamsConvexHullArea < 0.0 )
            {
              bestParamsConvexHullArea = getPt1ConvexHullArea( 
                bestParams.m_tiePoints );
            }
            
            // Calculating the convexhull area covered by the tie-points (PT1 space)
            
            consensusSetConvexHullArea = getPt1ConvexHullArea( 
              consensusSetParams.m_tiePoints );
              
            if( consensusSetConvexHullArea > bestParamsConvexHullArea )
            {
              bestParams = consensusSetParams;
              bestParamsDRMSE = consensusSetDRMSE;
              bestParamsIRMSE = consensusSetIRMSE;
              
              ++consecutiveValidBaseSets;
            
              consecutiveInvalidBaseSetsLeft = maxConsecutiveInvalidBaseSets;            
            }
            else
            {
              // This consensus set isn't good enough
              
              --consecutiveInvalidBaseSetsLeft;
            }
          }
          else
          {
            // This consensus set isn't good enough
                        
            --consecutiveInvalidBaseSetsLeft;
          }
        }
        else
        {
          /* This isn't an acceptable consensus */           

          --consecutiveInvalidBaseSetsLeft;
        }
      } 
      else
      {
        // decrement the number of remaining consecutive
        // invalid base sets left
                    
        --consecutiveInvalidBaseSetsLeft;
      }
                        
      if( consecutiveInvalidBaseSetsLeft  == 0 ) break;
      
      if( ( consecutiveValidBaseSets > minConsecutiveValidBaseSets ) &&
        ( consensusSetDRMSEDiff < consecValidBaseSetsDRMSEThr ) &&
        ( consensusSetIRMSEDiff < consecValidBaseSetsIRMSEThr ) )
      {
        break;
      }
      
      --iterationsLeft;
    }
    
    // Returning the smoothed transformation
      
    return outTransf->initialize( bestParams );
  }  
}

double te::gm::GTFilter::getPt1ConvexHullArea( 
  const std::vector< GTParameters::TiePoint >& tiePoints )
{
  if( tiePoints.size() < 3 )
  {
    return 0;
  }
  else
  {
    te::gm::MultiPoint points( 0, te::gm::MultiPointType );
    
    for( unsigned int tiePointsIdx = 0 ; tiePointsIdx < tiePoints.size() ;
      ++tiePointsIdx )
    {
      points.add( new te::gm::Point( tiePoints[ tiePointsIdx ].first.x,
        tiePoints[ tiePointsIdx ].first.y ) );
    }
    
    return ( (te::gm::Surface*)points.convexHull() )->getArea();
  }
}
