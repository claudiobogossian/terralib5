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
  \file terralib/rp/Skeleton.cpp
  \brief Creation of skeleton imagems.
*/

#include "Skeleton.h"
#include "Macros.h"

#include "../raster/Band.h"
#include "../raster/Utils.h"
#include "../common/PlatformUtils.h"

#include <boost/lexical_cast.hpp>
#include <boost/scoped_array.hpp>

#include <memory>
#include <cmath> 
#include <cstring>
    
#define ADDNEIGHBOR( neighborRow, neighborCol ) \
  neiX = bufferX[ neighborRow ][ neighborCol ]; \
  neiY = bufferY[ neighborRow ][ neighborCol ]; \
  neiMag = std::sqrt( ( neiX * neiX ) + ( neiY * neiY ) ); \
  if( neiMag != 0.0 ) \
  { \
    newCenterX += ( neiX / neiMag ); \
    newCenterY += ( neiY / neiMag ); \
  }
  
namespace te
{
  namespace rp
  {

    Skeleton::InputParameters::InputParameters()
    {
      reset();
    }

    Skeleton::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    Skeleton::InputParameters::~InputParameters()
    {
      reset();
    }

    void Skeleton::InputParameters::reset() throw( te::rp::Exception )
    {
      m_inputRasterPtr = 0;
      m_inputRasterBand = 0;
      m_inputMaskRasterPtr = 0;
      m_diffusionThreshold = 0.1;
      m_diffusionRegularitation = 0.5;
      m_enableMultiThread = true;
    }

    const Skeleton::InputParameters& Skeleton::InputParameters::operator=(
      const Skeleton::InputParameters& params )
    {
      reset();

      m_inputRasterPtr = params.m_inputRasterPtr;
      m_inputRasterBand = params.m_inputRasterBand;
      m_inputMaskRasterPtr = params.m_inputMaskRasterPtr;
      m_diffusionThreshold = params.m_diffusionThreshold;
      m_diffusionRegularitation = params.m_diffusionRegularitation;
      m_enableMultiThread = params.m_enableMultiThread;

      return *this;
    }

    te::common::AbstractParameters* Skeleton::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    Skeleton::OutputParameters::OutputParameters()
    {
      reset();
    }

    Skeleton::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    Skeleton::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void Skeleton::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const Skeleton::OutputParameters& Skeleton::OutputParameters::operator=(
      const Skeleton::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* Skeleton::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    Skeleton::Skeleton()
    {
      reset();
    }

    Skeleton::~Skeleton()
    {
    }

    bool Skeleton::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      Skeleton::OutputParameters* outParamsPtr = dynamic_cast<
        Skeleton::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      std::auto_ptr< te::rp::Matrix< double > > rasterDataPtr( 
        new te::rp::Matrix< double > );      ;
      rasterDataPtr->reset( te::rp::Matrix< double >::AutoMemPol );       
      TERP_TRUE_OR_RETURN_FALSE( loadData( *rasterDataPtr ),
        "Gradient maps build error" );      
//      createTifFromMatrix( *rasterDataPtr, true, "rasterData" );    
      
      {
        std::auto_ptr< te::rp::Matrix< double > > smoothedRasterDataPtr( 
          new te::rp::Matrix< double > );      ;
        smoothedRasterDataPtr->reset( te::rp::Matrix< double >::AutoMemPol );       
        TERP_TRUE_OR_RETURN_FALSE( applyMeanSmooth( *rasterDataPtr, *smoothedRasterDataPtr ),
          "Raster data smoothing error" );
//        createTifFromMatrix( *smoothedRasterDataPtr, true, "smoothedRasterData" );              
        rasterDataPtr.reset( smoothedRasterDataPtr.release() );        
      }
        
      te::rp::Matrix< double > edgeStrengthMap;
      edgeStrengthMap.reset( te::rp::Matrix< double >::AutoMemPol );    
      TERP_TRUE_OR_RETURN_FALSE( getEdgeStrengthMap( *rasterDataPtr, 
        edgeStrengthMap ), "Edge strength map build error" ); 
//      createTifFromMatrix( edgeStrengthMap, true, "edgeStrengthMap" );
   
      te::rp::Matrix< double > vecXMap;
      vecXMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      te::rp::Matrix< double > vecYMap;
      vecYMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( getEdgeVecField( edgeStrengthMap, vecXMap, vecYMap ),
        "Vector maps build error" );      
//      createTifFromMatrix( vecXMap, true, "vecXMap" );
//      createTifFromMatrix( vecYMap, true, "vecYMap" );
//      createTifFromVecField( vecXMap, vecYMap, *rasterDataPtr, "vecMap" );
      
      te::rp::Matrix< double > diffusedVecXMap;
      diffusedVecXMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      te::rp::Matrix< double > diffusedVecYMap;
      diffusedVecYMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( applyVecDiffusion( vecXMap, vecYMap, *rasterDataPtr,
        diffusedVecXMap, diffusedVecYMap ),
        "Vector maps build error" );      
      createTifFromMatrix( diffusedVecXMap, true, "diffusedVecXMap" );
      createTifFromMatrix( diffusedVecYMap, true, "diffusedVecYMap" );
      createTifFromVecField( diffusedVecXMap, diffusedVecYMap, *rasterDataPtr, "diffusedVecMap" );      
      
      return true;
    }

    void Skeleton::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool Skeleton::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      Skeleton::InputParameters const* inputParamsPtr = dynamic_cast<
        Skeleton::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the input raster

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRasterPtr,
        "Invalid m_inputRasterPtr" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_inputRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_inputRasterBand <
        m_inputParameters.m_inputRasterPtr->getNumberOfBands(), 
        "Invalid raster band" );   
        
