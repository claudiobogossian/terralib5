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
  \file terralib/rp/Contrast.cpp
  \brief Contrast enhancement.
*/

#include "Contrast.h"

#include "RasterHandler.h"
#include "Functions.h"

#include "../raster/Raster.h"
#include "../raster/Band.h"
#include "../raster/BandIterator.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterProperty.h"
#include "../raster/RasterSummaryManager.h"
#include "../raster/RasterFactory.h"
#include "Macros.h"

#include "float.h"
#include <boost/concept_check.hpp>
#include <terralib/raster/Grid.h>

namespace te
{
  namespace rp
  {
    Contrast::InputParameters::InputParameters()
    {
      reset();
    }
    
    Contrast::InputParameters::~InputParameters()
    {
      reset();
    }
    
    void Contrast::InputParameters::reset() throw( te::rp::Exception )
    {
      m_type = InputParameters::InvalidContrastT;
      m_lCMinInput = DBL_MAX;
      m_lCMaxInput = ( -1.0 * DBL_MAX );
      m_hECMaxInput = 0;
      m_sMASCMeanInput = 0;
      m_sMASCStdInput = 0;
      
      m_inRasterPtr = 0;
      m_inRasterBands.clear();
    }
    
    const Contrast::InputParameters& Contrast::InputParameters::operator=( 
      const Contrast::InputParameters& params )
    {
      reset();
        
      m_type = params.m_type;
      m_lCMinInput = params.m_lCMinInput;
      m_lCMaxInput = params.m_lCMaxInput;
      m_hECMaxInput = params.m_hECMaxInput;
      m_sMASCMeanInput = params.m_sMASCMeanInput;
      m_sMASCStdInput = params.m_sMASCStdInput;
      
      m_inRasterPtr = params.m_inRasterPtr;
      m_inRasterBands = params.m_inRasterBands;
      
      return *this;
    }
    
    te::common::AbstractParameters* Contrast::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }
    
    Contrast::OutputParameters::OutputParameters()
    {
      reset();
    }
    
