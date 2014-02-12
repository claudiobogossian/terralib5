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
  \file terralib/rp/ClassifierSAMStrategy.cpp

  \brief Maximum a posteriori probability strategy.
*/

// TerraLib

#include "ClassifierSAMStrategy.h"
#include "Macros.h"
#include "../common/progress/TaskProgress.h"

#include <cfloat>
#include <cmath>

namespace
{
  static te::rp::ClassifierSAMStrategyFactory classifierMAPStrategyFactoryInstance;
}

//-----------------------------------------------------------------------------

te::rp::ClassifierSAMStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::ClassifierSAMStrategy::Parameters::~Parameters()
{
}

const te::rp::ClassifierSAMStrategy::Parameters& te::rp::ClassifierSAMStrategy::Parameters::operator=(const te::rp::ClassifierSAMStrategy::Parameters& rhs)
{
  reset();

  m_trainSamplesPtr = rhs.m_trainSamplesPtr;
  m_maxAngularDistances = rhs.m_maxAngularDistances;

  return *this;
}

void te::rp::ClassifierSAMStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_trainSamplesPtr.reset();
  m_maxAngularDistances.clear();
}

te::common::AbstractParameters* te::rp::ClassifierSAMStrategy::Parameters::clone() const
{
  return new te::rp::ClassifierSAMStrategy::Parameters( *this );
}

//-----------------------------------------------------------------------------

te::rp::ClassifierSAMStrategy::ClassifierSAMStrategy()
{
  m_isInitialized = false;
}

te::rp::ClassifierSAMStrategy::~ClassifierSAMStrategy()
{
}

bool te::rp::ClassifierSAMStrategy::initialize(
  te::rp::StrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;
  m_initParams.reset();
  m_classesMeans.clear();
  m_classesIndex2ID.clear();
  
  ClassifierSAMStrategy::Parameters const * const castParamsPtr = 
    dynamic_cast< ClassifierSAMStrategy::Parameters const * >( strategyParams );
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr, "Invalid parameters" );
  
  // Checking the angluar distances
  
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_maxAngularDistances.size() ==
    castParamsPtr->m_trainSamplesPtr->size(),
    "Mismatch between the classes number and the max angular distances" );
    
  for( unsigned int maxAngularDistancesIdx = 0 ; maxAngularDistancesIdx <
    castParamsPtr->m_maxAngularDistances.size() ; ++ maxAngularDistancesIdx )
  {
    TERP_TRUE_OR_RETURN_FALSE( ( castParamsPtr->m_maxAngularDistances[
      maxAngularDistancesIdx ] >= 0.0 ),
      "Invalid max angular distance" );
  }  
  
  // Calculating m_classesMeans and m_classesIndex2ID
  
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr,
    "Invalid classes samples pointer" )
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr->size() > 0,
    "Invalid classes samples number" )
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr->begin()->second.size() > 0,
    "Invalid classe samples number" )    
  const unsigned int dimsNumber = (unsigned int)
    castParamsPtr->m_trainSamplesPtr->begin()->second.operator[]( 0 ).size();
  TERP_TRUE_OR_RETURN_FALSE( dimsNumber > 0,
    "Invalid dimensions number" )       
  
  {
    ClassesSamplesT::const_iterator classesIt = 
      castParamsPtr->m_trainSamplesPtr->begin();
    const ClassesSamplesT::const_iterator classesItE = 
      castParamsPtr->m_trainSamplesPtr->end();
    unsigned int dimIdx = 0;
    SamplesT::const_iterator samplesIt;
    SamplesT::const_iterator samplesItE;
      
    while( classesIt != classesItE )
    {
      const ClassIDT& classID = classesIt->first;
      TERP_TRUE_OR_RETURN_FALSE( classID > 0, "Invalid class ID" );
      
      const SamplesT& classSamples = classesIt->second;
      TERP_TRUE_OR_RETURN_FALSE( classSamples.size() > 0,
        "Invalid class samples number" );
        
      SampleT classMeans( dimsNumber );      
      
      for( dimIdx = 0 ; dimIdx < dimsNumber ; ++dimIdx )
      {
        double& dimMean = classMeans[ dimIdx ];
        dimMean = 0.0;
        
        samplesIt = classSamples.begin();
        samplesItE = classSamples.end();
          
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
  
  // Finalizing
  
  m_initParams = (*castParamsPtr);
  
  m_isInitialized = true;
  
  return true;
}

bool te::rp::ClassifierSAMStrategy::execute(const te::rst::Raster& inputRaster, 
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
    progressPtr->setTotalSteps( inputRaster.getNumberOfRows() );
    progressPtr->setMessage( "Classifying" );
  }    
    
  // Classifying
  
  const unsigned int classesNumber = (unsigned int)m_classesMeans.size();
  const unsigned int nRows = (unsigned int)inputRaster.getNumberOfRows();
  const unsigned int nCols = (unsigned int)inputRaster.getNumberOfColumns();
  const unsigned int nDims = (unsigned int)m_classesMeans[ 0 ].size();
  unsigned int col = 0;
  unsigned int dim = 0;
  unsigned int classIdx = 0;
  double angularTR = 0;
  double angularTT = 0;
  double angularRR = 0;  
  double angularDist = 0;
  double readedValue = 0;
  double minAngularDist = 0;
  unsigned int minAngularDistClassIdx = 0;
  
  for( unsigned int row = 0 ; row < nRows ; ++row )
  {
    for( col = 0 ; col < nCols ; ++col )
    {
      // Looking the the closest class
      
      minAngularDist = DBL_MAX;
      minAngularDistClassIdx = classesNumber;
      
      for( classIdx = 0 ; classIdx < classesNumber ; ++classIdx )
      {
        const SampleT& classMeans = m_classesMeans[ classIdx ];
              
        angularTR = 0;
        angularTT = 0;
        angularRR = 0;           
        
        for( dim = 0 ; dim < nDims ; ++dim )
        {
          const double& meanValue = classMeans[ dim ];
          
          inputRaster.getValue( col, row, readedValue, inputRasterBands[ dim ] );
          
          angularTR += readedValue * meanValue;
          angularRR += meanValue * meanValue;
          angularTT += readedValue * readedValue;
        }
        
        angularDist = angularTR / ( sqrt( angularTT ) *
          sqrt( angularRR ) );
          
        if( std::abs( angularDist ) > 1.0 )
        {
          angularDist = DBL_MAX;
        }
        else
        {
          angularDist = acos( angularDist );
        }
      
        if( ( angularDist < minAngularDist ) && ( angularDist < 
          m_initParams.m_maxAngularDistances[ classIdx ] ) )
        {
          minAngularDist = angularDist;
          minAngularDistClassIdx = classIdx;
        }
      }
      
      if( minAngularDistClassIdx == classesNumber )
        outputRaster.setValue( col, row, 0, 0 );
      else
        outputRaster.setValue( col, row, m_classesIndex2ID[ minAngularDistClassIdx ], 
          0 );      
    }    
    
    if( enableProgressInterface )
    {
      progressPtr->pulse();
      if( ! progressPtr->isActive() ) return false;
    }         
  }    
  
  return true;
}

//-----------------------------------------------------------------------------

te::rp::ClassifierSAMStrategyFactory::ClassifierSAMStrategyFactory()
  : te::rp::ClassifierStrategyFactory("sam")
{
}

te::rp::ClassifierSAMStrategyFactory::~ClassifierSAMStrategyFactory()
{
}

te::rp::ClassifierStrategy* te::rp::ClassifierSAMStrategyFactory::build()
{
  return new te::rp::ClassifierSAMStrategy();
}
