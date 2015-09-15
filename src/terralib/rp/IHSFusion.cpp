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
  \file terralib/rp/IHSFusion.cpp
  \brief Creation of skeleton imagems.
*/

#include "IHSFusion.h"
#include "Macros.h"
#include "Functions.h"

#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Band.h"
#include "../raster/Grid.h"
#include "../geometry/Envelope.h"
#include "../common/progress/TaskProgress.h"

#include <cmath>
#include <limits>

#ifndef M_PI
  #define M_PI       3.14159265358979323846
#endif
  
namespace te
{
  namespace rp
  {

    IHSFusion::InputParameters::InputParameters()
    {
      reset();
    }

    IHSFusion::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    IHSFusion::InputParameters::~InputParameters()
    {
      reset();
    }

    void IHSFusion::InputParameters::reset() throw( te::rp::Exception )
    {
      m_lowResRasterPtr = 0;
      m_lowResRasterRedBandIndex = 0;
      m_lowResRasterGreenBandIndex = 1;
      m_lowResRasterBlueBandIndex = 2;
      m_highResRasterPtr = 0;
      m_highResRasterBand = 0;
      m_enableProgress = false;
      m_interpMethod = te::rst::NearestNeighbor;
      m_RGBMin = 0.0;
      m_RGBMax = 0.0;
    }

    const IHSFusion::InputParameters& IHSFusion::InputParameters::operator=(
      const IHSFusion::InputParameters& params )
    {
      reset();

      m_lowResRasterPtr = params.m_lowResRasterPtr;
      m_lowResRasterRedBandIndex = params.m_lowResRasterRedBandIndex;
      m_lowResRasterGreenBandIndex = params.m_lowResRasterGreenBandIndex;
      m_lowResRasterBlueBandIndex = params.m_lowResRasterBlueBandIndex;
      m_highResRasterPtr = params.m_highResRasterPtr;
      m_highResRasterBand = params.m_highResRasterBand;
      m_enableProgress = params.m_enableProgress;
      m_interpMethod = params.m_interpMethod;
      m_RGBMin = params.m_RGBMin;
      m_RGBMax = params.m_RGBMax;

      return *this;
    }

    te::common::AbstractParameters* IHSFusion::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    IHSFusion::OutputParameters::OutputParameters()
    {
      reset();
    }

    IHSFusion::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    IHSFusion::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void IHSFusion::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const IHSFusion::OutputParameters& IHSFusion::OutputParameters::operator=(
      const IHSFusion::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* IHSFusion::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    IHSFusion::IHSFusion()
    {
      reset();
    }

    IHSFusion::~IHSFusion()
    {
    }

    bool IHSFusion::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      IHSFusion::OutputParameters* outParamsPtr = dynamic_cast<
        IHSFusion::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 4 );
        
        progressPtr->setMessage( "Fusing images" );
      }        
      
      // Getting RGB range
      
      double rgbMin = 0.0;
      double rgbMax = 0.0;
      TERP_TRUE_OR_RETURN_FALSE( getRGBRange( rgbMin, rgbMax ),
        "Unable to get RGB range" );
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }  
      
      // Loading IHS data    
      
      te::rp::Matrix< float > intensityData;
      intensityData.reset( te::rp::Matrix< float >::AutoMemPol );
      te::rp::Matrix< float > hueData;
      hueData.reset( te::rp::Matrix< float >::AutoMemPol );
      te::rp::Matrix< float > saturationData;
      saturationData.reset( te::rp::Matrix< float >::AutoMemPol );
      
      TERP_TRUE_OR_RETURN_FALSE( loadIHSData( rgbMin, rgbMax, intensityData,
        hueData, saturationData ), "Error loading IHS data" );
        
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }          
        
//       CreateRasterFileFromMatrix( intensityData, true, "intensityData.tif" );
//       CreateRasterFileFromMatrix( hueData, true, "hueData.tif" );  
//       CreateRasterFileFromMatrix( saturationData, true, "saturationData.tif" );
      
      // Swapping Itensity
      
      TERP_TRUE_OR_RETURN_FALSE( swapIntensity( intensityData ),
        "Intensity channel swap error" );
        
