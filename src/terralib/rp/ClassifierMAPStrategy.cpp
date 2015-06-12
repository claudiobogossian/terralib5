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
  \file terralib/rp/ClassifierMAPStrategy.cpp

  \brief Maximum a posteriori probability strategy.
*/

// TerraLib

#include "ClassifierMAPStrategy.h"
#include "Macros.h"
#include "../raster/Band.h"
#include "../common/MatrixUtils.h"

#include <cfloat>

namespace
{
  static te::rp::ClassifierMAPStrategyFactory classifierMAPStrategyFactoryInstance;
}

//-----------------------------------------------------------------------------

te::rp::ClassifierMAPStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::ClassifierMAPStrategy::Parameters::~Parameters()
{
}

const te::rp::ClassifierMAPStrategy::Parameters& te::rp::ClassifierMAPStrategy::Parameters::operator=(const te::rp::ClassifierMAPStrategy::Parameters& rhs)
{
  reset();

  m_trainSamplesPtr = rhs.m_trainSamplesPtr;
  m_prioriProbs = rhs.m_prioriProbs;
  m_prioriCalcSampleStep = rhs.m_prioriCalcSampleStep;

  return *this;
}

void te::rp::ClassifierMAPStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_trainSamplesPtr.reset();
  m_prioriProbs.clear();
  m_prioriCalcSampleStep = 2;
}

te::common::AbstractParameters* te::rp::ClassifierMAPStrategy::Parameters::clone() const
{
  return new te::rp::ClassifierMAPStrategy::Parameters( *this );
}

//-----------------------------------------------------------------------------

te::rp::ClassifierMAPStrategy::ClassifierMAPStrategy()
{
  m_isInitialized = false;
}

te::rp::ClassifierMAPStrategy::~ClassifierMAPStrategy()
{
}

