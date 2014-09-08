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
  \file terralib/rp/WisperFusion.cpp
  \brief Creation of skeleton imagems.
*/

#include "WisperFusion.h"
#include "SpectralResponseFunctions.h"
#include "Macros.h"
#include "Functions.h"
#include "Matrix.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Band.h"
#include "../raster/Grid.h"
#include "../raster/Utils.h"
#include "../geometry/Envelope.h"
#include "../common/progress/TaskProgress.h"
#include "../memory/ExpansibleRaster.h"

#include <cmath>
#include <limits>
  
namespace te
{
  namespace rp
  {

    WisperFusion::InputParameters::InputParameters()
    {
      reset();
    }

    WisperFusion::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    WisperFusion::InputParameters::~InputParameters()
    {
      reset();
    }

    void WisperFusion::InputParameters::reset() throw( te::rp::Exception )
    {
      m_lowResRasterPtr = 0;
      m_lowResRasterBands.clear();
      m_lowResRasterBandSensors.clear();
      m_lowResRasterBandsSRFs.clear();
      m_highResRasterPtr = 0;
      m_highResRasterBand = 0;
      m_hiResRasterBandSensor = te::rp::srf::InvalidSensor;
      m_hiResRasterBandsSRFs.clear();
      m_hiResRasterWaveletLevels = 0;
      m_enableProgress = false;
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
      m_waveletFilterType = te::rp::B3SplineWAFilter;
      m_userWaveletFilterPtr = 0;
      m_enableMultiThread = true;
    }

    const WisperFusion::InputParameters& WisperFusion::InputParameters::operator=(
      const WisperFusion::InputParameters& params )
    {
      reset();

      m_lowResRasterPtr = params.m_lowResRasterPtr;
      m_lowResRasterBands = params.m_lowResRasterBands;
      m_lowResRasterBandSensors = params.m_lowResRasterBandSensors;
      m_lowResRasterBandsSRFs = params.m_lowResRasterBandsSRFs;
      m_highResRasterPtr = params.m_highResRasterPtr;
      m_highResRasterBand = params.m_highResRasterBand;
      m_hiResRasterBandSensor = params.m_hiResRasterBandSensor;
      m_hiResRasterBandsSRFs = params.m_hiResRasterBandsSRFs;
      m_hiResRasterWaveletLevels = params.m_hiResRasterWaveletLevels;
      m_enableProgress = params.m_enableProgress;
      m_interpMethod = params.m_interpMethod;
      m_waveletFilterType = params.m_waveletFilterType;
      m_userWaveletFilterPtr = params.m_userWaveletFilterPtr;
      m_enableMultiThread = params.m_enableMultiThread;
      
      return *this;
    }

    te::common::AbstractParameters* WisperFusion::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    WisperFusion::OutputParameters::OutputParameters()
    {
      reset();
    }

    WisperFusion::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    WisperFusion::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void WisperFusion::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const WisperFusion::OutputParameters& WisperFusion::OutputParameters::operator=(
      const WisperFusion::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* WisperFusion::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    WisperFusion::WisperFusion()
    {
      reset();
    }

    WisperFusion::~WisperFusion()
    {
    }

    bool WisperFusion::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      WisperFusion::OutputParameters* outParamsPtr = dynamic_cast<
        WisperFusion::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 3 );
        
        progressPtr->setMessage( "Fusing images" );
      }        

      // defining the wavelet filter
      
      boost::numeric::ublas::matrix< double > waveletFilter;
      
      if( m_inputParameters.m_userWaveletFilterPtr )
      {
        waveletFilter = *( m_inputParameters.m_userWaveletFilterPtr );
      }
      else
      {
        waveletFilter = te::rp::CreateWaveletAtrousFilter( 
          m_inputParameters.m_waveletFilterType );
      }
      
      // loading the SRFs
        
      std::vector< std::map< double, double > > lowResSRFs;
      std::map< double, double > highResSRFs;
      
      {  
        if( m_inputParameters.m_lowResRasterBandsSRFs.empty() )
        {
          std::map< double, double > auxMap;
          
          for( unsigned int sensorIdx = 0 ; sensorIdx < 
            m_inputParameters.m_lowResRasterBandSensors.size() ; ++sensorIdx )
          {
            auxMap.clear();
            te::rp::srf::getSRF( m_inputParameters.m_lowResRasterBandSensors[ sensorIdx ],
              auxMap );
            lowResSRFs.push_back( auxMap );
          }
        }
        else
        {
          lowResSRFs = m_inputParameters.m_lowResRasterBandsSRFs;
        }
        
        if( m_inputParameters.m_hiResRasterBandsSRFs.empty() )
        {
          te::rp::srf::getSRF( m_inputParameters.m_hiResRasterBandSensor,
            highResSRFs );
        }
        else
        {
          highResSRFs = m_inputParameters.m_hiResRasterBandsSRFs;
        }
      }
      
