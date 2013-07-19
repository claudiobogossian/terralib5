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
#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
#include "../raster/Utils.h"

#include <climits>
#include <cfloat>

#define UPDATEDOUTDIFVECTOR( nighborCol, nighborRow ) \
  { \
    neighborX = iX[ nighborRow ][ nighborCol ]; \
    neighborY = iY[ nighborRow ][ nighborCol ]; \
    if( ( ( outputVecX == 0.0 ) && ( outputVecY == 0.0 ) ) || ( ( neighborX == 0.0 ) && ( neighborY == 0.0 ) ) ) \
    { \
      decreasingFunctionValue = 1.0; \
    } \
    else \
    { \
      vectorsAngleCos =  \
        ( ( outputVecX * neighborX ) + ( outputVecY * neighborY ) ) \
        / \
        ( \
          std::sqrt( ( outputVecX * outputVecX ) + ( outputVecY * outputVecY )  ) \
          * \
          std::sqrt( ( neighborX * neighborX ) + ( neighborY * neighborY ) ) \
        ); \
      decreasingFunctionValue = std::exp( vectorsAngleCos - 1.0  ); \
    } \
    outputVecX += \
      ( \
        ( neighborX - outputVecX ) \
        * \
        decreasingFunctionValue  \
      ); \
    outputVecY += \
      ( \
        ( neighborY - outputVecY )  \
        * \
        decreasingFunctionValue  \
      ); \
  }
  
