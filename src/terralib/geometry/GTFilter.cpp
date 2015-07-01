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
#include <memory>

// boost
#include <boost/math/special_functions/binomial.hpp>
#include <boost/random.hpp>

#define RANSACGETMAXINVALIDITERATIONS( assurance, dynamicMaxIterations ) \
  std::max( \
    (RansacIntegerCounterT)1 \
    , \
    ( \
      (RansacIntegerCounterT)( \
        ((RansacFloatCounterT)dynamicMaxIterations) \
        * \
        ((RansacFloatCounterT)assurance) \
      ) \
    ) \
  ) \
  
#define RANSACGETMAXITERATIONS( goodTPNumber, totalTPNumber, modelRequiredTPNumber, procsNumber, assurance ) \
  ( \
    ( \
      (RansacIntegerCounterT) \
      ( \
        std::log( \
          (RansacFloatCounterT)( 1.0 - ((RansacFloatCounterT)assurance) ) \
        ) \
        / \
        std::log( \
          ((RansacFloatCounterT)1.0) \
          - \
          std::pow( \
            ( \
              ((RansacFloatCounterT)goodTPNumber ) \
              / \
              ((RansacFloatCounterT)totalTPNumber) \
            ) \
            , \
            ((RansacFloatCounterT)modelRequiredTPNumber) \
          ) \
        ) \
      ) \
    ) \
    / \
    ((RansacIntegerCounterT)procsNumber) \
  ) \
  
  #define RANSACSYNCTHREAD \
    { \
      mutex.lock(); \
      if( bestTransfPtr->initialize( bestParams ) ) \
      { \
        if( \
            ( paramsPtr->m_bestTransformationPtrPtr->get() == 0 ) \
            || \
            ( \
              ( bestTiePoins.size() > \
                (*paramsPtr->m_bestTransformationPtrPtr)->getParameters().m_tiePoints.size() ) \
              || \
              ( \
                ( bestTiePoins.size() == \
                  (*paramsPtr->m_bestTransformationPtrPtr)->getParameters().m_tiePoints.size() ) \
                && \
                ( \
                  ( bestParamsConvexHullArea > (*paramsPtr->m_bestParamsConvexHullAreaPtr) ) \
                  || \
                  ( \
                    ( bestParamsConvexHullArea == (*paramsPtr->m_bestParamsConvexHullAreaPtr) ) \
                    && \
                    ( \
                      ( bestParamsMaxDMapErrorPtr < (*paramsPtr->m_bestParamsMaxDMapErrorPtr) ) \
                      && \
                      ( bestParamsMaxIMapErrorPtr < (*paramsPtr->m_bestParamsMaxIMapErrorPtr) ) \
                    ) \
                  ) \
                ) \
              ) \
            ) \
          ) \
        { \
          (*paramsPtr->m_bestTransformationPtrPtr).reset(  \
            bestTransfPtr->clone() ); \
          (*paramsPtr->m_bestParamsMaxDMapErrorPtr) = bestParamsMaxDMapErrorPtr; \
          (*paramsPtr->m_bestParamsMaxIMapErrorPtr) = bestParamsMaxIMapErrorPtr; \
          (*paramsPtr->m_bestParamsConvexHullAreaPtr) = bestParamsConvexHullArea; \
          (*paramsPtr->m_bestTiePoinsPtr) = bestTiePoins; \
        } \
      } \
      if( dynamicMaxIterations < globalDynamicMaxIterations ) \
        globalDynamicMaxIterations = dynamicMaxIterations; \
      mutex.unlock();  \
    }; 

// ---------------------------------------------------------------------------

te::gm::GTFilter::ApplyRansacThreadEntryThreadParams::ApplyRansacThreadEntryThreadParams()
{
  m_transfNamePtr = 0;
  m_inputGTParamsPtr = 0;
  m_maxDirectMapError = 0.0;
  m_maxInverseMapError = 0.0;
  m_assurance = 0.0;
  m_useDynamicIterationsNumber = 0;
  m_dynamicMaxIterationsPtr = 0;
  m_procsNumber = 1;
  m_returnValuePtr = 0;
  m_mutexPtr = 0;
  m_keepRunningFlagPtr = 0;
  m_tpsMapPtr = 0;
  m_bestTransformationPtrPtr = 0;
  m_bestParamsMaxDMapErrorPtr = 0;
  m_bestParamsMaxIMapErrorPtr = 0;
  m_bestParamsConvexHullAreaPtr = 0;
  m_bestTiePoinsPtr = 0;
}

