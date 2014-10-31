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
  \file terralib/rp/TiePointsLocatorStrategy.cpp
  \brief Tie-Pointsr locator strategy.
*/

#include "TiePointsLocatorStrategy.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"

#include <memory.h>

namespace te
{
  namespace rp
  {
    TiePointsLocatorStrategy::TiePointsLocatorStrategy()
    {
    }
    
    TiePointsLocatorStrategy::~TiePointsLocatorStrategy()
    {
    }
    
    TiePointsLocatorStrategy::TiePointsLocatorStrategy( const TiePointsLocatorStrategy& )
    {
    }
    
    const TiePointsLocatorStrategy& TiePointsLocatorStrategy::operator=( const TiePointsLocatorStrategy& )
    {
      return *this;
    }
    
    bool TiePointsLocatorStrategy::loadRasterData( 
      te::rst::Raster const* rasterPtr,
      const std::vector< unsigned int >& rasterBands,
      te::rst::Raster const* maskRasterPtr,
      const unsigned int maskRasterBand,
      const unsigned int rasterTargetAreaLineStart,
      const unsigned int rasterTargetAreaColStart,
      const unsigned int rasterTargetAreaWidth,
      const unsigned int rasterTargetAreaHeight,
      const double rescaleFactorX,
      const double rescaleFactorY,
      const te::rst::Interpolator::Method rasterInterpMethod,
      const unsigned char maxMemPercentUsage, 
      std::vector< boost::shared_ptr< FloatsMatrix > >& loadedRasterData,
      UCharsMatrix& loadedMaskRasterData )
    {
      // Allocating the output matrixes
      
      const unsigned int rescaledNLines = (unsigned int)(
        ((double)rasterTargetAreaHeight) * rescaleFactorY );
      const unsigned int rescaledNCols = (unsigned int)(
        ((double)rasterTargetAreaWidth) * rescaleFactorX );
    
      {
        unsigned char maxMemPercentUsagePerMatrix = MAX( 1u, maxMemPercentUsage / 
          ( 
            ( maskRasterPtr ? 1 : 0 ) 
            + 
            ((unsigned int)rasterBands.size()) ) 
          );
        
        loadedRasterData.resize( rasterBands.size() );
        
        for( unsigned int rasterBandsIdx = 0 ; rasterBandsIdx < rasterBands.size() ;
          ++rasterBandsIdx )
        {
          loadedRasterData[ rasterBandsIdx ].reset( new FloatsMatrix );
          TERP_TRUE_OR_RETURN_FALSE( loadedRasterData[ rasterBandsIdx ]->reset( 
            rescaledNLines, rescaledNCols, FloatsMatrix::AutoMemPol, 
            maxMemPercentUsagePerMatrix ),
            "Cannot allocate image 1 matrix" );
          maxMemPercentUsagePerMatrix *= 2;
        }
        
        if( maskRasterPtr ) 
        {
          TERP_TRUE_OR_RETURN_FALSE( loadedMaskRasterData.reset( 
            rescaledNLines, rescaledNCols,
            UCharsMatrix::AutoMemPol, maxMemPercentUsagePerMatrix ),
            "Cannot allocate image 1 mask matrix" );          
        }        
      }
      
      // loading mask data
      
      if( maskRasterPtr )
      {
        te::rst::Band const* inMaskRasterBand = maskRasterPtr->getBand( maskRasterBand );
        assert( inMaskRasterBand );
        
        unsigned char* outMaskLinePtr = 0;
        unsigned int outLine = 0;
        unsigned int outCol = 0;
        unsigned int inLine = 0;
        unsigned int inCol = 0;
        double value = 0;
        
        for( outLine = 0 ; outLine < rescaledNLines ; ++outLine ) 
        {
          inLine = (unsigned int)( ( ( (double)outLine ) / 
            rescaleFactorY ) + ( (double)rasterTargetAreaLineStart ) );      
          
          outMaskLinePtr = loadedMaskRasterData[ outLine ];
          
          for( outCol = 0 ; outCol < rescaledNCols ; ++outCol ) 
          {          
            inCol = (unsigned int)( ( ( (double)outCol ) / 
                rescaleFactorX ) + ( (double)rasterTargetAreaColStart ) );        
                
            inMaskRasterBand->getValue( inCol, inLine, value );
            
            if( value == 0 )
              outMaskLinePtr[ outCol ] = 0;
            else
              outMaskLinePtr[ outCol ] = 255;
          }
        }
      }
      
      // loading raster data     
      {
        const double rasterTargetAreaLineStartDouble = (double)
          rasterTargetAreaLineStart;
        const double rasterTargetAreaColStartDouble = (double)
          rasterTargetAreaColStart;
        te::rst::Interpolator interpInstance( rasterPtr, rasterInterpMethod );
        float* floatLinePtr = 0;
        double* doubleLinePtr = 0;
        unsigned int outLine = 0;
        unsigned int outCol = 0;
        double inLine = 0;
        double inCol = 0;     
        std::complex< double > interpolatedValue;
        unsigned int bandIdx = 0;
        double bandMin = 0;
        double bandMax = 0;
        double gain = 0;
        
        DoublesMatrix auxBandData;
        TERP_TRUE_OR_RETURN_FALSE( auxBandData.reset( 
          rescaledNLines, rescaledNCols, DoublesMatrix::AutoMemPol, 40 ),
          "Cannot allocate auxiliar matrix" );        
        
        for( unsigned int rasterBandsIdx = 0 ; rasterBandsIdx < rasterBands.size() ;
          ++rasterBandsIdx )
        {
          bandIdx= rasterBands[ rasterBandsIdx ];
          bandMin = DBL_MAX;
          bandMax = -1.0 * DBL_MAX;
          
          for( outLine = 0 ; outLine < rescaledNLines ; ++outLine ) 
          {
            inLine = ( ( (double)outLine ) /  rescaleFactorY ) + 
              rasterTargetAreaLineStartDouble;
              
            doubleLinePtr = auxBandData[ outLine ];
            
            for( outCol = 0 ; outCol < rescaledNCols ; ++outCol ) 
            {          
              inCol = ( ( (double)outCol ) / rescaleFactorX ) + 
                rasterTargetAreaColStartDouble;

              interpInstance.getValue( inCol, inLine, interpolatedValue,
                bandIdx );
              
              doubleLinePtr[ outCol ] = interpolatedValue.real();              
              
              if( interpolatedValue.real() > bandMax ) bandMax = interpolatedValue.real();
              if( interpolatedValue.real() < bandMin ) bandMin = interpolatedValue.real();
            }
          }
          
          if( bandMin == bandMax )
            gain = 0.0;
          else
            gain = ( 1.0 / ( bandMax - bandMin ) );
          
          for( outLine = 0 ; outLine < rescaledNLines ; ++outLine ) 
          {
            doubleLinePtr = auxBandData[ outLine ];
            floatLinePtr = loadedRasterData[ rasterBandsIdx ]->operator[]( outLine  );
            
            for( outCol = 0 ; outCol < rescaledNCols ; ++outCol ) 
            {          
              floatLinePtr[ outCol ] = (float)( ( doubleLinePtr[ outCol ] - bandMin ) * gain );
            }
          }
        }
      }
      
      return true;
    }    
    