    Contrast::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }
    
    Contrast::OutputParameters::~OutputParameters()
    {
      reset();
    }
    
    void Contrast::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_outRasterPtr.reset();
      m_outRasterBands.clear();
      m_rType.clear();
      m_rInfo.clear();
    }
    
    const Contrast::OutputParameters& Contrast::OutputParameters::operator=( 
      const Contrast::OutputParameters& params )
    {
      reset();
        
      m_outRasterPtr = params.m_outRasterPtr;
      m_outRasterBands = params.m_outRasterBands;
      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;
      
      return *this;
    }
    
    te::common::AbstractParameters* Contrast::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }    

    Contrast::Contrast()
    {
      reset();
    }

    Contrast::~Contrast()
    {
    }

    bool Contrast::execute( AlgorithmOutputParameters& outputParams ) 
      throw( te::rp::Exception )
    {
      TERP_TRUE_OR_RETURN_FALSE( m_isInitialized, "Algoritm not initialized" );
      
      // Initializing the output raster
      
      Contrast::OutputParameters* outputParamsPtr = dynamic_cast< 
        Contrast::OutputParameters* >( &outputParams );          
      TERP_TRUE_OR_RETURN_FALSE( outputParamsPtr, "Invalid parameters" );      
      
      if( outputParamsPtr->m_outRasterPtr.get() == 0 )
      {
        outputParamsPtr->m_outRasterBands.clear();

        std::vector< te::rst::BandProperty* > bandsProperties;
        for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
          m_inputParameters.m_inRasterBands.size() ; ++inRasterBandsIdx )
        {
          assert( m_inputParameters.m_inRasterBands[ inRasterBandsIdx ] < 
            m_inputParameters.m_inRasterPtr->getNumberOfBands() );
            
          bandsProperties.push_back( new te::rst::BandProperty(
            *( m_inputParameters.m_inRasterPtr->getBand( 
            m_inputParameters.m_inRasterBands[ 
            inRasterBandsIdx ] )->getProperty() ) ) );  

          outputParamsPtr->m_outRasterBands.push_back( inRasterBandsIdx );
        }
        
        outputParamsPtr->m_outRasterPtr.reset( 
          te::rst::RasterFactory::make(
            outputParamsPtr->m_rType, 
            new te::rst::Grid( *( m_inputParameters.m_inRasterPtr->getGrid() ) ),
            bandsProperties,
            outputParamsPtr->m_rInfo,
            0,
            0 ) );
        TERP_TRUE_OR_RETURN_FALSE( outputParamsPtr->m_outRasterPtr.get(),
          "Output raster creation error" );
      }
      else
      {
        if( ( outputParamsPtr->m_outRasterPtr->getAccessPolicy() & te::common::WAccess )
          && ( outputParamsPtr->m_outRasterPtr->getNumberOfColumns() ==
          m_inputParameters.m_inRasterPtr->getNumberOfColumns() )
          && ( outputParamsPtr->m_outRasterPtr->getNumberOfRows() ==
          m_inputParameters.m_inRasterPtr->getNumberOfRows() ) )
        {
          for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
            m_inputParameters.m_inRasterBands.size() ; ++inRasterBandsIdx )
          {
            TERP_TRUE_OR_RETURN_FALSE( 
              outputParamsPtr->m_outRasterBands[ inRasterBandsIdx ] <
              outputParamsPtr->m_outRasterPtr->getNumberOfBands(),
              "Invalid output raster band" )
          }
        }
        else
        {
          TERP_LOG_AND_RETURN_FALSE( "Invalid output raster" );
        }
      }
      
      // Executing the contrast on the selected bands

      switch( m_inputParameters.m_type )
      {
        case InputParameters::LinearContrastT :
        {
          return execLinearContrast();
          break;
        }
        case InputParameters::HistogramEqualizationContrastT :
        {
          return execHistogramEqualizationContrast();
          break;
        }
        case InputParameters::SetMeanAndStdContrastT :
        {
          return execSetMeanAndStdContrast();
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid contrast type" );
          break;
        }
      }
    }

    void Contrast::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_outputParametersPtr = 0;
      m_isInitialized = false;
    }

    bool Contrast::initialize( const AlgorithmInputParameters& inputParams ) 
      throw( te::rp::Exception )
    {
      reset();

      Contrast::InputParameters const* inputParamsPtr = dynamic_cast< 
        Contrast::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr, "Invalid parameters" );
      
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRasterPtr, 
        "Invalid raster pointer" );
      TERP_TRUE_OR_RETURN_FALSE( 
        inputParamsPtr->m_inRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );   
        
      for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
        inputParamsPtr->m_inRasterBands.size() ; ++inRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          inputParamsPtr->m_inRasterBands[ inRasterBandsIdx ] < 
          inputParamsPtr->m_inRasterPtr->getNumberOfBands(),
          "Invalid input raster band" );
      }        

      m_inputParameters = *inputParamsPtr;
      m_isInitialized = true;

      return true;
    }
    
    bool Contrast::isInitialized() const
    {
      return m_isInitialized;
    }

    bool Contrast::execLinearContrast()
    {
      assert( m_inputParameters.m_inRasterPtr );
      assert( m_outputParametersPtr->m_outRasterPtr.get() );
      
      for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
        m_inputParameters.m_inRasterBands.size() ; ++inRasterBandsIdx )
      {
        te::rst::Band* inRasterBandPtr = m_inputParameters.m_inRasterPtr->getBand( 
          m_inputParameters.m_inRasterBands[ inRasterBandsIdx ] );
        te::rst::Band* outRasterBandPtr =m_outputParametersPtr->m_outRasterPtr->getBand( 
         m_outputParametersPtr->m_outRasterBands[ inRasterBandsIdx ] );
        
        double outRangeMin = 0.0;
        double outRangeMax = 0.0;
        getDataTypeRange( outRasterBandPtr->getProperty()->getType(), 
          outRangeMin, outRangeMax );
        const double outRasterRange = outRangeMax - outRangeMin;
        
        const double inRasterRange = m_inputParameters.m_lCMaxInput - 
          m_inputParameters.m_lCMinInput;

        if( ( outRasterRange != 0.0 ) && ( inRasterRange != 0.0 ) )
        {
          m_offSetGainRemap_gain = outRasterRange / inRasterRange;
          m_offSetGainRemap_offset = outRangeMin - m_inputParameters.m_lCMinInput;
        }
        
        if( ! remapBandLevels( *inRasterBandPtr, *outRasterBandPtr,
          &Contrast::offSetGainRemap ) )
        {
          return false;
        }
      }
      
      return true;
    }

    bool Contrast::execHistogramEqualizationContrast()
    {
      assert( m_inputParameters.m_inRasterPtr );
      assert(m_outputParametersPtr->m_outRasterPtr.get() );

// ensure maxInput has a value, to normalize resultant image
      if(m_inputParameters.m_hECMaxInput == ( -1.0 * DBL_MAX ))
        m_inputParameters.m_hECMaxInput = 255;

      const te::rst::RasterSummary* rsummary = 
        te::rst::RasterSummaryManager::getInstance().get(
        m_inputParameters.m_inRasterPtr, te::rst::SUMMARY_R_HISTOGRAM);

      double value;
      double newvalue;
      unsigned int N = m_inputParameters.m_inRasterPtr->getNumberOfRows() * 
        m_inputParameters.m_inRasterPtr->getNumberOfColumns();
      const double lutfactor = 
        m_inputParameters.m_hECMaxInput / (double) N;

      for( unsigned int b = 0 ; b < m_inputParameters.m_inRasterBands.size() ; b++ )
      {
        unsigned int niband = m_inputParameters.m_inRasterBands[b];
        unsigned int noband =m_outputParametersPtr->m_outRasterBands[b];

        te::rst::Band* iband = m_inputParameters.m_inRasterPtr->getBand(niband);
        te::rst::Band* oband =m_outputParametersPtr->m_outRasterPtr->getBand(noband);

        te::rst::BandIterator<double> ibandit = te::rst::BandIterator<double>::begin(iband);
        te::rst::BandIterator<double> ibanditend = te::rst::BandIterator<double>::end(iband);

        std::map<double, double> lut;
        const double minp = rsummary->at(niband).m_minVal->real();
        const double maxp = rsummary->at(niband).m_maxVal->real();
        for (double pixel = minp; pixel <= maxp; pixel++)
        {
          newvalue = 0.0;
          for (double nj = minp; nj <= pixel; nj++)
            newvalue += rsummary->at(niband).m_histogramR->operator[](nj);
          lut[pixel] = lutfactor * newvalue;
        }

        while (ibandit != ibanditend)
        {
          value = ibandit.getValue();
          oband->setValue(ibandit.getCol(), ibandit.getRow(), lut[value]);
          ++ibandit;
        }
      }

      return true;
    }

    bool Contrast::execSetMeanAndStdContrast()
    {
      assert( m_inputParameters.m_inRasterPtr );
      assert(m_outputParametersPtr->m_outRasterPtr.get() );

      const te::rst::RasterSummary* rsummary = 
        te::rst::RasterSummaryManager::getInstance().get(
        m_inputParameters.m_inRasterPtr, 
        (te::rst::SummaryTypes) (te::rst::SUMMARY_MEAN | te::rst::SUMMARY_STD));

      for( unsigned int b = 0 ; b < m_inputParameters.m_inRasterBands.size() ; b++ )
      {
        unsigned int niband = m_inputParameters.m_inRasterBands[b];
        unsigned int noband =m_outputParametersPtr->m_outRasterBands[b];

        te::rst::Band* iband = m_inputParameters.m_inRasterPtr->getBand(niband);
        te::rst::Band* oband =m_outputParametersPtr->m_outRasterPtr->getBand(noband);

        te::rst::BandIterator<double> ibandit = te::rst::BandIterator<double>::begin(iband);
        te::rst::BandIterator<double> ibanditend = te::rst::BandIterator<double>::end(iband);

        const double meanp = rsummary->at(niband).m_meanVal->real();
        const double stdp = rsummary->at(niband).m_stdVal->real();
        const double offset = m_inputParameters.m_sMASCMeanInput / m_inputParameters.m_sMASCStdInput;
        const double c1 = m_inputParameters.m_sMASCStdInput / stdp;
        const double c2 = offset * m_inputParameters.m_sMASCStdInput;

        double value;
        double newvalue;

        for (unsigned r = 0; r < iband->getRaster()->getNumberOfRows(); r++)
          for (unsigned c = 0; c < iband->getRaster()->getNumberOfColumns(); c++)
          {
            iband->getValue(c, r, value);
            newvalue = (value - meanp) * c1 + c2;
            oband->setValue(c, r, newvalue);
          }
      }

      return true;
    }

    bool Contrast::remapBandLevels( te::rst::Band& inRasterBand,
      te::rst::Band& outRasterBand, RemapFuncPtrT remapFuncPtr )
    {
      const unsigned int inBlkWidthPixels = inRasterBand.getProperty()->m_blkw;
      const unsigned int inBlkHeightPixels = inRasterBand.getProperty()->m_blkh;
      const unsigned int outBlkWidthPixels = outRasterBand.getProperty()->m_blkw;
      const unsigned int outBlkHeightPixels = outRasterBand.getProperty()->m_blkh;
      double outRangeMin = 0.0;
      double outRangeMax = 0.0;
      getDataTypeRange( outRasterBand.getProperty()->getType(), 
        outRangeMin, outRangeMax );

      if( ( inBlkWidthPixels == outBlkWidthPixels ) && 
        ( inBlkHeightPixels == outBlkHeightPixels ) )
      { // block version
        const unsigned int blockSizePixels = inBlkWidthPixels * 
          inBlkHeightPixels;      
        const unsigned int xBlocksNmb = inRasterBand.getProperty()->m_nblocksx;
        const unsigned int yBlocksNmb = inRasterBand.getProperty()->m_nblocksy;
        unsigned char* inputBuffer = new unsigned char[ inRasterBand.getBlockSize() ];
        unsigned char* outputBuffer = new unsigned char[ outRasterBand.getBlockSize() ];
        double* inDoublesBuffer = new double[ blockSizePixels ];
        double* outDoublesBuffer = new double[ blockSizePixels ];
        const int inputVectorDataType = inRasterBand.getProperty()->getType();
        const int outputVectorDataType = outRasterBand.getProperty()->getType();
        unsigned int blockXIndex = 0;
        unsigned int blockOffset = 0;
        double outValue = 0;

        for( unsigned int blockYIndex = 0 ; blockYIndex < yBlocksNmb ; 
          ++blockYIndex )
        {
          for( blockXIndex = 0 ; blockXIndex < xBlocksNmb ; 
            ++blockXIndex )
          {
            inRasterBand.read( blockXIndex, blockYIndex, inputBuffer );

            convert2DoublesVector( inputBuffer, inputVectorDataType, 
              blockSizePixels, inDoublesBuffer );

            for( blockOffset = 0 ; blockOffset < blockSizePixels ; ++blockOffset )
            {
              (this->*remapFuncPtr)( inDoublesBuffer[blockOffset], 
                outValue);
              outDoublesBuffer[blockOffset] = std::max( outRangeMin, std::min( 
                outRangeMax, outValue ) );
            }

            convertDoublesVector( outDoublesBuffer, blockSizePixels, 
              outputVectorDataType, outputBuffer );

            outRasterBand.write( blockXIndex, blockYIndex, outputBuffer );
          }
        }

        delete[] inputBuffer;
        delete[] outputBuffer;
        delete[] inDoublesBuffer;
        delete[] outDoublesBuffer;
      }
      else
      { // pixel by pixel version
        const unsigned int linesNumber = m_inputParameters.m_inRasterPtr->getNumberOfRows();
        const unsigned int columnsNumber =  m_inputParameters.m_inRasterPtr->getNumberOfColumns();

        unsigned int col = 0;
        double inputValue = 0;
        double outputValue = 0;

        for( unsigned int line = 0 ; line < linesNumber ; ++line )
        {
          for( col = 0 ; col < columnsNumber ; ++col )
          {
            inRasterBand.getValue( col, line, inputValue );
            (this->*remapFuncPtr)( inputValue, outputValue );
            outRasterBand.setValue( col, line, std::max( outRangeMin, std::min( 
              outRangeMax, outputValue ) ) );
          }
        }
      }

      return true;
    }

  } // end namespace rp
}   // end namespace te

