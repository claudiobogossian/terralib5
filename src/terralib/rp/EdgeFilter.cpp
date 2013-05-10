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
  \file terralib/rp/EdgeFilter.cpp
  \brief Tie points locator.
*/

#include "EdgeFilter.h"
#include "Macros.h"
#include "Functions.h"
#include "../raster/RasterFactory.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/Band.h"

#include <memory>
#include <cmath>

namespace te
{
  namespace rp
  {

    EdgeFilter::InputParameters::InputParameters()
    {
      reset();
    }
    
    EdgeFilter::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }    

    EdgeFilter::InputParameters::~InputParameters()
    {
      reset();
    }

    void EdgeFilter::InputParameters::reset() throw( te::rp::Exception )
    {
      m_filterType = InputParameters::InvalidFilterT;
      m_inRasterPtr = 0;
      m_inRasterBands.clear();
      m_iterationsNumber = 1;
    }

    const EdgeFilter::InputParameters& EdgeFilter::InputParameters::operator=(
      const EdgeFilter::InputParameters& params )
    {
      reset();
      
      m_filterType = params.m_filterType;
      m_inRasterPtr = params.m_inRasterPtr;
      m_inRasterBands = params.m_inRasterBands;
      m_iterationsNumber = params.m_iterationsNumber;

      return *this;
    }

    te::common::AbstractParameters* EdgeFilter::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    EdgeFilter::OutputParameters::OutputParameters()
    {
      reset();
    }

    EdgeFilter::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    EdgeFilter::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void EdgeFilter::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const EdgeFilter::OutputParameters& EdgeFilter::OutputParameters::operator=(
      const EdgeFilter::OutputParameters& params )
    {
      reset();
      
      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* EdgeFilter::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    EdgeFilter::EdgeFilter()
    {
      reset();
    }

    EdgeFilter::~EdgeFilter()
    {
    }
    
    bool EdgeFilter::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;
      
      EdgeFilter::OutputParameters* outParamsPtr = dynamic_cast<
        EdgeFilter::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // Initializing the output rasters
      
      std::auto_ptr< te::rst::Raster > bufferRaster1Ptr;
      std::auto_ptr< te::rst::Raster > bufferRaster2Ptr;
      
      {
        std::vector< te::rst::BandProperty* > outRasterBandsProperties;
        std::vector< te::rst::BandProperty* > bufferRaster1BandsProperties;
        std::vector< te::rst::BandProperty* > bufferRaster2BandsProperties;
        
        for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
          m_inputParameters.m_inRasterBands.size() ; ++inRasterBandsIdx )
        {
          assert( m_inputParameters.m_inRasterBands[ inRasterBandsIdx ] <
            m_inputParameters.m_inRasterPtr->getNumberOfBands() );
            
          outRasterBandsProperties.push_back( new te::rst::BandProperty(
            *( m_inputParameters.m_inRasterPtr->getBand(
            m_inputParameters.m_inRasterBands[
            inRasterBandsIdx ] )->getProperty() ) ) );
            
          if( m_inputParameters.m_iterationsNumber > 1 )
          {
            bufferRaster1BandsProperties.push_back( new te::rst::BandProperty(
              *( m_inputParameters.m_inRasterPtr->getBand(
              m_inputParameters.m_inRasterBands[
              inRasterBandsIdx ] )->getProperty() ) ) );
            bufferRaster1BandsProperties[ inRasterBandsIdx ]->m_type =
              te::dt::DOUBLE_TYPE;
          }
            
          if( m_inputParameters.m_iterationsNumber > 2 )
          {
            bufferRaster2BandsProperties.push_back( new te::rst::BandProperty(
              *( m_inputParameters.m_inRasterPtr->getBand(
              m_inputParameters.m_inRasterBands[
              inRasterBandsIdx ] )->getProperty() ) ) );             
            bufferRaster2BandsProperties[ inRasterBandsIdx ]->m_type =
              te::dt::DOUBLE_TYPE;
          }
        }

        outParamsPtr->m_outputRasterPtr.reset(
          te::rst::RasterFactory::make(
            outParamsPtr->m_rType,
            new te::rst::Grid( *( m_inputParameters.m_inRasterPtr->getGrid() ) ),
            outRasterBandsProperties,
            outParamsPtr->m_rInfo,
            0,
            0 ) );
        TERP_TRUE_OR_RETURN_FALSE( outParamsPtr->m_outputRasterPtr.get(),
          "Output raster creation error" );
          
        std::map< std::string, std::string > dummyRInfo;

        if( m_inputParameters.m_iterationsNumber > 1 )
        {
          bufferRaster1Ptr.reset(
            te::rst::RasterFactory::make(
              "MEM",
              new te::rst::Grid( *( m_inputParameters.m_inRasterPtr->getGrid() ) ),
              bufferRaster1BandsProperties,
              dummyRInfo,
              0,
              0 ) );
          TERP_TRUE_OR_RETURN_FALSE( bufferRaster1Ptr.get(),
            "Output raster creation error" );
        }
        
        if( m_inputParameters.m_iterationsNumber > 2 )
        {
          bufferRaster2Ptr.reset(
            te::rst::RasterFactory::make(
              "MEM",
              new te::rst::Grid( *( m_inputParameters.m_inRasterPtr->getGrid() ) ),
              bufferRaster2BandsProperties,
              dummyRInfo,
              0,
              0 ) );
          TERP_TRUE_OR_RETURN_FALSE( bufferRaster2Ptr.get(),
            "Output raster creation error" );
        }          
      }      
      
