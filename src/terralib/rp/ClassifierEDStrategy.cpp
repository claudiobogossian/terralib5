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
  \file terralib/rp/ClassifierEDStrategy.cpp

  \brief Euclidean Distance Classifier strategy.
*/

// TerraLib

#include "ClassifierEDStrategy.h"
#include "Macros.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../common/MatrixUtils.h"

// STL
#include <memory>

#include <cfloat>

namespace
{
  static te::rp::ClassifierEDStrategyFactory classifierEDStrategyFactoryInstance;
}

//-----------------------------------------------------------------------------

te::rp::ClassifierEDStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::ClassifierEDStrategy::Parameters::~Parameters()
{
}

const te::rp::ClassifierEDStrategy::Parameters& te::rp::ClassifierEDStrategy::Parameters::operator=(const te::rp::ClassifierEDStrategy::Parameters& rhs)
{
  reset();

  m_trainSamplesPtr = rhs.m_trainSamplesPtr;

  return *this;
}

void te::rp::ClassifierEDStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_trainSamplesPtr.reset();
}

te::common::AbstractParameters* te::rp::ClassifierEDStrategy::Parameters::clone() const
{
  return new te::rp::ClassifierEDStrategy::Parameters( *this );
}

//-----------------------------------------------------------------------------

te::rp::ClassifierEDStrategy::ClassifierEDStrategy()
{
  m_isInitialized = false;
}

te::rp::ClassifierEDStrategy::~ClassifierEDStrategy()
{
}

std::vector< int > te::rp::ClassifierEDStrategy::getOutputDataTypes() const
{
  std::vector< int > dt;
  dt.push_back( te::dt::UINT32_TYPE );
  return dt;
}

bool te::rp::ClassifierEDStrategy::initialize(
  te::rp::ClassifierStrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;
  m_initParams.reset();
  m_classesIndex2ID.clear();
  m_classesMeans.clear();

  ClassifierEDStrategy::Parameters const* const castParamsPtr =
    dynamic_cast< ClassifierEDStrategy::Parameters const * >( strategyParams );
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr, "Invalid parameters" );

  // Checking the input parameters

  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr,
    "Invalid classes samples pointer" )
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr->size() > 0,
    "Invalid classes samples number" )
  TERP_TRUE_OR_RETURN_FALSE( castParamsPtr->m_trainSamplesPtr->begin()->second.size() > 0,
    "Invalid classes samples number" )    

  m_initParams = (*castParamsPtr);

  // Calculating m_classesMeans and m_classesIndex2ID

  // Dimension number (bands number)
  const unsigned int dimsNumber = (unsigned int)
    castParamsPtr->m_trainSamplesPtr->begin()->second.operator[]( 0 ).size();

  {
    Parameters::MClassesSamplesCT::const_iterator classesIt = 
      m_initParams.m_trainSamplesPtr->begin();
    const Parameters::MClassesSamplesCT::const_iterator classesItE = 
      m_initParams.m_trainSamplesPtr->end();

    // Iterate class
    while( classesIt != classesItE )
    {
      const Parameters::ClassIDT& classID = classesIt->first;

      const Parameters::ClassSamplesContainerT& classSamples = classesIt->second;
      TERP_TRUE_OR_RETURN_FALSE( classSamples.size() > 0,
        "Invalid class samples number" );

      Parameters::ClassSampleT classMeans( dimsNumber, 0.0 );      

      unsigned int dimIdx = 0; 

      // Iterate bands
      for( dimIdx = 0 ; dimIdx < dimsNumber ; ++dimIdx )
      {
        double& dimMean = classMeans[ dimIdx ];
        dimMean = 0.0;
        
        Parameters::ClassSamplesContainerT::const_iterator samplesIt = 
          classSamples.begin();
        const Parameters::ClassSamplesContainerT::const_iterator samplesItE = 
          classSamples.end();

        // Iterate samples
        while( samplesIt != samplesItE )
        {
          TERP_TRUE_OR_RETURN_FALSE( samplesIt->size() == dimsNumber,
            "Sample size mismatch" )
          dimMean += samplesIt->operator[]( dimIdx );

          ++samplesIt;
        }

        // sum / simple size
        dimMean /= (double)( classSamples.size() );
      }

      // Adding to container
      m_classesMeans.push_back( classMeans );
      m_classesIndex2ID.push_back( classID );

      ++classesIt;
    }

  }

  // Finalizing
  
  m_isInitialized = true;
  
  return true;
}

bool te::rp::ClassifierEDStrategy::execute(const te::rst::Raster& inputRaster, 
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
  
  std::unique_ptr< te::common::TaskProgress > progressPtr;
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
  const unsigned int nDims = (unsigned int)inputRasterBands.size();
  unsigned int col = 0;
  unsigned int dim = 0;
  double dummy = 0;
  bool isDummy;
  boost::numeric::ublas::matrix< double > sample( nDims, 1 );
  unsigned int bandIdx = 0;
  unsigned int classIdx = 0;
  double dist = 0;
  double bestDist = 0;
  unsigned int bestClass = 0;

  
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

        const te::rst::Band& band = *inputRaster.getBand(bandIdx);
        dummy = band.getProperty()->m_noDataValue;

        isDummy = false;
        if( dummy == sample(dim, 0) )
          isDummy = true;

      }

      if(isDummy) {
        outputRaster.setValue(col, row, dummy);
        continue;
      }

      unsigned int refClass = bestClass;
      bestDist = 0;

      for( unsigned int band = 0 ; band < nDims ; ++band )
//        bestDist += (sample(band, 0) - m_classesMeans[refClass][band]) * (sample(band, 0) - m_classesMeans[refClass][band]) * 1/2;
        bestDist += (sample(band, 0) - m_classesMeans[refClass][band]) * (sample(band, 0) - m_classesMeans[refClass][band]);

      for( classIdx = 0 ; classIdx < classesNumber ; ++classIdx )
      {
        const Parameters::ClassSampleT& classMeans = m_classesMeans[ classIdx ];


        if( classIdx == refClass )
          continue;

        dist = 0;


        for( dim = 0 ; dim < nDims ; ++dim )
        {
//          dist += (sample(dim, 0) - classMeans[classIdx]) * (sample(dim, 0) - classMeans[classIdx]) * 1/2;
          dist += (sample(dim, 0) - classMeans[classIdx]) * (sample(dim, 0) - classMeans[classIdx]);

          if(dist <= bestDist)
           break;
        }


        // considerar a condição invertida, inverter atribuições internas
        if( dist < bestDist )
        {
          bestDist = dist;
          bestClass = classIdx;
        }
      }

//      double vClass = bestclass + 1;
      outputRaster.setValue( col, row, m_classesIndex2ID[ bestClass ] );
    }
    
    if( enableProgressInterface )
    {
      progressPtr->pulse();
      if( ! progressPtr->isActive() ) return false;
    }         
  }    
  
  return true;
}

unsigned int te::rp::ClassifierEDStrategy::getOutputNumberBands(void) {
  return 0;
}
//-----------------------------------------------------------------------------

te::rp::ClassifierEDStrategyFactory::ClassifierEDStrategyFactory()
  : te::rp::ClassifierStrategyFactory("ed")
{
}

te::rp::ClassifierEDStrategyFactory::~ClassifierEDStrategyFactory()
{
}

te::rp::ClassifierStrategy* te::rp::ClassifierEDStrategyFactory::build()
{
  return new te::rp::ClassifierEDStrategy();
}
