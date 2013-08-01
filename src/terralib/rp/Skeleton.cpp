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

#include <boost/lexical_cast.hpp>

#include <memory>
#include <cmath>  

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
      m_finiteDifferencesThreshold = 0.1;
    }

    const Skeleton::InputParameters& Skeleton::InputParameters::operator=(
      const Skeleton::InputParameters& params )
    {
      reset();

      m_inputRasterPtr = params.m_inputRasterPtr;
      m_inputRasterBand = params.m_inputRasterBand;
      m_inputMaskRasterPtr = params.m_inputMaskRasterPtr;
      m_finiteDifferencesThreshold = params.m_finiteDifferencesThreshold;

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
      
      te::rp::Matrix< double > gradXMap;
      gradXMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      te::rp::Matrix< double > gradYMap;
      gradYMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( getGradientMaps( *rasterDataPtr, gradXMap, gradYMap ),
        "Gradient maps build error" );      
//      createTifFromMatrix( gradXMap, true, "gradXMap" );
//      createTifFromMatrix( gradYMap, true, "gradYMap" );
        
      te::rp::Matrix< double > edgeStrengthMap;
      edgeStrengthMap.reset( te::rp::Matrix< double >::AutoMemPol );    
      TERP_TRUE_OR_RETURN_FALSE( getEdgeStrengthMap( gradXMap, gradYMap, 
        edgeStrengthMap ), "Edge strength map build error" ); 
      createTifFromMatrix( edgeStrengthMap, true, "edgeStrengthMap" );
   
      te::rp::Matrix< double > vecXMap;
      vecXMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      te::rp::Matrix< double > vecYMap;
      vecYMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( getGradientMaps( edgeStrengthMap, vecXMap, vecYMap ),
        "Vector maps build error" );      
      createTifFromMatrix( vecXMap, true, "vecXMap" );
      createTifFromMatrix( vecYMap, true, "vecYMap" );
      createTifFromVecField( vecXMap, vecYMap, "vecMap" );
      
      te::rp::Matrix< double > diffusedVecXMap;
      diffusedVecXMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      te::rp::Matrix< double > diffusedVecYMap;
      diffusedVecYMap.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( applyVecDiffusion( vecXMap, vecYMap, diffusedVecXMap,
        diffusedVecYMap ),
        "Vector maps build error" );      
      createTifFromMatrix( diffusedVecXMap, true, "diffusedVecXMap" );
      createTifFromMatrix( diffusedVecYMap, true, "diffusedVecYMap" );
      createTifFromVecField( diffusedVecXMap, diffusedVecYMap, "diffusedVecMap" );      
      
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
        ( m_inputParameters.m_finiteDifferencesThreshold >= 0.0 ) &&
        ( m_inputParameters.m_finiteDifferencesThreshold <= 1.0 ),
        "Invalid edge treshold" );           

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

    bool Skeleton::getGradientMaps( const te::rp::Matrix< double >& rasterData,
      te::rp::Matrix< double >& gradXMap,
      te::rp::Matrix< double >& gradYMap ) const
    {
      const unsigned int nRows = rasterData.getLinesNumber();
      const unsigned int nCols = rasterData.getColumnsNumber();
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
      double maskValue = 0;
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
          
          value1 = rasterData[ prevRow ][ prevCol ];
          value2 = rasterData[ prevRow ][ col ];
          value3 = rasterData[ prevRow ][ nextCol ];
          value4 = rasterData[ row ][ prevCol ];
          value5 = rasterData[ row ][ nextCol ];
          value6 = rasterData[ nextRow ][ prevCol ];
          value7 = rasterData[ nextRow ][ col ];
          value8 = rasterData[ nextRow  ][ nextCol ];
          
          if( 
              ( rasterData[ row ][ col ] == DBL_MAX ) ||
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
      const te::rp::Matrix< double >& gradXMap,
      const te::rp::Matrix< double >& gradYMap,
      te::rp::Matrix< double >& edgeStrengthMap ) const
    {
      assert( gradXMap.getColumnsNumber() == gradYMap.getColumnsNumber() );
      assert( gradXMap.getLinesNumber() == gradYMap.getLinesNumber() );
      
      const unsigned int nRows = gradXMap.getLinesNumber();
      const unsigned int nCols = gradXMap.getColumnsNumber();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      
      if( ! edgeStrengthMap.reset( nRows,nCols ) )
        return false;      
      
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
      double strengthX = 0;
      double strengthY = 0;
      double strengthMag = 0;
            
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
          
          edgeStrengthMap[ row ][ col ] = ( strengthXRight - strengthXLeft ) +
            ( strengthYUp - strengthYDown );
        }
      }
      
      return true;
    }
    
    bool Skeleton::getEdgeBinaryMap( 
      const te::rp::Matrix< double >& gradXMap,
      const te::rp::Matrix< double >& gradYMap,
      const te::rp::Matrix< double >& edgeStrengthMap,
      te::rp::Matrix< unsigned char >& edgeBinMap ) const
    {
      assert( gradXMap.getColumnsNumber() == gradYMap.getColumnsNumber() );
      assert( gradXMap.getLinesNumber() == gradYMap.getLinesNumber() );
      assert( gradXMap.getColumnsNumber() == edgeStrengthMap.getColumnsNumber() );
      assert( gradXMap.getLinesNumber() == edgeStrengthMap.getLinesNumber() );      
      
      const unsigned int nRows = gradXMap.getLinesNumber();
      const unsigned int nCols = gradXMap.getColumnsNumber();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      
      if( ! edgeBinMap.reset( nRows,nCols ) )
        return false;      
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        edgeBinMap[ row ][ 0 ] = 0;
        edgeBinMap[ row ][ lastColIdx ] = 0;
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        edgeBinMap[ 0 ][ col ] = 0;
        edgeBinMap[ lastRowIdx ][ col ] = 0;
      }       
      
      unsigned int nextRow = 0;
      unsigned int nextCol = 0;
      unsigned int prevRow = 0;
      unsigned int prevCol = 0;      
      double centralStrenght = 0;
            
      for( row = 1 ; row < lastRowIdx ; ++row )
      {
        prevRow = row - 1;
        nextRow = row + 1;
        
        for( col = 1 ; col < lastColIdx ; ++col )
        {
          prevCol = col - 1;
          nextCol = col + 1;
          
          centralStrenght = edgeStrengthMap[ row ][ col ];
          
          if(
              ( centralStrenght >= edgeStrengthMap[ prevRow ][ prevCol ] )
              &&
              ( centralStrenght >= edgeStrengthMap[ prevRow ][ col ] )
              &&
              ( centralStrenght >= edgeStrengthMap[ prevRow ][ nextCol ] )
              &&
              ( centralStrenght >= edgeStrengthMap[ row ][ prevCol ] )
              &&
              ( centralStrenght >= edgeStrengthMap[ row ][ nextCol ] )
              &&
              ( centralStrenght >= edgeStrengthMap[ nextRow ][ prevCol ] )
              &&
              ( centralStrenght >= edgeStrengthMap[ nextRow ][ col ] )
              &&
              ( centralStrenght >= edgeStrengthMap[ nextRow ][ nextCol ] )
            )
          {
            edgeBinMap[ row ][ col ] = 1;
          }          
          else
          {
            edgeBinMap[ row ][ col ] = 0;
          }
        }
      }
      
      return true;
    }    

    void Skeleton::createTifFromVecField( 
      const te::rp::Matrix< double >& inputVecFieldX, 
      const te::rp::Matrix< double >& inputVecFieldY,
      const std::string& tifFileName ) const 
    {
      assert( inputVecFieldX.getColumnsNumber() == inputVecFieldY.getColumnsNumber() );
      assert( inputVecFieldX.getLinesNumber() == inputVecFieldY.getLinesNumber() );      
      
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
      
      for( line = 0 ; line < nLines ; ++line )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          outputRasterPtr->setValue( col, line, 0, 0 );
          outputRasterPtr->setValue( col, line, 0, 1 );
          outputRasterPtr->setValue( col, line, 0, 2 );
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
       te::rp::Matrix< double >& outputX, 
       te::rp::Matrix< double >& outputY ) const
    {
      assert( inputX.getColumnsNumber() == inputY.getColumnsNumber() );
      assert( inputX.getLinesNumber() == inputY.getLinesNumber() );      
      
      const unsigned int nRows = inputX.getLinesNumber();
      const unsigned int nCols = inputX.getColumnsNumber();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;   
      unsigned int row = 0;
      unsigned int col = 0;      
      
      if( ! outputX.reset( nRows,nCols ) )
        return false;       
      if( ! outputY.reset( nRows,nCols ) )
        return false;          
      
      std::auto_ptr< te::rp::Matrix< double > > xBuf1Ptr;
      std::auto_ptr< te::rp::Matrix< double > > yBuf1Ptr;
      {
        xBuf1Ptr.reset( new te::rp::Matrix< double >() );
        if( ! xBuf1Ptr->reset( nRows,nCols ) )
          return false;        
        
        yBuf1Ptr.reset( new te::rp::Matrix< double >() );
        if( ! yBuf1Ptr->reset( nRows,nCols ) )
          return false;  
        
        for( row = 0 ; row < nRows ; ++row )
        {
          (*xBuf1Ptr)[ row ][ 0 ] = 0;
          (*xBuf1Ptr)[ row ][ lastColIdx ] = 0;
          (*yBuf1Ptr)[ row ][ 0 ] = 0;
          (*yBuf1Ptr)[ row ][ lastColIdx ] = 0;          
        }
        
        for( col = 0 ; col < nCols ; ++col )
        {
          (*xBuf1Ptr)[ 0 ][ col ] = 0;
          (*xBuf1Ptr)[ lastRowIdx ][ col ] = 0;
          (*yBuf1Ptr)[ 0 ][ col ] = 0;
          (*yBuf1Ptr)[ lastRowIdx ][ col ] = 0;          
        }             
      }
      
      std::auto_ptr< te::rp::Matrix< double > > xBuf2Ptr;
      std::auto_ptr< te::rp::Matrix< double > > yBuf2Ptr;
      {
        xBuf2Ptr.reset( new te::rp::Matrix< double >() );
        if( ! xBuf2Ptr->reset( nRows,nCols ) )
          return false;        
        
        yBuf2Ptr.reset( new te::rp::Matrix< double >() );
        if( ! yBuf2Ptr->reset( nRows,nCols ) )
          return false; 
        
        for( row = 0 ; row < nRows ; ++row )
        {
          (*xBuf2Ptr)[ row ][ 0 ] = 0;
          (*xBuf2Ptr)[ row ][ lastColIdx ] = 0;
          (*yBuf2Ptr)[ row ][ 0 ] = 0;
          (*yBuf2Ptr)[ row ][ lastColIdx ] = 0;          
        }
        
        for( col = 0 ; col < nCols ; ++col )
        {
          (*xBuf2Ptr)[ 0 ][ col ] = 0;
          (*xBuf2Ptr)[ lastRowIdx ][ col ] = 0;
          (*yBuf2Ptr)[ 0 ][ col ] = 0;
          (*yBuf2Ptr)[ lastRowIdx ][ col ] = 0;          
        }         
      }
      
      te::rp::Matrix< double > const * iBufXPtr = 0;
      te::rp::Matrix< double > const * iBufYPtr = 0;
      te::rp::Matrix< double > * oBufXPtr = 0;
      te::rp::Matrix< double > * oBufYPtr = 0;
      
      te::rp::Matrix< double > inputMag;
      inputMag.reset( te::rp::Matrix< double >::AutoMemPol );  
      if( ! inputMag.reset( nRows,nCols ) )
        return false;       
      for( row = 0 ; row < nRows ; ++row )
      {
        inputMag[ row ][ 0 ] = 0;
        inputMag[ row ][ lastColIdx ] = 0;
      }
      for( col = 0 ; col < nCols ; ++col )
      {
        inputMag[ 0 ][ col ] = 0;
        inputMag[ lastRowIdx ][ col ] = 0;
      }    
      
      double maxResidueX = 0.0;
      double maxResidueY = 0.0;
      double currentIterationXResidue = DBL_MAX;
      double currentIterationYResidue = DBL_MAX;
      unsigned int iteration = 0;
            
      while( ( currentIterationXResidue > maxResidueX ) ||
        ( currentIterationYResidue > maxResidueY ) )
      {
        if( iteration == 0 )
        {
          iBufXPtr = &inputX;
          iBufYPtr = &inputY;
          oBufXPtr = xBuf1Ptr.get();
          oBufYPtr = yBuf1Ptr.get();
        }
        else
        {
          if( oBufXPtr == xBuf1Ptr.get() )
          {
            iBufXPtr = xBuf1Ptr.get();
            iBufYPtr = yBuf1Ptr.get();
            oBufXPtr = xBuf2Ptr.get();
            oBufYPtr = yBuf2Ptr.get();
          }
          else
          {
            iBufXPtr = xBuf2Ptr.get();
            iBufYPtr = yBuf2Ptr.get();
            oBufXPtr = xBuf1Ptr.get();
            oBufYPtr = yBuf1Ptr.get();
          }
        }
        
        const te::rp::Matrix< double >& iBufX = *iBufXPtr;
        const te::rp::Matrix< double >& iBufY = *iBufYPtr;
        te::rp::Matrix< double >& oBufX = *oBufXPtr;
        te::rp::Matrix< double >& oBufY = *oBufYPtr;        
        
        if( ! getMagnitude( iBufX, iBufY, inputMag ) ) return false;
        
        unsigned int nextRow = 0;
        unsigned int nextCol = 0;
        unsigned int prevRow = 0;
        unsigned int prevCol = 0;  
        double centerX = 0;
        double centerY = 0;
        double centerMag = 0;
        double newCenterX = 0;
        double newCenterY = 0;
        double newCenterMag = 0;
        double neiX = 0;
        double neiY = 0;
        double nMag = 0;
        double maxCenterX = (-1.0) * DBL_MIN;
        double maxCenterY = (-1.0) * DBL_MIN;
        double minCenterX = DBL_MIN;
        double minCenterY = DBL_MIN;
        currentIterationXResidue = 0.0;
        currentIterationYResidue = 0.0;
        
        for( row = 1 ; row < lastRowIdx ; ++row )
        {
          prevRow = row - 1;
          nextRow = row + 1;
          
          for( col = 1 ; col < lastColIdx ; ++col )
          {
            prevCol = col - 1;
            nextCol = col + 1;        
            
            centerMag = nMag = inputMag[ row ][ col ];
            centerX = neiX = iBufX[ row ][ col ];
            centerY = neiY = iBufY[ row ][ col ];
            
            maxCenterX = std::max( maxCenterX, centerX );
            maxCenterY = std::max( maxCenterY, centerY );
            minCenterX = std::min( minCenterX, centerX );
            minCenterY = std::min( minCenterY, centerY );
            
            if( nMag < inputMag[ lastRowIdx ][ col ] )
            {
              neiX = iBufX[ lastRowIdx ][ col ];
              neiY = iBufY[ lastRowIdx ][ col ];
              nMag = inputMag[ lastRowIdx ][ col ];
            }
            
            if( nMag < inputMag[ row ][ lastColIdx ] )
            {
              neiX = iBufX[ row ][ lastColIdx ];
              neiY = iBufY[ row ][ lastColIdx ];
              nMag = inputMag[ row ][ lastColIdx ];
            }         
            
            if( nMag < inputMag[ row ][ nextCol ] )
            {
              neiX = iBufX[ row ][ nextCol ];
              neiY = iBufY[ row ][ nextCol ];
              nMag = inputMag[ row ][ nextCol ];
            }       
            
            if( nMag < inputMag[ nextRow ][ col ] )
            {
              neiX = iBufX[ nextRow ][ col ];
              neiY = iBufY[ nextRow ][ col ];
              nMag = inputMag[ nextRow ][ col ];
            }
            
            newCenterX = ( centerX + neiX ) / 2.0;
            newCenterY = ( centerY + neiY ) / 2.0;
            newCenterMag = std::sqrt( ( newCenterX * newCenterX ) +
              ( newCenterY * newCenterY ) );
            newCenterX *= ( centerMag / newCenterMag );
            newCenterY *= ( centerMag / newCenterMag );
            
            oBufX[ row ][ col ] = newCenterX;
            oBufY[ row ][ col ] = newCenterY;
            
            currentIterationXResidue = std::max( currentIterationXResidue,
              std::abs( centerX - newCenterX ) );
            currentIterationYResidue = std::max( currentIterationYResidue,
              std::abs( centerY - newCenterY ) );              
          }
        }
        
        maxResidueX = ( maxCenterX - minCenterX ) * m_inputParameters.m_finiteDifferencesThreshold;
        maxResidueY = ( maxCenterY - minCenterY ) * m_inputParameters.m_finiteDifferencesThreshold;
        
/*        createTifFromMatrix( oBufX, true, "diffusedX_" + 
          boost::lexical_cast< std::string >( iteration ) );
        createTifFromMatrix( oBufY, true, "diffusedY_" + 
          boost::lexical_cast< std::string >( iteration ) ); */         

        ++iteration;
      }
      
      return true;
    }

    

  } // end namespace rp


}   // end namespace te

