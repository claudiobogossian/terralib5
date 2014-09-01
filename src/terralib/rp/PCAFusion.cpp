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
  \file terralib/rp/PCAFusion.cpp
  \brief Creation of skeleton imagems.
*/

#include "PCAFusion.h"
#include "Macros.h"
#include "Functions.h"

#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Band.h"
#include "../raster/Grid.h"
#include "../geometry/Envelope.h"
#include "../common/progress/TaskProgress.h"
#include "../memory/ExpansibleRaster.h"

#include <cmath>
#include <limits>
  
namespace te
{
  namespace rp
  {

    PCAFusion::InputParameters::InputParameters()
    {
      reset();
    }

    PCAFusion::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    PCAFusion::InputParameters::~InputParameters()
    {
      reset();
    }

    void PCAFusion::InputParameters::reset() throw( te::rp::Exception )
    {
      m_lowResRasterPtr = 0;
      m_lowResRasterBands.clear();
      m_highResRasterPtr = 0;
      m_highResRasterBand = 0;
      m_enableProgress = false;
      m_enableThreadedProcessing = true;
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
    }

    const PCAFusion::InputParameters& PCAFusion::InputParameters::operator=(
      const PCAFusion::InputParameters& params )
    {
      reset();

      m_lowResRasterPtr = params.m_lowResRasterPtr;
      m_lowResRasterBands = params.m_lowResRasterBands;
      m_highResRasterPtr = params.m_highResRasterPtr;
      m_highResRasterBand = params.m_highResRasterBand;
      m_enableProgress = params.m_enableProgress;
      m_enableThreadedProcessing = params.m_enableThreadedProcessing;
      m_interpMethod = params.m_interpMethod;

      return *this;
    }

    te::common::AbstractParameters* PCAFusion::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    PCAFusion::OutputParameters::OutputParameters()
    {
      reset();
    }

    PCAFusion::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    PCAFusion::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void PCAFusion::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const PCAFusion::OutputParameters& PCAFusion::OutputParameters::operator=(
      const PCAFusion::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* PCAFusion::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    PCAFusion::PCAFusion()
    {
      reset();
    }

    PCAFusion::~PCAFusion()
    {
    }

    bool PCAFusion::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      PCAFusion::OutputParameters* outParamsPtr = dynamic_cast<
        PCAFusion::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 4 );
        
        progressPtr->setMessage( "Fusing images" );
      }   
      
      // Loading ressampled data
      
      std::auto_ptr< te::rst::Raster > ressampledRasterPtr;
      
      TERP_TRUE_OR_RETURN_FALSE( loadRessampledRaster( ressampledRasterPtr ),
        "Ressampled raster data loading error" );   
        
//      te::rp::Copy2DiskRaster( *ressampledRasterPtr, "ressampledRaster.tif" );
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }
      
      // Compute PCA space raster
      
      std::auto_ptr< te::rst::Raster > pcaRasterPtr;
      boost::numeric::ublas::matrix< double > pcaMatrix;
      
      {
        te::rst::Grid* gridPtr = new te::rst::Grid( *ressampledRasterPtr->getGrid() );
        std::vector< te::rst::BandProperty * > bandProperties;
        std::vector< unsigned int > ressampledRasterBands;
          
        for( unsigned int bandIdx = 0 ; bandIdx <
          ressampledRasterPtr->getNumberOfBands() ; ++bandIdx )
        {
          bandProperties.push_back( new te::rst::BandProperty( 
            *ressampledRasterPtr->getBand( bandIdx )->getProperty() ) );
          bandProperties[ bandIdx ]->m_type = te::dt::DOUBLE_TYPE;
          
          ressampledRasterBands.push_back( bandIdx );
        } 
        
        try
        {
          pcaRasterPtr.reset( new te::mem::ExpansibleRaster( 20, gridPtr,
            bandProperties ) );    
        }
        catch( te::common::Exception& e )
        {
          TERP_LOG_AND_RETURN_FALSE( e.what() );
        }    
        
        TERP_TRUE_OR_RETURN_FALSE( DirectPrincipalComponents( *ressampledRasterPtr, 
          ressampledRasterBands, pcaMatrix, *pcaRasterPtr, ressampledRasterBands,
          m_inputParameters.m_enableThreadedProcessing ? 0 : 1 ),
          "Principal components generation error" );
          
//        te::rp::Copy2DiskRaster( *pcaRasterPtr, "pcaRaster.tif" );
      }
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }        
      
      // Swapping the first PCA band by the high resolution raster
      
      TERP_TRUE_OR_RETURN_FALSE( swapBandByHighResRaster( *pcaRasterPtr, 0 ),
         "Band swap error" );
         