      // Filtering
      
      FilterMethodPointerT filterPointer = 0;
      switch( m_inputParameters.m_filterType )
      {
        case InputParameters::RobertsFilterT :
        {
          filterPointer = &EdgeFilter::RobertsFilter;
          break;
        }
        case InputParameters::SobelFilterT :
        {
          filterPointer = &EdgeFilter::SobelFilter;
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid filter type" );
          break;
        }
      }
      
      te::rst::Raster const* srcRasterPtr = 0;
      te::rst::Raster* dstRasterPtr = 0;
      te::rst::Raster const* auxRasterPtr = 0;      
      
      for( unsigned int iteration = 0 ; iteration < 
        m_inputParameters.m_iterationsNumber ; ++iteration )
      {
        // defining the source raster and
        // destination raster
        
        if( iteration == 0 )
        {
          srcRasterPtr = m_inputParameters.m_inRasterPtr;
          
          if( m_inputParameters.m_iterationsNumber == 1 )
          {
            dstRasterPtr = outParamsPtr->m_outputRasterPtr.get();
          }
          else
          {
            dstRasterPtr = bufferRaster1Ptr.get();
          }
        }
        else if( iteration == ( m_inputParameters.m_iterationsNumber - 1 ) )
        {
          srcRasterPtr = dstRasterPtr;
          
          dstRasterPtr = outParamsPtr->m_outputRasterPtr.get();
        }
        else if( iteration == 1 )
        {
          srcRasterPtr = dstRasterPtr;
          
          dstRasterPtr = bufferRaster2Ptr.get();
        }
        else
        {
          auxRasterPtr = srcRasterPtr;
          srcRasterPtr = dstRasterPtr;
          dstRasterPtr = (te::rst::Raster*)auxRasterPtr;
        }
        
        for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
          m_inputParameters.m_inRasterBands.size() ; ++inRasterBandsIdx )
        {
          (this->*(filterPointer))( *srcRasterPtr, ( iteration == 0 ) ? 
            m_inputParameters.m_inRasterBands[ inRasterBandsIdx ] : 
            inRasterBandsIdx, *dstRasterPtr, inRasterBandsIdx );          
        }

      }
      
      return true;
    }
    
    void EdgeFilter::reset() throw( te::rp::Exception )
    {
      m_isInitialized = false;
      m_inputParameters.reset();
    }
    
    bool EdgeFilter::initialize( const AlgorithmInputParameters& inputParams ) 
      throw( te::rp::Exception )
    {
      reset();
      
      EdgeFilter::InputParameters const* inputParamsPtr = dynamic_cast< 
        EdgeFilter::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr, "Invalid parameters" );
      
      
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_filterType != 
        InputParameters::InvalidFilterT,
        "Invalid filter type" );
      
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRasterPtr, 
        "Invalid raster pointer" );
      TERP_TRUE_OR_RETURN_FALSE( 
        inputParamsPtr->m_inRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
        
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRasterBands.size() > 0,
        "Invalid raster bands number" );        
      for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
        inputParamsPtr->m_inRasterBands.size() ; ++inRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          inputParamsPtr->m_inRasterBands[ inRasterBandsIdx ] <
          inputParamsPtr->m_inRasterPtr->getNumberOfBands(), 
          "Invalid raster bands" );
      }
      
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_iterationsNumber > 0,
        "Indalid iterations number" );
          
      m_inputParameters = *inputParamsPtr;
      m_isInitialized = true;
      
      return true;
    }    

    bool EdgeFilter::isInitialized() const
    {
      return m_isInitialized;
    }

    void EdgeFilter::RobertsFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx )
    {
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfColumns() ==
        dstRaster.getNumberOfColumns(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfRows() ==
        dstRaster.getNumberOfRows(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcBandIdx < srcRaster.getNumberOfBands(), 
        "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( dstBandIdx < dstRaster.getNumberOfBands(), 
        "Internal error" );
        
      const unsigned int nRows = (unsigned int)( srcRaster.getNumberOfRows() );
      const unsigned int rowsBound = (unsigned int)( nRows ? 
        ( nRows - 1 ) : 0 );
      
      const unsigned int nCols = (unsigned int)( srcRaster.getNumberOfColumns() );
      const unsigned int colsBound = (unsigned int)( nCols ? 
        ( nCols - 1 ) : 0 );
        
      const te::rst::Band& srcBand = *srcRaster.getBand( srcBandIdx );
      te::rst::Band& dstBand = *dstRaster.getBand( dstBandIdx );
      
      const double srcNoDataValue = srcBand.getProperty()->m_noDataValue;
      const double dstNoDataValue = dstBand.getProperty()->m_noDataValue;
      
      double dstBandAllowedMin = 0;
      double dstBandAllowedMax = 0;
      te::rp::getDataTypeRange( dstBand.getProperty()->getType(), dstBandAllowedMin,
        dstBandAllowedMax );
      
      unsigned int col = 0;
      double value1diag = 0;
      double value2diag = 0;
      double value1adiag = 0;
      double value2adiag = 0;      
      double diagDiff = 0;
      double adiagDiff = 0;
      double outValue = 0;
      
      for( unsigned int row = 0; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          if( ( row < rowsBound ) && ( col < colsBound ) )
          {
            srcBand.getValue( col, row, value1diag );
            if( value1diag == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col + 1, row + 1, value2diag );
            if( value2diag == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col, row + 1, value1adiag );
            if( value1adiag == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col + 1, row, value2adiag );
            if( value2adiag == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            diagDiff = value1diag - value2diag;
            adiagDiff = value1adiag - value2adiag;
            
            outValue = std::sqrt( ( diagDiff * diagDiff ) +
              ( adiagDiff * adiagDiff ) );
            outValue = std::max( outValue, dstBandAllowedMin );
            outValue = std::min( outValue, dstBandAllowedMax );
            
            dstBand.setValue( col, row, outValue );
          }
          else
          {
            dstBand.setValue( col, row, dstNoDataValue );
          }
        }
      }
    }
    
    void EdgeFilter::SobelFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx )
    {
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfColumns() ==
        dstRaster.getNumberOfColumns(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfRows() ==
        dstRaster.getNumberOfRows(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcBandIdx < srcRaster.getNumberOfBands(), 
        "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( dstBandIdx < dstRaster.getNumberOfBands(), 
        "Internal error" );
        
      const unsigned int nRows = (unsigned int)( srcRaster.getNumberOfRows() );
      const unsigned int rowsBound = (unsigned int)( nRows ? 
        ( nRows - 1 ) : 0 );
      
      const unsigned int nCols = (unsigned int)( srcRaster.getNumberOfColumns() );
      const unsigned int colsBound = (unsigned int)( nCols ? 
        ( nCols - 1 ) : 0 );
        
      const te::rst::Band& srcBand = *srcRaster.getBand( srcBandIdx );
      te::rst::Band& dstBand = *dstRaster.getBand( dstBandIdx );
      
      const double srcNoDataValue = srcBand.getProperty()->m_noDataValue;
      const double dstNoDataValue = dstBand.getProperty()->m_noDataValue;
      
      double dstBandAllowedMin = 0;
      double dstBandAllowedMax = 0;
      te::rp::getDataTypeRange( dstBand.getProperty()->getType(), dstBandAllowedMin,
        dstBandAllowedMax );      
      
      unsigned int col = 0;
      double value1 = 0;
      double value2 = 0;
      double value3 = 0;
      double value4 = 0;
      double value5 = 0;
      double value6 = 0;
      double value7 = 0;
      double value8 = 0;
      double gY = 0;
      double gX = 0;
      double outValue = 0;
      
      for( unsigned int row = 0; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          if( ( row > 0 ) && ( col > 0 ) && ( row < rowsBound ) && 
            ( col < colsBound ) )
          {
            srcBand.getValue( col - 1, row - 1, value1 );
            if( value1 == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col, row - 1, value2 );
            if( value2 == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }

            srcBand.getValue( col + 1, row - 1, value3 );
            if( value3 == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col - 1, row, value4 );
            if( value4 == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col + 1, row, value5 );
            if( value5 == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col - 1, row + 1, value6 );
            if( value6 == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col, row + 1, value7 );
            if( value7 == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            srcBand.getValue( col + 1, row + 1, value8 );
            if( value8 == srcNoDataValue )
            {
              dstBand.setValue( col, row, dstNoDataValue );
              continue;
            }
            
            gY = value6 + ( 2.0 * value7 ) + value8 -
              ( value1 + ( 2.0 * value2 ) + value3 );
            gX = value3 + ( 2.0 * value5 ) + value8 -
              ( value1 + ( 2.0 * value4 ) + value6 );
              
            outValue = std::sqrt( ( gY * gY ) +
              ( gX * gX ) );
            outValue = std::max( outValue, dstBandAllowedMin );
            outValue = std::min( outValue, dstBandAllowedMax );              
            
            dstBand.setValue( col, row, outValue );
          }
          else
          {
            dstBand.setValue( col, row, dstNoDataValue );
          }
        }
      }
    }

  } // end namespace rp
}   // end namespace te