bool te::rp::ClassifierMAPStrategy::initialize(
  te::rp::StrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;
  m_initParams.reset();
  m_classesIndex2ID.clear();
  m_classesMeans.clear();
  m_classesCovarianceMatrixes.clear();
  m_classesCovarianceInvMatrixes.clear();
  m_classesOptizedMAPDiscriminantTerm.clear();  
  
  ClassifierMAPStrategy::Parameters const * const castParamsPtr = 
    dynamic_cast< ClassifierMAPStrategy::Parameters const * >( strategyParams );
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr, "Invalid parameters" );
  
  // Checking the input parameters
  
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr,
    "Invalid classes samples pointer" )
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr->size() > 0,
    "Invalid classes samples number" )
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr->begin()->second.size() > 0,
    "Invalid classes samples number" )    
    
  if( ! castParamsPtr->m_prioriProbs.empty() )
  {
    TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_prioriProbs.size() ==
      castParamsPtr->m_trainSamplesPtr->size(), "Invalid classes priory probabilities" );
    for( std::vector< double >::size_type pIdx = 0 ; pIdx < 
      castParamsPtr->m_prioriProbs.size() ; ++pIdx )
    {
      TERP_TRUE_OR_RETURN_FALSE( 
        ( castParamsPtr->m_prioriProbs[ pIdx ] >= 0.0 ) &&
        ( castParamsPtr->m_prioriProbs[ pIdx ] <= 1.0 ),
        "Invalid classes priory probabilities" );                    
    }
  }
  
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_prioriCalcSampleStep > 0,
    "Invalid sample step" );
  
  m_initParams = (*castParamsPtr);
  
  // Calculating m_classesMeans and m_classesIndex2ID
  
  const unsigned int dimsNumber = (unsigned int)
    castParamsPtr->m_trainSamplesPtr->begin()->second.operator[]( 0 ).size();  
  
  {
    Parameters::MClassesSamplesCT::const_iterator classesIt = 
      m_initParams.m_trainSamplesPtr->begin();
    const Parameters::MClassesSamplesCT::const_iterator classesItE = 
      m_initParams.m_trainSamplesPtr->end();
      
    while( classesIt != classesItE )
    {
      const Parameters::ClassIDT& classID = classesIt->first;
      
      const Parameters::ClassSamplesContainerT& classSamples = classesIt->second;
      TERP_TRUE_OR_RETURN_FALSE( classSamples.size() > 0,
        "Invalid class samples number" );
        
      Parameters::ClassSampleT classMeans( dimsNumber, 0.0 );      
      
      unsigned int dimIdx = 0; 
      
      for( dimIdx = 0 ; dimIdx < dimsNumber ; ++dimIdx )
      {
        double& dimMean = classMeans[ dimIdx ];
        dimMean = 0.0;
        
        Parameters::ClassSamplesContainerT::const_iterator samplesIt = 
          classSamples.begin();
        const Parameters::ClassSamplesContainerT::const_iterator samplesItE = 
          classSamples.end();
          
        while( samplesIt != samplesItE )
        {
          TERP_TRUE_OR_RETURN_FALSE( samplesIt->size() == dimsNumber,
            "Sample size mismatch" )
          dimMean += samplesIt->operator[]( dimIdx );
          
          ++samplesIt;
        }
        
        dimMean /= (double)( classSamples.size() );
      }
      
      m_classesMeans.push_back( classMeans );
      m_classesIndex2ID.push_back( classID );
      
      ++classesIt;
    }
    
  }
  
  // Calculating m_classesCovarianceMatrixes, m_classesCovarianceInvMatrixes
  // and m_classesCovarianceMatrixesHalfDetLog
  
  {
    Parameters::MClassesSamplesCT::const_iterator classesIt = 
      m_initParams.m_trainSamplesPtr->begin();
    const Parameters::MClassesSamplesCT::const_iterator classesItE = 
      m_initParams.m_trainSamplesPtr->end();
    unsigned int classIdx = 0;
      
    while( classesIt != classesItE )
    {
      const Parameters::ClassSamplesContainerT& classSamples = classesIt->second;
      TERP_TRUE_OR_RETURN_FALSE( classSamples.size() > 0,
        "Invalid class samples number" );
        
      const std::vector< double >& classMeans = m_classesMeans[ classIdx ];
        
      boost::numeric::ublas::matrix< double > classCovarianceMatrix( dimsNumber, dimsNumber );      
           
      unsigned int dimIdx1 = 0;
      unsigned int dimIdx2 = 0; 
      
      for( dimIdx1 = 0 ; dimIdx1 < dimsNumber ; ++dimIdx1 )
      {
        const double& dimMean1 = classMeans[ dimIdx1 ];
        
        for( dimIdx2 = 0 ; dimIdx2 < dimsNumber ; ++dimIdx2 )
        {  
          const double& dimMean2 = classMeans[ dimIdx2 ];
          
          double& covariance = classCovarianceMatrix( dimIdx1, dimIdx2 );
          covariance = 0.0;
        
          Parameters::ClassSamplesContainerT::const_iterator samplesIt = 
            classSamples.begin();
          const Parameters::ClassSamplesContainerT::const_iterator samplesItE = 
            classSamples.end();
            
          while( samplesIt != samplesItE )
          {
            covariance += 
              (
                ( samplesIt->operator[]( dimIdx1 ) - dimMean1 )
                *
                ( samplesIt->operator[]( dimIdx2 ) - dimMean2 )
              );
            
            ++samplesIt;
          }
          
          covariance /= (double)( classSamples.size() );
        }
      }
      
      m_classesCovarianceMatrixes.push_back( classCovarianceMatrix );
      
      double classCovarianceMatrixDet = 0;
      TERP_TRUE_OR_RETURN_FALSE( te::common::GetDeterminant( classCovarianceMatrix,
        classCovarianceMatrixDet ), "Determinant matrix error" );
      if( classCovarianceMatrixDet > 0.0 )
      {
        m_classesOptizedMAPDiscriminantTerm.push_back( -0.5 * std::log( 
          classCovarianceMatrixDet ) );
      }
      else
      {
        m_classesOptizedMAPDiscriminantTerm.push_back( 0.0 );
      }
      
      boost::numeric::ublas::matrix< double > classCovarianceInvMatrix;
      TERP_TRUE_OR_RETURN_FALSE( te::common::GetInverseMatrix( classCovarianceMatrix,
        classCovarianceInvMatrix ), "Inverse matrix calcule error" );
      m_classesCovarianceInvMatrixes.push_back( classCovarianceInvMatrix );
      
      ++classIdx;
      ++classesIt;
    }
    
  }  
  
  // Finalizing
  
  m_isInitialized = true;
  
  return true;
}