//      te::rp::Copy2DiskRaster( *pcaRasterPtr, "swappedpcaRaster.tif" );
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }  
      
      // Generating the output fused raster
      
      {
        te::rst::Grid* gridPtr = new te::rst::Grid( *pcaRasterPtr->getGrid() );
        std::vector< te::rst::BandProperty * > bandProperties;
        std::vector< unsigned int > outputRasterBands;
          
        for( unsigned int bandIdx = 0 ; bandIdx <
          pcaRasterPtr->getNumberOfBands() ; ++bandIdx )
        {
          bandProperties.push_back( new te::rst::BandProperty( 
            *pcaRasterPtr->getBand( bandIdx )->getProperty() ) );
          bandProperties[ bandIdx ]->m_type = 
            ressampledRasterPtr->getBand( bandIdx )->getProperty()->m_type;
            
          outputRasterBands.push_back( bandIdx );
        } 
        
        outParamsPtr->m_outputRasterPtr.reset(
          te::rst::RasterFactory::make(
            outParamsPtr->m_rType,
            gridPtr,
            bandProperties,
            outParamsPtr->m_rInfo,
            0,
            0 ) );
        TERP_TRUE_OR_RETURN_FALSE( outParamsPtr->m_outputRasterPtr.get(),
          "Output raster creation error" );
          
        TERP_TRUE_OR_RETURN_FALSE( InversePrincipalComponents( *pcaRasterPtr,
          pcaMatrix, *outParamsPtr->m_outputRasterPtr, outputRasterBands,
          m_inputParameters.m_enableThreadedProcessing ? 0 : 1 ),
          "Inverse PCA error" );
      }                  

      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }  
      
      return true;
    }

    void PCAFusion::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool PCAFusion::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      PCAFusion::InputParameters const* inputParamsPtr = dynamic_cast<
        PCAFusion::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the m_lowResRasterPtr and m_lowResRasterBands

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterPtr,
        "Invalid low Resolution Raster Pointer" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_lowResRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
        
      for( unsigned int lowResRasterBandsIdx = 0 ; lowResRasterBandsIdx <
        inputParamsPtr->m_lowResRasterBands.size() ; ++lowResRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          inputParamsPtr->m_lowResRasterBands[ lowResRasterBandsIdx ] <
          m_inputParameters.m_lowResRasterPtr->getNumberOfBands(), 
          "Invalid raster band" );   
      }
      
      // Checking the m_highResRasterPtr and m_highResRasterBand

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_highResRasterPtr,
        "Invalid high resolution Raster Pointer" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_highResRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
        
      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_highResRasterBand <
        m_inputParameters.m_highResRasterPtr->getNumberOfBands(), 
        "Invalid raster band" );   
        
      m_isInitialized = true;

      return true;
    }

    bool PCAFusion::isInitialized() const
    {
      return m_isInitialized;
    }
    
    bool PCAFusion::loadRessampledRaster( std::auto_ptr< te::rst::Raster >& ressampledRasterPtr ) const
    {
      // Creating the ressampled raster
      
      const unsigned int outNCols = m_inputParameters.m_highResRasterPtr->getNumberOfColumns();
      const unsigned int outNRows = m_inputParameters.m_highResRasterPtr->getNumberOfRows();
      unsigned int lowResRasterBandsIdx = 0;
      unsigned int lowResRasterBandIdx = 0;
      
      te::rst::Grid* ressampledGrid = new te::rst::Grid( outNCols, outNRows,
        new te::gm::Envelope( *m_inputParameters.m_lowResRasterPtr->getGrid()->getExtent() ), 
        m_inputParameters.m_lowResRasterPtr->getSRID() );
        
      std::vector< te::rst::BandProperty * > ressampledBandProperties;
        
      for( lowResRasterBandsIdx = 0 ; lowResRasterBandsIdx <
        m_inputParameters.m_lowResRasterBands.size() ; ++lowResRasterBandsIdx )
      {
        lowResRasterBandIdx = m_inputParameters.m_lowResRasterBands[ lowResRasterBandsIdx ];
        
        ressampledBandProperties.push_back( new te::rst::BandProperty( 
          *m_inputParameters.m_lowResRasterPtr->getBand( lowResRasterBandIdx )->getProperty() ) );
      } 
      
      try
      {
        ressampledRasterPtr.reset( new te::mem::ExpansibleRaster( 20, ressampledGrid,
          ressampledBandProperties ) );     
      }
      catch( te::common::Exception& e )
      {
        TERP_LOG_AND_RETURN_FALSE( e.what() );
      }
      
      const double colsRescaleFactor =
        ((double)m_inputParameters.m_lowResRasterPtr->getNumberOfColumns()) /
        ((double)outNCols);
      const double rowsRescaleFactor =
        ((double)m_inputParameters.m_lowResRasterPtr->getNumberOfRows()) /
        ((double)outNRows);
      unsigned int outRow = 0;
      unsigned int outCol = 0;
      double inRow = 0;
      double inCol = 0;  
      te::rst::Interpolator interpol( m_inputParameters.m_lowResRasterPtr,
        m_inputParameters.m_interpMethod );      
      std::complex< double > value = 0;
      te::rst::Raster& ressampledRaster = *ressampledRasterPtr;
      double inNoDataValue = 0;
      double outNoDataValue = 0;
        
      for( lowResRasterBandsIdx = 0 ; lowResRasterBandsIdx <
        m_inputParameters.m_lowResRasterBands.size() ; ++lowResRasterBandsIdx )
      {      
        lowResRasterBandIdx = m_inputParameters.m_lowResRasterBands[ lowResRasterBandsIdx ];
        inNoDataValue = m_inputParameters.m_lowResRasterPtr->getBand( lowResRasterBandIdx )->getProperty()->m_noDataValue;
        outNoDataValue = ressampledRaster.getBand( lowResRasterBandsIdx )->getProperty()->m_noDataValue;
        
        for( outRow = 0 ; outRow < outNRows ; ++outRow )
        {
          inRow = ((double)outRow) * rowsRescaleFactor;
          
          for( outCol = 0 ; outCol < outNCols ; ++outCol )
          {
            inCol = ((double)outCol) * colsRescaleFactor;
            
            interpol.getValue( inCol, inRow, value, lowResRasterBandIdx );
            
            if( value.real() == inNoDataValue )
            {
              ressampledRaster.setValue( outCol, outRow, outNoDataValue, lowResRasterBandsIdx );
            }
            else
            {
              ressampledRaster.setValue( outCol, outRow, value.real(), lowResRasterBandsIdx );
            }
          }
        }
      }
     
      return true; 
    }
    
    bool PCAFusion::swapBandByHighResRaster( te::rst::Raster& pcaRaster, const unsigned int pcaRasterBandIdx )
    {
      assert( pcaRaster.getNumberOfColumns() == m_inputParameters.m_highResRasterPtr->getNumberOfColumns() );
      assert( pcaRaster.getNumberOfRows() == m_inputParameters.m_highResRasterPtr->getNumberOfRows() );
      
      assert( pcaRasterBandIdx < pcaRaster.getNumberOfBands() );
      te::rst::Band& pcaBand = (*pcaRaster.getBand( pcaRasterBandIdx ));
      const te::rst::Band& hrBand = (*m_inputParameters.m_highResRasterPtr->getBand( m_inputParameters.m_highResRasterBand ));
        
      double pcaZeroMean = 0.0;
      if( ! te::rp::GetMeanValue( pcaBand, m_inputParameters.m_enableThreadedProcessing ?
         0 : 1, pcaZeroMean ) )
      {
        return false;
      }
      
      double pcaZeroStdDev = 0.0;
      if( ! te::rp::GetStdDevValue( pcaBand, m_inputParameters.m_enableThreadedProcessing ?
         0 : 1, &pcaZeroMean, pcaZeroStdDev ) )
      {
        return false;
      }
      
      double hrMean = 0.0;
      if( ! te::rp::GetMeanValue( hrBand, m_inputParameters.m_enableThreadedProcessing ? 0 : 1, hrMean ) )
      {
        return false;
      }
      
      double hrStdDev = 0.0;
      if( ! te::rp::GetStdDevValue( hrBand, m_inputParameters.m_enableThreadedProcessing ? 0 : 1, &hrMean, hrStdDev ) )
      {
        return false;
      }
      
      const double gain = ( ( hrStdDev == 0.0 ) ? 0.0 : ( pcaZeroStdDev / hrStdDev ) );
      const double& pcaNoDataValue = pcaBand.getProperty()->m_noDataValue;
      const double& hrNoDataValue = hrBand.getProperty()->m_noDataValue;
      const unsigned int nRows = pcaRaster.getNumberOfRows();
      const unsigned int nCols = pcaRaster.getNumberOfColumns();
      unsigned int col = 0;
      unsigned int row = 0;
      double value = 0;
      double pcaAllowedMin = 0;
      double pcaAllowedMax = 0;
      te::rp::GetDataTypeRange( pcaBand.getProperty()->getType(), pcaAllowedMin,
        pcaAllowedMax );
      
      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          hrBand.getValue( col, row, value );
          if( value == hrNoDataValue )
          {
            pcaBand.setValue( col, row, pcaNoDataValue );
          }
          else
          {
            value = ( ( value - hrMean ) * gain ) + pcaZeroMean;
            value = std::max( pcaAllowedMin, value );
            value = std::min( pcaAllowedMax, value );
            
            pcaBand.setValue( col, row, value );
          }
        }
      }      
        
      return true;
    }
    
  }
}   // end namespace te

