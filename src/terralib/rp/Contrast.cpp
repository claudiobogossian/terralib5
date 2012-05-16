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

#include "float.h"
#include <boost/concept_check.hpp>
#include <terralib/raster/Grid.h>

namespace te
{
  namespace rp
  {
    Contrast::Parameters::Parameters()
    {
      reset();
    }
    
    Contrast::Parameters::~Parameters()
    {
      reset();
    }
    
    void Contrast::Parameters::reset() throw( te::rp::Exception )
    {
      m_type = Parameters::InvalidContrastT;
      m_minInput = DBL_MAX;
      m_maxInput = ( -1.0 * DBL_MAX );
      m_meanInput = 0;
      m_stdInput = 1;
      m_inRasterPtr = 0;
      m_inRasterBands.clear();
      m_outRasterPtr = 0;
      m_outRasterBands.clear();
      m_outDataSourcePtr = 0;
      m_outDataSetName.clear();
      m_outRasterHandlerPtr.reset();
    }
    
    const AlgorithmParameters& Contrast::Parameters::operator=( 
      const AlgorithmParameters& params )
    {
      reset();
      
      Contrast::Parameters const* paramsPtr = 
        dynamic_cast< Contrast::Parameters const* >(&params );
        
      if( paramsPtr )
      {
        m_type = paramsPtr->m_type;
        m_minInput = paramsPtr->m_minInput;
        m_maxInput = paramsPtr->m_maxInput;
        m_inRasterPtr = paramsPtr->m_inRasterPtr;
        m_inRasterBands = paramsPtr->m_inRasterBands;
        m_outRasterPtr = paramsPtr->m_outRasterPtr;
        m_outRasterBands = paramsPtr->m_outRasterBands;
        m_outDataSourcePtr = paramsPtr->m_outDataSourcePtr;
        m_outDataSetName = paramsPtr->m_outDataSetName;
        m_outRasterHandlerPtr = paramsPtr->m_outRasterHandlerPtr;
      }
      
      return *this;
    }
    
    te::common::AbstractParameters* Contrast::Parameters::clone() const
    {
      return new Parameters( *this );
    }

    Contrast::Contrast()
    {
      reset();
    }

    Contrast::~Contrast()
    {
    }

    bool Contrast::execute() throw( te::rp::Exception )
    {
      if( ( m_parameters.m_inRasterPtr != 0 ) && 
        ( m_parameters.m_outRasterPtr != 0 ) )
      {
        // Executing the contrast on the selected bands

        switch( m_parameters.m_type )
        {
          case Parameters::LinearContrastT :
          {
            return execLinearContrast();
            break;
          }
          case Parameters::HistogramEqualizationContrastT :
          {
            return execHistogramEqualizationContrast();
            break;
          }
          case Parameters::SetMeanAndStdContrastT :
          {
            return execSetMeanAndStdContrast();
            break;
          }
          default :
          {
            return false;
            break;
          }
        }

        return true;
      }
      else
      {
        return false;
      }
    }

    void Contrast::reset() throw( te::rp::Exception )
    {
      m_parameters.reset();
    }

    bool Contrast::initialize( AlgorithmParameters& params ) 
      throw( te::rp::Exception )
    {
      reset();

      Contrast::Parameters* paramsPtr = dynamic_cast< Contrast::Parameters* >(
        &params );

      if( ( paramsPtr != 0 ) 
        && ( paramsPtr->m_inRasterPtr != 0 )
        && ( paramsPtr->m_inRasterPtr->getAccessPolicy() & te::common::RAccess ) )
      {
        if( paramsPtr->m_outRasterPtr == 0 )
        {
          if( ( paramsPtr->m_outDataSourcePtr != 0 ) 
            && ( paramsPtr->m_outDataSourcePtr->isValid() ) )
          {
            // Allocating the output raster

            paramsPtr->m_outRasterBands.clear();

            std::vector< te::rst::BandProperty* > bandsProperties;
            for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
              paramsPtr->m_inRasterBands.size() ; ++inRasterBandsIdx )
            {
              assert(  paramsPtr->m_inRasterBands[ inRasterBandsIdx ] < 
                paramsPtr->m_inRasterPtr->getNumberOfBands() );
              bandsProperties.push_back( new te::rst::BandProperty(
                *( paramsPtr->m_inRasterPtr->getBand( paramsPtr->m_inRasterBands[ 
                inRasterBandsIdx ] )->getProperty() ) ) );  

              paramsPtr->m_outRasterBands.push_back( inRasterBandsIdx );
            }

            if( paramsPtr->m_outRasterHandlerPtr.get() == 0 )
            {
              paramsPtr->m_outRasterHandlerPtr.reset( new RasterHandler );
            }

            if( ! createNewRaster( *( paramsPtr->m_inRasterPtr->getGrid() ),
              bandsProperties, paramsPtr->m_outDataSetName, 
              *paramsPtr->m_outDataSourcePtr, 
              *paramsPtr->m_outRasterHandlerPtr ) )
            {
              paramsPtr->m_outRasterHandlerPtr.reset();
              return false;
            }

            paramsPtr->m_outRasterPtr = 
              paramsPtr->m_outRasterHandlerPtr->getRasterPtr();
          }
          else
          {
            return false;
          }
        }
        else
        {
          if( ( paramsPtr->m_outRasterPtr->getAccessPolicy() & te::common::WAccess )
            && ( paramsPtr->m_outRasterPtr->getNumberOfColumns() ==
            paramsPtr->m_inRasterPtr->getNumberOfColumns() )
            && ( paramsPtr->m_outRasterPtr->getNumberOfRows() ==
            paramsPtr->m_inRasterPtr->getNumberOfRows() ) )
          {
            for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
              paramsPtr->m_inRasterBands.size() ; ++inRasterBandsIdx )
            {
              if( ( paramsPtr->m_inRasterBands[ inRasterBandsIdx ] >=
                paramsPtr->m_inRasterPtr->getNumberOfBands() ) ||
                ( paramsPtr->m_outRasterBands[ inRasterBandsIdx ] >=
                paramsPtr->m_outRasterPtr->getNumberOfBands() ) )
              {
                return false;
              }
            }
          }
          else
          {
            return false;
          }
        }

