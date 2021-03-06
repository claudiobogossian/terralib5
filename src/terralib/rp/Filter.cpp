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
  \file terralib/rp/Filter.cpp
  \brief A series of well-known filtering algorithms for images, linear and non-linear.
*/

#include "Filter.h"
#include "Macros.h"
#include "Functions.h"
#include "../statistics/core/SummaryFunctions.h"
#include "../raster/RasterFactory.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/Band.h"
#include "../raster/BandIterator.h"
#include "../raster/BandIteratorWindow.h"
#include "../common/progress/TaskProgress.h"

#include <memory>
#include <cmath>

namespace te
{
  namespace rp
  {

    Filter::InputParameters::InputParameters()
    {
      reset();
    }

    Filter::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    Filter::InputParameters::~InputParameters()
    {
      reset();
    }

    void Filter::InputParameters::reset() throw( te::rp::Exception )
    {
      m_filterType = InputParameters::InvalidFilterT;
      m_inRasterPtr = 0;
      m_inRasterBands.clear();
      m_iterationsNumber = 1;
      m_windowH = 3;
      m_windowW = 3;
      m_enableProgress = false;
      m_window.clear();
    }

    const Filter::InputParameters& Filter::InputParameters::operator=(
      const Filter::InputParameters& params )
    {
      reset();

      m_filterType = params.m_filterType;
      m_inRasterPtr = params.m_inRasterPtr;
      m_inRasterBands = params.m_inRasterBands;
      m_iterationsNumber = params.m_iterationsNumber;
      m_windowH = params.m_windowH;
      m_windowW = params.m_windowW;
      m_enableProgress = params.m_enableProgress;

      if (m_filterType == InputParameters::UserDefinedWindowT)
        m_window = params.m_window;

      return *this;
    }

    te::common::AbstractParameters* Filter::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    Filter::OutputParameters::OutputParameters()
    {
      reset();
    }

    Filter::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    Filter::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void Filter::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const Filter::OutputParameters& Filter::OutputParameters::operator=(
      const Filter::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* Filter::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    Filter::Filter()
    {
      reset();
    }

    Filter::~Filter()
    {
      if (m_convBuffer != 0) {
        for (unsigned int line = 0; line < m_convBufferLines; ++line) {
          delete[] m_convBuffer[line];
        }

        delete[] m_convBuffer;

        m_convBuffer = 0;
        m_convBufferLines = 0;
        m_convBufferColumns = 0;
      }
    }

    bool Filter::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      Filter::OutputParameters* outParamsPtr = dynamic_cast<
        Filter::OutputParameters* >( &outputParams );
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
          const unsigned int& inRasterBandIndex =
            m_inputParameters.m_inRasterBands[ inRasterBandsIdx ];

          assert( inRasterBandIndex <
            m_inputParameters.m_inRasterPtr->getNumberOfBands() );

          outRasterBandsProperties.push_back( new te::rst::BandProperty(
            *( m_inputParameters.m_inRasterPtr->getBand( inRasterBandIndex )->getProperty() ) ) );

          if( m_inputParameters.m_iterationsNumber > 1 )
          {
            bufferRaster1BandsProperties.push_back( new te::rst::BandProperty(
              *outRasterBandsProperties[ inRasterBandsIdx ] ) );
            bufferRaster1BandsProperties[ inRasterBandsIdx ]->m_type =
              te::dt::DOUBLE_TYPE;
          }

          if( m_inputParameters.m_iterationsNumber > 2 )
          {
            bufferRaster2BandsProperties.push_back( new te::rst::BandProperty(
              *outRasterBandsProperties[ inRasterBandsIdx ] ) );
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
          filterPointer = &Filter::RobertsFilter;
          break;
        }
        case InputParameters::SobelFilterT :
        {
          filterPointer = &Filter::SobelFilter;
          break;
        }
        case InputParameters::MeanFilterT :
        {
          filterPointer = &Filter::MeanFilter;
          break;
        }
        case InputParameters::ModeFilterT :
        {
          filterPointer = &Filter::ModeFilter;
          break;
        }
        case InputParameters::MedianFilterT :
        {
          filterPointer = &Filter::MedianFilter;
          break;
        }
        case InputParameters::DilationFilterT :
        {
          filterPointer = &Filter::DilationFilter;
          break;
        }
        case InputParameters::ErosionFilterT :
        {
          filterPointer = &Filter::ErosionFilter;
          break;
        }
        case InputParameters::UserDefinedWindowT :
        {
          filterPointer = &Filter::UserDefinedFilter;
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid filter type" );
          break;
        }
      }

