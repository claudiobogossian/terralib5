/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "../common/PlatformUtils.h"
#include "GTFactory.h"
#include "GTFilter.h"
#include "MultiPoint.h"
#include "Point.h"
#include "Surface.h"

// STL
#include <cmath>
#include <cstdlib>
#include <limits>

// boost
#include <boost/math/special_functions/binomial.hpp>
#include <boost/random.hpp>

te::gm::GTFilter::~GTFilter()
{
}

te::gm::GTFilter::GTFilter()
{
}

bool te::gm::GTFilter::applyRansac(const std::string& transfName,
                                   const GTParameters& inputParams,
                                   const double expectedDirectMapRmse,
                                   const double expectedInverseMapRmse,
                                   const RansacItCounterT& maxIterations,
                                   const double& assurance,
                                   const bool enableMultiThread,
                                   const std::vector<double>& tiePointsWeights,
                                   std::vector< te::gm::GTParameters::TiePoint >& outTiePoints,
                                   std::auto_ptr< GeometricTransformation >& outTransf
                                   )
{
  if(expectedDirectMapRmse < 0)
    return false;

  if(expectedInverseMapRmse < 0)
    return false;

  if((assurance < 0.0) || (assurance > 1.0))
    return false;
  
  outTiePoints.clear();
  outTransf.reset();  

  // generating the tie-points accumulated probabilities map
  // with positive values between 0 and 1
  std::map< double, GTParameters::TiePoint > tpsMap;
  {
    const unsigned int inputTPNmb = (unsigned int)
      inputParams.m_tiePoints.size();
    double tiePointsWeightsSum = 0;
      
    if( tiePointsWeights.size() > 0 )
    {
      if( tiePointsWeights.size() != inputParams.m_tiePoints.size () )
        return false;        
      
      // finding normalization factors

      unsigned int tpIdx = 0;

      for( tpIdx = 0 ; tpIdx < inputTPNmb ; ++tpIdx )
      {
        if( tiePointsWeights[ tpIdx ] <= 0.0 ) return false;
        
        tiePointsWeightsSum += tiePointsWeights[ tpIdx ];
      }

      if( tiePointsWeightsSum <= 0.0 ) return false;

      // map fill 
      
      double newWSum = 0.0;
      double newW = 0.0;
      
      for( tpIdx = 0 ; tpIdx < inputTPNmb ; ++tpIdx )
      {
        newW = tiePointsWeights[ tpIdx ];
        newW /= tiePointsWeightsSum;

        newWSum += newW;

        tpsMap[ newWSum ] = inputParams.m_tiePoints[ tpIdx ];
      }
    }
    else
    {
      double wSum = 0;
      const double increment = 1.0 / 
        ( (double)inputParams.m_tiePoints.size() );
      
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
  double bestParamsDRMSE = expectedDirectMapRmse;
  double bestParamsIRMSE = expectedInverseMapRmse;
  double bestParamsConvexHullArea = -1.0;
  bool returnValue = true;
  bool keepRunningFlag = true;
  
  ApplyRansacThreadEntryThreadParams baseThreadParams;
  baseThreadParams.m_inputGTParamsPtr = &inputParams;
  baseThreadParams.m_expectedDirectMapRmse = expectedDirectMapRmse;
  baseThreadParams.m_expectedInverseMapRmse = expectedInverseMapRmse;
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
  baseThreadParams.m_maxIterations = maxIterations;
  baseThreadParams.m_maxIterationsDivFactor = enableMultiThread ?
    ((RansacItCounterT)procsNumber) : 1;
  baseThreadParams.m_bestTiePoinsPtr = &outTiePoints;

  // Calling the ransac thread entry

  if( enableMultiThread )
  {
    std::vector< ApplyRansacThreadEntryThreadParams > threadsParameters;
    threadsParameters.resize( procsNumber, baseThreadParams );
    boost::thread_group threads;
    
    for( unsigned int threadIdx = 0 ; threadIdx < procsNumber ;
      ++threadIdx )
    {
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

void te::gm::GTFilter::applyRansacThreadEntry(te::gm::GTFilter::ApplyRansacThreadEntryThreadParams* paramsPtr)
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
  
  // initializing the random number generator to assure that each thread has
  // a different seed
  boost::random::mt19937 generator( (boost::random::mt19937::result_type)time(0) );
  
  paramsPtr->m_mutexPtr->unlock();
  
  // external globals
  
  const RansacItCounterT& maxIterationsDivFactor = paramsPtr->m_maxIterationsDivFactor;
  const double& expectedDirectMapRmse = paramsPtr->m_expectedDirectMapRmse;
  const double& expectedInverseMapRmse = paramsPtr->m_expectedInverseMapRmse;
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
  double bestParamsDRMSE = paramsPtr->m_expectedDirectMapRmse;
  double bestParamsIRMSE = paramsPtr->m_expectedInverseMapRmse;
  double bestParamsConvexHullArea = -1.0;
  std::vector< te::gm::GTParameters::TiePoint > bestTiePoins;
  
  // variables used by the ransac loop
  
  boost::random::uniform_01< double > distribution; 

  const bool useDynamicMaxIterations = ( paramsPtr->m_maxIterations == 0 );
  const RansacItCounterT fixedMaxIterations = useDynamicMaxIterations ? 
    ( 
      (RansacItCounterT)boost::math::binomial_coefficient< long double >(
        (unsigned int)tiePoints.size(), 
        bestTransfPtr->getMinRequiredTiePoints() )
    ) / maxIterationsDivFactor
    :
    ( paramsPtr->m_maxIterations / maxIterationsDivFactor );
  const RansacItCounterT fixedMaxConsecutiveInvalidIterations = 
    std::max( 
      (RansacItCounterT)1, 
      (
        ( assurance > 0.0 )
        ?
        (
          (RansacItCounterT)( 
            ((long double)fixedMaxIterations) 
            * 
            ((long double)assurance) 
          )
        )
        :
        ( fixedMaxIterations / 2 )
      )
    );
    
  GTParameters consensusSetParams;  
  std::vector< te::gm::GTParameters::TiePoint > consensusSetTiePoints;
  consensusSetParams.m_tiePoints.reserve( tiePoints.size() );
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
  RansacItCounterT currentIteration = 0;
  const long double iterationsFactor = ( assurance != 0.0 ) ?
    std::log( (long double)( 1.0 - assurance ) )
    :
    std::log( 
      (long double) 
      ( 
        ((long double)1.0)  
        - 
        ( 
          std::pow( (long double)0.5, ( (long double)reqTPsNmb ) )
        )
      )
    );
  RansacItCounterT dynamicMaxIterations = fixedMaxIterations;
  RansacItCounterT dynamicMaxConsecutiveInvalidIterations = fixedMaxConsecutiveInvalidIterations;
  RansacItCounterT dynamicMaxIterationsEstimation = fixedMaxIterations;
  RansacItCounterT consecutiveInvalidIterations = 0;
  double randomValue = 0;
    
  while( keepRunningFlag && ( currentIteration < dynamicMaxIterations ) && 
    ( consecutiveInvalidIterations < dynamicMaxConsecutiveInvalidIterations ) )
  {
    // Trying to find a valid base consensus set
    // with the minimum number of required tie-points
    // Random selecting n distinc tpoints from the original set

    consensusSetParams.reset();;
    selectedTpsCounter = 0;

    while( selectedTpsCounter < reqTPsNmb )
    {
      randomValue = distribution( generator  );
      tpsMapIt = tpsMap.upper_bound( randomValue );
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
      // finding those tie-points in agreement with the generated
      // consensus basic transformation

      consensusSetTiePoints.clear();
      consensusSetDRMSE = 0;
      consensusSetIRMSE = 0;

      for( inputTpIdx = 0 ; inputTpIdx < inputTPNmb ; ++inputTpIdx )
      {
        const GTParameters::TiePoint& curTP = tiePoints[ inputTpIdx ];
        
        tiePointDMapErr = bestTransfPtr->getDirectMappingError( curTP, consensusSetParams );
        tiePointIMapErr = bestTransfPtr->getInverseMappingError( curTP, consensusSetParams );

        if( ( tiePointDMapErr <= expectedDirectMapRmse ) && 
          ( tiePointIMapErr <= expectedInverseMapRmse ) )
        {
          consensusSetTiePoints.push_back( curTP );
          consensusSetDRMSE += ( tiePointDMapErr * tiePointDMapErr );
          consensusSetIRMSE += ( tiePointIMapErr * tiePointIMapErr );
        }
      }
      
      consensusSetSize = (unsigned int)consensusSetTiePoints.size();
      consensusSetDRMSE = sqrt( consensusSetDRMSE / ((double)consensusSetSize) );
      consensusSetIRMSE = sqrt( consensusSetIRMSE / ((double)consensusSetSize) );
      consensusSetConvexHullArea = getPt1ConvexHullArea( 
        consensusSetTiePoints );
      
      /* Is this an acceptable consensus set ?? */
      
      if( 
          ( consensusSetSize >= reqTPsNmb ) 
          &&
          ( consensusSetDRMSE <= expectedDirectMapRmse ) 
          &&
          ( consensusSetIRMSE <= expectedInverseMapRmse ) 
          &&
          (
            ( consensusSetSize > bestTiePoins.size() )
            ||
            (
              ( consensusSetSize == bestTiePoins.size() )
              &&
              (
                ( consensusSetConvexHullArea > bestParamsConvexHullArea )
                ||
                (
                  ( consensusSetConvexHullArea == bestParamsConvexHullArea )
                  &&
                  (
                    ( bestParamsDRMSE > consensusSetDRMSE )
                    &&
                    ( bestParamsIRMSE > consensusSetIRMSE )
                  )
                )
              )
            )
          )
        )
      {
        bestParams = consensusSetParams;
        bestParamsDRMSE = consensusSetDRMSE;
        bestParamsIRMSE = consensusSetIRMSE;
        bestParamsConvexHullArea = consensusSetConvexHullArea;
        bestTiePoins = consensusSetTiePoints;
        
        consecutiveInvalidIterations = 0;
      }
      else
      {
        ++consecutiveInvalidIterations;
      }
    }
    else
    {
      ++consecutiveInvalidIterations;
    };
    
    // Updating iteration related variables
    
    if( useDynamicMaxIterations && ( currentIteration != 0 ) )
    {
      if( bestTiePoins.size() == inputTPNmb )
      {
        dynamicMaxIterations = 0;
      }
      else if( ! bestTiePoins.empty() )
      {
        dynamicMaxIterationsEstimation =
          (
            (
              (RansacItCounterT)(
                iterationsFactor
                / 
                std::log( 
                  ((long double)1.0) 
                  - 
                  std::pow(
                    (  
                      ((long double)bestTiePoins.size() )
                      /
                      ((long double)inputTPNmb)
                    )
                    ,
                    ((long double)reqTPsNmb)
                  )
                )
              )
            )
            /
            maxIterationsDivFactor
          );
          
        dynamicMaxIterationsEstimation = std::min( dynamicMaxIterationsEstimation,
          fixedMaxIterations );                                                   

        dynamicMaxIterations = ( dynamicMaxIterationsEstimation > dynamicMaxIterations ) ?
          ( dynamicMaxIterations + ( ( dynamicMaxIterationsEstimation - dynamicMaxIterations ) / 2 ) )
          :
          ( dynamicMaxIterations - ( ( dynamicMaxIterations - dynamicMaxIterationsEstimation ) / 2 ) );
          
        dynamicMaxConsecutiveInvalidIterations =
          std::max( 
            (RansacItCounterT)1, 
            (
              ( assurance > 0.0 )
              ?
              (
                (RansacItCounterT)( 
                  ((long double)dynamicMaxIterations) 
                  * 
                  ((long double)assurance) 
                )
              )
              :
              ( dynamicMaxIterations / 2 )
            )
          );        
      }
    }
    
    ++currentIteration;
  }
  
  // Comparing with the global best transformation
    
  if( bestTransfPtr->initialize( bestParams ) )
  {
    bestParamsDRMSE = bestTransfPtr->getDirectMapRMSE( bestTiePoins );
    bestParamsIRMSE = bestTransfPtr->getInverseMapRMSE( bestTiePoins );
    
    paramsPtr->m_mutexPtr->lock();
    
    if(
        ( paramsPtr->m_bestTransformationPtrPtr->get() == 0 )
        ||
        (
          ( bestTiePoins.size() >
            (*paramsPtr->m_bestTransformationPtrPtr)->getParameters().m_tiePoints.size() )
          ||
          (
            ( bestTiePoins.size() ==
              (*paramsPtr->m_bestTransformationPtrPtr)->getParameters().m_tiePoints.size() )
            &&
            (
              ( bestParamsConvexHullArea > (*paramsPtr->m_bestParamsConvexHullAreaPtr) )
              ||
              (
                ( bestParamsConvexHullArea == (*paramsPtr->m_bestParamsConvexHullAreaPtr) )
                &&
                (
                  ( bestParamsDRMSE < (*paramsPtr->m_bestParamsDRMSEPtr) )
                  &&
                  ( bestParamsIRMSE < (*paramsPtr->m_bestParamsIRMSEPtr) )
                )
              )
            )
          )
        )
      )
    {
      (*paramsPtr->m_bestTransformationPtrPtr).reset( 
        bestTransfPtr.release() );
      (*paramsPtr->m_bestParamsDRMSEPtr) = bestParamsDRMSE;
      (*paramsPtr->m_bestParamsIRMSEPtr) = bestParamsIRMSE;
      (*paramsPtr->m_bestParamsConvexHullAreaPtr) = bestParamsConvexHullArea;
      (*paramsPtr->m_bestTiePoinsPtr) = bestTiePoins;
    }
      
    paramsPtr->m_mutexPtr->unlock();
  }
}

double te::gm::GTFilter::getPt1ConvexHullArea(const std::vector< GTParameters::TiePoint >& tiePoints)
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