    void TiePointsLocatorStrategy::createTifFromMatrix( 
      const DoublesMatrix& rasterData,
      const InterestPointsSetT& interestPoints,
      const std::string& tifFileName )
    {
      std::map<std::string, std::string> rInfo;
      rInfo["URI"] = tifFileName + ".tif";
      
      std::vector<te::rst::BandProperty*> bandsProperties;
      bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
      bandsProperties[0]->m_colorInterp = te::rst::RedCInt;
      bandsProperties[0]->m_noDataValue = 0;
      bandsProperties.push_back(new te::rst::BandProperty( *bandsProperties[0] ));
      bandsProperties[1]->m_colorInterp = te::rst::GreenCInt;
      bandsProperties.push_back(new te::rst::BandProperty( *bandsProperties[0] ));
      bandsProperties[2]->m_colorInterp = te::rst::BlueCInt;

      te::rst::Grid* newgrid = new te::rst::Grid( rasterData.getColumnsNumber(),
        rasterData.getLinesNumber(), 0, -1 );

      std::auto_ptr< te::rst::Raster > outputRasterPtr(
        te::rst::RasterFactory::make( "GDAL", newgrid, bandsProperties, rInfo, 0, 0));
      TERP_TRUE_OR_THROW( outputRasterPtr.get(), "Output raster creation error");
          
      unsigned int line = 0;
      unsigned int col = 0;
      const unsigned int nLines = rasterData.getLinesNumber();
      const unsigned int nCols = rasterData.getColumnsNumber();
      double rasterDataMin = DBL_MAX;
      double rasterDataMax = (-1.0) * DBL_MAX;
      double value = 0;
      
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          value = rasterData[ line ][ col ];
          
          if( rasterDataMin > value )
            rasterDataMin = value;
          if( rasterDataMax < value )
            rasterDataMax = value;
        }     
        
