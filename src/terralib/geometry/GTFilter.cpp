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
#include "GTFactory.h"
#include "GTFilter.h"
#include "MultiPoint.h"
#include "Point.h"
#include "Surface.h"
#include "../common/PlatformUtils.h"

// STL
#include <cmath>
#include <cstdlib>
#include <limits>

// boost
#include <boost/math/special_functions/binomial.hpp>

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

bool te::gm::GTFilter::applyRansac( const std::string& transfName, 
  const GTParameters& inputParams, const double maxDMapError, 
  const double maxIMapError, const double maxDMapRmse, 
  const double maxIMapRmse, const RansacItCounterT& maxIterations,
  const double& assurance,
  const bool enableMultiThread, std::auto_ptr< GeometricTransformation >& outTransf,
  const std::vector< double >& tiePointsWeights )
{
  if( maxDMapError < 0 ) return false;
  if( maxIMapError < 0 ) return false;
  if( maxDMapRmse < 0 ) return false;
  if( maxIMapRmse < 0 ) return false;
  if( ( assurance <= 0.0 ) || ( assurance >= 1.0 ) ) return false;
  
  // initializing the random numbers generator
  srand( rand() );    
  
  // generating the tie-points accumulated probabilities map
  // with positive values up to RAND_MAX + 1
  std::map< double, GTParameters::TiePoint > tpsMap;
  {
    const unsigned int inputTPNmb = (unsigned int)
      inputParams.m_tiePoints.size();
      
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
  
  // creating global execution parameters
  
  std::auto_ptr< GeometricTransformation > bestTransformationPtr(
    te::gm::GTFactory::make( transfName ) );  
  if( bestTransformationPtr.get() == 0 ) return false;
  
  const unsigned int procsNumber = te::common::GetPhysProcNumber();
  boost::mutex syncMutex;
  double bestParamsDRMSE = maxDMapRmse;
  double bestParamsIRMSE = maxIMapRmse;
  double bestParamsConvexHullArea = -1.0;
  bool returnValue = true;
  bool keepRunningFlag = true;
  
  ApplyRansacThreadEntryThreadParams baseThreadParams;
  baseThreadParams.m_inputGTParamsPtr = &inputParams;
  baseThreadParams.m_maxDMapError = maxDMapError;
  baseThreadParams.m_maxDMapRmse = maxDMapRmse;
  baseThreadParams.m_maxIMapError = maxIMapError;
  baseThreadParams.m_maxIMapRmse = maxIMapRmse;
  baseThreadParams.m_returnValuePtr = &returnValue;
  baseThreadParams.m_transfNamePtr = &transfName;
  baseThreadParams.m_mutexPtr = &syncMutex;
  baseThreadParams.m_keepRunningFlagPtr = &keepRunningFlag;
  baseThreadParams.m_tpsMapPtr = &tpsMap;
  baseThreadParams.m_bestParamsDRMSEPtr = &bestParamsDRMSE;
  baseThreadParams.m_bestParamsIRMSEPtr = &bestParamsIRMSE;
  baseThreadParams.m_assurance = assurance;
  baseThreadParams.m_bestParamsConvexHullAreaPtr = &bestParamsConvexHullArea;
  baseThreadParams.m_bestTransformationPtrPtr = &bestTransformationPtr;
  baseThreadParams.m_randSeed = rand();
  baseThreadParams.m_maxIterations = maxIterations;
  baseThreadParams.m_maxIterationsDivFactor = enableMultiThread ?
    ((RansacItCounterT)procsNumber) : 1;

  // Calling the ransac thread entry

  if( enableMultiThread )
  {
    std::vector< ApplyRansacThreadEntryThreadParams > threadsParameters;
    threadsParameters.resize( procsNumber, baseThreadParams );
    boost::thread_group threads;
    
    for( unsigned int threadIdx = 0 ; threadIdx < procsNumber ;
      ++threadIdx )
    {
      threadsParameters[ threadIdx ].m_randSeed = rand();
      threads.add_thread( new boost::thread( applyRansacThreadEntry, 
        &threadsParameters[ threadIdx ] ) );
    }
    
    threads.join_all();
  }
  else
  {
    applyRansacThreadEntry( &baseThreadParams );
  }
  
  if( returnValue )
  {
    if( bestTransformationPtr.get() )
    {
      outTransf.reset( bestTransformationPtr.release() );
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {    
    return false;
  }
}

void te::gm::GTFilter::applyRansacThreadEntry( 
  te::gm::GTFilter::ApplyRansacThreadEntryThreadParams* paramsPtr )
{
  assert( paramsPtr->m_maxIterationsDivFactor > 0 );
  
  // accessing global shared objects
  
  paramsPtr->m_mutexPtr->lock();
  
  std::auto_ptr< te::gm::GeometricTransformation > bestTransfPtr( 
    te::gm::GTFactory::make( *paramsPtr->m_transfNamePtr ) );
  if( bestTransfPtr.get() == 0 )
  {
    *(paramsPtr->m_returnValuePtr) = false;
    *(paramsPtr->m_keepRunningFlagPtr) = false;
    paramsPtr->m_mutexPtr->unlock();
    return;
  }
  
  const std::vector< te::gm::GTParameters::TiePoint > tiePoints = 
    paramsPtr->m_inputGTParamsPtr->m_tiePoints;
  std::map< double, GTParameters::TiePoint > tpsMap = *(paramsPtr->m_tpsMapPtr);  
  
  paramsPtr->m_mutexPtr->unlock();
  
  // external globals
  
  const RansacItCounterT& maxIterationsDivFactor = paramsPtr->m_maxIterationsDivFactor;
  const double& maxDMapError = paramsPtr->m_maxDMapError;
  const double& maxIMapError = paramsPtr->m_maxIMapError;
  const double& maxDMapRmse = paramsPtr->m_maxDMapRmse;
  const double& maxIMapRmse = paramsPtr->m_maxIMapRmse;  
  bool& keepRunningFlag = (*(paramsPtr->m_keepRunningFlagPtr));    
  const double& assurance = paramsPtr->m_assurance;
  
  // Checking the number of required tie-points
    
  const unsigned int reqTPsNmb = bestTransfPtr->getMinRequiredTiePoints();
  const unsigned int inputTPNmb = (unsigned int)tiePoints.size();
  
  if( inputTPNmb < reqTPsNmb )
  {
    paramsPtr->m_mutexPtr->lock();
    *(paramsPtr->m_returnValuePtr) = false;
    *(paramsPtr->m_keepRunningFlagPtr) = false;
    paramsPtr->m_mutexPtr->unlock();
    return;
  }  
  
  // best parameters found by this thread

  GTParameters bestParams;
  double bestParamsDRMSE = paramsPtr->m_maxDMapRmse;
  double bestParamsIRMSE = paramsPtr->m_maxIMapRmse;
  double bestParamsConvexHullArea = -1.0;
  RansacItCounterT bestParamsGeneratedNumber = 0; // the number of acceptable generated parameters
  
  // variables used by the ransac loop

  const bool dynamicMaxIterations = ( paramsPtr->m_maxIterations == 0 );
  RansacItCounterT maxIterations = paramsPtr->m_maxIterations ? 
    ( paramsPtr->m_maxIterations / maxIterationsDivFactor ) 
    :
    ( 
      (RansacItCounterT)boost::math::binomial_coefficient< long double >(
        (unsigned int)tiePoints.size(), 
        bestTransfPtr->getMinRequiredTiePoints() )
    ) / maxIterationsDivFactor;
    
  GTParameters consensusSetParams;  
  consensusSetParams.m_tiePoints.reserve( tiePoints.size() );
  
  double consensusSetMaxDMapErr = 0;
  double consensusSetMaxIMapErr = 0;
  double consensusSetDRMSE = 0;
  double consensusSetIRMSE = 0;
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
  unsigned int randSeed = 0;
  RansacItCounterT currentIteration = 0;
    
  while( ( currentIteration < maxIterations ) && keepRunningFlag )
  {
    // Trying to find a valid base consensus set
    // with the minimum number of required tie-points
    // Random selecting n distinc tpoints from the original set

    consensusSetParams.reset();;
    selectedTpsCounter = 0;

    while( selectedTpsCounter < reqTPsNmb )
    {
      tpsMapIt = tpsMap.upper_bound( (double)rand_r( &randSeed ) );
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
    
    if( bestTransfPtr->computeParameters( consensusSetParams ) ) 
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
        const GTParameters::TiePoint& curTP = tiePoints[ inputTpIdx ];
        
        tiePointDMapErr = bestTransfPtr->getDirectMappingError( curTP, consensusSetParams );
        tiePointIMapErr = bestTransfPtr->getInverseMappingError( curTP, consensusSetParams );

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
          ++bestParamsGeneratedNumber;
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
            ++bestParamsGeneratedNumber;
          }
        }
      }
    }
    
    // Updating iteration related variables
    
    if( dynamicMaxIterations && ( bestParamsGeneratedNumber > 0 ) )
    {
      maxIterations = 
        (
          (RansacItCounterT)
          ( 
            ( (long double)std::log( 1.0 - assurance )) 
            / 
            (long double)std::log( 
              1.0 
              - 
              (  
                ((long double)bestParams.m_tiePoints.size() )
                /
                ((long double)inputTPNmb)
              ) 
            )
          )
        )
        /
        maxIterationsDivFactor;
    }
    
    ++currentIteration;
  }
  
  // Generating the smoothed transformation
    
  if( bestTransfPtr->initialize( bestParams ) )
  {
    bestParamsDRMSE = bestTransfPtr->getDirectMapRMSE();
    bestParamsIRMSE = bestTransfPtr->getInverseMapRMSE();
    
    paramsPtr->m_mutexPtr->lock();
    
    if( paramsPtr->m_bestTransformationPtrPtr->get() )
    {
      if( bestTransfPtr->getParameters().m_tiePoints.size() ==
        (*paramsPtr->m_bestTransformationPtrPtr)->getParameters().m_tiePoints.size() )
      {
        if( bestParamsConvexHullArea > (*paramsPtr->m_bestParamsConvexHullAreaPtr) )
        {
          (*paramsPtr->m_bestTransformationPtrPtr).reset( 
            bestTransfPtr.release() );
          (*paramsPtr->m_bestParamsDRMSEPtr) = bestParamsDRMSE;
          (*paramsPtr->m_bestParamsIRMSEPtr) = bestParamsIRMSE;
          (*paramsPtr->m_bestParamsConvexHullAreaPtr) = bestParamsConvexHullArea;
        }
      }
      else if( bestTransfPtr->getParameters().m_tiePoints.size() >
        (*paramsPtr->m_bestTransformationPtrPtr)->getParameters().m_tiePoints.size() )
      {
        (*paramsPtr->m_bestTransformationPtrPtr).reset( 
          bestTransfPtr.release() );
        (*paramsPtr->m_bestParamsDRMSEPtr) = bestParamsDRMSE;
        (*paramsPtr->m_bestParamsIRMSEPtr) = bestParamsIRMSE;
        (*paramsPtr->m_bestParamsConvexHullAreaPtr) = bestParamsConvexHullArea;
      }
    }
    else
    { // no previous global best parameters was generated
      (*paramsPtr->m_bestTransformationPtrPtr).reset( 
        bestTransfPtr.release() );
      (*paramsPtr->m_bestParamsDRMSEPtr) = bestParamsDRMSE;
      (*paramsPtr->m_bestParamsIRMSEPtr) = bestParamsIRMSE;
      (*paramsPtr->m_bestParamsConvexHullAreaPtr) = bestParamsConvexHullArea;
    }
      
    paramsPtr->m_mutexPtr->unlock();
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