te::gm::GTFilter::ApplyRansacThreadEntryThreadParams::ApplyRansacThreadEntryThreadParams(
  const te::gm::GTFilter::ApplyRansacThreadEntryThreadParams& other )
{
  operator=( other );
}

te::gm::GTFilter::ApplyRansacThreadEntryThreadParams::~ApplyRansacThreadEntryThreadParams()
{
}

const te::gm::GTFilter::ApplyRansacThreadEntryThreadParams& 
  te::gm::GTFilter::ApplyRansacThreadEntryThreadParams::operator=(
  const te::gm::GTFilter::ApplyRansacThreadEntryThreadParams& other )
{
  m_transfNamePtr = other.m_transfNamePtr;
  m_inputGTParamsPtr = other.m_inputGTParamsPtr;
  m_maxDirectMapError = other.m_maxDirectMapError;
  m_maxInverseMapError = other.m_maxInverseMapError;
  m_assurance = other.m_assurance;
  m_useDynamicIterationsNumber = other.m_useDynamicIterationsNumber;
  m_dynamicMaxIterationsPtr = other.m_dynamicMaxIterationsPtr;
  m_procsNumber = other.m_procsNumber;
  m_returnValuePtr = other.m_returnValuePtr;
  m_mutexPtr = other.m_mutexPtr;
  m_keepRunningFlagPtr = other.m_keepRunningFlagPtr;
  m_tpsMapPtr = other.m_tpsMapPtr;
  m_bestTransformationPtrPtr = other.m_bestTransformationPtrPtr;
  m_bestParamsMaxDMapErrorPtr = other.m_bestParamsMaxDMapErrorPtr;
  m_bestParamsMaxIMapErrorPtr = other.m_bestParamsMaxIMapErrorPtr;
  m_bestParamsConvexHullAreaPtr = other.m_bestParamsConvexHullAreaPtr;
  m_bestTiePoinsPtr = other.m_bestTiePoinsPtr;
  
  return other;
}

// ---------------------------------------------------------------------------

te::gm::GTFilter::~GTFilter()
{
}

te::gm::GTFilter::GTFilter()
{
}