bool te::rp::ClassifierMAPStrategy::execute(const te::rst::Raster& inputRaster, 
  const std::vector<unsigned int>& inputRasterBands,
  const std::vector<te::gm::Polygon*>&, te::rst::Raster& outputRaster,
  const unsigned int outputRasterBand, 
  const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE( m_isInitialized, 
    "Classification strategy not initialized" );
  TERP_TRUE_OR_RETURN_FALSE( inputRasterBands.size() > 0, "Invalid input bands" );
  TERP_TRUE_OR_RETURN_FALSE( inputRasterBands.size() == m_classesMeans[ 0 ].size(), 
    "Invalid input bands" );
  TERP_DEBUG_TRUE_OR_THROW( outputRasterBand < outputRaster.getNumberOfBands(),
    "Invalid output band" );
  TERP_DEBUG_TRUE_OR_THROW( inputRaster.getNumberOfColumns() ==
    outputRaster.getNumberOfColumns(), "Rasters dims mismatch" );
  TERP_DEBUG_TRUE_OR_THROW( inputRaster.getNumberOfRows() ==
    outputRaster.getNumberOfRows(), "Rasters dims mismatch" );
    
  // progress
  
  std::auto_ptr< te::common::TaskProgress > progressPtr;
  if( enableProgressInterface )
  {
    progressPtr.reset( new te::common::TaskProgress );
    
    if( m_initParams.m_prioriProbs.empty() )
      progressPtr->setTotalSteps( inputRaster.getNumberOfRows() +
        ( inputRaster.getNumberOfRows() / m_initParams.m_prioriCalcSampleStep ) );
    else
      progressPtr->setTotalSteps( inputRaster.getNumberOfRows() );
    
    progressPtr->setMessage( "Classifying" );
  }    
    
  // Dealing with the logarithm of priori probabilities
  
  std::vector< double > logPrioriProbs;
  
  if( m_initParams.m_prioriProbs.empty() )
  {
    TERP_TRUE_OR_RETURN_FALSE( getPrioriProbabilities( inputRaster,
      inputRasterBands, progressPtr.get(), logPrioriProbs ), "Priori probabilities calcule error" );
      
    for( unsigned int pIdx = 0 ; pIdx < logPrioriProbs.size() ; ++pIdx )
      logPrioriProbs[ pIdx ] = ( logPrioriProbs[ pIdx ] > 0.0 ) ?
        std::log( logPrioriProbs[ pIdx ] ) : 0.0;
  }
  else
  {
    for( unsigned int pIdx = 0 ; pIdx < m_initParams.m_prioriProbs.size() ;
      ++pIdx )
      logPrioriProbs.push_back( std::log( m_initParams.m_prioriProbs[ pIdx ] ) );
  }
  
  // Classifying
  
  const unsigned int classesNumber = (unsigned int)m_classesMeans.size();
  const unsigned int nRows = (unsigned int)inputRaster.getNumberOfRows();
  const unsigned int nCols = (unsigned int)inputRaster.getNumberOfColumns();
  const unsigned int nDims = (unsigned int)inputRasterBands.size();
  unsigned int col = 0;
  unsigned int dim = 0;
  boost::numeric::ublas::matrix< double > sample( nDims, 1 );
  boost::numeric::ublas::matrix< double > sampleMinusMean( nDims, 1 );
  boost::numeric::ublas::matrix< double > sampleMinusMeanT( 1, nDims );
  boost::numeric::ublas::matrix< double > auxMatrix;
  boost::numeric::ublas::matrix< double > mahalanobisDistanceMatrix;
  unsigned int bandIdx = 0;
  unsigned int classIdx = 0;
  double discriminantFunctionValue = 0;
  double closestClassdiscriminantFunctionValue = 0;
  unsigned int closestClassIdx = 0;
  
  for( unsigned int row = 0 ; row < nRows ; ++row )
  {
    for( col = 0 ; col < nCols ; ++col )
    {
      // Creating the sample
      
      for( dim = 0 ; dim < nDims ; ++dim )
      {
        bandIdx = inputRasterBands[ dim ];
        TERP_DEBUG_TRUE_OR_THROW( bandIdx < inputRaster.getNumberOfBands(),
          "Invalid band index" );
          
        inputRaster.getValue( col, row, sample( dim, 0 ), bandIdx );
      }
      
      // Looking the the closest class
      
      closestClassdiscriminantFunctionValue = -1.0 * DBL_MAX;
      
      for( classIdx = 0 ; classIdx < classesNumber ; ++classIdx )
      {
        const Parameters::ClassSampleT& classMeans = m_classesMeans[ classIdx ];
        
        for( dim = 0 ; dim < nDims ; ++dim )
        {
          sampleMinusMean( dim, 0 ) = sampleMinusMeanT( 0, dim ) =
            ( sample( dim, 0 ) - classMeans[ dim ] );
        }
        
        //calculate the mahalanobis distance: (x-mean)T * CovMatrizInvert * (x-mean)
        auxMatrix = boost::numeric::ublas::prod( sampleMinusMeanT, 
          m_classesCovarianceInvMatrixes[ classIdx ] );
        mahalanobisDistanceMatrix = boost::numeric::ublas::prod( auxMatrix, sampleMinusMean );
        TERP_DEBUG_TRUE_OR_THROW( mahalanobisDistanceMatrix.size1() == 1, "Internal error" );
        TERP_DEBUG_TRUE_OR_THROW( mahalanobisDistanceMatrix.size2() == 1, "Internal error" );
        
        discriminantFunctionValue = logPrioriProbs[ classIdx ] 
          + m_classesOptizedMAPDiscriminantTerm[ classIdx ]
          - ( 0.5 * mahalanobisDistanceMatrix( 0, 0 ) );
        
        if( discriminantFunctionValue > closestClassdiscriminantFunctionValue )
        {
          closestClassdiscriminantFunctionValue = discriminantFunctionValue;
          closestClassIdx = classIdx;
        }
      }
      
      outputRaster.setValue( col, row, m_classesIndex2ID[ closestClassIdx ] );      
    }    
    
    if( enableProgressInterface )
    {
      progressPtr->pulse();
      if( ! progressPtr->isActive() ) return false;
    }         
  }    
  
  return true;
}