      // Computing the the intersetion of the area covered by each low resolution band pair
      // and with the high resolution band
      
      te::rp::Matrix< double > lRBandXlRBandSRFIntersectionAreas; // the LRxLR bands intersection areas.
      std::vector< unsigned int > lRInterceptedBandIndex; // the index of the band intercepted by another.
      std::vector< double > lowResBandsSRFAreas;
      std::vector< double > lRBandXHRBandIntersectionAreas;
      double lRBandsXHRBandTotalExclusiveIntersectionSRFArea = 0.0; // The LRxLR bands overlap is taken into account.
      const double hiResBandSRFArea = getSRFArea( highResSRFs );
      
      {
        unsigned int lowResBandIdx1 = 0;
        unsigned int lowResBandIdx2 = 0;
        const unsigned int nLowResBands = m_inputParameters.m_lowResRasterBands.size();        
        
        // low resolution
        
        std::map< double, double > lRBandsXHRBandTotalExclusiveIntersectionSRF;
        
        lRBandXlRBandSRFIntersectionAreas.reset( nLowResBands, nLowResBands );
        lowResBandsSRFAreas.resize( nLowResBands, 0.0 );
        lRBandXHRBandIntersectionAreas.resize( nLowResBands, 0.0 );
        for( lowResBandIdx1 = 0 ; lowResBandIdx1 < nLowResBands ; ++lowResBandIdx1 )
        {
          for( lowResBandIdx2 = lowResBandIdx1 ; lowResBandIdx2 < nLowResBands ; 
            ++lowResBandIdx2 )
          {
            if( lowResBandIdx1 != lowResBandIdx2 )
            {
              std::map< double, double > lRXLRIntersectionSRF;
              getIntersectionSRF( lowResSRFs[ lowResBandIdx1 ], 
                lowResSRFs[ lowResBandIdx2 ], lRXLRIntersectionSRF );
              
              lRBandXlRBandSRFIntersectionAreas[ lowResBandIdx1 ][ lowResBandIdx2 ] = 
                getSRFArea( lRXLRIntersectionSRF );
              lRBandXlRBandSRFIntersectionAreas[ lowResBandIdx2 ][ lowResBandIdx1 ] =
                lRBandXlRBandSRFIntersectionAreas[ lowResBandIdx1 ][ lowResBandIdx2 ];
            }
            else
            {
              lRBandXlRBandSRFIntersectionAreas[ lowResBandIdx1 ][ lowResBandIdx2 ] = 0.0;
            }
          }
          
          lowResBandsSRFAreas[ lowResBandIdx1 ] = getSRFArea( lowResSRFs[ lowResBandIdx1 ] );
          TERP_TRUE_OR_RETURN_FALSE( lowResBandsSRFAreas[ lowResBandIdx1 ] > 0.0,
            "One low resolution band SRF is invalid" );          
          
          std::map< double, double > lRXHRIntersectionSRF;
          getIntersectionSRF( lowResSRFs[ lowResBandIdx1 ], highResSRFs, lRXHRIntersectionSRF );
          
          lRBandXHRBandIntersectionAreas[ lowResBandIdx1 ] = getSRFArea( lRXHRIntersectionSRF );
          TERP_TRUE_OR_RETURN_FALSE( lRBandXHRBandIntersectionAreas[ lowResBandIdx1 ] > 0.0,
            "One low resolution band SRF does not intersects the high resolution band SRF" );
          
          std::map< double, double > auxUnionSRF;
          getUnionSRF( lRXHRIntersectionSRF, lRBandsXHRBandTotalExclusiveIntersectionSRF, auxUnionSRF );
          lRBandsXHRBandTotalExclusiveIntersectionSRF = auxUnionSRF;
        }        
        
        lRBandsXHRBandTotalExclusiveIntersectionSRFArea = getSRFArea( 
          lRBandsXHRBandTotalExclusiveIntersectionSRF );
        
        std::multimap< double, unsigned int > centralFrequency2BandIdxMap;
        for( lowResBandIdx1 = 0 ; lowResBandIdx1 < nLowResBands ; ++lowResBandIdx1 )
        {
          std::map< double, double >::const_iterator lowResSRFsIt = 
            lowResSRFs[ lowResBandIdx1 ].begin();
          std::map< double, double >::const_iterator lowResSRFsItEnd = 
            lowResSRFs[ lowResBandIdx1 ].end();
          double highestResponse = -1.0 * std::numeric_limits< double >::max();
          double highestResponseFrequency = 0;
          
          while( lowResSRFsIt != lowResSRFsItEnd )
          {
            if( highestResponse < lowResSRFsIt->second )
            {
              highestResponse = lowResSRFsIt->second;
              highestResponseFrequency = lowResSRFsIt->first;
            }
            
            ++lowResSRFsIt;
          }
          
          centralFrequency2BandIdxMap.insert( std::pair< double, unsigned int>( 
            highestResponseFrequency, lowResBandIdx1 ) );
        }
        
        lRInterceptedBandIndex.resize( nLowResBands, nLowResBands + 1 );
        std::map< double, unsigned int >::iterator centralFrequency2BandIdxMapIt =
          centralFrequency2BandIdxMap.begin();
        std::map< double, unsigned int >::iterator centralFrequency2BandIdxMapItPrev;
        std::map< double, unsigned int >::iterator centralFrequency2BandIdxMapItEnd =
          centralFrequency2BandIdxMap.end();          
        while( centralFrequency2BandIdxMapIt != centralFrequency2BandIdxMapItEnd )
        {
          if( centralFrequency2BandIdxMapIt == centralFrequency2BandIdxMap.begin() )
          {
            lRInterceptedBandIndex[ centralFrequency2BandIdxMapIt->second ] = 
              centralFrequency2BandIdxMapIt->second;
            centralFrequency2BandIdxMapItPrev = centralFrequency2BandIdxMapIt;
          }
          else
          {
            lRInterceptedBandIndex[ centralFrequency2BandIdxMapIt->second ] =
              centralFrequency2BandIdxMapItPrev->second;
            
            ++centralFrequency2BandIdxMapItPrev;
          }
            
          ++centralFrequency2BandIdxMapIt;
        }
      }       
     