      if( rasterDataMax == rasterDataMin )
      {
        rasterDataMin = 0.0;
        rasterDataMax = 1.0;
      }
      
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          value = rasterData[ line ][ col ];
          value = ( value - rasterDataMin ) / ( rasterDataMax - rasterDataMin );
          value *= 255.0;
          value = std::max( 0.0, value );
          value = std::min( 255.0, value );
          
          outputRasterPtr->setValue( col, line, value, 0 );
          outputRasterPtr->setValue( col, line, value, 1 );
          outputRasterPtr->setValue( col, line, value, 2 );
        }
        
      InterestPointsSetT::const_iterator itB = interestPoints.begin();
      InterestPointsSetT::const_iterator itE = interestPoints.end();
      
      while( itB != itE )
      {
        outputRasterPtr->setValue( itB->m_x, itB->m_y, 0, 0 );
        outputRasterPtr->setValue( itB->m_x, itB->m_y, 255, 1 );
        outputRasterPtr->setValue( itB->m_x, itB->m_y, 0, 2 );
        
        ++itB;
      }
    }    
    
    void TiePointsLocatorStrategy::features2Tiff( const DoublesMatrix& features,
      const InterestPointsSetT& interestPoints,
      const std::string& fileNameBeginning )
    {
      const unsigned int tifLinesNumber = (unsigned int)std::sqrt( (double)
        features.getColumnsNumber() );
      const unsigned int featuresColsNumber = features.getColumnsNumber();
      
      double const* featureLinePtr = 0;
      
      InterestPointsSetT::const_iterator iIt = interestPoints.begin();

      for( unsigned int featuresIdx = 0 ; featuresIdx < features.getLinesNumber() ;
        ++featuresIdx )
      {
        featureLinePtr = features[ featuresIdx ];
        
        std::vector<te::rst::BandProperty*> bandsProperties;
        bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
        bandsProperties[0]->m_colorInterp = te::rst::RedCInt;
        bandsProperties[0]->m_noDataValue = 0;
        
        std::map<std::string, std::string> rInfo;
        rInfo["URI"] = fileNameBeginning + "_" + boost::lexical_cast< std::string >( iIt->m_x ) 
          + "_" + boost::lexical_cast< std::string >( iIt->m_y ) + ".tif";        
          
        te::rst::Grid* newgrid = new te::rst::Grid( tifLinesNumber,
          tifLinesNumber, 0, -1 );          
        
        std::auto_ptr< te::rst::Raster > outputRasterPtr(
          te::rst::RasterFactory::make( "GDAL", newgrid, bandsProperties, rInfo, 0, 0));
        TERP_TRUE_OR_THROW( outputRasterPtr.get(), "Output raster creation error");
          
        unsigned int line = 0;
        unsigned int col = 0;
        double value = 0;
        double min = 0;
        double max = 0;
        double gain = 1.0;
        
        for( col = 0 ; col < featuresColsNumber ; ++col )
        {
          if( min > featureLinePtr[ col ] ) min = featureLinePtr[ col ];
          if( max < featureLinePtr[ col ] ) max = featureLinePtr[ col ];
        }
        
        gain = 255.0 / ( max - min );
        
        for( line = 0 ; line < tifLinesNumber ; ++line )
          for( col = 0 ; col < tifLinesNumber ; ++col )
          {
            value = featureLinePtr[ ( line * tifLinesNumber ) + col ];
            value *= gain;
            value -= min;
            value = MIN( 255.0, value );
            value = MAX( 0.0, value );
            
            outputRasterPtr->setValue( col, line, value, 0 );
          }
          
        ++iIt;
      }
    }    
    
  } // end namespace rp
}   // end namespace te    