//       CreateRasterFileFromMatrix( intensityData, true, "swappedintensityData.tif" );
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }        
      
      // Saving RGB data
      
      TERP_TRUE_OR_RETURN_FALSE( saveIHSData( rgbMin, rgbMax, intensityData,
        hueData, saturationData, outParamsPtr->m_rType, outParamsPtr->m_rInfo,
        outParamsPtr->m_outputRasterPtr),
        "RGB raster creation error" );

      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }              
      
      return true;
    }

    void IHSFusion::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool IHSFusion::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      IHSFusion::InputParameters const* inputParamsPtr = dynamic_cast<
        IHSFusion::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the m_lowResRasterPtr and m_lowResRasterBands

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterPtr,
        "Invalid low Resolution Raster Pointer" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_lowResRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
        
      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_lowResRasterRedBandIndex <
        m_inputParameters.m_lowResRasterPtr->getNumberOfBands(), 
        "Invalid raster band" );   
        
      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_lowResRasterGreenBandIndex <
        m_inputParameters.m_lowResRasterPtr->getNumberOfBands(), 
        "Invalid raster band" );  
        
      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_lowResRasterBlueBandIndex <
        m_inputParameters.m_lowResRasterPtr->getNumberOfBands(), 
        "Invalid raster band" );          
      
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
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_RGBMin <=
        m_inputParameters.m_RGBMax, "Invalid RGB channel ranges" );
        
      m_isInitialized = true;

      return true;
    }

    bool IHSFusion::isInitialized() const
    {
      return m_isInitialized;
    }
    
    bool IHSFusion::getRGBRange( double& rgbMin, double& rgbMax ) const
    {
      if( ( m_inputParameters.m_RGBMax == 0.0 ) && 
        ( m_inputParameters.m_RGBMin == 0.0 ) )
      {
        
        const unsigned int nCols = m_inputParameters.m_lowResRasterPtr->getNumberOfColumns();
        const unsigned int nRows = m_inputParameters.m_lowResRasterPtr->getNumberOfRows();
        const te::rst::Band& redBand = *( m_inputParameters.m_lowResRasterPtr->getBand(
          m_inputParameters.m_lowResRasterRedBandIndex ) );
        const te::rst::Band& greenBand = *( m_inputParameters.m_lowResRasterPtr->getBand(
          m_inputParameters.m_lowResRasterGreenBandIndex ) );
        const te::rst::Band& blueBand = *( m_inputParameters.m_lowResRasterPtr->getBand(
          m_inputParameters.m_lowResRasterBlueBandIndex ) );
        const double redNoData = redBand.getProperty()->m_noDataValue;
        const double greenNoData = greenBand.getProperty()->m_noDataValue;
        const double blueNoData = blueBand.getProperty()->m_noDataValue;
        
        rgbMin = std::numeric_limits<double>::max();
        rgbMax = -1.0 * std::numeric_limits<double>::max();

        unsigned int row = 0;
        unsigned int col = 0;
        double redValue = 0;
        double greenValue = 0;
        double blueValue = 0;
        
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            redBand.getValue( col, row, redValue );
            greenBand.getValue( col, row, greenValue );
            blueBand.getValue( col, row, blueValue );
            
            if( ( redValue != redNoData ) && ( greenValue != greenNoData ) &&
              ( blueValue != blueNoData ) )
            {
              if( redValue > rgbMax ) rgbMax = redValue;
              if( greenValue > rgbMax ) rgbMax = greenValue;
              if( blueValue > rgbMax ) rgbMax = blueValue;
              
              if( redValue < rgbMin ) rgbMin = redValue;
              if( greenValue < rgbMin ) rgbMin = greenValue;
              if( blueValue < rgbMin ) rgbMin = blueValue;
            }
          }
        }
      }
      else
      {
        rgbMin = m_inputParameters.m_RGBMin;
        rgbMax = m_inputParameters.m_RGBMax;
      }
        
      return true;
    }
    
    bool IHSFusion::loadIHSData( const double& rgbMin, const double rgbMax,
      te::rp::Matrix< float >& intensityData,
      te::rp::Matrix< float >& hueData, te::rp::Matrix< float >& saturationData ) const
    {
      const unsigned int outNCols = m_inputParameters.m_highResRasterPtr->getNumberOfColumns();
      const unsigned int outNRows = m_inputParameters.m_highResRasterPtr->getNumberOfRows();
      const double colsRescaleFactor =
        ((double)m_inputParameters.m_lowResRasterPtr->getNumberOfColumns()) /
        ((double)outNCols);
      const double rowsRescaleFactor =
        ((double)m_inputParameters.m_lowResRasterPtr->getNumberOfRows()) /
        ((double)outNRows);
      const unsigned int redBandIdx = m_inputParameters.m_lowResRasterRedBandIndex;
      const unsigned int greenBandIdx = m_inputParameters.m_lowResRasterGreenBandIndex;
      const unsigned int blueBandIdx = m_inputParameters.m_lowResRasterBlueBandIndex;
      const double redNoData = m_inputParameters.m_lowResRasterPtr->getBand( 
        redBandIdx )->getProperty()->m_noDataValue;
      const double greenNoData = m_inputParameters.m_lowResRasterPtr->getBand( 
        greenBandIdx )->getProperty()->m_noDataValue;
      const double blueNoData = m_inputParameters.m_lowResRasterPtr->getBand( 
        blueBandIdx )->getProperty()->m_noDataValue;
        
      if( ! intensityData.reset( outNRows, outNCols ) ) return false;
      if( ! hueData.reset( outNRows, outNCols ) ) return false;
      if( ! saturationData.reset( outNRows, outNCols ) ) return false;
      
      te::rst::Interpolator interpol( m_inputParameters.m_lowResRasterPtr,
        m_inputParameters.m_interpMethod );
      unsigned int outRow = 0;
      unsigned int outCol = 0;
      double inRow = 0;
      double inCol = 0;
      std::complex< double > redC = 0;
      std::complex< double > greenC = 0;
      std::complex< double > blueC = 0;
      float* intensityRow = 0;
      float* hueRow = 0;
      float* saturationRow = 0;
      
      double teta = 0;
      double redNorm = 0, greenNorm = 0, blueNorm = 0;
      double rMinusG = 0, rMinusB = 0;
      double rgbSum = 0;
      double cosValue = 0;
      const double twoPi = 2.0 * ((double)M_PI);
      const double pi2 = ((double)M_PI) / 2.0;   
      const double rgbNormFac = ( rgbMax == rgbMin ) ? 0.0 :
        ( 1.0 / ( rgbMax - rgbMin ) );
      
      for( outRow = 0 ; outRow < outNRows ; ++outRow )
      {
        inRow = ((double)outRow) * rowsRescaleFactor;
        intensityRow = intensityData[ outRow ];
        hueRow = hueData[ outRow ];
        saturationRow = saturationData[ outRow ];
        
        for( outCol = 0 ; outCol < outNCols ; ++outCol )
        {
          inCol = ((double)outCol) * colsRescaleFactor;
          
          interpol.getValue( inCol, inRow, redC, redBandIdx );
          interpol.getValue( inCol, inRow, greenC, greenBandIdx );
          interpol.getValue( inCol, inRow, blueC, blueBandIdx );
          
          if( ( redC.real() == redNoData ) || ( greenC.real() == greenNoData ) ||
            ( blueC.real() == blueNoData ) )
          {
            intensityRow[ outCol ] = 0.0;
            hueRow[ outCol ] = 0.0;
            saturationRow[ outCol ] = 0.0;
          }            
          else
          {
            if( ( redC.real() == greenC.real() ) && ( greenC.real() == blueC.real() ) ) 
            { // Gray scale case
              // From Wikipedia:
              // h = 0 is used for grays though the hue has no geometric 
              // meaning there, where the saturation s = 0. Similarly, 
              // the choice of 0 as the value for s when l is equal to 0 or 1 
              // is arbitrary.        
              
              hueRow[ outCol ] = 0.0;
              saturationRow[ outCol ] = 0.0;
              intensityRow[ outCol ] = (float)( redC.real() * rgbNormFac ); // or green or blue since they all are the same.
            }
            else
            { // Color case
              redNorm = ( redC.real() - rgbMin ) * rgbNormFac;
              greenNorm = ( greenC.real() - rgbMin ) * rgbNormFac;
              blueNorm = ( blueC.real() - rgbMin ) * rgbNormFac;
              
              rMinusG = redNorm - greenNorm;
              rMinusB = redNorm - blueNorm;
              
              cosValue = sqrt( ( rMinusG * rMinusG ) + ( rMinusB * 
                ( greenNorm - blueNorm ) ) );
                
              if( cosValue == 0.0 )
              {
                teta = pi2;
              }
              else
              {
                cosValue =  ( 0.5 * ( rMinusG + rMinusB )  ) /
                  cosValue;
                teta = std::acos( cosValue );  
              }
                
              assert( ( cosValue >= (-1.0) ) && ( cosValue <= (1.0) ) );
                
              if( blueNorm > greenNorm )
              {
                hueRow[ outCol ] = (float)( twoPi - teta );
              }
              else
              {
                hueRow[ outCol ] = (float)teta;
              }
                
              rgbSum = ( redNorm + greenNorm + blueNorm );
              
              saturationRow[ outCol ] = (float)( 1.0 - ( 3 * std::min( std::min( redNorm, greenNorm ), blueNorm ) /
                rgbSum ) );
                
              intensityRow[ outCol ] = (float)( rgbSum / 3.0 );            
            }
          }
        }
      }
        
      return true;
    }
    
    bool IHSFusion::getStatistics( const te::rp::Matrix< float >& matrix, float& mean, 
      float& variance ) const
    {
      const unsigned int nRows = matrix.getLinesNumber();
      const unsigned int nCols = matrix.getColumnsNumber();
      
      if( ( nRows == 0 ) || ( nCols == 0 ) ) return false;
      
      unsigned int col = 0;
      unsigned int row = 0;
      float const * rowPtr = 0;
      
      mean = 0.0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        rowPtr = matrix[ row ];
        
        for( col = 0 ; col < nCols ; ++col )
        {
          mean += rowPtr[ col ];
        }
      }
      
      mean /= (float)( nCols * nRows );
      
      float diff = 0.0;
      variance = 0.0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        rowPtr = matrix[ row ];
        
        for( col = 0 ; col < nCols ; ++col )
        {
          diff = rowPtr[ col ] - mean;
          variance += ( diff * diff );
        }
      }
      
      return true;
    }
    
    bool IHSFusion::swapIntensity( te::rp::Matrix< float >& intensityData )
    {
      const unsigned int nRows = intensityData.getLinesNumber();
      const unsigned int nCols = intensityData.getColumnsNumber();
      
      if( ( nRows == 0 ) || ( nCols == 0 ) ) return false;
      
      assert( nRows == m_inputParameters.m_highResRasterPtr->getNumberOfRows() );
      assert( nCols == m_inputParameters.m_highResRasterPtr->getNumberOfColumns() );
      
      // intensity statistics
      
      float intensityMean = 0.0;
      float intensityVariance = 0.0;
      getStatistics( intensityData, intensityMean, intensityVariance );     
      
      // high resolution raster statistics
      
      double rasterMean = 0;
      double rasterVariance = 0;
      
      unsigned int col = 0;
      unsigned int row = 0;
      double value = 0;
      const te::rst::Band& band = *( m_inputParameters.m_highResRasterPtr->getBand(
        m_inputParameters.m_highResRasterBand ) );
      double diff = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          band.getValue( col, row, value );
          rasterMean += value;
        }
      }
      
      rasterMean /= (double)( nCols * nRows );

      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          band.getValue( col, row, value );
          diff = value - rasterMean;
          rasterVariance += ( diff * diff );
        }
      }
      
      const double gain = ( ( rasterVariance == 0.0 ) ? 0.0 :
        std::sqrt( (double)intensityVariance ) / std::sqrt( rasterVariance ) );
      float* intensityRow = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        intensityRow = intensityData[ row ];
        
        for( col = 0 ; col < nCols ; ++col )
        {
          band.getValue( col, row, value );
          intensityRow[ col ] = (float)std::min( 1.0, std::max( 0.0, ( ( value - rasterMean ) * gain ) + ((double)intensityMean) ) );
        }
      }      
      
      return true;
    }
    
    bool IHSFusion::saveIHSData( const double& rgbMin, const double rgbMax, const te::rp::Matrix< float >& intensityData,
      const te::rp::Matrix< float >& hueData, const te::rp::Matrix< float >& saturationData,
      const std::string& rType, const std::map< std::string, std::string >& rInfo,
      std::auto_ptr< te::rst::Raster >& outputRasterPtr ) const
    {
      assert( intensityData.getColumnsNumber() == m_inputParameters.m_highResRasterPtr->getNumberOfColumns() );
      assert( intensityData.getLinesNumber() == m_inputParameters.m_highResRasterPtr->getNumberOfRows() );
      assert( intensityData.getColumnsNumber() == hueData.getColumnsNumber() );
      assert( intensityData.getColumnsNumber() == saturationData.getColumnsNumber() );
      assert( intensityData.getLinesNumber() == hueData.getLinesNumber() );
      assert( intensityData.getLinesNumber() == saturationData.getLinesNumber() );      
      
      const unsigned int nRows = intensityData.getLinesNumber();
      const unsigned int nCols = intensityData.getColumnsNumber();
      
      std::vector< te::rst::BandProperty* > outRasterBandsProperties;
      outRasterBandsProperties.push_back( new te::rst::BandProperty(
        *( m_inputParameters.m_lowResRasterPtr->getBand( 
        m_inputParameters.m_lowResRasterRedBandIndex )->getProperty() ) ) );    
      outRasterBandsProperties[ 0 ]->m_blkw = nCols;
      outRasterBandsProperties[ 0 ]->m_blkh = 1;
      outRasterBandsProperties[ 0 ]->m_nblocksx = 1;
      outRasterBandsProperties[ 0 ]->m_nblocksy = nRows;
      outRasterBandsProperties.push_back( new te::rst::BandProperty(
        *( m_inputParameters.m_lowResRasterPtr->getBand( 
        m_inputParameters.m_lowResRasterGreenBandIndex )->getProperty() ) ) ); 
      outRasterBandsProperties[ 1 ]->m_blkw = nCols;
      outRasterBandsProperties[ 1 ]->m_blkh = 1;
      outRasterBandsProperties[ 1 ]->m_nblocksx = 1;
      outRasterBandsProperties[ 1 ]->m_nblocksy = nRows;      
      outRasterBandsProperties.push_back( new te::rst::BandProperty(
        *( m_inputParameters.m_lowResRasterPtr->getBand( 
        m_inputParameters.m_lowResRasterBlueBandIndex )->getProperty() ) ) );         
      outRasterBandsProperties[ 2 ]->m_blkw = nCols;
      outRasterBandsProperties[ 2 ]->m_blkh = 1;
      outRasterBandsProperties[ 2 ]->m_nblocksx = 1;
      outRasterBandsProperties[ 2 ]->m_nblocksy = nRows;      
      
      te::rst::Grid* gridPtr = new te::rst::Grid( *m_inputParameters.m_highResRasterPtr->getGrid() );

      outputRasterPtr.reset(
        te::rst::RasterFactory::make(
          rType,
          gridPtr,
          outRasterBandsProperties,
          rInfo,
          0,
          0 ) );
      TERP_TRUE_OR_RETURN_FALSE( outputRasterPtr.get(),
        "Output raster creation error" );  
        
      const double rgbNormFac = ( rgbMax == rgbMin ) ? 0.0 :
        ( rgbMax - rgbMin ); 
      const double pi3 = M_PI / 3.0; // 60
      const double twoPi3 = 2.0 * M_PI / 3.0; // 120
      const double fourPi3 = 4.0 * M_PI / 3.0; // 240        
      unsigned int row = 0;
      unsigned int col = 0;  
      double hue = 0;
      double lig = 0;
      double sat = 0;
      double red = 0;
      double green = 0;
      double blue = 0;  
      te::rst::Band& redBand = *outputRasterPtr->getBand( 0 );
      te::rst::Band& greenBand = *outputRasterPtr->getBand( 1 );
      te::rst::Band& blueBand = *outputRasterPtr->getBand( 2 );
      
      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {     
          hue = hueData[ row ][ col ];
          lig = intensityData[ row ][ col ];
          sat = saturationData[ row ][ col ];
          
          if( ( hue == 0.0 ) && ( sat == 0.0 ) )
          { // Gray scale case
            red = green = blue = ( lig * rgbNormFac );
          }
          else
          { // color case
            /* Hue inside RG sector */
            if( hue < twoPi3 )
            {
              blue = lig * ( 1.0 - sat );
              red = lig * ( 1.0 + ( sat * std::cos( hue ) / 
                std::cos( pi3 - hue ) ) );
              green = ( 3.0 * lig ) - ( red + blue );
            }
            else if( hue < fourPi3 )
            { /* Hue inside GB sector */
            
              hue -= twoPi3;
              
              red = lig * ( 1.0 - sat );
              green = lig * ( 1.0 + ( sat * std::cos( hue ) / 
                std::cos( pi3 - hue ) ) );
              blue = ( 3.0 * lig ) - ( red + green );
            }
            else
            { /* Hue inside BR sector */
            
              hue -= fourPi3;
              
              green = lig * ( 1.0 - sat );
              blue = lig * ( 1.0 + ( sat * std::cos( hue ) / 
                std::cos( pi3 - hue ) ) );
              red = ( 3.0 * lig ) - ( green + blue );
            }
            
            red = ( red * rgbNormFac ) + rgbMin;
            green = ( green * rgbNormFac ) + rgbMin;
            blue = ( blue * rgbNormFac ) + rgbMin;
          }
          
          red = MIN( red, rgbMax );
          green = MIN( green, rgbMax );
          blue = MIN( blue, rgbMax );
          
          red = MAX( red, rgbMin );
          green = MAX( green, rgbMin );
          blue = MAX( blue, rgbMin );           
          
          redBand.setValue( col, row, red );
          greenBand.setValue( col, row, green );
          blueBand.setValue( col, row, blue );
        }
      }
      
      return true;
    }
    
  }
}   // end namespace te