bool te::rp::ClassifierMAPStrategy::getPrioriProbabilities(
  const te::rst::Raster& inputRaster, 
  const std::vector<unsigned int>& inputRasterBands,
  te::common::TaskProgress * const progressPtr,
  std::vector< double >& prioriProbabilities ) const
{
  TERP_DEBUG_TRUE_OR_THROW( m_isInitialized, 
    "Classification strategy not initialized" );
  TERP_DEBUG_TRUE_OR_THROW( inputRasterBands.size() > 0, "Invalid input bands" );
  TERP_DEBUG_TRUE_OR_THROW( inputRasterBands.size() == m_classesMeans[ 0 ].size(), 
    "Invalid input bands" );
    
  prioriProbabilities.clear();
  
  // Classifying
  
  const unsigned int classesNumber = (unsigned int)m_classesMeans.size();
  const unsigned int nRows = (unsigned int)inputRaster.getNumberOfRows();
  const unsigned int nCols = (unsigned int)inputRaster.getNumberOfColumns();
  const unsigned int nDims = (unsigned int)inputRasterBands.size();
  const double initialPrioriProbLog = std::log( 1.0 / ( (double)classesNumber ) );
  unsigned int col = 0;
  unsigned int dim = 0;
  boost::numeric::ublas::matrix< double > sample( nDims, 1 );
  boost::numeric::ublas::matrix< double > sampleMinusMean( nDims, 1 );
  boost::numeric::ublas::matrix< double > sampleMinusMeanT( 1, nDims );
  boost::numeric::ublas::matrix< double > auxMatrix;
  boost::numeric::ublas::matrix< double > mahalanobisDistanceMatrix;
  unsigned int bandIdx = 0;
  unsigned int classIdx = 0;
  double discriminantFunctionValue = 0;
  double closestClassdiscriminantFunctionValue = 0;
  unsigned int closestClassIdx = 0;
  std::vector< unsigned long int > elementsNumberByClass( classesNumber, 0 );
  unsigned int totalSamplesNumber = 0;
  
  for( unsigned int row = 0 ; row < nRows ; row += m_initParams.m_prioriCalcSampleStep )
  {
    for( col = 0 ; col < nCols ; col += m_initParams.m_prioriCalcSampleStep )
    {
      // Creating the sample
      
      for( dim = 0 ; dim < nDims ; ++dim )
      {
        bandIdx = inputRasterBands[ dim ];
        TERP_DEBUG_TRUE_OR_THROW( bandIdx < inputRaster.getNumberOfBands(),
          "Invalid band index" );
          
        inputRaster.getValue( col, row, sample( dim, 0 ), bandIdx );
      }
      
      // Looking the the closest class
      
      closestClassdiscriminantFunctionValue = -1.0 * DBL_MAX;
      
      for( classIdx = 0 ; classIdx < classesNumber ; ++classIdx )
      {
        const Parameters::ClassSampleT& classMeans = m_classesMeans[ classIdx ];
        
        for( dim = 0 ; dim < nDims ; ++dim )
        {
          sampleMinusMean( dim, 0 ) = sampleMinusMeanT( 0, dim ) =
            ( sample( dim, 0 ) - classMeans[ dim ] );
        }
        
        //calculate the mahalanobis distance: (x-mean)T * CovMatrizInvert * (x-mean)
        auxMatrix = boost::numeric::ublas::prod( sampleMinusMeanT, 
          m_classesCovarianceInvMatrixes[ classIdx ] );
        mahalanobisDistanceMatrix = boost::numeric::ublas::prod( auxMatrix, sampleMinusMean );
        TERP_DEBUG_TRUE_OR_THROW( mahalanobisDistanceMatrix.size1() == 1, "Internal error" );
        TERP_DEBUG_TRUE_OR_THROW( mahalanobisDistanceMatrix.size2() == 1, "Internal error" );
        
        discriminantFunctionValue = initialPrioriProbLog 
          + m_classesOptizedMAPDiscriminantTerm[ classIdx ]
          - ( 0.5 * mahalanobisDistanceMatrix( 0, 0 ) );
        
        if( discriminantFunctionValue > closestClassdiscriminantFunctionValue )
        {
          closestClassdiscriminantFunctionValue = discriminantFunctionValue;
          closestClassIdx = classIdx;
        }
      }
      
      ++( elementsNumberByClass[ closestClassIdx ] );
      ++totalSamplesNumber;
    } 
    
    if( progressPtr )
    {
      progressPtr->pulse();
      if( ! progressPtr->isActive() ) return false;
    }    
  }   
  
  if( totalSamplesNumber )
  {
    for( classIdx = 0 ; classIdx < classesNumber ; ++classIdx )
    {    
      prioriProbabilities.push_back( ( (double)elementsNumberByClass[ classIdx ] ) /
        ( (double)( totalSamplesNumber ) ) );
    }  
  
    return true;
  }
  else
  {
    return false;
  }
}

//-----------------------------------------------------------------------------

te::rp::ClassifierMAPStrategyFactory::ClassifierMAPStrategyFactory()
  : te::rp::ClassifierStrategyFactory("map")
{
}

te::rp::ClassifierMAPStrategyFactory::~ClassifierMAPStrategyFactory()
{
}

te::rp::ClassifierStrategy* te::rp::ClassifierMAPStrategyFactory::build()
{
  return new te::rp::ClassifierMAPStrategy();
}