      if( m_inputParameters.m_inputMaskRasterPtr )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inputMaskRasterPtr->getAccessPolicy() & te::common::RAccess, 
          "Invalid mask raster" );

        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inputMaskRasterPtr->getNumberOfBands() > 0, 
          "Invalid mask raster band" );
          
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inputMaskRasterPtr->getNumberOfRows() ==
          m_inputParameters.m_inputRasterPtr->getNumberOfRows(), 
          "Invalid mask raster number of rows" );
          
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inputMaskRasterPtr->getNumberOfColumns() ==
          m_inputParameters.m_inputRasterPtr->getNumberOfColumns(), 
          "Invalid mask raster number of columns" );                    
      }
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_diffusionThreshold >= 0.0 ) &&
        ( m_inputParameters.m_diffusionThreshold <= 1.0 ),
        "Invalid diffusion threshold" );          
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_diffusionRegularitation >= 0.0 ) &&
        ( m_inputParameters.m_diffusionRegularitation <= 1.0 ),
        "Invalid diffusion regularitation" );           

      m_isInitialized = true;

      return true;
    }

    bool Skeleton::isInitialized() const
    {
      return m_isInitialized;
    }
    
    bool Skeleton::loadData( te::rp::Matrix< double >& rasterData ) const
    {
      const unsigned int nRows = m_inputParameters.m_inputRasterPtr->getNumberOfRows();
      const unsigned int nCols = m_inputParameters.m_inputRasterPtr->getNumberOfColumns();
      
      if( ! rasterData.reset( nRows,nCols ) )
        return false;     
      
      unsigned int row = 0;
      unsigned int col = 0;
      const te::rst::Band& band = *(m_inputParameters.m_inputRasterPtr->getBand( 
        m_inputParameters.m_inputRasterBand ));
      te::rst::Band const * const maskBandPtr = m_inputParameters.m_inputMaskRasterPtr ?
        m_inputParameters.m_inputMaskRasterPtr->getBand( 0 ) : 0;
      double maskValue = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          if( maskBandPtr )
          {
            maskBandPtr->getValue( col, row, maskValue );
            if( maskValue == 0.0 ) 
            {
              rasterData[ row ][ col ] = 0;
              continue;
            }
          }
          
          band.getValue( col, row , rasterData[ row ][ col ] );
        }
      }
      
      return true;
    }    

    bool Skeleton::getGradientMaps( const te::rp::Matrix< double >& inputData,
      te::rp::Matrix< double >& gradXMap,
      te::rp::Matrix< double >& gradYMap ) const
    {
      const unsigned int nRows = inputData.getLinesNumber();
      TERP_TRUE_OR_RETURN_FALSE( nRows > 2, "Invalid rows number" );
      
      const unsigned int nCols = inputData.getColumnsNumber();
      TERP_TRUE_OR_RETURN_FALSE( nCols > 2, "Invalid columns number" );
      
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      
      if( ! gradXMap.reset( nRows,nCols ) )
        return false;
      if( ! gradYMap.reset( nRows,nCols ) )
        return false;      
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          gradXMap[ row ][ col ] = 0;
          gradYMap[ row ][ col ] = 0;
        }
      }
      
      double value1 = 0;
      double value2 = 0;
      double value3 = 0;
      double value4 = 0;
      double value5 = 0;
      double value6 = 0;
      double value7 = 0;
      double value8 = 0;
      unsigned int nextRow = 0;
      unsigned int nextCol = 0;
      unsigned int prevRow = 0;
      unsigned int prevCol = 0;
      
      for( row = 1 ; row < lastRowIdx ; ++row )
      {
        prevRow = row - 1;
        nextRow = row + 1;
        
        for( col = 1 ; col < lastColIdx ; ++col )
        {
          prevCol = col - 1;
          nextCol = col + 1;
          
          value1 = inputData[ prevRow ][ prevCol ];
          value2 = inputData[ prevRow ][ col ];
          value3 = inputData[ prevRow ][ nextCol ];
          value4 = inputData[ row ][ prevCol ];
          value5 = inputData[ row ][ nextCol ];
          value6 = inputData[ nextRow ][ prevCol ];
          value7 = inputData[ nextRow ][ col ];
          value8 = inputData[ nextRow  ][ nextCol ];
          
          if( 
              ( inputData[ row ][ col ] == DBL_MAX ) ||
              ( value1 == DBL_MAX ) || ( value2 == DBL_MAX ) ||
              ( value3 == DBL_MAX ) || ( value4 == DBL_MAX ) ||
              ( value5 == DBL_MAX ) || ( value6 == DBL_MAX ) ||
              ( value7 == DBL_MAX ) || ( value8 == DBL_MAX ) 
            )
          {
            continue;
          }
          else
          {
            gradXMap[ row ][ col ] = ( ( value3 + value5 + value8 ) -
              ( value1 + value4 + value6 ) ) / 6.0;
            gradYMap[ row ][ col ] = ( ( value1 + value2 + value3 ) -
              ( value6 + value7 + value8 ) ) / 6.0;
          }
        }
      }
      
      return true;
    }
    
    bool Skeleton::getEdgeStrengthMap( 
      const te::rp::Matrix< double >& inputMap,
      te::rp::Matrix< double >& edgeStrengthMap ) const
    {
      const unsigned int nRows = inputMap.getLinesNumber();
      TERP_TRUE_OR_RETURN_FALSE( nRows > 4, "Invalid rows number" );
      
      const unsigned int nCols = inputMap.getColumnsNumber();
      TERP_TRUE_OR_RETURN_FALSE( nCols > 4, "Invalid columns number" );
      
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      
      if( ! edgeStrengthMap.reset( nRows,nCols ) )
        return false;      
      
      te::rp::Matrix< double > gradXMap;
      gradXMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      te::rp::Matrix< double > gradYMap;
      gradYMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( getGradientMaps( inputMap, gradXMap, gradYMap ),
        "Gradient maps build error" );       
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        edgeStrengthMap[ row ][ 0 ] = 0;
        edgeStrengthMap[ row ][ lastColIdx ] = 0;        
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        edgeStrengthMap[ 0 ][ col ] = 0;
        edgeStrengthMap[ lastRowIdx ][ col ] = 0;
      }       
      
      double x1 = 0;
      double y1 = 0;
      double x2 = 0;
      double y2 = 0;
      double x3 = 0;
      double y3 = 0;
      double x4 = 0;
      double y4 = 0;
      double x5 = 0;
      double y5 = 0;
      double x6 = 0;
      double y6 = 0;
      double x7 = 0;
      double y7 = 0;
      double x8 = 0;
      double y8 = 0;
      unsigned int nextRow = 0;
      unsigned int nextCol = 0;
      unsigned int prevRow = 0;
      unsigned int prevCol = 0;      
      double strengthYUp = 0;
      double strengthYDown = 0;
      double strengthXLeft = 0;
      double strengthXRight = 0;
      double strength = 0;
      double minStrength = DBL_MAX;
            
      for( row = 1 ; row < lastRowIdx ; ++row )
      {
        prevRow = row - 1;
        nextRow = row + 1;
        
        for( col = 1 ; col < lastColIdx ; ++col )
        {
          prevCol = col - 1;
          nextCol = col + 1;
          
          x1 = gradXMap[ prevRow ][ prevCol ];
          x2 = gradXMap[ prevRow ][ col ];
          x3 = gradXMap[ prevRow ][ nextCol ];
          x4 = gradXMap[ row ][ prevCol ];
          x5 = gradXMap[ row ][ nextCol ];
          x6 = gradXMap[ nextRow ][ prevCol ];
          x7 = gradXMap[ nextRow ][ col ];
          x8 = gradXMap[ nextRow ][ nextCol ];
          
          y1 = gradYMap[ prevRow ][ prevCol ];
          y2 = gradYMap[ prevRow ][ col ];
          y3 = gradYMap[ prevRow ][ nextCol ];
          y4 = gradYMap[ row ][ prevCol ];
          y5 = gradYMap[ row ][ nextCol ];
          y6 = gradYMap[ nextRow ][ prevCol ];
          y7 = gradYMap[ nextRow ][ col ];
          y8 = gradYMap[ nextRow ][ nextCol ];

          strengthXRight = std::max( 0.0, x1 + x4 + x6 );
          strengthXLeft = std::min( 0.0, x3 + x5 + x8 );
          strengthYUp = std::max( 0.0, y6 + y7 + y8 );
          strengthYDown = std::min( 0.0, y1 + y2 + y3 );
          
          strength = ( strengthXRight - strengthXLeft ) +
            ( strengthYUp - strengthYDown );
          
          edgeStrengthMap[ row ][ col ] = strength;
            
          if( minStrength > strength ) minStrength = strength;
        }
      }
      
      for( row = 2 ; row < lastRowIdx ; ++row )
      {
        for( col = 2 ; col < lastColIdx ; ++col )
        {
          edgeStrengthMap[ row ][ col ] -= minStrength;
        }
      }      
      
      return true;
    }
    
    bool Skeleton::getEdgeVecField( 
      const te::rp::Matrix< double >& edgeStrengthMap,
      te::rp::Matrix< double >& edgeVecXMap,
      te::rp::Matrix< double >& edgeVecYMap ) const
    {
      const unsigned int nRows = edgeStrengthMap.getLinesNumber();
      TERP_TRUE_OR_RETURN_FALSE( nRows > 6, "Invalid rows number" );
      
      const unsigned int nCols = edgeStrengthMap.getColumnsNumber();      
      TERP_TRUE_OR_RETURN_FALSE( nCols > 6, "Invalid columns number" );
      
      te::rp::Matrix< double > gradXMap;
      gradXMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      te::rp::Matrix< double > gradYMap;
      gradYMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( getGradientMaps( edgeStrengthMap, gradXMap, gradYMap ),
        "Gradient maps build error" );
//      createTifFromMatrix( gradXMap, true, "gradXMap" );
//      createTifFromMatrix( gradYMap, true, "gradYMap" );
      
      te::rp::Matrix< double > gradMagMap;
      gradMagMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( getMagnitude( gradXMap, gradYMap, gradMagMap ),
        "Magnitude calcule error" );        
//      createTifFromMatrix( gradMagMap, true, "gradMagMap" );

      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;        
      
      unsigned int row = 0;
      unsigned int col = 0;       
        
      if( ! edgeVecXMap.reset( nRows,nCols ) )
        return false;   
      if( ! edgeVecYMap.reset( nRows,nCols ) )
        return false;         
      
      for( row = 0 ; row < nRows ; ++row )
      {
        edgeVecXMap[ row ][ 0 ] = 0;
        edgeVecXMap[ row ][ lastColIdx ] = 0;
        
        edgeVecYMap[ row ][ 0 ] = 0;
        edgeVecYMap[ row ][ lastColIdx ] = 0;
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        edgeVecXMap[ 0 ][ col ] = 0;
        edgeVecXMap[ lastRowIdx ][ col ] = 0;
        
        edgeVecYMap[ 0 ][ col ] = 0;
        edgeVecYMap[ lastRowIdx ][ col ] = 0;
      } 
      
      unsigned int nextRow = 0;
      unsigned int nextCol = 0;
      unsigned int prevRow = 0;
      unsigned int prevCol = 0; 
      double centerMag = 0;
      double centerX = 0;
      double centerY = 0;
      double nGrad = 0;
      double bestNMag = 0;
      double bestNX = 0;
      double bestNY = 0;
      double newCenterX = 0;
      double newCenterY = 0;
      double newCenterMag = 0;
      double magsSum = 0;
      
      for( row = 1 ; row < lastRowIdx ; ++row )
      {
        prevRow = row - 1;
        nextRow = row + 1;
        
        for( col = 1 ; col < lastColIdx ; ++col )
        {
          prevCol = col - 1;
          nextCol = col + 1;  
          
          centerMag = gradMagMap[ row ][ col ];
          centerX = gradXMap[ row ][ col ];
          centerY = gradYMap[ row ][ col ];
          
          bestNMag = centerMag;
          bestNX = centerX;
          bestNY = centerY;
          
          nGrad = gradMagMap[ prevRow ][ prevCol ];
          if( bestNMag <= nGrad )
          {
            bestNMag = nGrad;
            bestNX =  gradXMap[ prevRow ][ prevCol ];
            bestNY =  gradYMap[ prevRow ][ prevCol ];
          }
          
          nGrad = gradMagMap[ prevRow ][ col ];
          if( bestNMag <= nGrad )
          {
            bestNMag = nGrad;
            bestNX =  gradXMap[ prevRow ][ col ];
            bestNY =  gradYMap[ prevRow ][ col ];
          }      
          
          nGrad = gradMagMap[ prevRow ][ nextCol ];
          if( bestNMag <= nGrad )
          {
            bestNMag = nGrad;
            bestNX =  gradXMap[ prevRow ][ nextCol ];
            bestNY =  gradYMap[ prevRow ][ nextCol ];
          }     
          
          nGrad = gradMagMap[ row ][ prevCol ];
          if( bestNMag <= nGrad )
          {
            bestNMag = nGrad;
            bestNX =  gradXMap[ row ][ prevCol ];
            bestNY =  gradYMap[ row ][ prevCol ];
          }
          
          nGrad = gradMagMap[ row ][ nextCol ];
          if( bestNMag <= nGrad )
          {
            bestNMag = nGrad;
            bestNX =  gradXMap[ row ][ nextCol ];
            bestNY =  gradYMap[ row ][ nextCol ];
          }           
          
          nGrad = gradMagMap[ nextRow ][ prevCol ];
          if( bestNMag <= nGrad )
          {
            bestNMag = nGrad;
            bestNX =  gradXMap[ nextRow ][ prevCol ];
            bestNY =  gradYMap[ nextRow ][ prevCol ];
          }
          
          nGrad = gradMagMap[ nextRow ][ col ];
          if( bestNMag <= nGrad )
          {
            bestNMag = nGrad;
            bestNX =  gradXMap[ nextRow ][ col ];
            bestNY =  gradYMap[ nextRow ][ col ];
          }      
          
          nGrad = gradMagMap[ nextRow ][ nextCol ];
          if( bestNMag <= nGrad )
          {
            bestNMag = nGrad;
            bestNX =  gradXMap[ nextRow ][ nextCol ];
            bestNY =  gradYMap[ nextRow ][ nextCol ];
          }   
          
          magsSum = ( bestNMag + centerMag );
          
          if( magsSum == 0.0 )
          {
            edgeVecXMap[ row ][ col ] = 0;
            edgeVecYMap[ row ][ col ] = 0;
          }
          else
          {
            newCenterX = ( ( bestNX * bestNMag ) + ( centerX * centerMag ) ) /
              magsSum;
            newCenterY = ( ( bestNY * bestNMag ) + ( centerY * centerMag ) ) /
              magsSum;
            newCenterMag = std::sqrt( ( newCenterX * newCenterX ) + ( newCenterY * newCenterY ) );
            
            if( newCenterMag == 0.0 )
            {
              edgeVecXMap[ row ][ col ] = 0;
              edgeVecYMap[ row ][ col ] = 0;
            }
            else
            {
              edgeVecXMap[ row ][ col ] = newCenterX / newCenterMag;
              edgeVecYMap[ row ][ col ] = newCenterY / newCenterMag;
            }
          }
        }
      }
      
      return true;
    }    

    void Skeleton::createTifFromVecField( 
      const te::rp::Matrix< double >& inputVecFieldX, 
      const te::rp::Matrix< double >& inputVecFieldY,
      const te::rp::Matrix< double >& backGroundMap,
      const std::string& tifFileName ) const 
    {
      assert( inputVecFieldX.getColumnsNumber() == inputVecFieldY.getColumnsNumber() );
      assert( inputVecFieldX.getLinesNumber() == inputVecFieldY.getLinesNumber() );      
      assert( inputVecFieldX.getColumnsNumber() == backGroundMap.getColumnsNumber() );
      assert( inputVecFieldX.getLinesNumber() == backGroundMap.getLinesNumber() );      
      
      std::map<std::string, std::string> rInfo;
      rInfo["URI"] = tifFileName + ".tif";
      
      std::vector<te::rst::BandProperty*> bandsProperties;
      bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
      bandsProperties[0]->m_colorInterp = te::rst::RedCInt;
      bandsProperties[0]->m_noDataValue = -1.0 * DBL_MAX;
      bandsProperties.push_back(new te::rst::BandProperty( *bandsProperties[0] ));
      bandsProperties[0]->m_colorInterp = te::rst::GreenCInt;
      bandsProperties.push_back(new te::rst::BandProperty( *bandsProperties[0] ));
      bandsProperties[0]->m_colorInterp = te::rst::BlueCInt;

      te::rst::Grid* newgrid = new te::rst::Grid( inputVecFieldX.getColumnsNumber(),
        inputVecFieldX.getLinesNumber(), 0, -1 );

      std::auto_ptr< te::rst::Raster > outputRasterPtr(
        te::rst::RasterFactory::make( "GDAL", newgrid, bandsProperties, rInfo, 0, 0));
      TERP_TRUE_OR_THROW( outputRasterPtr.get(), "Output raster creation error");
          
      unsigned int line = 0;
      unsigned int col = 0;
      const unsigned int nLines = inputVecFieldX.getLinesNumber();
      const unsigned int nCols = inputVecFieldX.getColumnsNumber();
      const unsigned int lastLineIdx = inputVecFieldX.getLinesNumber() - 1;
      const unsigned int lastColIdx = inputVecFieldX.getColumnsNumber() - 1;
      double x = 0.0;
      double y = 0.0;
      double minBGValue = DBL_MAX;
      double maxBGValue = -1.0 * DBL_MAX;
      double bGValue = 0;
      
      for( line = 0 ; line < nLines ; ++line )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          bGValue = backGroundMap[ line ][ col ];
          
          if( minBGValue > bGValue )
          {
            minBGValue = bGValue;
          }
          
          if( maxBGValue < bGValue )
          {
            maxBGValue = bGValue;
          }
        }
      }
      
      for( line = 0 ; line < nLines ; ++line )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          bGValue = backGroundMap[ line ][ col ];
          bGValue -= minBGValue;
          bGValue *= ( 254.0 / ( maxBGValue - minBGValue ) );
          
          outputRasterPtr->setValue( col, line, bGValue, 0 );
          outputRasterPtr->setValue( col, line, bGValue, 1 );
          outputRasterPtr->setValue( col, line, bGValue, 2 );
        }
      }      
      
      for( line = 1 ; line < lastLineIdx ; line += 5 )
      {
        for( col = 1 ; col < lastColIdx ; col += 5 )
        {
          x = inputVecFieldX[ line ][ col ];
          y = inputVecFieldY[ line ][ col ];
          outputRasterPtr->setValue( col, line, 255, 0 );
         
          if( x == 0 )
          {
            if( y > 0 )
            {
              outputRasterPtr->setValue( col, line - 1, 255, 0 );
              outputRasterPtr->setValue( col, line - 1, 255, 1 );
              outputRasterPtr->setValue( col, line - 1, 255, 2 );
            }
            else if ( y < 0 )
            {
              outputRasterPtr->setValue( col, line + 1, 255, 0 );
              outputRasterPtr->setValue( col, line + 1, 255, 1 );
              outputRasterPtr->setValue( col, line + 1, 255, 2 );
            }
          }          
          else if( x > 0 )
          {
            if( y == 0 )
            {
              outputRasterPtr->setValue( col + 1, line, 255, 0 );
              outputRasterPtr->setValue( col + 1, line, 255, 1 );
              outputRasterPtr->setValue( col + 1, line, 255, 2 );
            }
            else if( y > 0 )
            {
              outputRasterPtr->setValue( col + 1, line - 1, 255, 0 );
              outputRasterPtr->setValue( col + 1, line - 1, 255, 1 );
              outputRasterPtr->setValue( col + 1, line - 1, 255, 2 );
            }
            else
            {
              outputRasterPtr->setValue( col + 1, line + 1, 255, 0 );
              outputRasterPtr->setValue( col + 1, line + 1, 255, 1 );
              outputRasterPtr->setValue( col + 1, line + 1, 255, 2 );
            }
          }
          else // x < 0
          {
            if( y == 0 )
            {
              outputRasterPtr->setValue( col - 1, line, 255, 0 );
              outputRasterPtr->setValue( col - 1, line, 255, 1 );
              outputRasterPtr->setValue( col - 1, line, 255, 2 );
            }
            else if( y > 0 )
            {
              outputRasterPtr->setValue( col - 1, line - 1, 255, 0 );
              outputRasterPtr->setValue( col - 1, line - 1, 255, 1 );
              outputRasterPtr->setValue( col - 1, line - 1, 255, 2 );
            }
            else
            {
              outputRasterPtr->setValue( col - 1, line + 1, 255, 0 );
              outputRasterPtr->setValue( col - 1, line + 1, 255, 1 );
              outputRasterPtr->setValue( col - 1, line + 1, 255, 2 );
            }
          }          
        }
      }
    }    

    bool Skeleton::applyVecDiffusion( 
       const te::rp::Matrix< double >& inputX, 
       const te::rp::Matrix< double >& inputY,
       const te::rp::Matrix< double >& backgroundData, 
       te::rp::Matrix< double >& outputX, 
       te::rp::Matrix< double >& outputY ) const
    {
      assert( inputX.getColumnsNumber() == inputY.getColumnsNumber() );
      assert( inputX.getLinesNumber() == inputY.getLinesNumber() );      
      
      const unsigned int nRows = inputX.getLinesNumber();
      TERP_TRUE_OR_RETURN_FALSE( nRows > 8, "Invalid rows number" );
      
      const unsigned int nCols = inputX.getColumnsNumber();
      TERP_TRUE_OR_RETURN_FALSE( nCols > 8, "Invalid columns number" );
      
      const unsigned int rowSizeBytes = sizeof( double ) * nCols;

      if( ! outputX.reset( nRows,nCols ) )
        return false;       
      if( ! outputY.reset( nRows,nCols ) )
        return false;          
      
      std::auto_ptr< te::rp::Matrix< double > > xBuf1Ptr;
      xBuf1Ptr.reset( new te::rp::Matrix< double >() );
      if( ! xBuf1Ptr->reset( nRows,nCols ) )
        return false;
      
      std::auto_ptr< te::rp::Matrix< double > > yBuf1Ptr;
      yBuf1Ptr.reset( new te::rp::Matrix< double >() );
      if( ! yBuf1Ptr->reset( nRows,nCols ) )
        return false;  
      
      std::auto_ptr< te::rp::Matrix< double > > xBuf2Ptr;
      xBuf2Ptr.reset( new te::rp::Matrix< double >() );
      if( ! xBuf2Ptr->reset( nRows,nCols ) )
        return false;  
      
      std::auto_ptr< te::rp::Matrix< double > > yBuf2Ptr;      
      yBuf2Ptr.reset( new te::rp::Matrix< double >() );
      if( ! yBuf2Ptr->reset( nRows,nCols ) )
        return false;         
     
      double currentIterationXResidue = DBL_MAX;
      double currentIterationYResidue = DBL_MAX; 
      
      boost::mutex mutex;
      
      ApplyVecDiffusionThreadParams threadParams;
      threadParams.m_initialXBufPtr = &inputX;
      threadParams.m_initialYBufPtr = &inputY;
      threadParams.m_currentIterationXResiduePtr = &currentIterationXResidue;
      threadParams.m_currentIterationYResiduePtr = &currentIterationYResidue;
      threadParams.m_mutexPtr = &mutex;
      threadParams.m_diffusionRegularitation = m_inputParameters.m_diffusionRegularitation;
      
      const unsigned int threadsNumber = m_inputParameters.m_enableMultiThread ?
        te::common::GetPhysProcNumber() : 0;
      const unsigned int rowsPerThread = m_inputParameters.m_enableMultiThread ?
        std::ceil( ((double)(nRows - 8 )) / ((double)threadsNumber) ) : nRows;

      unsigned int iteration = 0;
            
      while( ( currentIterationXResidue > m_inputParameters.m_diffusionThreshold ) ||
        ( currentIterationYResidue > m_inputParameters.m_diffusionThreshold ) )
      {
        currentIterationXResidue = 0;
        currentIterationYResidue = 0;
        
        if( iteration == 0 )
        {
          threadParams.m_inputBufXPtr = &inputX;
          threadParams.m_inputBufYPtr = &inputY;
          threadParams.m_outputBufXPtr = xBuf1Ptr.get();
          threadParams.m_outputBufYPtr = yBuf1Ptr.get();
        }
        else
        {
          if( threadParams.m_outputBufXPtr == xBuf1Ptr.get() )
          {
            threadParams.m_inputBufXPtr = xBuf1Ptr.get();
            threadParams.m_inputBufYPtr = yBuf1Ptr.get();
            threadParams.m_outputBufXPtr = xBuf2Ptr.get();
            threadParams.m_outputBufYPtr = yBuf2Ptr.get();
          }
          else
          {
            threadParams.m_inputBufXPtr = xBuf2Ptr.get();
            threadParams.m_inputBufYPtr = yBuf2Ptr.get();
            threadParams.m_outputBufXPtr = xBuf1Ptr.get();
            threadParams.m_outputBufYPtr = yBuf1Ptr.get();
          }
        }
        
        memcpy( threadParams.m_outputBufXPtr->operator[]( 0 ), 
          threadParams.m_inputBufXPtr->operator[]( 0 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufXPtr->operator[]( 1 ), 
          threadParams.m_inputBufXPtr->operator[]( 1 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufXPtr->operator[]( 2 ), 
          threadParams.m_inputBufXPtr->operator[]( 2 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufXPtr->operator[]( 3 ), 
          threadParams.m_inputBufXPtr->operator[]( 3 ), rowSizeBytes );
          
        memcpy( threadParams.m_outputBufXPtr->operator[]( nRows - 4 ), 
          threadParams.m_inputBufXPtr->operator[]( nRows - 4 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufXPtr->operator[]( nRows - 3 ), 
          threadParams.m_inputBufXPtr->operator[]( nRows - 3 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufXPtr->operator[]( nRows - 2 ), 
          threadParams.m_inputBufXPtr->operator[]( nRows - 2 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufXPtr->operator[]( nRows - 1 ), 
          threadParams.m_inputBufXPtr->operator[]( nRows - 1 ), rowSizeBytes );
          
        memcpy( threadParams.m_outputBufYPtr->operator[]( 0 ), 
          threadParams.m_inputBufYPtr->operator[]( 0 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufYPtr->operator[]( 1 ), 
          threadParams.m_inputBufYPtr->operator[]( 1 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufYPtr->operator[]( 2 ), 
          threadParams.m_inputBufYPtr->operator[]( 2 ), rowSizeBytes );
        memcpy( threadParams.m_outputBufYPtr->operator[]( 3 ), 
          threadParams.m_inputBufYPtr->operator[]( 3 ), rowSizeBytes );
          
        memcpy( threadParams.m_outputBufYPtr->operator[]( nRows - 4 ), 
          threadParams.m_inputBufYPtr->operator[]( nRows - 4 ), rowSizeBytes );          
        memcpy( threadParams.m_outputBufYPtr->operator[]( nRows - 3 ), 
          threadParams.m_inputBufYPtr->operator[]( nRows - 3 ), rowSizeBytes );          
        memcpy( threadParams.m_outputBufYPtr->operator[]( nRows - 2 ), 
          threadParams.m_inputBufYPtr->operator[]( nRows - 2 ), rowSizeBytes );          
        memcpy( threadParams.m_outputBufYPtr->operator[]( nRows - 1 ), 
          threadParams.m_inputBufYPtr->operator[]( nRows - 1 ), rowSizeBytes );          
        
        if( threadsNumber )
        {
          unsigned int startingRow = 4;
          std::vector< ApplyVecDiffusionThreadParams > threadsParamesVec;
          threadsParamesVec.resize( threadsNumber, threadParams );
          boost::thread_group threads;
          
          for( unsigned int threadIdx = 0 ; threadIdx < threadsNumber ;
            ++threadIdx )
          {
            threadsParamesVec[ threadIdx ].m_firstRowIdx = startingRow;
            threadsParamesVec[ threadIdx ].m_lastRowIdx = std::min( nRows - 5,
              startingRow + rowsPerThread - 1 );
            threads.add_thread( new boost::thread( applyVecDiffusionThreadEntry, 
              &threadsParamesVec[ threadIdx ] ) );
            startingRow += rowsPerThread;
          }
          
          threads.join_all();
        }
        else
        {
          threadParams.m_firstRowIdx = 4;
          threadParams.m_lastRowIdx = nRows - 5;
          applyVecDiffusionThreadEntry( &threadParams );
        };
        
        if( ( iteration % 50 ) == 0 )
        {
          createTifFromMatrix( *threadParams.m_outputBufXPtr, true,  
            boost::lexical_cast< std::string >( iteration ) + "_diffusedX" );        
          createTifFromMatrix( *threadParams.m_outputBufYPtr, true,  
            boost::lexical_cast< std::string >( iteration ) + "_diffusedY");
          createTifFromVecField( *threadParams.m_outputBufXPtr, *threadParams.m_outputBufYPtr, backgroundData,  
            boost::lexical_cast< std::string >( iteration ) + "_diffusedVecs");
        }

        ++iteration;
      }
      
      for( unsigned int row = 0 ; row < nRows ; ++row )
      {
        memcpy( outputX[ row ], threadParams.m_outputBufXPtr->operator[]( row ),
          rowSizeBytes );
        memcpy( outputY[ row ], threadParams.m_outputBufYPtr->operator[]( row ),
          rowSizeBytes );          
      }
      
      return true;
    }

    void Skeleton::applyVecDiffusionThreadEntry( ApplyVecDiffusionThreadParams* paramsPtr)
    {
      const te::rp::Matrix< double >& initBufX = *(paramsPtr->m_initialXBufPtr);
      const te::rp::Matrix< double >& initBufY = *(paramsPtr->m_initialYBufPtr);
      const te::rp::Matrix< double >& iBufX = *(paramsPtr->m_inputBufXPtr);
      const te::rp::Matrix< double >& iBufY = *(paramsPtr->m_inputBufYPtr);
      te::rp::Matrix< double >& oBufX = *(paramsPtr->m_outputBufXPtr);
      te::rp::Matrix< double >& oBufY = *(paramsPtr->m_outputBufYPtr);
      const double& diffusionRegularization = paramsPtr->m_diffusionRegularitation;
      
      const unsigned int nCols = iBufX.getColumnsNumber();
      const unsigned int rowSizeBytes = sizeof( double ) * nCols;
      unsigned int row = 0;
      unsigned int col = 0;      
      
      boost::scoped_array< double > bufXRow0Handler( new double[ nCols ] );
      boost::scoped_array< double > bufXRow1Handler( new double[ nCols ] );
      boost::scoped_array< double > bufXRow2Handler( new double[ nCols ] );
      boost::scoped_array< double > bufYRow0Handler( new double[ nCols ] );
      boost::scoped_array< double > bufYRow1Handler( new double[ nCols ] );
      boost::scoped_array< double > bufYRow2Handler( new double[ nCols ] );
      boost::scoped_array< double > bufInitXRow0Handler( new double[ nCols ] );
      boost::scoped_array< double > bufInitXRow1Handler( new double[ nCols ] );
      boost::scoped_array< double > bufInitXRow2Handler( new double[ nCols ] );
      boost::scoped_array< double > bufInitYRow0Handler( new double[ nCols ] );
      boost::scoped_array< double > bufInitYRow1Handler( new double[ nCols ] );
      boost::scoped_array< double > bufInitYRow2Handler( new double[ nCols ] );
      double* bufferInitX[ 3 ];
      double* bufferInitY[ 3 ];
      double* bufferX[ 3 ];
      double* bufferY[ 3 ];
      bufferInitX[ 0 ] = bufInitXRow0Handler.get();
      bufferInitX[ 1 ] = bufInitXRow1Handler.get();
      bufferInitX[ 2 ] = bufInitXRow2Handler.get();
      bufferInitY[ 0 ] = bufInitYRow0Handler.get();
      bufferInitY[ 1 ] = bufInitYRow1Handler.get();
      bufferInitY[ 2 ] = bufInitYRow2Handler.get();      
      bufferX[ 0 ] = bufXRow0Handler.get();
      bufferX[ 1 ] = bufXRow1Handler.get();
      bufferX[ 2 ] = bufXRow2Handler.get();
      bufferY[ 0 ] = bufYRow0Handler.get();
      bufferY[ 1 ] = bufYRow1Handler.get();
      bufferY[ 2 ] = bufYRow2Handler.get();
      
      // Loading the two initial rows
        
      paramsPtr->m_mutexPtr->lock();
      memcpy( bufferInitX[ 1 ], initBufX[ paramsPtr->m_firstRowIdx - 1 ], 
        rowSizeBytes );
      memcpy( bufferInitX[ 2 ], initBufX[ paramsPtr->m_firstRowIdx ], 
        rowSizeBytes );
      memcpy( bufferInitY[ 1 ], initBufY[ paramsPtr->m_firstRowIdx - 1 ], 
        rowSizeBytes );
      memcpy( bufferInitY[ 2 ], initBufY[ paramsPtr->m_firstRowIdx ], 
        rowSizeBytes );      
      memcpy( bufferX[ 1 ], iBufX[ paramsPtr->m_firstRowIdx - 1 ], 
        rowSizeBytes );
      memcpy( bufferX[ 2 ], iBufX[ paramsPtr->m_firstRowIdx ], 
        rowSizeBytes );
      memcpy( bufferY[ 1 ], iBufY[ paramsPtr->m_firstRowIdx - 1 ], 
        rowSizeBytes );
      memcpy( bufferY[ 2 ], iBufY[ paramsPtr->m_firstRowIdx ], 
        rowSizeBytes );
      paramsPtr->m_mutexPtr->unlock();
      
      const unsigned int rowsBound = paramsPtr->m_lastRowIdx + 2;
      const unsigned int colsBound = nCols - 4;
      double centerX = 0;
      double centerY = 0;
      double centerInitX = 0;
      double centerInitY = 0;      
      double newCenterX = 0;
      double newCenterY = 0;
      double neiX = 0;
      double neiY = 0;
      double neiMag = 0;
   
      double* rowPtr = 0;
      unsigned int prevRow = 0;
      unsigned int prevCol = 0;
      unsigned int nextCol = 0;
      double currentIterationXResidue = 0;
      double currentIterationYResidue = 0;
      
      boost::scoped_array< double > outRowXBuffHandler( new double[ nCols ] );
      double* outRowXBuff = outRowXBuffHandler.get();
      
      boost::scoped_array< double > outRowYBuffHandler( new double[ nCols ] );
      double* outRowYBuff = outRowXBuffHandler.get();
      
      for( row = ( paramsPtr->m_firstRowIdx + 1 ) ; row < rowsBound ; ++row )
      {
        prevRow = row - 1;
        
        // Rolling-up the internal buffers
        
        rowPtr = bufferInitX[ 0 ];
        bufferInitX[ 0 ] = bufferInitX[ 1 ];
        bufferInitX[ 1 ] = bufferInitX[ 2 ];
        bufferInitX[ 2 ] = rowPtr;
        rowPtr = bufferInitY[ 0 ];
        bufferInitY[ 0 ] = bufferInitY[ 1 ];
        bufferInitY[ 1 ] = bufferInitY[ 2 ];
        bufferInitY[ 2 ] = rowPtr;         
        
        rowPtr = bufferX[ 0 ];
        bufferX[ 0 ] = bufferX[ 1 ];
        bufferX[ 1 ] = bufferX[ 2 ];
        bufferX[ 2 ] = rowPtr;
        rowPtr = bufferY[ 0 ];
        bufferY[ 0 ] = bufferY[ 1 ];
        bufferY[ 1 ] = bufferY[ 2 ];
        bufferY[ 2 ] = rowPtr;       
        
        // Getting a new line
        
        paramsPtr->m_mutexPtr->lock();
        memcpy( bufferInitX[ 2 ], initBufX[ row ], rowSizeBytes );
        memcpy( bufferInitY[ 2 ], initBufY[ row ], rowSizeBytes );        
        memcpy( bufferX[ 2 ], iBufX[ row ], rowSizeBytes );
        memcpy( bufferY[ 2 ], iBufY[ row ], rowSizeBytes );        
        paramsPtr->m_mutexPtr->unlock();        
        
        // Diffusing the buffer center line
        
        for( col = 4 ; col < colsBound ; ++col )
        {
          prevCol = col - 1;
          nextCol = col + 1;
          
          centerInitX = bufferInitX[ 1 ][ col ];
          centerInitY = bufferInitY[ 1 ][ col ];
          centerX = bufferX[ 1 ][ col ];
          centerY = bufferY[ 1 ][ col ];
          
          newCenterX = 0;
          newCenterY = 0;
          
          ADDNEIGHBOR( 0, prevCol )
          ADDNEIGHBOR( 0, col )
          ADDNEIGHBOR( 0, nextCol )
          ADDNEIGHBOR( 1, prevCol )
          ADDNEIGHBOR( 1, nextCol )
          ADDNEIGHBOR( 2, prevCol )
          ADDNEIGHBOR( 2, col )
          ADDNEIGHBOR( 2, nextCol )          
          
          newCenterX -= ( 8.0 * centerX );
          newCenterY -= ( 8.0 * centerY );
          
          newCenterX *= diffusionRegularization;
          newCenterY *= diffusionRegularization;
          
          newCenterX -= ( ( centerX - centerInitX ) * 
            ( ( centerInitX * centerInitX ) + ( centerInitY * centerInitY ) ) );
          newCenterY -= ( ( centerY - centerInitY ) * 
            ( ( centerInitX * centerInitX ) + ( centerInitY * centerInitY ) ) );
            
          newCenterX += centerX;
          newCenterY += centerY;
            
          outRowXBuff[ col ] = newCenterX;
          outRowYBuff[ col ] = newCenterY;
            
          if( centerX == 0.0 )
          {
            if( newCenterX != 0.0 )
              currentIterationXResidue = std::max( currentIterationXResidue,
                1.0 );
          }
          else
          {
            currentIterationXResidue = std::max( currentIterationXResidue,
              std::abs( ( centerX - newCenterX ) / centerX ) );
          }
          
          if( centerY == 0.0 )
          {
            if( newCenterY != 0.0 )
              currentIterationYResidue = std::max( currentIterationYResidue,
                1.0 );
          }
          else
          {
            currentIterationYResidue = std::max( currentIterationYResidue,
              std::abs( ( centerY - newCenterY ) / centerY ) );              
          }
        }
        
        // copying the result to output 
        
        paramsPtr->m_mutexPtr->lock();
        
        memcpy( oBufX[ prevRow ], outRowXBuff, rowSizeBytes );
        memcpy( oBufY[ prevRow ], outRowYBuff, rowSizeBytes );
        
        oBufX[ prevRow ][ 0 ] = bufferX[ 1 ][ 0 ];
        oBufX[ prevRow ][ 1 ] = bufferX[ 1 ][ 1 ];
        oBufX[ prevRow ][ 2 ] = bufferX[ 1 ][ 2 ];
        oBufX[ prevRow ][ 3 ] = bufferX[ 1 ][ 3 ];
        oBufX[ prevRow ][ colsBound ] = bufferX[ 1 ][ colsBound ];
        oBufX[ prevRow ][ colsBound + 1 ] = bufferX[ 1 ][ colsBound + 1 ];
        oBufX[ prevRow ][ colsBound + 2 ] = bufferX[ 1 ][ colsBound + 2 ];
        oBufX[ prevRow ][ colsBound + 3 ] = bufferX[ 1 ][ colsBound + 3 ];
        
        oBufY[ prevRow ][ 0 ] = bufferY[ 1 ][ 0 ];
        oBufY[ prevRow ][ 1 ] = bufferY[ 1 ][ 1 ];
        oBufY[ prevRow ][ 2 ] = bufferY[ 1 ][ 2 ];
        oBufY[ prevRow ][ 3 ] = bufferY[ 1 ][ 3 ];
        oBufY[ prevRow ][ colsBound ] = bufferY[ 1 ][ colsBound ];
        oBufY[ prevRow ][ colsBound + 1 ] = bufferY[ 1 ][ colsBound + 1 ];
        oBufY[ prevRow ][ colsBound + 2 ] = bufferY[ 1 ][ colsBound + 2 ];
        oBufY[ prevRow ][ colsBound + 3 ] = bufferY[ 1 ][ colsBound + 3 ];
        
        (*paramsPtr->m_currentIterationXResiduePtr) = 
          std::max( (*paramsPtr->m_currentIterationXResiduePtr), 
          currentIterationXResidue );
          
        (*paramsPtr->m_currentIterationYResiduePtr) = 
          std::max( (*paramsPtr->m_currentIterationYResiduePtr), 
          currentIterationYResidue );          
        
        paramsPtr->m_mutexPtr->unlock();         
      }      
      
      
    }

  } // end namespace rp


}   // end namespace te