        m_parameters = *paramsPtr;

        return true;
      }
      else
      {
        return false;
      }
    }

    bool Contrast::execLinearContrast()
    {
      assert( m_parameters.m_inRasterPtr );
      assert( m_parameters.m_outRasterPtr );
      
      for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
        m_parameters.m_inRasterBands.size() ; ++inRasterBandsIdx )
      {
        te::rst::Band* inRasterBandPtr = m_parameters.m_inRasterPtr->getBand( 
          m_parameters.m_inRasterBands[ inRasterBandsIdx ] );
        te::rst::Band* outRasterBandPtr = m_parameters.m_outRasterPtr->getBand( 
          m_parameters.m_outRasterBands[ inRasterBandsIdx ] );
        
        double outRangeMin = 0.0;
        double outRangeMax = 0.0;
        getDataTypeRange( outRasterBandPtr->getProperty()->getType(), 
          outRangeMin, outRangeMax );
        const double outRasterRange = outRangeMax - outRangeMin;
        
        const double inRasterRange = m_parameters.m_maxInput - m_parameters.m_minInput;

        if( ( outRasterRange != 0.0 ) && ( inRasterRange != 0.0 ) )
        {
          m_offSetGainRemap_gain = outRasterRange / inRasterRange;
          m_offSetGainRemap_offset = outRangeMin - m_parameters.m_minInput;
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
      assert( m_parameters.m_inRasterPtr );
      assert( m_parameters.m_outRasterPtr );

// ensure maxInput has a value, to normalize resultant image
      if(m_parameters.m_maxInput == ( -1.0 * DBL_MAX ))
        m_parameters.m_maxInput = 255;

      const te::rst::RasterSummary* rsummary = te::rst::RasterSummaryManager::getInstance().get(m_parameters.m_inRasterPtr, te::rst::SUMMARY_R_HISTOGRAM);

      double value;
      double newvalue;
      unsigned int N = m_parameters.m_inRasterPtr->getNumberOfRows() * m_parameters.m_inRasterPtr->getNumberOfColumns();
      const double lutfactor = m_parameters.m_maxInput / (double) N;

      for( unsigned int b = 0 ; b < m_parameters.m_inRasterBands.size() ; b++ )
      {
        unsigned int niband = m_parameters.m_inRasterBands[b];
        unsigned int noband = m_parameters.m_outRasterBands[b];

        te::rst::Band* iband = m_parameters.m_inRasterPtr->getBand(niband);
        te::rst::Band* oband = m_parameters.m_outRasterPtr->getBand(noband);

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
      assert( m_parameters.m_inRasterPtr );
      assert( m_parameters.m_outRasterPtr );

      const te::rst::RasterSummary* rsummary = te::rst::RasterSummaryManager::getInstance().get(m_parameters.m_inRasterPtr, (te::rst::SummaryTypes) (te::rst::SUMMARY_MEAN | te::rst::SUMMARY_STD));

      for( unsigned int b = 0 ; b < m_parameters.m_inRasterBands.size() ; b++ )
      {
        unsigned int niband = m_parameters.m_inRasterBands[b];
        unsigned int noband = m_parameters.m_outRasterBands[b];

        te::rst::Band* iband = m_parameters.m_inRasterPtr->getBand(niband);
        te::rst::Band* oband = m_parameters.m_outRasterPtr->getBand(noband);

        te::rst::BandIterator<double> ibandit = te::rst::BandIterator<double>::begin(iband);
        te::rst::BandIterator<double> ibanditend = te::rst::BandIterator<double>::end(iband);

        const double meanp = rsummary->at(niband).m_meanVal->real();
        const double stdp = rsummary->at(niband).m_stdVal->real();
        const double offset = m_parameters.m_meanInput / m_parameters.m_stdInput;
        const double c1 = m_parameters.m_stdInput / stdp;
        const double c2 = offset * m_parameters.m_stdInput;

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
        const unsigned int linesNumber = m_parameters.m_inRasterPtr->getNumberOfRows();
        const unsigned int columnsNumber =  m_parameters.m_inRasterPtr->getNumberOfColumns();

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