      // The wavelet decomposition levels
      
      const unsigned int highResWaveletLevels = (unsigned int)
        m_inputParameters.m_hiResRasterWaveletLevels ?
          m_inputParameters.m_hiResRasterWaveletLevels 
          :
          (
            0.5
            +
            std::max(
              (
                std::log(      
                  (double)
                  (
                    m_inputParameters.m_highResRasterPtr->getNumberOfColumns()
                  )
                  /
                  (double)
                  (  
                    m_inputParameters.m_lowResRasterPtr->getNumberOfColumns()
                  )
                )
                /
                log( 2.0 )
              )
              ,
              (
                std::log(      
                  (double)
                  (
                    m_inputParameters.m_highResRasterPtr->getNumberOfRows()
                  )
                  /
                  (double)
                  (  
                    m_inputParameters.m_lowResRasterPtr->getNumberOfRows()
                  )
                )
                /
                log( 2.0 )
              )
            )
          );  
      TERP_TRUE_OR_RETURN_FALSE( highResWaveletLevels > 0, 
        "Minimal number of wavelet decompositions not reached" );   
      
      // creating the ressampled input raster
      
      std::auto_ptr< te::rst::Raster > resampledLlowResRasterPtr;
      
      {
        std::map< std::string, std::string > rinfo;
        rinfo["MAXMEMPERCENTUSED"] = "30";  
        
        TERP_TRUE_OR_RETURN_FALSE( te::rp::RasterResample(
          *m_inputParameters.m_lowResRasterPtr,
          m_inputParameters.m_lowResRasterBands,
          m_inputParameters.m_interpMethod,
          0,
          0,
          m_inputParameters.m_lowResRasterPtr->getNumberOfRows(),
          m_inputParameters.m_lowResRasterPtr->getNumberOfColumns(),
          m_inputParameters.m_highResRasterPtr->getNumberOfRows(),
          m_inputParameters.m_highResRasterPtr->getNumberOfColumns(),  
          rinfo,
          "EXPANSIBLE",
          resampledLlowResRasterPtr ), 
          "Low resolution raster resample error" );
      }
      
//       TERP_TRUE_OR_THROW( te::rp::Copy2DiskRaster( *resampledLlowResRasterPtr,
//         "resampledLlowResRaster.tif" ), "" );

      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }              
            
        
      // creating the high resolution raster wavelets
        
      std::auto_ptr< te::rst::Raster > highResWaveletsRasterPtr;
      
      {
        std::map<std::string, std::string> auxRasterInfo;
        
        std::vector< te::rst::BandProperty * > bandProps;

        for( unsigned int levelIdx = 0 ; levelIdx < highResWaveletLevels ;
          ++levelIdx )
        {
          bandProps.push_back( new te::rst::BandProperty( 
            *( m_inputParameters.m_highResRasterPtr->getBand( 
            m_inputParameters.m_highResRasterBand )->getProperty() ) ) );
          bandProps.back()->m_blkh = 1;
          bandProps.back()->m_blkw = m_inputParameters.m_highResRasterPtr->getNumberOfColumns();
          bandProps.back()->m_nblocksx = 1;
          bandProps.back()->m_nblocksy = m_inputParameters.m_highResRasterPtr->getNumberOfRows();
          bandProps.back()->m_type = te::dt::DOUBLE_TYPE;
          
          bandProps.push_back( new te::rst::BandProperty( 
            *( m_inputParameters.m_highResRasterPtr->getBand( 
            m_inputParameters.m_highResRasterBand )->getProperty() ) ) );
          bandProps.back()->m_blkh = 1;
          bandProps.back()->m_blkw = m_inputParameters.m_highResRasterPtr->getNumberOfColumns();
          bandProps.back()->m_nblocksx = 1;
          bandProps.back()->m_nblocksy = m_inputParameters.m_highResRasterPtr->getNumberOfRows();
          bandProps.back()->m_type = te::dt::DOUBLE_TYPE;      
        }
        
        std::vector< unsigned int > rasterBands;
        rasterBands.push_back( m_inputParameters.m_highResRasterBand );
        
        highResWaveletsRasterPtr.reset( new te::mem::ExpansibleRaster( 
          60,
          new te::rst::Grid( *m_inputParameters.m_highResRasterPtr->getGrid() ),
          bandProps ) );
        TERP_TRUE_OR_RETURN_FALSE( highResWaveletsRasterPtr.get(), "Raster allocation error" );
        
        TERP_TRUE_OR_RETURN_FALSE( te::rp::DirectWaveletAtrous( 
          *m_inputParameters.m_highResRasterPtr, 
          rasterBands,
          *highResWaveletsRasterPtr,
          highResWaveletLevels,
          waveletFilter ),
          "Low resolution raster wavelets creation error" ); 
      }
      