      const bool useGlobalProgress = ( ( m_inputParameters.m_iterationsNumber *
        m_inputParameters.m_inRasterBands.size() ) != 1 );

      std::auto_ptr< te::common::TaskProgress > task;
      if( m_inputParameters.m_enableProgress && useGlobalProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("Filtering"),
          te::common::TaskProgress::UNDEFINED,
         ( m_inputParameters.m_iterationsNumber *
          m_inputParameters.m_inRasterBands.size() ) ) );
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
          if( (this->*(filterPointer))( *srcRasterPtr, ( iteration == 0 ) ?
            m_inputParameters.m_inRasterBands[ inRasterBandsIdx ] :
            inRasterBandsIdx, *dstRasterPtr, inRasterBandsIdx,
            ( m_inputParameters.m_enableProgress && (!useGlobalProgress) ) ) ==
            false )
          {
            TERP_LOG_AND_RETURN_FALSE( TE_TR( "Filter error" ) );
          }
        }

      }

      return true;
    }

    void Filter::reset() throw( te::rp::Exception )
    {
      m_isInitialized = false;
      m_inputParameters.reset();

      m_convBuffer = 0;
      m_convBufferLines = 0;
      m_convBufferColumns = 0;
    }

    bool Filter::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      Filter::InputParameters const* inputParamsPtr = dynamic_cast<
        Filter::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr, TE_TR( "Invalid parameters" ) );


      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_filterType !=
        InputParameters::InvalidFilterT,
        TE_TR( "Invalid filter type" ) );

      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRasterPtr,
        TE_TR( "Invalid raster pointer" ) );
      TERP_TRUE_OR_RETURN_FALSE(
        inputParamsPtr->m_inRasterPtr->getAccessPolicy() & te::common::RAccess,
        TE_TR( "Invalid raster" ) );

      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRasterBands.size() > 0,
        TE_TR( "Invalid raster bands number" ) );
      for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
        inputParamsPtr->m_inRasterBands.size() ; ++inRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE(
          inputParamsPtr->m_inRasterBands[ inRasterBandsIdx ] <
          inputParamsPtr->m_inRasterPtr->getNumberOfBands(),
          TE_TR( "Invalid raster bands" ) );
      }

      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_iterationsNumber > 0,
        TE_TR( "Invalid iterations number" ) );

      TERP_TRUE_OR_RETURN_FALSE( ( inputParamsPtr->m_windowH > 2 ) &&
        ( ( inputParamsPtr->m_windowH % 2 ) != 0 ),
        TE_TR( "Invalid mask window height" ) );
      
      TERP_TRUE_OR_RETURN_FALSE( 
        ( 
          inputParamsPtr->m_windowH 
          <= 
          inputParamsPtr->m_inRasterPtr->getNumberOfRows() 
        ) 
        &&
        ( 
          inputParamsPtr->m_windowW 
          <= 
          inputParamsPtr->m_inRasterPtr->getNumberOfColumns()
        ),
        TE_TR( "Invalid mask window sizes" ) );      

      TERP_TRUE_OR_RETURN_FALSE( ( inputParamsPtr->m_windowW > 2 ) &&
        ( ( inputParamsPtr->m_windowW % 2 ) != 0 ),
        TE_TR( "Invalid mask window width" ) );

      if( inputParamsPtr->m_filterType == InputParameters::UserDefinedWindowT )
      {
        TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_window.size1() ==
          inputParamsPtr->m_window.size2(),
          TE_TR( "Invalid convolution matrix" ) );

        TERP_TRUE_OR_RETURN_FALSE( ( ( inputParamsPtr->m_window.size1() % 2 ) != 0 ),
          TE_TR( "Invalid convolution matrix" ) );
      }

      m_inputParameters = *inputParamsPtr;
      m_isInitialized = true;

      return true;
    }

    bool Filter::isInitialized() const
    {
      return m_isInitialized;
    }

    bool Filter::RobertsFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx, const bool useProgress )
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

      std::auto_ptr< te::common::TaskProgress > task;
      if( useProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("Mean Filter"),
          te::common::TaskProgress::UNDEFINED,
         srcRaster.getNumberOfRows() - 2) );
      }

      const te::rst::Band& srcBand = *srcRaster.getBand( srcBandIdx );
      te::rst::Band& dstBand = *dstRaster.getBand( dstBandIdx );

      const double srcNoDataValue = srcBand.getProperty()->m_noDataValue;
      const double dstNoDataValue = dstBand.getProperty()->m_noDataValue;

      double dstBandAllowedMin = 0;
      double dstBandAllowedMax = 0;
      te::rp::GetDataTypeRange( dstBand.getProperty()->getType(), dstBandAllowedMin,
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

        if( useProgress )
        {
          task->pulse();
          if( !task->isActive() ) return false;
        }
      }

      return true;
    }

    bool Filter::SobelFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx, const bool useProgress )
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
      const unsigned int rowsBound = (unsigned int)( nRows - 2 );

      const unsigned int nCols = (unsigned int)( srcRaster.getNumberOfColumns() );
      const unsigned int colsBound = (unsigned int)( nCols - 2 );

      ResetConvBuffer(3, nCols);

      std::auto_ptr< te::common::TaskProgress > task;
      if( useProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("Sobel Filter"),
          te::common::TaskProgress::UNDEFINED,
         srcRaster.getNumberOfRows() - 2) );
      }

      const te::rst::Band& srcBand = *srcRaster.getBand( srcBandIdx );
      te::rst::Band& dstBand = *dstRaster.getBand( dstBandIdx );

      double dstBandAllowedMin = 0;
      double dstBandAllowedMax = 0;
      te::rp::GetDataTypeRange( dstBand.getProperty()->getType(), dstBandAllowedMin,
        dstBandAllowedMax );

      unsigned int col = 0;
      double gY = 0;
      double gX = 0;
      double outValue = 0;

      /* Fills the convolution buffer with the first "mask_lines" from the raster */

      for (unsigned int line = 0; line < 2; ++line) {
        UpdateConvBuffer(srcBand, line);
      }

      /* raster convolution */

      for( unsigned int row = 0; row < rowsBound ; ++row )
      {
        /* Getting one more line from the source raster and adding to buffer */
        UpdateConvBuffer(srcBand, row + 2);

        for( col = 0 ; col < colsBound ; ++col )
        {
          gX = m_convBuffer[2][col] +
            (2 * m_convBuffer[2][col + 1]) +
            m_convBuffer[2][col + 2] -
            m_convBuffer[0][col] -
            (2 * m_convBuffer[0][col + 1]) -
            m_convBuffer[0][col + 2];

          gY = m_convBuffer[0][col + 2] +
            (2 * m_convBuffer[1][col + 2]) +
            m_convBuffer[2][col + 2] -
            m_convBuffer[0][col] -
            (2 * m_convBuffer[1][col]) -
            m_convBuffer[2][col];

          outValue = std::sqrt( ( gY * gY ) +
            ( gX * gX ) );
          outValue = std::max( outValue, dstBandAllowedMin );
          outValue = std::min( outValue, dstBandAllowedMax );

          dstBand.setValue( col, row, outValue );
        }

        if( useProgress )
        {
          task->pulse();
          if( !task->isActive() ) return false;
        }
      }

      return true;
    }

    bool Filter::MeanFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx, const bool useProgress )
    {
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfColumns() ==
        dstRaster.getNumberOfColumns(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfRows() ==
        dstRaster.getNumberOfRows(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcBandIdx < srcRaster.getNumberOfBands(),
        "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( dstBandIdx < dstRaster.getNumberOfBands(),
        "Internal error" );

      const unsigned int nRows = srcRaster.getNumberOfRows();
      const unsigned int nCols = srcRaster.getNumberOfColumns();
      const unsigned int windowHeight = m_inputParameters.m_windowH;
      const unsigned int windowWidth = m_inputParameters.m_windowW;
      const unsigned int windowRowRadius = ( windowHeight / 2 );
      const unsigned int windowColRadius = ( windowWidth / 2 );
      const unsigned int filterStartRowOffsetBound = nRows - windowHeight + 1;
      const unsigned int filterStartColOffsetBound = nCols - windowWidth + 1;
      const unsigned int validDataRowsBound = nRows - windowRowRadius;
      const unsigned int validDataColsBound = nCols - windowColRadius;
      unsigned int filterStartRow = 0;
      unsigned int filterStartCol = 0;
      unsigned int rowOffset = 0;
      unsigned int colOffset = 0;
      std::map< double , unsigned int > frequencies;
      const te::rst::Band& srcBand = *( srcRaster.getBand( srcBandIdx ) );
      te::rst::Band& dstBand = *( dstRaster.getBand( dstBandIdx ) );
      const double srcNoDataValue = srcBand.getProperty()->m_noDataValue;
      const double dstNoDataValue = dstBand.getProperty()->m_noDataValue;
      double value = 0;
      unsigned int row = 0;
      unsigned int col = 0;
      std::map< double , unsigned int >::iterator frequenciesIt;
      std::map< double , unsigned int >::iterator frequenciesItEnd;
      double meanValue = 0;
      unsigned int meanValidPixelsNmb = 0;
      
      std::auto_ptr< te::common::TaskProgress > task;
      if( useProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("Mode Filter"),
          te::common::TaskProgress::UNDEFINED, filterStartRowOffsetBound + nRows ) );
      }      
      
      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          if( ( row < windowRowRadius ) || ( row >= validDataRowsBound ) ||
            ( col < windowColRadius ) || ( col >= validDataColsBound ) )
          {
            dstBand.setValue( col, row, dstNoDataValue );
          }
        }
        
        if( useProgress )
        {
          task->pulse();
          if( !task->isActive() ) return false;
        }         
      }
      
      for( filterStartRow = 0 ; filterStartRow < filterStartRowOffsetBound ; ++filterStartRow )
      {
        for( filterStartCol = 0 ; filterStartCol < filterStartColOffsetBound ; ++filterStartCol )
        {
          meanValue = 0;
          meanValidPixelsNmb = 0;
          
          for( rowOffset = 0 ; rowOffset < windowHeight ; ++rowOffset )
          {
            row = filterStartRow + rowOffset;
            
            for( colOffset = 0 ; colOffset < windowWidth ; ++colOffset )
            {
              col = filterStartCol + colOffset;
              srcBand.getValue( col, row, value );
              if( value != srcNoDataValue )
              {
                meanValue += value;
                ++meanValidPixelsNmb;
              }
            }
          }
          
          if( meanValidPixelsNmb )
          {
            meanValue /= (double)meanValidPixelsNmb;
            dstBand.setValue( filterStartCol + windowColRadius, 
              filterStartRow + windowRowRadius, meanValue );            
          }
          else
          {
            dstBand.setValue( filterStartCol + windowColRadius, 
              filterStartRow + windowRowRadius, dstNoDataValue );
          }
        }
        
        if( useProgress )
        {
          task->pulse();
          if( !task->isActive() ) return false;
        }        
      }

      return true;
    }

    bool Filter::ModeFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx, const bool useProgress )
    {
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfColumns() ==
        dstRaster.getNumberOfColumns(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfRows() ==
        dstRaster.getNumberOfRows(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcBandIdx < srcRaster.getNumberOfBands(),
        "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( dstBandIdx < dstRaster.getNumberOfBands(),
        "Internal error" );

      const unsigned int nRows = srcRaster.getNumberOfRows();
      const unsigned int nCols = srcRaster.getNumberOfColumns();
      const unsigned int windowHeight = m_inputParameters.m_windowH;
      const unsigned int windowWidth = m_inputParameters.m_windowW;
      const unsigned int windowRowRadius = ( windowHeight / 2 );
      const unsigned int windowColRadius = ( windowWidth / 2 );
      const unsigned int filterStartRowOffsetBound = nRows - windowHeight + 1;
      const unsigned int filterStartColOffsetBound = nCols - windowWidth + 1;
      const unsigned int validDataRowsBound = nRows - windowRowRadius;
      const unsigned int validDataColsBound = nCols - windowColRadius;
      unsigned int filterStartRow = 0;
      unsigned int filterStartCol = 0;
      unsigned int rowOffset = 0;
      unsigned int colOffset = 0;
      std::map< double , unsigned int > frequencies;
      const te::rst::Band& srcBand = *( srcRaster.getBand( srcBandIdx ) );
      te::rst::Band& dstBand = *( dstRaster.getBand( dstBandIdx ) );
      const double srcNoDataValue = srcBand.getProperty()->m_noDataValue;
      const double dstNoDataValue = dstBand.getProperty()->m_noDataValue;
      double value = 0;
      unsigned int row = 0;
      unsigned int col = 0;
      std::map< double , unsigned int >::iterator frequenciesIt;
      std::map< double , unsigned int >::iterator frequenciesItEnd;
      unsigned int higherFrequency = 0;
      double higherFrequencyValue = 0;
      
      std::auto_ptr< te::common::TaskProgress > task;
      if( useProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("Mode Filter"),
          te::common::TaskProgress::UNDEFINED, filterStartRowOffsetBound + nRows ) );
      }      
      
      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          if( ( row < windowRowRadius ) || ( row >= validDataRowsBound ) ||
            ( col < windowColRadius ) || ( col >= validDataColsBound ) )
          {
            dstBand.setValue( col, row, dstNoDataValue );
          }
        }
        
        if( useProgress )
        {
          task->pulse();
          if( !task->isActive() ) return false;
        }         
      }
      
      for( filterStartRow = 0 ; filterStartRow < filterStartRowOffsetBound ; ++filterStartRow )
      {
        for( filterStartCol = 0 ; filterStartCol < filterStartColOffsetBound ; ++filterStartCol )
        {
          frequencies.clear();
          
          for( rowOffset = 0 ; rowOffset < windowHeight ; ++rowOffset )
          {
            row = filterStartRow + rowOffset;
            
            for( colOffset = 0 ; colOffset < windowWidth ; ++colOffset )
            {
              col = filterStartCol + colOffset;
              srcBand.getValue( col, row, value );
              if( value != srcNoDataValue )
              {
                ++frequencies[ value ];
              }
            }
          }
          
          frequenciesIt = frequencies.begin();
          frequenciesItEnd = frequencies.end();
          higherFrequency = 0;
          while( frequenciesIt != frequenciesItEnd )
          {
            if( frequenciesIt->second > higherFrequency )
            {
              higherFrequency = frequenciesIt->second;
              higherFrequencyValue = frequenciesIt->first;              
            }
            
            ++frequenciesIt;
          }
          
          if( higherFrequency == 0.0 )
          {
            dstBand.setValue( filterStartCol + windowColRadius, 
              filterStartRow + windowRowRadius, dstNoDataValue );
          }
          else
          {
            dstBand.setValue( filterStartCol + windowColRadius, 
              filterStartRow + windowRowRadius, higherFrequencyValue );
          }
        }
        
        if( useProgress )
        {
          task->pulse();
          if( !task->isActive() ) return false;
        }        
      }

      return true;
    }

    bool Filter::MedianFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx, const bool useProgress )
    {
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfColumns() ==
        dstRaster.getNumberOfColumns(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfRows() ==
        dstRaster.getNumberOfRows(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcBandIdx < srcRaster.getNumberOfBands(),
        "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( dstBandIdx < dstRaster.getNumberOfBands(),
        "Internal error" );

      const unsigned int H = m_inputParameters.m_windowH;
      const unsigned int W = m_inputParameters.m_windowW;

      std::auto_ptr< te::common::TaskProgress > task;
      if( useProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("Median Filter"),
          te::common::TaskProgress::UNDEFINED, 100 ) );
      }

      unsigned int R = 0;
      unsigned int C = 0;
      const unsigned int MR = srcRaster.getNumberOfRows();
      const unsigned int MC = srcRaster.getNumberOfColumns();

      double dstBandAllowedMin = 0;
      double dstBandAllowedMax = 0;
      te::rp::GetDataTypeRange( dstRaster.getBand( dstBandIdx )->getProperty()->getType(), dstBandAllowedMin,
        dstBandAllowedMax );

      std::vector<double> pixels_in_window;
      double pixel_median = 0.0;

      te::rst::Band const * const band = srcRaster.getBand(srcBandIdx);
      te::rst::BandIteratorWindow<double> it = te::rst::BandIteratorWindow<double>::begin(band, W, H);
      te::rst::BandIteratorWindow<double> itend = te::rst::BandIteratorWindow<double>::end(band, W, H);

      while (it != itend)
      {
        R = it.getRow();
        C = it.getColumn();

        if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
            (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
        {
          pixel_median = 0.0;
          pixels_in_window.clear();
          for (int r = -1 * (int) (H / 2); r <= (int) (H / 2); r++)
            for (int c = -1 * (int) (W / 2); c <= (int) (W / 2); c++)
              pixels_in_window.push_back(it.getValue(c, r));

          std::sort(pixels_in_window.begin(), pixels_in_window.end(), OrderFunction);
          pixel_median = pixels_in_window[pixels_in_window.size() / 2];
        }
        else
          pixel_median = it.getValue();

        pixel_median = std::max( pixel_median, dstBandAllowedMin );
        pixel_median = std::min( pixel_median, dstBandAllowedMax );
        dstRaster.setValue(C, R, pixel_median, dstBandIdx);

        if( useProgress )
        {
          task->setCurrentStep( R / srcRaster.getNumberOfRows() );
          if( !task->isActive() ) return false;
        }

        ++it;
      }

      return true;
    }

    bool Filter::DilationFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx, const bool useProgress )
    {
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfColumns() ==
        dstRaster.getNumberOfColumns(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfRows() ==
        dstRaster.getNumberOfRows(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcBandIdx < srcRaster.getNumberOfBands(),
        "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( dstBandIdx < dstRaster.getNumberOfBands(),
        "Internal error" );

      const unsigned int H = m_inputParameters.m_windowH;
      const unsigned int W = m_inputParameters.m_windowW;

      std::auto_ptr< te::common::TaskProgress > task;
      if( useProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("Dilation Filter"),
          te::common::TaskProgress::UNDEFINED, 100 ) );
      }

      unsigned int R = 0;
      unsigned int C = 0;
      const unsigned int MR = srcRaster.getNumberOfRows();
      const unsigned int MC = srcRaster.getNumberOfColumns();

      double dstBandAllowedMin = 0;
      double dstBandAllowedMax = 0;
      te::rp::GetDataTypeRange( dstRaster.getBand( dstBandIdx )->getProperty()->getType(), dstBandAllowedMin,
        dstBandAllowedMax );

      double pixel_dilation = 0;
      double pixel = 0;

      te::rst::Band const * const band = srcRaster.getBand(srcBandIdx);
      te::rst::BandIteratorWindow<double> it = te::rst::BandIteratorWindow<double>::begin(band, W, H);
      te::rst::BandIteratorWindow<double> itend = te::rst::BandIteratorWindow<double>::end(band, W, H);

      while (it != itend)
      {
        R = it.getRow();
        C = it.getColumn();

        if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
            (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
        {
          pixel_dilation = -1.0 * std::numeric_limits<double>::max();
          for (int r = -1 * (int) (H / 2); r <= (int) (H / 2); r++)
            for (int c = -1 * (int) (W / 2); c <= (int) (W / 2); c++)
            {
              pixel = it.getValue(c, r);
              if (pixel > pixel_dilation)
                pixel_dilation = pixel;
            }
        }
        else
          pixel_dilation = it.getValue();

        pixel_dilation = std::max( pixel_dilation, dstBandAllowedMin );
        pixel_dilation = std::min( pixel_dilation, dstBandAllowedMax );
        dstRaster.setValue(C, R, pixel_dilation, dstBandIdx);

        if( useProgress )
        {
          task->setCurrentStep( R / srcRaster.getNumberOfRows() );
          if( !task->isActive() ) return false;
        }

        ++it;
      }

      return true;
    }

    bool Filter::ErosionFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx, const bool useProgress )
    {
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfColumns() ==
        dstRaster.getNumberOfColumns(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfRows() ==
        dstRaster.getNumberOfRows(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcBandIdx < srcRaster.getNumberOfBands(),
        "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( dstBandIdx < dstRaster.getNumberOfBands(),
        "Internal error" );

      const unsigned int H = m_inputParameters.m_windowH;
      const unsigned int W = m_inputParameters.m_windowW;

      std::auto_ptr< te::common::TaskProgress > task;
      if( useProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("Erosion Filter"),
          te::common::TaskProgress::UNDEFINED, 100 ) );
      }

      unsigned int R = 0;
      unsigned int C = 0;
      const unsigned int MR = srcRaster.getNumberOfRows();
      const unsigned int MC = srcRaster.getNumberOfColumns();

      double dstBandAllowedMin = 0;
      double dstBandAllowedMax = 0;
      te::rp::GetDataTypeRange( dstRaster.getBand( dstBandIdx )->getProperty()->getType(), dstBandAllowedMin,
        dstBandAllowedMax );

      double pixel_erosion = 0;
      double pixel = 0;

      te::rst::Band const * const band = srcRaster.getBand(srcBandIdx);
      te::rst::BandIteratorWindow<double> it = te::rst::BandIteratorWindow<double>::begin(band, W, H);
      te::rst::BandIteratorWindow<double> itend = te::rst::BandIteratorWindow<double>::end(band, W, H);

      while (it != itend)
      {
        R = it.getRow();
        C = it.getColumn();

        if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
            (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
        {
          pixel_erosion = std::numeric_limits<double>::max();
          for (int r = -1 * (int) (H / 2); r <= (int) (H / 2); r++)
            for (int c = -1 * (int) (W / 2); c <= (int) (W / 2); c++)
            {
              pixel = it.getValue(c, r);
              if (pixel < pixel_erosion)
                pixel_erosion = pixel;
            }
        }
        else
          pixel_erosion = it.getValue();

        pixel_erosion = std::max( pixel_erosion, dstBandAllowedMin );
        pixel_erosion = std::min( pixel_erosion, dstBandAllowedMax );
        dstRaster.setValue(C, R, pixel_erosion, dstBandIdx);

        if( useProgress )
        {
          task->setCurrentStep( R / srcRaster.getNumberOfRows() );
          if( !task->isActive() ) return false;
        }

        ++it;
      }

      return true;
    }

    bool Filter::UserDefinedFilter( const te::rst::Raster& srcRaster,
      const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
      const unsigned int dstBandIdx, const bool useProgress )
    {
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfColumns() ==
        dstRaster.getNumberOfColumns(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcRaster.getNumberOfRows() ==
        dstRaster.getNumberOfRows(), "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( srcBandIdx < srcRaster.getNumberOfBands(),
        "Internal error" );
      TERP_DEBUG_TRUE_OR_THROW( dstBandIdx < dstRaster.getNumberOfBands(),
        "Internal error" );

      const unsigned int H = m_inputParameters.m_windowH;
      const unsigned int W = m_inputParameters.m_windowW;

      std::auto_ptr< te::common::TaskProgress > task;
      if( useProgress )
      {
        task.reset( new te::common::TaskProgress(TE_TR("User Defined Filter"),
          te::common::TaskProgress::UNDEFINED, 100 ) );
      }

      unsigned int R = 0;
      unsigned int C = 0;
      const unsigned int MR = srcRaster.getNumberOfRows();
      const unsigned int MC = srcRaster.getNumberOfColumns();
      double pixels_in_window = 0;

      double dstBandAllowedMin = 0;
      double dstBandAllowedMax = 0;
      te::rp::GetDataTypeRange( dstRaster.getBand( dstBandIdx )->getProperty()->getType(), dstBandAllowedMin,
        dstBandAllowedMax );

      te::rst::Band const * const band = srcRaster.getBand(srcBandIdx);
      te::rst::BandIteratorWindow<double> it = te::rst::BandIteratorWindow<double>::begin(band, W, H);
      te::rst::BandIteratorWindow<double> itend = te::rst::BandIteratorWindow<double>::end(band, W, H);

      while (it != itend)
      {
        R = it.getRow();
        C = it.getColumn();
        if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
            (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
        {
          pixels_in_window = 0.0;
          for (int r = -1 * (int) (H / 2), rw = 0; r <= (int) (H / 2); r++, rw++)
            for (int c = -1 * (int) (W / 2), cw = 0; c <= (int) (W / 2); c++, cw++)
              pixels_in_window += m_inputParameters.m_window(rw, cw) * it.getValue(c, r);
        }
        else
          pixels_in_window = it.getValue();

        pixels_in_window = std::max( pixels_in_window, dstBandAllowedMin );
        pixels_in_window = std::min( pixels_in_window, dstBandAllowedMax );
        dstRaster.setValue(C, R, pixels_in_window, dstBandIdx);

        if( useProgress )
        {
          task->setCurrentStep( R / srcRaster.getNumberOfRows() );
          if( !task->isActive() ) return false;
        }

        ++it;
      }

      return true;
    }

    bool Filter::OrderFunction(double i, double j)
    {
      return (i < j);
    }

    void Filter::ResetConvBuffer(unsigned int lines, unsigned int columns)
    {
      if (m_convBuffer != 0) {
        for (unsigned int line = 0; line < m_convBufferLines; ++line) {
          delete[] m_convBuffer[line];
        }

        delete[] m_convBuffer;

        m_convBuffer = 0;
        m_convBufferLines = 0;
        m_convBufferColumns = 0;
      }

      if ((lines > 0) && (columns > 0)) {
        m_convBuffer = new double*[lines];

        TERP_DEBUG_TRUE_OR_THROW(m_convBuffer != 0, "Memory allocation error");

        for (unsigned int line = 0; line < lines; ++line) {
          m_convBuffer[line] = new double[columns];

          TERP_DEBUG_TRUE_OR_THROW(m_convBuffer[line] != 0, "Memory allocation error");
        }

        m_convBufferLines = lines;
        m_convBufferColumns = columns;
      }
    }

    void Filter::UpdateConvBuffer(const te::rst::Band& inRaster, unsigned int line)
    {
      TERP_DEBUG_TRUE_OR_THROW((inRaster.getRaster()->getNumberOfRows() > (int)line),
        "Trying to get a non existent line from raster");
      TERP_DEBUG_TRUE_OR_THROW((inRaster.getRaster()->getNumberOfColumns() ==
        (int)m_convBufferColumns),
        "Buffer columns number not equal to raster columns");


      /* Buffer roll up */

      ConvBufferRoolup(1);

      /* Updating the last line */

      unsigned int convBufferLastLine = m_convBufferLines - 1;

      for (unsigned int bufcolumn = 0; bufcolumn < m_convBufferColumns; ++bufcolumn) {
        inRaster.getValue(bufcolumn, line,
          m_convBuffer[convBufferLastLine][bufcolumn]);
      }
    }

    void Filter::ConvBufferRoolup(unsigned int count)
    {
      TERP_DEBUG_TRUE_OR_THROW((m_convBufferLines > 0), "Invalid convolution buffer lines");

      double* firstBufferLinePtr;
      unsigned int convBufferLastLine = m_convBufferLines - 1;
      unsigned int bufline;

      for (unsigned int curCount = 0; curCount < count; ++curCount) {
        firstBufferLinePtr = m_convBuffer[0];

        for (bufline = 1; bufline < m_convBufferLines; ++bufline) {
          m_convBuffer[bufline - 1] = m_convBuffer[bufline];
        }

        m_convBuffer[convBufferLastLine] = firstBufferLinePtr;
      }
    }
  } // end namespace rp
}   // end namespace te

