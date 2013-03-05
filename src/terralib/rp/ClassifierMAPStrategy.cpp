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
  \file terralib/rp/ClassifierMAPStrategy.cpp

  \brief Maximum a posteriori probability strategy.
*/

// TerraLib

#include "ClassifierMAPStrategy.h"
#include "Macros.h"
#include "../raster/Band.h"

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
  m_classThresholds = rhs.m_classThresholds;
  m_prioriProbs = rhs.m_prioriProbs;

  return *this;
}

void te::rp::ClassifierMAPStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_trainSamplesPtr = 0;
  m_classThresholds.clear();
  m_prioriProbs.clear();
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
  m_initParams.reset();
  m_classesMeans.clear();
  m_classesCovarianceMatrixes.clear();
  m_isInitialized = false;
  
  ClassifierMAPStrategy::Parameters const * const castParamsPtr = 
    dynamic_cast< ClassifierMAPStrategy::Parameters const * >( strategyParams );
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr, "Invalid parameters" );
  
  // Checking the input parameters
  
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr,
    "Invalid classes samples pointer" )
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr->size() > 0,
    "Invalid classes samples number" )
    
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_classThresholds.size() ==
    castParamsPtr->m_trainSamplesPtr->size(), "Invalid classes thresholds" );
  for( std::vector< double >::size_type csIdx = 0 ; csIdx < 
    castParamsPtr->m_classThresholds.size() ; ++csIdx )
  {
    TERP_TRUE_OR_RETURN_FALSE( 
      ( castParamsPtr->m_classThresholds[ csIdx ] >= 0.0 ) &&
      ( castParamsPtr->m_classThresholds[ csIdx ] <= 1.0 ),
      "Invalid classes thresholds" );                    
  }

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
  
  m_initParams = (*castParamsPtr);
  
  // Calculating the classes means vectors
  
  {
    Parameters::MClassesSamplesCT::const_iterator classesIt = 
      m_initParams.m_trainSamplesPtr->begin();
    const Parameters::MClassesSamplesCT::const_iterator classesItE = 
      m_initParams.m_trainSamplesPtr->end();
      
    while( classesIt != classesItE )
    {
      const Parameters::ClassSamplesContainerT& classSamples = classesIt->second;
      TERP_TRUE_OR_RETURN_FALSE( classSamples.size() > 0,
        "Invalid class samples number" );
        
      const unsigned int dimsNumber = (unsigned int)classSamples.begin()->size();
        
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
          dimMean += samplesIt->operator[]( dimIdx );
          
          ++samplesIt;
        }
      }
      
      for( dimIdx = 0 ; dimIdx < dimsNumber ; ++dimIdx )
      {
        classMeans[ dimIdx ] /= (double)( classSamples.size() );
      }      
      
      m_classesMeans.push_back( classMeans );
      
      ++classesIt;
    }
    
  }
  
  // Calculating the classes covariances matrixes
  
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
        
      const Parameters::ClassSampleT& classMeans = m_classesMeans[ classIdx ];
        
      const unsigned int dimsNumber = (unsigned int)classSamples.begin()->size();
        
      boost::numeric::ublas::matrix< double > classCovarianceMatrix( dimsNumber, dimsNumber );      
      
      unsigned int dimIdx1 = 0;
      unsigned int dimIdx2 = 0; 
      
      for( dimIdx1 = 0 ; dimIdx1 < dimsNumber ; ++dimIdx1 )
      {
        const double& dimMean1 = classMeans[ dimIdx1 ];
        
        for( dimIdx1 = 0 ; dimIdx1 < dimsNumber ; ++dimIdx1 )
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
                +
                ( samplesIt->operator[]( dimIdx2 ) - dimMean2 )
              );
            
            ++samplesIt;
          }
        }
      }
      
      for( dimIdx1 = 0 ; dimIdx1 < dimsNumber ; ++dimIdx1 )
      {
        for( dimIdx1 = 0 ; dimIdx1 < dimsNumber ; ++dimIdx1 )
        {  
          classCovarianceMatrix( dimIdx1, dimIdx2 ) /= (double)( classSamples.size() );
        }
      }      
      
      m_classesCovarianceMatrixes.push_back( classCovarianceMatrix );
      
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
  TERP_TRUE_OR_RETURN_FALSE( outputRasterBand < outputRaster.getNumberOfBands(),
    "Invalid output band" );
    
  // Calculating priory probabilities
  
  boost::numeric::ublas::matrix< double > prioriProbs( inputRasterBands.size(), 1 );
  
  if( m_initParams.m_prioriProbs.empty() )
  {
    TERP_TRUE_OR_RETURN_FALSE( calcPrioriProbs(), "Priori probabilities calculer error" );
  }
  else
  {
    for( std::vector< double >::size_type pIdx = 0 ; pIdx < 
      m_initParams.m_prioriProbs.size() ; ++pIdx )
    {
      prioriProbs( pIdx, 0 ) = m_initParams.m_prioriProbs[ pIdx ];
    }
  }
  
  // Calculating the means vector
  
  boost::numeric::ublas::matrix< double > means( inputRasterBands.size(), 1 );

  {
    std::vector< unsigned int >::size_type inputRasterBandsIdx = 0;
    const unsigned int nCols = inputRaster.getNumberOfColumns();
    const unsigned int nRows = inputRaster.getNumberOfRows();
    unsigned int col = 0;
    unsigned int row = 0;
    double value = 0;
    
    for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
      m_initParams.m_prioriProbs.size() ; ++inputRasterBandsIdx )
    {
      const unsigned int bandIdx = inputRasterBands[ inputRasterBandsIdx ];
      TERP_TRUE_OR_THROW( bandIdx < inputRaster.getNumberOfBands(),
        "Invalid band index" );
      const te::rst::Band& band = ( *inputRaster.getBand( bandIdx ) );
      
      means( inputRasterBandsIdx, 0 ) = 0;
      
      for( row = 0 ; row < nRows ; ++row )
        for( col = 0 ; col < nCols ; ++col )
        {
          band.getValue( col, row, value );
          means( inputRasterBandsIdx, 0 ) += value;          
        }
    }
    
    for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
      m_initParams.m_prioriProbs.size() ; ++inputRasterBandsIdx )
    {
       means( inputRasterBandsIdx, 0 ) /= (double)( nCols * nRows );
    }  
  }
  
  // Calculating the covariance matrix
  
  boost::numeric::ublas::matrix< double > covariance( inputRasterBands.size(), 
    inputRasterBands.size() );
  
  {
    const unsigned int nCols = inputRaster.getNumberOfColumns();
    const unsigned int nRows = inputRaster.getNumberOfRows();
    unsigned int col = 0;
    unsigned int row = 0;
    double value1 = 0;
    double value2 = 0;
    std::vector< unsigned int >::size_type inputRasterBandsIdx1 = 0;
    std::vector< unsigned int >::size_type inputRasterBandsIdx2 = 0;
    
    for( inputRasterBandsIdx1 = 0 ; 
      inputRasterBandsIdx1 <  m_initParams.m_prioriProbs.size() ; 
      ++inputRasterBandsIdx1 )
    {
      const unsigned int bandIdx1 = inputRasterBands[ inputRasterBandsIdx1 ];
      const te::rst::Band& band1 = ( *inputRaster.getBand( bandIdx1 ) );
      
      for( inputRasterBandsIdx2 = 0 ; 
        inputRasterBandsIdx2 <  m_initParams.m_prioriProbs.size() ; 
        ++inputRasterBandsIdx2 )
      {
        const unsigned int bandIdx2 = inputRasterBands[ inputRasterBandsIdx2 ];
        const te::rst::Band& band2 = ( *inputRaster.getBand( bandIdx2 ) );
        
        covariance( inputRasterBandsIdx1, inputRasterBandsIdx2 ) = 0;
        
        for( row = 0 ; row < nRows ; ++row )
          for( col = 0 ; col < nCols ; ++col )
          {
            band1.getValue( col, row, value1 );
            band2.getValue( col, row, value2 );
            
            covariance( inputRasterBandsIdx1, inputRasterBandsIdx2 ) += 
              (
                ( value1 - means( inputRasterBandsIdx1, 0 ) )
                +
                ( value2 - means( inputRasterBandsIdx2, 0 ) )
              );
          }        
      }
    }
    
    for( inputRasterBandsIdx1 = 0 ; 
      inputRasterBandsIdx1 <  m_initParams.m_prioriProbs.size() ; 
      ++inputRasterBandsIdx1 )
    {
      for( inputRasterBandsIdx2 = 0 ; 
        inputRasterBandsIdx2 <  m_initParams.m_prioriProbs.size() ; 
        ++inputRasterBandsIdx2 )
      {    
        covariance( inputRasterBandsIdx1, inputRasterBandsIdx2 ) /=
          (double)( nCols * nRows  );
      }
    }
  }
  
  return true;
}

bool te::rp::ClassifierMAPStrategy::calcPrioriProbs()
{
  return false;
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