bool te::gm::GTFilter::applyRansac(const std::string& transfName,
                                   const GTParameters& inputParams,
                                   const double maxDirectMapError,
                                   const double maxInverseMapError,
                                   const RansacIntegerCounterT& maxIterations,
                                   const double& assurance,
                                   const bool enableMultiThread,
                                   const std::vector<double>& tiePointsWeights,
                                   std::vector< te::gm::GTParameters::TiePoint >& outTiePoints,
                                   std::auto_ptr< GeometricTransformation >& outTransf
                                   )
{
  if(maxDirectMapError < 0)
    return false;

  if(maxInverseMapError < 0)
    return false;

  if((assurance <= 0.0) || (assurance > 1.0))
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
        if( tiePointsWeights[ tpIdx ] < 0.0 ) return false;
        
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
  double bestParamsMaxDMapError = maxDirectMapError;
  double bestParamsMaxIMapError = maxInverseMapError;
  double bestParamsConvexHullArea = -1.0;
  bool returnValue = true;
  bool keepRunningFlag = true;
  RansacIntegerCounterT dynamicMaxIterations = 
    maxIterations ?
      ( 
        maxIterations 
        / 
        ( 
          enableMultiThread ? procsNumber : 1 
        ) 
      )
    :
      RANSACGETMAXITERATIONS( 
        bestTransformationPtr->getMinRequiredTiePoints(),
        inputParams.m_tiePoints.size(),
        bestTransformationPtr->getMinRequiredTiePoints(),
        enableMultiThread ? procsNumber : 1,
        assurance );                                                                     
  
  ApplyRansacThreadEntryThreadParams baseThreadParams;
  baseThreadParams.m_transfNamePtr = &transfName;
  baseThreadParams.m_inputGTParamsPtr = &inputParams;
  baseThreadParams.m_maxDirectMapError = maxDirectMapError;
  baseThreadParams.m_maxInverseMapError = maxInverseMapError;
  baseThreadParams.m_assurance = assurance;
  baseThreadParams.m_useDynamicIterationsNumber = ( maxIterations == 0 );
  baseThreadParams.m_dynamicMaxIterationsPtr = &dynamicMaxIterations;
  baseThreadParams.m_procsNumber = enableMultiThread ?
    ((RansacIntegerCounterT)procsNumber) : 1;
  baseThreadParams.m_returnValuePtr = &returnValue;
  baseThreadParams.m_mutexPtr = &syncMutex;
  baseThreadParams.m_keepRunningFlagPtr = &keepRunningFlag;
  baseThreadParams.m_tpsMapPtr = &tpsMap;
  baseThreadParams.m_bestTransformationPtrPtr = &bestTransformationPtr;
  baseThreadParams.m_bestParamsMaxDMapErrorPtr = &bestParamsMaxDMapError;
  baseThreadParams.m_bestParamsMaxIMapErrorPtr = &bestParamsMaxIMapError;
  baseThreadParams.m_bestParamsConvexHullAreaPtr = &bestParamsConvexHullArea;
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
  assert( paramsPtr->m_procsNumber > 0 );
  
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
  
  const RansacIntegerCounterT& procsNumber = paramsPtr->m_procsNumber;
  const double& maxDirectMapError = paramsPtr->m_maxDirectMapError;
  const double& maxInverseMapError = paramsPtr->m_maxInverseMapError;
  bool& keepRunningFlag = (*(paramsPtr->m_keepRunningFlagPtr));    
  const double& assurance = paramsPtr->m_assurance;
  const bool& useDynamicIterationsNumber = paramsPtr->m_useDynamicIterationsNumber;
  RansacIntegerCounterT& globalDynamicMaxIterations = (*(paramsPtr->m_dynamicMaxIterationsPtr));
  boost::mutex& mutex = (*(paramsPtr->m_mutexPtr));
  
  // Checking the number of required tie-points
    
  const unsigned int reqTPsNmb = bestTransfPtr->getMinRequiredTiePoints();
  const unsigned int inputTPNmb = (unsigned int)tiePoints.size();
  
  if( inputTPNmb < reqTPsNmb )
  {
    mutex.lock();
    *(paramsPtr->m_returnValuePtr) = false;
    *(paramsPtr->m_keepRunningFlagPtr) = false;
    mutex.unlock();
    return;
  }  
  
  // best parameters found by this thread

  GTParameters bestParams;
  double bestParamsMaxDMapErrorPtr = paramsPtr->m_maxDirectMapError;
  double bestParamsMaxIMapErrorPtr = paramsPtr->m_maxInverseMapError;
  double bestParamsConvexHullArea = -1.0;
  std::vector< te::gm::GTParameters::TiePoint > bestTiePoins;
  
  // variables used by the ransac loop
  
  boost::random::uniform_01< double > distribution; 
  
  GTParameters consensusSetParams;  
  std::vector< te::gm::GTParameters::TiePoint > consensusSetTiePoints;
  consensusSetParams.m_tiePoints.reserve( tiePoints.size() );
  double consensusSetMaxDMapError = 0;
  double consensusSetMaxIMapError = 0;
  unsigned int consensusSetSize = 0;
  double consensusSetConvexHullArea = 0.0;
  std::vector< Coord2D > consensusSetPt1ConvexHullPoits;
  
  double tiePointDMapErr = 0;
  double tiePointIMapErr = 0;    
  
  std::map< double, GTParameters::TiePoint >::const_iterator tpsMapIt;
  unsigned int inputTpIdx = 0;
    
  RansacIntegerCounterT selectedTpsCounter = 0;
  std::vector< GTParameters::TiePoint* > selectedTpsPtrsVec;
  selectedTpsPtrsVec.resize( reqTPsNmb, 0 );
  unsigned int selectedTpsPtrsVecIdx = 0;
  bool selectedTpsNotSelectedBefore = false;
  RansacIntegerCounterT currentIteration = 0;

  RansacIntegerCounterT dynamicMaxIterations = globalDynamicMaxIterations;
  RansacIntegerCounterT dynamicMaxConsecutiveInvalidIterations =
    RANSACGETMAXINVALIDITERATIONS( assurance, dynamicMaxIterations );
  const RansacIntegerCounterT threadSyncMaxIterations = procsNumber;
  RansacIntegerCounterT threadSyncIteration = 0;

  RansacIntegerCounterT consecutiveInvalidIterations = 0;
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
      consensusSetMaxDMapError = 0;
      consensusSetMaxIMapError = 0;

      for( inputTpIdx = 0 ; inputTpIdx < inputTPNmb ; ++inputTpIdx )
      {
        const GTParameters::TiePoint& curTP = tiePoints[ inputTpIdx ];
        
        tiePointDMapErr = bestTransfPtr->getDirectMappingError( curTP, consensusSetParams );
        tiePointIMapErr = bestTransfPtr->getInverseMappingError( curTP, consensusSetParams );

        if( ( tiePointDMapErr <= maxDirectMapError ) && 
          ( tiePointIMapErr <= maxInverseMapError ) )
        {
          consensusSetTiePoints.push_back( curTP );
          
          if( tiePointDMapErr > consensusSetMaxDMapError ) 
            consensusSetMaxDMapError = tiePointDMapErr;
          if( tiePointIMapErr > consensusSetMaxIMapError ) 
            consensusSetMaxIMapError = tiePointIMapErr;
        }
      }
      
      consensusSetSize = (unsigned int)consensusSetTiePoints.size();
      consensusSetConvexHullArea = getPt1ConvexHullArea( 
        consensusSetTiePoints );
      
      /* Is this an acceptable consensus set ?? */
      
      if( 
          ( consensusSetSize >= reqTPsNmb ) 
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
                    ( bestParamsMaxDMapErrorPtr > consensusSetMaxDMapError )
                    &&
                    ( bestParamsMaxIMapErrorPtr > consensusSetMaxIMapError )
                  )
                )
              )
            )
          )
        )
      {
        bestParams = consensusSetParams;
        bestParamsMaxDMapErrorPtr = consensusSetMaxDMapError;
        bestParamsMaxIMapErrorPtr = consensusSetMaxIMapError;
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
    
    if( useDynamicIterationsNumber && ( currentIteration != 0 ) )
    {
      if( bestTiePoins.size() == inputTPNmb )
      {
        mutex.lock();
        
        keepRunningFlag = false;
        
        mutex.unlock();
        
        break;
      }
      else if( ! bestTiePoins.empty() )
      {
        const RansacIntegerCounterT dynamicMaxIterationsEstimation = RANSACGETMAXITERATIONS(
          bestTiePoins.size(),
          inputTPNmb,
          reqTPsNmb,
          procsNumber,
          assurance );
        
        if( dynamicMaxIterationsEstimation < dynamicMaxIterations )
        {
          dynamicMaxIterations =
            ( dynamicMaxIterations - ( ( dynamicMaxIterations - dynamicMaxIterationsEstimation ) / 2 ) );
            
          dynamicMaxConsecutiveInvalidIterations =
            RANSACGETMAXINVALIDITERATIONS( assurance, dynamicMaxIterations );
        }
        
        if( globalDynamicMaxIterations < dynamicMaxIterations )
        {
          dynamicMaxIterations = globalDynamicMaxIterations;
            
          dynamicMaxConsecutiveInvalidIterations =
            RANSACGETMAXINVALIDITERATIONS( assurance, dynamicMaxIterations );
        } 
      }
    }
    
    // Sync with the other threads
    
    if( 
        ( threadSyncIteration >= threadSyncMaxIterations )
        ||
        ( currentIteration >= dynamicMaxIterations )
        ||
        ( consecutiveInvalidIterations >= dynamicMaxConsecutiveInvalidIterations )
      )
    {
      threadSyncIteration = 0;
      
      RANSACSYNCTHREAD
    }
    
    ++threadSyncIteration;
    ++currentIteration;
  }
  
  // Sync the final result
  
  RANSACSYNCTHREAD
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
    
    std::auto_ptr< te::gm::Geometry > convexHullPtr( points.convexHull() );
    
    if( dynamic_cast< te::gm::Surface* >( convexHullPtr.get() ) )
    {
      return dynamic_cast< te::gm::Surface* >( convexHullPtr.get() )->getArea();
    }
    else
    {
      return 0;
    }
  }
}