#define UPDATESTRENGHVALUE( nighborCol, nighborRow ) \
  { \
    neighborX = diffusedVecFieldX[ nighborRow ][ nighborCol ]; \
    neighborY = diffusedVecFieldY[ nighborRow ][ nighborCol ]; \
    xDiff = neighborX - inputVecX; \
    yDiff = neighborY - inputVecY; \
    normDiff = std::sqrt( ( xDiff * xDiff ) + ( yDiff * yDiff ) ); \
    strenghValue += ( ( neighborX * xDiff ) + ( neighborY * yDiff ) ) / normDiff; \
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
      m_finiteDifferencesThreshold = 0.5;
    }

    const Skeleton::InputParameters& Skeleton::InputParameters::operator=(
      const Skeleton::InputParameters& params )
    {
      reset();

      m_inputRasterPtr = params.m_inputRasterPtr;
      m_inputRasterBand = params.m_inputRasterBand;
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
      
      te::rp::Matrix< double > edgeIntensity;
      edgeIntensity.reset( te::rp::Matrix< double >::AutoMemPol ); 
      
      te::rp::Matrix< double > edgeX;
      edgeX.reset( te::rp::Matrix< double >::AutoMemPol );
      
      te::rp::Matrix< double > edgeY;
      edgeY.reset( te::rp::Matrix< double >::AutoMemPol );      
      
      TERP_TRUE_OR_RETURN_FALSE( getEdgeStrengthMap( edgeIntensity, edgeX, edgeY ),
        "Edge map build error" );      
//      createTifFromMatrix( edgeIntensity, true, "edgeIntensity" );
//      createTifFromMatrix( edgeX, true, "edgeX" );
//      createTifFromMatrix( edgeY, true, "edgeY" );
        
      te::rp::Matrix< double > gradVecFieldX;
      gradVecFieldX.reset( te::rp::Matrix< double >::AutoMemPol );
      
      te::rp::Matrix< double > gradVecFieldY;
      gradVecFieldY.reset( te::rp::Matrix< double >::AutoMemPol );
      
      TERP_TRUE_OR_RETURN_FALSE( getInitialVectorField( edgeIntensity, 
        edgeX, edgeY , gradVecFieldX, gradVecFieldY ),
        "Gradient Vector field build error" );   
//      createTifFromMatrix( gradVecFieldX, true, "gradVecFieldX" );
//      createTifFromMatrix( gradVecFieldY, true, "gradVecFieldY" );

      edgeIntensity.reset();
      edgeX.reset();
      edgeY.reset();
        
      te::rp::Matrix< double > diffusedVecFieldX;
      diffusedVecFieldX.reset( te::rp::Matrix< double >::AutoMemPol );
      
      te::rp::Matrix< double > diffusedVecFieldY;
      diffusedVecFieldY.reset( te::rp::Matrix< double >::AutoMemPol );
      
      TERP_TRUE_OR_RETURN_FALSE( getDiffusedVectorField(  
        gradVecFieldX, gradVecFieldY, 
        diffusedVecFieldX, diffusedVecFieldY ),
        "Gradient Vector field build error" );   
//      createTifFromMatrix( diffusedVecFieldX, true, "diffusedVecFieldX" );
//      createTifFromMatrix( diffusedVecFieldY, true, "diffusedVecFieldY" );
        
      gradVecFieldX.reset();
      gradVecFieldY.reset();
      
      te::rp::Matrix< double > strenghMap;
      strenghMap.reset( te::rp::Matrix< double >::AutoMemPol );
      TERP_TRUE_OR_RETURN_FALSE( getSkeletonStrengthMap( diffusedVecFieldX, 
        diffusedVecFieldY, strenghMap ),
        "Strengh map build error" );   
      createTifFromMatrix( strenghMap, true, "strenghMap" );

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
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_finiteDifferencesThreshold >= 0.0 ) &&
        ( m_inputParameters.m_finiteDifferencesThreshold <= 1.0 ),
        "Invalid finite Differences Threshold" );           

      m_isInitialized = true;

      return true;
    }

    bool Skeleton::isInitialized() const
    {
      return m_isInitialized;
    }

    bool Skeleton::getEdgeStrengthMap( te::rp::Matrix< double >& edgeIntensity,
      te::rp::Matrix< double >& edgeX, te::rp::Matrix< double >& edgeY ) const
    {
      const unsigned int nRows = m_inputParameters.m_inputRasterPtr->getNumberOfRows();
      const unsigned int nCols = m_inputParameters.m_inputRasterPtr->getNumberOfColumns();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      
      if( ! edgeIntensity.reset( nRows,nCols ) )
        return false;
      if( ! edgeX.reset( nRows,nCols ) )
        return false;
      if( ! edgeY.reset( nRows,nCols ) )
        return false;
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        edgeIntensity[ row ][ 0 ] = 0;
        edgeIntensity[ row ][ lastColIdx ] = 0;
        
        edgeX[ row ][ 0 ] = 0;
        edgeX[ row ][ lastColIdx ] = 0;
        
        edgeY[ row ][ 0 ] = 0;
        edgeY[ row ][ lastColIdx ] = 0;        
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        edgeIntensity[ 0 ][ col ] = 0;
        edgeIntensity[ lastRowIdx ][ col ] = 0;
        
        edgeX[ 0 ][ col ] = 0;
        edgeX[ lastRowIdx ][ col ] = 0;
        
        edgeY[ 0 ][ col ] = 0;
        edgeY[ lastRowIdx ][ col ] = 0;        
      }     
      
      double diffX = 0;
      double diffY = 0;
      const te::rst::Band& band = *(m_inputParameters.m_inputRasterPtr->getBand( 
        m_inputParameters.m_inputRasterBand ));
      const double noDataValue = band.getProperty()->m_noDataValue;
      double value1 = 0;
      double value2 = 0;
      double value3 = 0;
      double value4 = 0;
      
      for( row = 1 ; row < lastRowIdx ; ++row )
        for( col = 1 ; col < lastColIdx ; ++col )
        {
          band.getValue( col - 1, row, value1 );
          band.getValue( col + 1, row, value2 );
          band.getValue( col, row - 1, value3 );
          band.getValue( col, row + 1, value4 );
          
          if( ( value1 == noDataValue ) || ( value2 == noDataValue ) ||
            ( value3 == noDataValue ) || ( value4 == noDataValue ) )
          {
            edgeX[ row ][ col ] = 0;
            edgeY[ row ][ col ] = 0;
            edgeIntensity[ row ][ col ] = 0;
          }
          else
          {
            diffX = value2 - value1;
            diffY = value3 - value4;

            edgeX[ row ][ col ] = diffX;
            edgeY[ row ][ col ] = diffY;
            edgeIntensity[ row ][ col ] = std::sqrt( ( diffX * diffX ) + ( diffY *
              diffY ) );
          }
        }
        
      return true;
    }
    
    bool Skeleton::getInitialVectorField( 
      const te::rp::Matrix< double >& edgeIntensity,
      const te::rp::Matrix< double >& edgeX, 
      const te::rp::Matrix< double >& edgeY,
      te::rp::Matrix< double >& vecFieldX, 
      te::rp::Matrix< double >& vecFieldY ) const
    {
      const unsigned int nRows = edgeIntensity.getLinesNumber();
      const unsigned int nCols = edgeIntensity.getColumnsNumber();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      const unsigned int rowsBoundIdx = nRows - 2;
      const unsigned int colsBoundIdx = nCols - 2;       
      
      if( ! vecFieldX.reset( nRows,nCols ) )
        return false;
      if( ! vecFieldY.reset( nRows,nCols ) )
        return false;
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        vecFieldX[ row ][ 0 ] = 0;
        vecFieldX[ row ][ 1 ] = 0;
        vecFieldX[ row ][ lastColIdx ] = 0;
        vecFieldX[ row ][ colsBoundIdx ] = 0;
        
        vecFieldY[ row ][ 0 ] = 0;
        vecFieldY[ row ][ 1 ] = 0;
        vecFieldY[ row ][ lastColIdx ] = 0;
        vecFieldY[ row ][ colsBoundIdx ] = 0;
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        vecFieldX[ 0 ][ col ] = 0;
        vecFieldX[ 1 ][ col ] = 0;
        vecFieldX[ lastRowIdx ][ col ] = 0;
        vecFieldX[ rowsBoundIdx ][ col ] = 0;
        
        vecFieldY[ 0 ][ col ] = 0;
        vecFieldY[ 1 ][ col ] = 0;
        vecFieldY[ lastRowIdx ][ col ] = 0;
        vecFieldY[ rowsBoundIdx ][ col ] = 0;
      }       
     
      double neighborIntensity = -1.0 * DBL_MAX;
      double neighborX = 0;
      double neighborY = 0;
      double intensityDiff = 0;
      double xDiff = 0;
      double yDiff = 0;
      double norm = 0;
      
      for( row = 2 ; row < rowsBoundIdx ; ++row )
        for( col = 2 ; col < colsBoundIdx ; ++col )
        {
          neighborIntensity = -1.0 * DBL_MAX;
          
          if( edgeIntensity[ row - 1 ][ col - 1 ] > neighborIntensity )
          {
            neighborIntensity = edgeIntensity[ row - 1 ][ col - 1 ];
            neighborX = edgeX[ row - 1 ][ col - 1 ];
            neighborY = edgeY[ row - 1 ][ col - 1 ];
          }
          
          if( edgeIntensity[ row - 1 ][ col ] > neighborIntensity )
          {
            neighborIntensity = edgeIntensity[ row - 1 ][ col ];
            neighborX = edgeX[ row - 1 ][ col ];
            neighborY = edgeY[ row - 1 ][ col ];
          }   
          
          if( edgeIntensity[ row - 1 ][ col + 1 ] > neighborIntensity )
          {
            neighborIntensity = edgeIntensity[ row - 1 ][ col + 1 ];
            neighborX = edgeX[ row - 1 ][ col + 1 ];
            neighborY = edgeY[ row - 1 ][ col + 1 ];
          }      
          
          if( edgeIntensity[ row ][ col - 1 ] > neighborIntensity )
          {
            neighborIntensity = edgeIntensity[ row ][ col - 1 ];
            neighborX = edgeX[ row ][ col - 1 ];
            neighborY = edgeY[ row ][ col - 1 ];
          }
          
          if( edgeIntensity[ row ][ col + 1 ] > neighborIntensity )
          {
            neighborIntensity = edgeIntensity[ row ][ col + 1 ];
            neighborX = edgeX[ row ][ col + 1 ];
            neighborY = edgeY[ row ][ col + 1 ];
          }     
          
          if( edgeIntensity[ row + 1 ][ col - 1 ] > neighborIntensity )
          {
            neighborIntensity = edgeIntensity[ row + 1 ][ col - 1 ];
            neighborX = edgeX[ row + 1 ][ col - 1 ];
            neighborY = edgeY[ row + 1 ][ col - 1 ];
          }
          
          if( edgeIntensity[ row + 1 ][ col ] > neighborIntensity )
          {
            neighborIntensity = edgeIntensity[ row + 1 ][ col ];
            neighborX = edgeX[ row + 1 ][ col ];
            neighborY = edgeY[ row + 1 ][ col ];
          }   
          
          if( edgeIntensity[ row + 1 ][ col + 1 ] > neighborIntensity )
          {
            neighborIntensity = edgeIntensity[ row + 1 ][ col + 1 ];
            neighborX = edgeX[ row + 1 ][ col + 1 ];
            neighborY = edgeY[ row + 1 ][ col + 1 ];
          }
          
          intensityDiff = neighborIntensity - edgeIntensity[ row ][ col ];
          xDiff = neighborX - edgeX[ row ][ col ];
          yDiff = neighborY - edgeY[ row ][ col ];          
          norm = std::sqrt( ( xDiff * xDiff ) + ( yDiff + yDiff ) );
          
          if( norm == 0.0 )
          {
            vecFieldX[ row ][ col ] = 0;
            vecFieldY[ row ][ col ] = 0;
          }
          else
          {
            intensityDiff /= norm;
          
            xDiff *= intensityDiff;
            yDiff *= intensityDiff;
            
            vecFieldX[ row ][ col ] = xDiff;
            vecFieldY[ row ][ col ] = yDiff;
          }
        }
        
        
      return true;
    }
    
    bool Skeleton::getDiffusedVectorField( 
      const te::rp::Matrix< double >& inputVecFieldX, 
      const te::rp::Matrix< double >& inputVecFieldY,
      te::rp::Matrix< double >& diffusedVecFieldX, 
      te::rp::Matrix< double >& diffusedVecFieldY ) const
    {
      assert( inputVecFieldX.getColumnsNumber() == inputVecFieldY.getColumnsNumber() );
      assert( inputVecFieldX.getLinesNumber() == inputVecFieldY.getLinesNumber() );
      
      const unsigned int nRows = inputVecFieldX.getLinesNumber();
      const unsigned int nCols = inputVecFieldX.getColumnsNumber();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      const unsigned int rowsBoundIdx = nRows - 2;
      const unsigned int colsBoundIdx = nCols - 2;       
      
      if( ! diffusedVecFieldX.reset( nRows,nCols ) )
        return false;
      if( ! diffusedVecFieldY.reset( nRows,nCols ) )
        return false;
      
      te::rp::Matrix< double > xBuf1;
      if( ! xBuf1.reset( nRows,nCols, te::rp::Matrix< double >::AutoMemPol ) )
        return false;
      te::rp::Matrix< double > xBuf2;
      if( ! xBuf2.reset( nRows,nCols, te::rp::Matrix< double >::AutoMemPol ) )
        return false;
      te::rp::Matrix< double > yBuf1;
      if( ! yBuf1.reset( nRows,nCols, te::rp::Matrix< double >::AutoMemPol ) )
        return false;
      te::rp::Matrix< double > yBuf2;
      if( ! yBuf2.reset( nRows,nCols, te::rp::Matrix< double >::AutoMemPol ) )
        return false;      
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        
        diffusedVecFieldX[ row ][ 0 ] = 0;
        diffusedVecFieldX[ row ][ 1 ] = 0;
        diffusedVecFieldX[ row ][ lastColIdx ] = 0;
        diffusedVecFieldX[ row ][ colsBoundIdx ] = 0;
        
        diffusedVecFieldY[ row ][ 0 ] = 0;
        diffusedVecFieldY[ row ][ 1 ] = 0;
        diffusedVecFieldY[ row ][ lastColIdx ] = 0;
        diffusedVecFieldY[ row ][ colsBoundIdx ] = 0;
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        diffusedVecFieldX[ 0 ][ col ] = 0;
        diffusedVecFieldX[ 1 ][ col ] = 0;
        diffusedVecFieldX[ lastRowIdx ][ col ] = 0;
        diffusedVecFieldX[ rowsBoundIdx ][ col ] = 0;
        
        diffusedVecFieldY[ 0 ][ col ] = 0;
        diffusedVecFieldY[ 1 ][ col ] = 0;
        diffusedVecFieldY[ lastRowIdx ][ col ] = 0;
        diffusedVecFieldY[ rowsBoundIdx ][ col ] = 0;
      }  
      
      double maxX = -1.0 * DBL_MAX;
      double minX = DBL_MAX;
      double maxY = -1.0 * DBL_MAX;
      double minY = DBL_MAX;      
      
      for( row = 2 ; row < rowsBoundIdx ; ++row )
      {
        for( col = 2 ; col < colsBoundIdx ; ++col )
        {
          if( maxX < inputVecFieldX[ row ][ col ] )
            maxX = inputVecFieldX[ row ][ col ];
          if( minX > inputVecFieldX[ row ][ col ] )
            minX = inputVecFieldX[ row ][ col ];

          if( maxY < inputVecFieldY[ row ][ col ] )
            maxY = inputVecFieldY[ row ][ col ];
          if( minY > inputVecFieldY[ row ][ col ] )
            minY = inputVecFieldY[ row ][ col ];          
        }
      }
      
      const double xThreshold = std::max( std::abs( maxX ),
        std::abs( minX ) ) * 
        m_inputParameters.m_finiteDifferencesThreshold;
      const double yThreshold = std::max( std::abs( maxY ),
        std::abs( minY ) ) * 
        m_inputParameters.m_finiteDifferencesThreshold;
      const unsigned int maxIterations = nRows * nCols;
      unsigned int iteration = 0;
      double xResidue = DBL_MAX;
      double yResidue = DBL_MAX;
      te::rp::Matrix< double > const * iXPtr = 0;
      te::rp::Matrix< double >* oXPtr = 0;
      te::rp::Matrix< double > const * iYPtr = 0;
      te::rp::Matrix< double >* oYPtr = 0;         
      
      while(
        ( iteration < maxIterations )
        &&
        (
          ( xResidue > xThreshold )
          ||
          ( yResidue > yThreshold )
        )
      )
      {
        // defining the input and output buffers
        
        if( iteration == 0 )
        {
          iXPtr = &inputVecFieldX;
          oXPtr = &xBuf1;          
          
          iYPtr = &inputVecFieldY;
          oYPtr = &yBuf1;          
        }
        else
        {
          if( oXPtr == (&xBuf1) )
          {
            iXPtr = &xBuf1;
            oXPtr = &xBuf2;

            iYPtr = &yBuf1;
            oYPtr = &yBuf2;
          }
          else
          {
            iXPtr = &xBuf2;
            oXPtr = &xBuf1;

            iYPtr = &yBuf2;
            oYPtr = &yBuf1;
            
          }
        }
        
        const te::rp::Matrix< double >& iX = *iXPtr;
        te::rp::Matrix< double >& oX = *oXPtr;          
        const te::rp::Matrix< double >& iY = *iYPtr;
        te::rp::Matrix< double >& oY = *oYPtr;          
        
        // difusing
        
        xResidue = 0;
        yResidue = 0;        
        
//        double neighborMag = -1.0 * DBL_MAX;
        double neighborX = 0;
        double neighborY = 0;  
        double vectorsAngleCos = 0.0;
        double decreasingFunctionValue = 0;
        double inputVecX = 0;
        double inputVecY = 0;
        double outputVecX = 0;
        double outputVecY = 0;
          
        for( row = 2 ; row < rowsBoundIdx ; ++row )
        {
          for( col = 2 ; col < colsBoundIdx ; ++col )
          {  
            inputVecX = iX[ row ][ col ];
            outputVecX = inputVecX;
            
            inputVecY = iY[ row ][ col ];    
            outputVecY = inputVecY;
            
            UPDATEDOUTDIFVECTOR( col - 1, row - 1 );
            UPDATEDOUTDIFVECTOR( col, row - 1 );
            UPDATEDOUTDIFVECTOR( col + 1, row - 1 );
            
            UPDATEDOUTDIFVECTOR( col - 1, row );
            UPDATEDOUTDIFVECTOR( col + 1, row );
            
            UPDATEDOUTDIFVECTOR( col - 1, row + 1 );
            UPDATEDOUTDIFVECTOR( col, row + 1 );
            UPDATEDOUTDIFVECTOR( col + 1, row + 1 );
              
            oX[ row ][ col ] = outputVecX;
            oY[ row ][ col ] = outputVecY;

            xResidue = std::max( xResidue, std::abs( outputVecX - inputVecX ) );
            yResidue = std::max( yResidue, std::abs( outputVecY - inputVecY ) );
          }
        }
        
        ++iteration;
      }
      
      // copying data from the last internal output buffer the the external output buffer
      
      if( oXPtr )
      {
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            diffusedVecFieldX[ row ][ col ] = oXPtr->operator[]( row )[ col ];
            diffusedVecFieldY[ row ][ col ] = oYPtr->operator[]( row )[ col ];
          }      
        }
      }
      
      return true;
    }        
    
    void Skeleton::createTifFromMatrix( 
      const te::rp::Matrix< double >& matrix,
      const bool normalize,
      const std::string& tifFileName ) const 
    {
      std::map<std::string, std::string> rInfo;
      rInfo["URI"] = tifFileName + ".tif";
      
      std::vector<te::rst::BandProperty*> bandsProperties;
      if( normalize )
        bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
      else
        bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE, "" ));
      bandsProperties[0]->m_colorInterp = te::rst::GrayIdxCInt;
      bandsProperties[0]->m_noDataValue = -1.0 * DBL_MAX;

      te::rst::Grid* newgrid = new te::rst::Grid( matrix.getColumnsNumber(),
        matrix.getLinesNumber(), 0, -1 );

      std::auto_ptr< te::rst::Raster > outputRasterPtr(
        te::rst::RasterFactory::make( "GDAL", newgrid, bandsProperties, rInfo, 0, 0));
      TERP_TRUE_OR_THROW( outputRasterPtr.get(), "Output raster creation error");
          
      unsigned int line = 0;
      unsigned int col = 0;
      const unsigned int nLines = matrix.getLinesNumber();
      const unsigned int nCols = matrix.getColumnsNumber();
      double value = 0;

      double gain = 1.0;
      double offset = 0.0;
      if( normalize )
      {
        double rasterDataMin = DBL_MAX;
        double rasterDataMax = -1.0 * DBL_MIN;
        for( line = 0 ; line < nLines ; ++line )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            value = matrix[ line ][ col ];
            if( value < rasterDataMin )
              rasterDataMin = value;
            if( value > rasterDataMax )
              rasterDataMax = value;
          }
        }
          
        if( rasterDataMax == rasterDataMin )
        {
          gain = 0.0;
          offset = 0.0;
        }
        else
        {
          gain = 255.0 / ( rasterDataMax - rasterDataMin );
          offset = -1.0 * ( rasterDataMin );
        }
      }
      
      for( line = 0 ; line < nLines ; ++line )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          value = matrix[ line ][ col ];
         
          if( normalize )
          {
            value += offset;
            value *= gain;
            value = std::max( 0.0, value );
            value = std::min( 255.0, value );
          }
          
          outputRasterPtr->setValue( col, line, value, 0 );
        }
      }
    }    
    
    bool Skeleton::getSkeletonStrengthMap( 
      const te::rp::Matrix< double >& diffusedVecFieldX, 
      const te::rp::Matrix< double >& diffusedVecFieldY,
      te::rp::Matrix< double >& strenghMap ) const
    {
      assert( diffusedVecFieldX.getColumnsNumber() == diffusedVecFieldY.getColumnsNumber() );
      assert( diffusedVecFieldX.getLinesNumber() == diffusedVecFieldY.getLinesNumber() );
      
      const unsigned int nRows = diffusedVecFieldX.getLinesNumber();
      const unsigned int nCols = diffusedVecFieldX.getColumnsNumber();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      const unsigned int auxRowsBoundIdx = nRows - 2;
      const unsigned int auxColsBoundIdx = nCols - 2;       
      const unsigned int rowsBoundIdx = nRows - 3;
      const unsigned int colsBoundIdx = nCols - 3;       
      
      if( ! strenghMap.reset( nRows,nCols ) )
        return false;
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        
        strenghMap[ row ][ 0 ] = 0;
        strenghMap[ row ][ 1 ] = 0;
        strenghMap[ row ][ 1 ] = 0;
        strenghMap[ row ][ lastColIdx ] = 0;
        strenghMap[ row ][ auxColsBoundIdx ] = 0;
        strenghMap[ row ][ colsBoundIdx ] = 0;        
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        strenghMap[ 0 ][ col ] = 0;
        strenghMap[ 1 ][ col ] = 0;
        strenghMap[ 2 ][ col ] = 0;
        strenghMap[ lastRowIdx ][ col ] = 0;
        strenghMap[ auxRowsBoundIdx ][ col ] = 0;
        strenghMap[ rowsBoundIdx ][ col ] = 0;
      }  
      
      double neighborX = 0;
      double neighborY = 0;  
      double inputVecX = 0;
      double inputVecY = 0;
      double strenghValue = 0;
      double xDiff = 0;
      double yDiff = 0; 
      double normDiff = 0;
        
      for( row = 3 ; row < rowsBoundIdx ; ++row )
      {
        for( col = 3 ; col < colsBoundIdx ; ++col )
        {  
          inputVecX = diffusedVecFieldX[ row ][ col ];
          inputVecY = diffusedVecFieldY[ row ][ col ];    
          strenghValue = 0;
          
          UPDATESTRENGHVALUE( col - 1, row - 1 );
          UPDATESTRENGHVALUE( col, row - 1 );
          UPDATESTRENGHVALUE( col + 1, row - 1 );
          
          UPDATESTRENGHVALUE( col - 1, row );
          UPDATESTRENGHVALUE( col + 1, row );
          
          UPDATESTRENGHVALUE( col - 1, row + 1 );
          UPDATESTRENGHVALUE( col, row + 1 );
          UPDATESTRENGHVALUE( col + 1, row + 1 );
            
          strenghMap[ row ][ col ] = std::max( 0.0, strenghValue );
        }
      }
           
      return true;
    }        
    

  } // end namespace rp
}   // end namespace te