/*      TERP_TRUE_OR_THROW( te::rp::Copy2DiskRaster( *highResWaveletsRasterPtr,
        "highResWaveletsRaster.tif" ), "" ); */     
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }
      
      // Creating the output raster
      
      {
        te::rst::Grid* gridPtr = new te::rst::Grid( *resampledLlowResRasterPtr->getGrid() );
        std::vector< te::rst::BandProperty * > bandProperties;
        std::vector< unsigned int > outputRasterBands;
          
        for( unsigned int bandIdx = 0 ; bandIdx <
          resampledLlowResRasterPtr->getNumberOfBands() ; ++bandIdx )
        {
          bandProperties.push_back( new te::rst::BandProperty( 
            *resampledLlowResRasterPtr->getBand( bandIdx )->getProperty() ) );
            
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
      }       
      
      // Recomposing levels for each band
      
      {
        const unsigned int nRows = outParamsPtr->m_outputRasterPtr->getNumberOfRows();
        const unsigned int nCols = outParamsPtr->m_outputRasterPtr->getNumberOfColumns();
        const unsigned int nBands = outParamsPtr->m_outputRasterPtr->getNumberOfBands();
        te::rst::Raster& resampledLlowResRaster = *resampledLlowResRasterPtr;
        te::rst::Raster& outputRaster = *(outParamsPtr->m_outputRasterPtr);
        te::rst::Raster& highResWaveletsRaster = *highResWaveletsRasterPtr;
        const unsigned int highResWaveletsRasterBands = highResWaveletsRaster.getNumberOfBands();
        unsigned int row = 0;
        unsigned int col = 0;
        unsigned int bandIdx = 0;
        std::vector< double > resLRRasterValues( nBands );
        unsigned int waveletBandIdx = 0;
        double outputRasterValue = 0;
        double resampledLlowResRasterValue = 0;
        double wisperTerm = 0;
        double highResWaveletsValue = 0.0;
        std::vector< double > ropi( nBands );
        double ropiMean = 0;
        
        std::vector< double > outBandsMinValue( nBands );
        std::vector< double > outBandsMaxValue( nBands );
        std::vector< double > resampledLlowResBandsGains( nBands );
        for( bandIdx = 0 ; bandIdx < nBands ;  ++bandIdx )
        {           
          te::rp::GetDataTypeRange( 
            outParamsPtr->m_outputRasterPtr->getBandDataType( bandIdx ),
            outBandsMinValue[ bandIdx ], outBandsMaxValue[ bandIdx ] );
        }

        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {     
            ropiMean = 0.0;
            for( bandIdx = 0 ; bandIdx < nBands ;  ++bandIdx )
            {
              resampledLlowResRaster.getValue( col, row, resampledLlowResRasterValue, bandIdx );
              
              resLRRasterValues[ bandIdx ] = resampledLlowResRasterValue;
              
              assert( lRBandXHRBandIntersectionAreas[ bandIdx ] > 0.0 );
              ropi[ bandIdx ] = 
                (
                  (
                    lRBandXHRBandIntersectionAreas[ bandIdx ]
                    *
                    lRBandXHRBandIntersectionAreas[ bandIdx ]
                    *
                    resLRRasterValues[ bandIdx ]
                  )
                  /
                  lowResBandsSRFAreas[ bandIdx ]
                );
                  
              ropiMean += ropi[ bandIdx ];
            }
            ropiMean /= ((double)nBands);
            
            for( bandIdx = 0 ; bandIdx < nBands ;  ++bandIdx )
            {      
              wisperTerm =
                (
                  // Si
                  (
                    ropi[ bandIdx ]
                    /
                    ropiMean
                  )
                  *
                  (
                    // alpha
                    (
                      lRBandsXHRBandTotalExclusiveIntersectionSRFArea
                      /
                      hiResBandSRFArea
                    )
                    *
                    // P( mi | pm )
                    (
                      lRBandXHRBandIntersectionAreas[ bandIdx ]
                      /
                      lRBandsXHRBandTotalExclusiveIntersectionSRFArea
                    )
                    /
                    // P( pm | mi )
                    (
                      lRBandXHRBandIntersectionAreas[ bandIdx ]
                      /
                      lowResBandsSRFAreas[ bandIdx ]
                    )
                  )
                  *
                  (
                    1.0
                    -
                    (
                      (
                        // beta
                        lRBandXlRBandSRFIntersectionAreas[ bandIdx ][ lRInterceptedBandIndex[ bandIdx ] ]
                        /
                        lRBandXHRBandIntersectionAreas[ bandIdx ]
                      )
                      /
                      2.0
                    )
                  )
                );
                
              outputRasterValue = resLRRasterValues[ bandIdx ];
              
              for( waveletBandIdx = 1 ; waveletBandIdx < highResWaveletsRasterBands ; 
                waveletBandIdx += 2 )
              {
                highResWaveletsRaster.getValue( col, row, highResWaveletsValue, 
                  waveletBandIdx );
                outputRasterValue += ( wisperTerm * highResWaveletsValue );
              }         

              outputRasterValue = std::max( outBandsMinValue[ bandIdx ], 
                outputRasterValue );
              outputRasterValue = std::min( outBandsMaxValue[ bandIdx ], 
                outputRasterValue );
              
              outputRaster.setValue( col, row, outputRasterValue, bandIdx );
            }
          }
        }
       
      } 
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }      
      
      return true;
    }

    void WisperFusion::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool WisperFusion::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      WisperFusion::InputParameters const* inputParamsPtr = dynamic_cast<
        WisperFusion::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the low Resolution related parameters

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterPtr,
        "Invalid low Resolution Raster Pointer" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_lowResRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
      
      for( unsigned int lowResRasterBandsIdx = 0; lowResRasterBandsIdx <
        m_inputParameters.m_lowResRasterBands.size() ; 
        ++lowResRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_lowResRasterBands[ lowResRasterBandsIdx ] <
          m_inputParameters.m_lowResRasterPtr->getNumberOfBands(), 
          "Invalid low resolution raster band" );   
      }
      
      if( m_inputParameters.m_lowResRasterBandsSRFs.empty() )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterBandSensors.size()
          == m_inputParameters.m_lowResRasterBands.size(),
          "Invalid low resolution bands sensors" );
        
        for( unsigned int idx = 0 ; idx < m_inputParameters.m_lowResRasterBandSensors.size() ;
          ++idx )
        {
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterBandSensors[ idx ] !=
            te::rp::srf::InvalidSensor, "Invalid low resolution sensor" );
        }
      }
      else
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterBandsSRFs.size()
          == m_inputParameters.m_lowResRasterBands.size(),
          "Missing low resolution bands SRFs" );
        
        for( unsigned int idx = 0 ; idx < m_inputParameters.m_lowResRasterBandsSRFs.size() ;
          ++idx )
        {
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterBandsSRFs[ idx ].size(), 
            "Invalid low resolution SRF" );
        }        
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
      
      if( m_inputParameters.m_hiResRasterBandsSRFs.empty() )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_hiResRasterBandSensor !=
          te::rp::srf::InvalidSensor, "Invalid high resolution sensor" );
      }
      else
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_hiResRasterBandSensor !=
          te::rp::srf::InvalidSensor, "Invalid high resolution sensor" );
      }     
      
      // others
      
      if( m_inputParameters.m_userWaveletFilterPtr )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          ( m_inputParameters.m_userWaveletFilterPtr->size1() > 2 ) &&
          ( m_inputParameters.m_userWaveletFilterPtr->size1() ==
          m_inputParameters.m_userWaveletFilterPtr->size2() ), 
          "Invalid user filter" );
      }
        
      m_isInitialized = true;

      return true;
    }

    bool WisperFusion::isInitialized() const
    {
      return m_isInitialized;
    }
    
    double WisperFusion::interpolateSRF( const std::map< double, double >& sRFs, 
      const double& frequency ) const
    {
      std::map< double, double >::const_iterator it2 = sRFs.find( frequency );
      
      if( it2 == sRFs.end() )
      {
        it2 = sRFs.upper_bound( frequency );
        
        if( ( it2 == sRFs.begin() ) || ( it2 == sRFs.end() ) )
        {
          return 0.0;
        }
        else
        {
          std::map< double, double >::const_iterator it1 = it2;
          --it1;
          
          double dist1 = frequency - it1->first;
          assert( dist1 > 0.0 );          
          double dist2 = it2->first - frequency;
          assert( dist2 > 0.0 );
          
          return ( 
                    ( it1->second * dist2 ) 
                    + 
                    ( it2->second * dist1 )
                 ) 
                 / 
                 ( dist1  + dist2 );
        }
      }
      else
      {
        return it2->second;
      }
    }
    
    double WisperFusion::getSRFArea( const std::map< double, double >& sRFs ) const
    {
      double returnValue = 0.0;
      
      std::map< double, double >::const_iterator it = sRFs.begin();
      std::map< double, double >::const_iterator itPrev;
      const std::map< double, double >::const_iterator itEnd = sRFs.end();      
      
      while( it != itEnd )
      {
        if( it == sRFs.begin() )
        {
          itPrev = it;
        }
        else
        {
          returnValue += std::abs( it->second - itPrev->second ) * 
            ( it->first - itPrev->first ) / 2.0;
          returnValue += ( it->first - itPrev->first ) *
            std::min( it->second, itPrev->second );
          
          ++itPrev;
        }
        
        ++it;
      }
      
      return returnValue;
    }
    
    void WisperFusion::getIntersectionSRF( const std::map< double, double >& sRF1, 
      const std::map< double, double >& sRF2,
      std::map< double, double >& intersectionSRF ) const
    {
      getUnionSRF( sRF1, sRF2, intersectionSRF );
      
      std::map< double, double >::const_iterator it = intersectionSRF.begin();
      const std::map< double, double >::const_iterator itEnd = intersectionSRF.end();
      
      while( it != itEnd )
      {
        intersectionSRF[ it->first ] =
          std::min(
            interpolateSRF( sRF1, it->first ),
            interpolateSRF( sRF2, it->first )
          );
          
        ++it;
      }
    }
    
    void WisperFusion::getUnionSRF( const std::map< double, double >& sRF1, 
      const std::map< double, double >& sRF2,
      std::map< double, double >& unionSRF ) const
    {
      unionSRF = sRF2;
      
      std::map< double, double >::const_iterator it1 = sRF1.begin();
      const std::map< double, double >::const_iterator it1End = sRF1.end();
      
      while( it1 != it1End )
      {
        unionSRF[ it1->first ] = 
          std::max( 
            interpolateSRF( unionSRF, it1->first ),
            it1->second
          );
          
        ++it1;
      }
    }    
    
  }
}   // end namespace te

