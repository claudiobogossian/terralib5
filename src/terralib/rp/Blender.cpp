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
  \file terralib/rp/Blender.cpp
  \brief Blended pixel value calculation for two overlaped rasters.
*/

#include "Blender.h"

#include "Macros.h"
#include "../geometry/LinearRing.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/Point.h"
#include "../geometry/CurvePolygon.h"
#include "../geometry/Envelope.h"
#include "../geometry/Enums.h"
#include "../raster/Raster.h"
#include "../raster/Grid.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/Utils.h"
#include "../raster/SynchronizedRaster.h"
#include "../common/PlatformUtils.h"
#include "../common/progress/TaskProgress.h"

#include <boost/thread.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/scoped_ptr.hpp>

#include <complex>
#include <limits>
#include <algorithm>
#include <memory>

// Get the perpendicular distance from a point P(pX,pY) from a line defined
// by the points A(lineAX,lineAY) and B(lineBX,lineBY)
// Requires two previously declared variables aux1 and aux2
#define getPerpendicularDistance( pX, pY, lineAX, lineAY, lineBX, lineBY, aux1, aux2, perpDist ) \
  aux1 = lineAX - lineBX; \
  aux2 = lineAY - lineBY; \
  if( aux1 == 0.0 ) \
  { \
    perpDist = std::abs( pX - lineAX ); \
  } \
  else if( aux2 == 0.0 ) \
  { \
    perpDist = std::abs( pY - lineAY ); \
  } \
  else \
  { \
    perpDist = \
      std::abs( \
        ( aux2 * pX ) - ( aux1 * pY ) + ( lineAX * lineBY ) - ( lineBX * lineAY ) \
      ) \
      / \
      std::sqrt( ( aux1 * aux1 ) + ( aux2 * aux2 ) ); \
  }

namespace te
{
  namespace rp
  {
    Blender::BlendIntoRaster1ThreadParams::BlendIntoRaster1ThreadParams()
    : m_returnValuePtr( 0 ), m_abortValuePtr( 0 ), m_sync1Ptr( 0 ), m_sync2Ptr( 0 ),
      m_raster1BlocksInfosPtr( 0 ), m_mutexPtr( 0 ), m_blockProcessedSignalMutexPtr( 0 ),
      m_blockProcessedSignalPtr( 0 ), m_runningThreadsCounterPtr( 0 ),
      m_blendMethod( te::rp::Blender::InvalidBlendMethod ),
      m_interpMethod2( te::rst::NearestNeighbor ),
      m_noDataValue( 0.0 ), m_forceRaster1NoDataValue( false ),
      m_forceRaster2NoDataValue( false ),  
      m_maxRasterCachedBlocks( 0 ), m_useProgress( false )
    {
    }
    
    Blender::BlendIntoRaster1ThreadParams::BlendIntoRaster1ThreadParams( 
      const BlendIntoRaster1ThreadParams& rhs )
    {
      operator=( rhs );
    }
    
    Blender::BlendIntoRaster1ThreadParams::~BlendIntoRaster1ThreadParams()
    {
    }
    
    te::rp::Blender::BlendIntoRaster1ThreadParams& Blender::BlendIntoRaster1ThreadParams::operator=( 
      const BlendIntoRaster1ThreadParams& rhs )
    {
      m_returnValuePtr = rhs.m_returnValuePtr;
      m_abortValuePtr = rhs.m_abortValuePtr;
      m_sync1Ptr = rhs.m_sync1Ptr;
      m_sync2Ptr = rhs.m_sync2Ptr;
      m_raster1BlocksInfosPtr = rhs.m_raster1BlocksInfosPtr;
      m_mutexPtr = rhs.m_mutexPtr;
      m_blockProcessedSignalMutexPtr = rhs.m_blockProcessedSignalMutexPtr;
      m_blockProcessedSignalPtr = rhs.m_blockProcessedSignalPtr;
      m_runningThreadsCounterPtr = rhs.m_runningThreadsCounterPtr;
      m_raster1Bands = rhs.m_raster1Bands;
      m_raster2Bands = rhs.m_raster2Bands;
      m_blendMethod = rhs.m_blendMethod;
      m_interpMethod2 = rhs.m_interpMethod2;
      m_noDataValue = rhs.m_noDataValue;
      m_forceRaster1NoDataValue = rhs.m_forceRaster1NoDataValue;
      m_forceRaster2NoDataValue = rhs.m_forceRaster2NoDataValue;
      m_maxRasterCachedBlocks = rhs.m_maxRasterCachedBlocks;
      m_pixelOffsets1 = rhs.m_pixelOffsets1;
      m_pixelScales1 = rhs.m_pixelScales1;
      m_pixelOffsets2 = rhs.m_pixelOffsets2;
      m_pixelScales2 = rhs.m_pixelScales2;
      m_useProgress = rhs.m_useProgress;
      
      if( rhs.m_r1ValidDataDelimiterPtr.get() )
      {
        m_r1ValidDataDelimiterPtr.reset( (te::gm::MultiPolygon*) rhs.m_r1ValidDataDelimiterPtr->clone() );
      }
      else
      {
        m_r1ValidDataDelimiterPtr.reset();
      }
      
      if( rhs.m_r2ValidDataDelimiterPtr.get() )
      {
        m_r2ValidDataDelimiterPtr.reset( (te::gm::MultiPolygon*) rhs.m_r2ValidDataDelimiterPtr->clone() );
      }      
      else
      {
        m_r2ValidDataDelimiterPtr.reset();
      }
      
      if( rhs.m_geomTransformationPtr.get() )
      {
        m_geomTransformationPtr.reset( rhs.m_geomTransformationPtr->clone() );
      }        
      else
      {
        m_geomTransformationPtr.reset();
      }
      
      return *this;
    }
    
    // ----------------------------------------------------------------------
    Blender::Blender()
    {
      initState();
    }
    
    Blender::~Blender()
    {
      clear();
    }    
    
    bool Blender::initialize( te::rst::Raster& raster1, 
      const std::vector< unsigned int >& raster1Bands, 
      const te::rst::Raster& raster2, 
      const std::vector< unsigned int >& raster2Bands,
      const BlendMethod& blendMethod, 
      const te::rst::Interpolator::Method& interpMethod1,
      const te::rst::Interpolator::Method& interpMethod2,
      const double& noDataValue,
      const bool forceRaster1NoDataValue,
      const bool forceRaster2NoDataValue,
      const std::vector< double >& pixelOffsets1,
      const std::vector< double >& pixelScales1,
      const std::vector< double >& pixelOffsets2,
      const std::vector< double >& pixelScales2,
      te::gm::MultiPolygon const * const r1ValidDataDelimiterPtr,
      te::gm::MultiPolygon const * const r2ValidDataDelimiterPtr,
      const te::gm::GeometricTransformation& geomTransformation,
      const unsigned int threadsNumber,
       const bool enableProgressInterface )
    {
      TERP_TRUE_OR_RETURN_FALSE( 
        raster1.getAccessPolicy() & te::common::RAccess, 
        "Invalid raster 1" );        
      TERP_TRUE_OR_RETURN_FALSE( 
        raster2.getAccessPolicy() & te::common::RAccess, 
        "Invalid raster 2" ); 
      TERP_TRUE_OR_RETURN_FALSE( raster1Bands.size() > 0,
        "Invalid raster bands vector" );      
      TERP_TRUE_OR_RETURN_FALSE( raster1Bands.size() ==
        raster2Bands.size(), "Invalid raster bands vector" );
      TERP_TRUE_OR_RETURN_FALSE( pixelOffsets1.size() ==  
        raster1Bands.size(), "Invalid pixel offsets" );
      TERP_TRUE_OR_RETURN_FALSE( pixelScales1.size() ==  
        raster1Bands.size(), "Invalid pixel scales" );        
      TERP_TRUE_OR_RETURN_FALSE( pixelOffsets2.size() ==  
        raster2Bands.size(), "Invalid pixel offsets" );
      TERP_TRUE_OR_RETURN_FALSE( pixelScales2.size() ==  
        raster2Bands.size(), "Invalid pixel scales" );        
      TERP_TRUE_OR_RETURN_FALSE( ( r1ValidDataDelimiterPtr ?
        ( r1ValidDataDelimiterPtr->getNumGeometries() > 0 ) : true ),
        "Invalid polygon 1" )
      TERP_TRUE_OR_RETURN_FALSE( ( r2ValidDataDelimiterPtr ?
        ( r2ValidDataDelimiterPtr->getNumGeometries() > 0 ) : true ),
        "Invalid polygon 2" )
      TERP_TRUE_OR_RETURN_FALSE( geomTransformation.isValid(),
        "Invalid transformation" );
        
      clear();
      
      // defining the input rasters
      
      m_raster1Ptr = &raster1;
      m_raster2Ptr = &raster2;
      
      // Generating the valid data area points
      
      if( r1ValidDataDelimiterPtr )
      {
        m_r1ValidDataDelimiterPtr.reset( (te::gm::MultiPolygon*)r1ValidDataDelimiterPtr->clone() );
      }
      if( r2ValidDataDelimiterPtr )
      {
        m_r2ValidDataDelimiterPtr.reset( (te::gm::MultiPolygon*)r2ValidDataDelimiterPtr->clone() );
      }      
      
      // indexed under raster 1 lines/cols
      std::auto_ptr< te::gm::MultiPolygon > indexedDelimiter1Ptr(
        new te::gm::MultiPolygon( 0, te::gm::MultiPolygonType, 0, 0 ) ); 
      
      if( r1ValidDataDelimiterPtr )
      {
        const te::rst::Grid& grid = (*raster1.getGrid());
        
        const std::size_t nGeoms = r1ValidDataDelimiterPtr->getNumGeometries();        
        
        for( std::size_t geomIdx = 0 ; geomIdx < nGeoms ; ++geomIdx )
        {
          te::gm::Polygon const* geomPtr = dynamic_cast< te::gm::Polygon* >(
            r1ValidDataDelimiterPtr->getGeometryN( geomIdx ) );
          TERP_DEBUG_TRUE_OR_THROW( geomPtr, "Invalid geometry pointer" );
          
          const std::size_t nRings = geomPtr->getNumRings();
          
          te::gm::Polygon* outPolPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );           
          
          for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
          {
            te::gm::LinearRing const* inRingPtr = dynamic_cast< te::gm::LinearRing const* >( 
              geomPtr->getRingN( ringIdx ) );
            assert( inRingPtr );
            
            const std::size_t nPoints = inRingPtr->getNPoints();
            te::gm::Coord2D const * inCoordsPtr = inRingPtr->getCoordinates();
            te::gm::Coord2D auxCoord;        
            
            te::gm::LinearRing* outRingPtr = new te::gm::LinearRing( nPoints, 
               te::gm::LineStringType, 0, 0 );
            
            for( std::size_t pIdx = 0 ; pIdx < nPoints ; ++pIdx )
            {
              grid.geoToGrid( inCoordsPtr[ pIdx ].x, inCoordsPtr[ pIdx ].y, 
                auxCoord.x, auxCoord.y ); 
              outRingPtr->setPoint( pIdx, auxCoord.x, auxCoord.y );
            }
            
            outPolPtr->add( outRingPtr );          
            
          }
          
          indexedDelimiter1Ptr->add( outPolPtr );
        }
      }
      else
      {
        te::gm::LinearRing* outRingPtr = new te::gm::LinearRing( 5, 
           te::gm::LineStringType, 0, 0 );        
        
        outRingPtr->setPoint( 0, 
           -0.5, 
           -0.5 );
        outRingPtr->setPoint( 1, 
           ((double)raster1.getNumberOfColumns()) - 0.5, 
           -0.5 );
        outRingPtr->setPoint( 2, 
           ((double)raster1.getNumberOfColumns()) - 0.5, 
           ((double)raster1.getNumberOfRows()) - 0.5 );
        outRingPtr->setPoint( 3, 
           -0.5, 
           ((double)raster1.getNumberOfRows()) - 0.5 );
        outRingPtr->setPoint( 4, 
           -0.5, 
           -0.5 );
        
        te::gm::Polygon* outPolPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );           
        
        outPolPtr->add( outRingPtr );
           
        indexedDelimiter1Ptr->add( outPolPtr );
      }
      
      // indexed under raster 1 lines/cols
      std::auto_ptr< te::gm::MultiPolygon > indexedDelimiter2Ptr(
        new te::gm::MultiPolygon( 0, te::gm::MultiPolygonType, 0, 0 ) ); 
      
      if( r2ValidDataDelimiterPtr )
      {
        const te::rst::Grid& grid = (*raster2.getGrid());
        
        const std::size_t nGeoms = r2ValidDataDelimiterPtr->getNumGeometries();
        
        for( std::size_t geomIdx = 0 ; geomIdx < nGeoms ; ++geomIdx )
        {        
          te::gm::Polygon const* geomPtr = dynamic_cast< te::gm::Polygon* >(
            r2ValidDataDelimiterPtr->getGeometryN( geomIdx ) );
          TERP_DEBUG_TRUE_OR_THROW( geomPtr, "Invalid geometry pointer" );          
          
          const std::size_t nRings = geomPtr->getNumRings();
          
          te::gm::Polygon* outPolPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 ); 
          
          for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
          {
            te::gm::LinearRing const* inRingPtr = dynamic_cast< te::gm::LinearRing const* >( 
              geomPtr->getRingN( ringIdx ) );
            assert( inRingPtr );
            
            const std::size_t nPoints = inRingPtr->getNPoints();
            te::gm::Coord2D const * inCoordsPtr = inRingPtr->getCoordinates();
            te::gm::Coord2D auxCoord;        
            te::gm::Coord2D auxCoord2;        
            
            te::gm::LinearRing* outRingPtr = new te::gm::LinearRing( nPoints, 
               te::gm::LineStringType, 0, 0 );
            
            for( std::size_t pIdx = 0 ; pIdx < nPoints ; ++pIdx )
            {
              grid.geoToGrid( inCoordsPtr[ pIdx ].x, inCoordsPtr[ pIdx ].y, 
                auxCoord.x, auxCoord.y ); 
              geomTransformation.inverseMap( auxCoord.x, auxCoord.y, auxCoord2.x, auxCoord2.y );
              outRingPtr->setPoint( pIdx, auxCoord2.x, auxCoord2.y );
            }
            
            outPolPtr->add( outRingPtr );
          }
          
          indexedDelimiter2Ptr->add( outPolPtr );
        }
      }
      else
      {
        te::gm::LinearRing* outRingPtr = new te::gm::LinearRing( 5, 
           te::gm::LineStringType, 0, 0 );          
        
        te::gm::Coord2D auxCoord;
        
        geomTransformation.inverseMap( 
          -0.5,
          -0.5,
          auxCoord.x, auxCoord.y );
        outRingPtr->setPoint( 0, auxCoord.x, auxCoord.y );
        outRingPtr->setPoint( 4, auxCoord.x, auxCoord.y ); 
        
        geomTransformation.inverseMap( 
          ((double)raster2.getNumberOfColumns()) - 0.5, 
           -0.5,
          auxCoord.x, auxCoord.y );
        outRingPtr->setPoint( 1, auxCoord.x, auxCoord.y );
        
        geomTransformation.inverseMap( 
          ((double)raster2.getNumberOfColumns()) - 0.5, 
          ((double)raster2.getNumberOfRows()) - 0.5,
          auxCoord.x, auxCoord.y );
        outRingPtr->setPoint( 2, auxCoord.x, auxCoord.y );
        
        geomTransformation.inverseMap( 
          -0.5, 
         ((double)raster2.getNumberOfRows()) - 0.5,
          auxCoord.x, auxCoord.y );
        outRingPtr->setPoint( 3, auxCoord.x, auxCoord.y );
        
        te::gm::Polygon* outPolPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );   
        
        outPolPtr->add( outRingPtr );
        
        indexedDelimiter2Ptr->add( outPolPtr );
      }
      
      // Calculating the intersection (raster 1 lines/cols)
      
      m_intersectionPtr.reset( indexedDelimiter2Ptr->intersection( 
        indexedDelimiter1Ptr.get() ) );
      
      // Initializing the intersection tile indexer
      
      if( m_intersectionPtr.get() )
      {
        TERP_TRUE_OR_THROW( getTileIndexers( m_intersectionPtr.get(), 
          m_intersectionTileIndexers ), "Intersection tile indexers creation error" );
      }
      
      // Extracting the intersection segments points
      
      if( m_intersectionPtr.get() )
      {
        std::size_t ringIdx = 0;
        std::auto_ptr< te::gm::Geometry > ringIntersectionPtr;
        std::size_t nPols = 0;
        std::size_t polIdx = 0;
        
        nPols = indexedDelimiter2Ptr->getNumGeometries();           
        for( polIdx = 0 ; polIdx < nPols ; ++polIdx )
        {
          te::gm::Polygon const* polPtr = dynamic_cast< te::gm::Polygon* >(
            indexedDelimiter2Ptr->getGeometryN( polIdx ) );
          TERP_DEBUG_TRUE_OR_THROW( polPtr, "Invalid geometry pointer" );        
        
          for( ringIdx = 0 ; ringIdx < polPtr->getNumRings() ;
            ++ringIdx )
          {
            ringIntersectionPtr.reset( indexedDelimiter1Ptr->intersection( 
              polPtr->getRingN( ringIdx ) ) );
            
            if( ringIntersectionPtr.get() != 0 ) 
            {
              TERP_TRUE_OR_THROW( getSegments( ringIntersectionPtr.get(),
                m_r2IntersectionSegmentsPoints ), "Error getting intersection segments" );
            }
          }
        }
        
        nPols = indexedDelimiter1Ptr->getNumGeometries();          
        for( polIdx = 0 ; polIdx < nPols ; ++polIdx )
        {
          te::gm::Polygon const* polPtr = dynamic_cast< te::gm::Polygon* >(
            indexedDelimiter1Ptr->getGeometryN( polIdx ) );
          TERP_DEBUG_TRUE_OR_THROW( polPtr, "Invalid geometry pointer" );         
        
          for( ringIdx = 0 ; ringIdx < polPtr->getNumRings() ;
            ++ringIdx )
          {
            ringIntersectionPtr.reset( indexedDelimiter2Ptr->intersection( 
              polPtr->getRingN( ringIdx ) ) );
            
            if( ringIntersectionPtr.get() != 0 ) 
            {
              TERP_TRUE_OR_THROW( getSegments( ringIntersectionPtr.get(),
                m_r1IntersectionSegmentsPoints ), "Error getting intersection segments" );
            }
          }    
        }    

/*        std::cout << std::endl;
        for( unsigned int idx = 0 ; idx < m_r1IntersectionSegmentsPoints.size() ; ++idx )
        {
          std::cout << std::endl << "m_r1IntersectionSegmentsPoints[" << idx << "]=" 
            << m_r1IntersectionSegmentsPoints[ idx ].first.x
            << " " << m_r1IntersectionSegmentsPoints[ idx ].first.y
            << " " << m_r1IntersectionSegmentsPoints[ idx ].second.x
            << " " << m_r1IntersectionSegmentsPoints[ idx ].second.y;            
        }
        for( unsigned int idx = 0 ; idx < m_r2IntersectionSegmentsPoints.size() ; ++idx )
        {
          std::cout << std::endl << "m_r2IntersectionSegmentsPoints[" << idx << "]=" 
            << m_r2IntersectionSegmentsPoints[ idx ].first.x
            << " " << m_r2IntersectionSegmentsPoints[ idx ].first.y
            << " " << m_r2IntersectionSegmentsPoints[ idx ].second.x
            << " " << m_r2IntersectionSegmentsPoints[ idx ].second.y;            
        }   
        std::cout << std::endl;  */   
          
        m_r2IntersectionSegmentsPointsSize = (unsigned int)m_r2IntersectionSegmentsPoints.size();
        m_r1IntersectionSegmentsPointsSize = (unsigned int)m_r1IntersectionSegmentsPoints.size();
      }
      
      // defining the blending method
      
      switch( blendMethod )
      {
        case NoBlendMethod :
        {
          m_blendMethod = NoBlendMethod;
          break;
        }
        case EuclideanDistanceMethod :
        {
          if( ( m_intersectionPtr.get() != 0 ) && 
            ( m_r1IntersectionSegmentsPointsSize > 1 ) && 
            ( m_r2IntersectionSegmentsPointsSize > 1 ) )
          {
            m_blendMethod = EuclideanDistanceMethod;
          }
          else
          {
            m_blendMethod = NoBlendMethod;
          }
          break;
        }
        case SumMethod :
        {
          if( ( m_intersectionPtr.get() != 0 ) && 
            ( m_r1IntersectionSegmentsPointsSize > 1 ) && 
            ( m_r2IntersectionSegmentsPointsSize > 1 ) )
          {
            m_blendMethod = SumMethod;
          }
          else
          {
            m_blendMethod = NoBlendMethod;
          }
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid blend method" );
          break;
        }
      }      
      
      // Defining the blending function pointers
      
      setBlendFunctionPonter( m_blendMethod );
              
      // defining the geometric transformation  
        
      m_geomTransformationPtr = geomTransformation.clone();
      
      // defining the interpolators
      
      if( forceRaster1NoDataValue )
      {
        std::vector< std::complex<double> > noDataValues1( raster1.getNumberOfBands(),
          std::complex<double>( noDataValue, 0.0 ) ); 
        m_interp1Ptr.reset( new te::rst::Interpolator( &raster1, interpMethod1,
          noDataValues1 ) );
      }
      else
      {
        m_interp1Ptr.reset( new te::rst::Interpolator( &raster1, interpMethod1 ) );
      }      
      
      if( forceRaster2NoDataValue )
      {
        std::vector< std::complex<double> > noDataValues2( raster2.getNumberOfBands(),
          std::complex<double>( noDataValue, 0.0 ) );
        m_interp2Ptr.reset( new te::rst::Interpolator( &raster2, interpMethod2,
          noDataValues2 ) );
      }
      else
      {
        m_interp2Ptr.reset( new te::rst::Interpolator( &raster2, interpMethod2 ) );
      }
        
      m_interpMethod1 = interpMethod1;
      m_interpMethod2 = interpMethod2;
      
      // defining dummy values
      
      m_forceRaster1NoDataValue = forceRaster1NoDataValue;
      m_forceRaster2NoDataValue = forceRaster2NoDataValue;
      
      for( std::vector< unsigned int >::size_type rasterBandsIdx = 0 ; 
        rasterBandsIdx < raster1Bands.size() ; ++rasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( raster1Bands[ rasterBandsIdx ] <
          raster1.getNumberOfBands(), "Invalid band" );
        TERP_TRUE_OR_RETURN_FALSE( raster2Bands[ rasterBandsIdx ] <
          raster2.getNumberOfBands(), "Invalid band" );            
        
        if( forceRaster1NoDataValue )
        {
          m_raster1NoDataValues.push_back( noDataValue );
        }
        else
        {
          m_raster1NoDataValues.push_back( raster1.getBand( raster1Bands[ 
            rasterBandsIdx ] )->getProperty()->m_noDataValue );
        }
        
        if( forceRaster2NoDataValue )
        {
          m_raster2NoDataValues.push_back( noDataValue );
        }
        else
        {
          m_raster2NoDataValues.push_back( raster2.getBand( raster2Bands[ 
            rasterBandsIdx ] )->getProperty()->m_noDataValue );
        }        
      }
      
      m_outputNoDataValue = noDataValue;
      
      // defining raster bands
      
      m_raster1Bands = raster1Bands;
      m_raster2Bands = raster2Bands;
      
      // defining pixel offsets
      
      m_pixelOffsets1 = pixelOffsets1;
      m_pixelScales1 = pixelScales1;
      
      m_pixelOffsets2 = pixelOffsets2;
      m_pixelScales2 = pixelScales2;
      
      // threads
      
      if( threadsNumber == 0 )
      {
        m_threadsNumber = te::common::GetPhysProcNumber();
      }
      else
      {
        m_threadsNumber = threadsNumber;
      }
      
      // progress interface
      
      m_enableProgressInterface = enableProgressInterface;
      
      return true;
    }
    
    void Blender::initState()
    {
      m_enableProgressInterface = false;
      m_forceRaster1NoDataValue = false;
      m_forceRaster2NoDataValue = false;
      m_threadsNumber = 0;
      m_blendMethod = InvalidBlendMethod;
      m_blendFuncPtr = 0;
      m_raster1Ptr = 0;
      m_raster2Ptr = 0;
      m_r1IntersectionSegmentsPointsSize = 0;
      m_r2IntersectionSegmentsPointsSize = 0;
      m_geomTransformationPtr = 0;
      m_interpMethod1 = te::rst::NearestNeighbor;
      m_interpMethod2 = te::rst::NearestNeighbor;
      m_outputNoDataValue = 0;
    }
    
    void Blender::clear()
    {
      m_r1ValidDataDelimiterPtr.reset();
      m_r2ValidDataDelimiterPtr.reset();
      m_intersectionPtr.reset();
      m_r1IntersectionSegmentsPoints.clear();
      m_r2IntersectionSegmentsPoints.clear();
      if( m_geomTransformationPtr ) delete m_geomTransformationPtr;
      m_interp1Ptr.reset();
      m_interp2Ptr.reset();
      m_raster1Bands.clear();
      m_raster2Bands.clear();
      m_pixelOffsets1.clear();
      m_pixelScales1.clear();
      m_pixelOffsets2.clear();
      m_pixelScales2.clear();
      m_raster1NoDataValues.clear();
      m_raster2NoDataValues.clear();
      m_intersectionTileIndexers.clear();

      initState();
    }
    
    void Blender::setBlendFunctionPonter( const BlendMethod blendMethod )
    {
      switch( blendMethod )
      {
        case NoBlendMethod :
        {
          m_blendFuncPtr = &Blender::noBlendMethodImp;
          break;
        }
        case EuclideanDistanceMethod :
        {
          m_blendFuncPtr = &Blender::euclideanDistanceMethodImp;
          break;
        }  
        case SumMethod :
        {
          m_blendFuncPtr = &Blender::sumMethodImp;
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid blend method" );
          break;
        }
      }  
    }
    
    void Blender::noBlendMethodImp( const double& line, const double& col,
      double* const values )
    {
      // Finding the point over the second raster
      
      m_geomTransformationPtr->directMap( col, line, m_noBlendMethodImp_Point2Col,
        m_noBlendMethodImp_Point2Line );
      
      // Blending values

      for( m_noBlendMethodImp_BandIdx = 0 ; m_noBlendMethodImp_BandIdx <
        m_raster1Bands.size() ; ++m_noBlendMethodImp_BandIdx )
      {
        m_interp1Ptr->getValue( col, line, m_noBlendMethodImp_cValue, 
          m_raster1Bands[ m_noBlendMethodImp_BandIdx ] ); 
        m_noBlendMethodImp_Value = m_noBlendMethodImp_cValue.real();      
    
        if( m_noBlendMethodImp_Value == m_raster1NoDataValues[ m_noBlendMethodImp_BandIdx ] )
        {
          m_interp2Ptr->getValue( m_noBlendMethodImp_Point2Col, 
            m_noBlendMethodImp_Point2Line, m_noBlendMethodImp_cValue, 
            m_raster2Bands[ m_noBlendMethodImp_BandIdx ] );
          m_noBlendMethodImp_Value =  m_noBlendMethodImp_cValue.real();          
          
          if( m_noBlendMethodImp_Value == m_raster2NoDataValues[ m_noBlendMethodImp_BandIdx ] )
          {
            values[ m_noBlendMethodImp_BandIdx ] = m_outputNoDataValue;
          }
          else
          {
            
            m_noBlendMethodImp_Value *= m_pixelScales2[ m_noBlendMethodImp_BandIdx ];
            m_noBlendMethodImp_Value += m_pixelOffsets2[ m_noBlendMethodImp_BandIdx ];
            
            values[ m_noBlendMethodImp_BandIdx ] = m_noBlendMethodImp_Value;
          }
        }
        else
        {
          
          m_noBlendMethodImp_Value *= m_pixelScales1[ m_noBlendMethodImp_BandIdx ];
          m_noBlendMethodImp_Value += m_pixelOffsets1[ m_noBlendMethodImp_BandIdx ]; 
          
          values[ m_noBlendMethodImp_BandIdx ] = m_noBlendMethodImp_Value;            
        }      
      }
    }
    
    void Blender::euclideanDistanceMethodImp( const double& line, const double& col,
      double* const values )
    {
      TERP_DEBUG_TRUE_OR_THROW( m_intersectionPtr.get(), "Invalid intersection pointer" );
      TERP_DEBUG_TRUE_OR_THROW( m_r1IntersectionSegmentsPointsSize > 1, "Invalid intersection points" );
      TERP_DEBUG_TRUE_OR_THROW( m_r2IntersectionSegmentsPointsSize > 1, "Invalid intersection points" );
      
      // Checking if it is inside the intersection
      
      m_euclideanDistanceMethodImp_auxPoint.setX( col );
      m_euclideanDistanceMethodImp_auxPoint.setY( line );
      
      m_euclideanDistanceMethodImp_PointInsideIntersection = false;
      for( m_euclideanDistanceMethodImp_IntersectionTileIndexersIdx = 0;
        m_euclideanDistanceMethodImp_IntersectionTileIndexersIdx < m_intersectionTileIndexers.size() ;
        ++m_euclideanDistanceMethodImp_IntersectionTileIndexersIdx )
      {
        if( m_intersectionTileIndexers[ 
          m_euclideanDistanceMethodImp_IntersectionTileIndexersIdx ].within( 
          m_euclideanDistanceMethodImp_auxPoint ) )
        {
          m_euclideanDistanceMethodImp_PointInsideIntersection = true;
          break;
        }
      }
      
      // Blending if the point is inside the intersection
      
      if( m_euclideanDistanceMethodImp_PointInsideIntersection )
      {
        // Finding distances to both rasters valid area delimiters
              
        m_euclideanDistanceMethodImp_dist1 = std::numeric_limits<double>::max();
        for( m_euclideanDistanceMethodImp_vecIdx = 0 ; 
          m_euclideanDistanceMethodImp_vecIdx < m_r1IntersectionSegmentsPointsSize ; 
          ++m_euclideanDistanceMethodImp_vecIdx )
        {
          
          getPerpendicularDistance( 
            col,
            line,
            m_r1IntersectionSegmentsPoints[ m_euclideanDistanceMethodImp_vecIdx ].first.x,
            m_r1IntersectionSegmentsPoints[ m_euclideanDistanceMethodImp_vecIdx ].first.y, 
            m_r1IntersectionSegmentsPoints[ m_euclideanDistanceMethodImp_vecIdx ].second.x,
            m_r1IntersectionSegmentsPoints[ m_euclideanDistanceMethodImp_vecIdx ].second.y,           
            m_euclideanDistanceMethodImp_aux1,
            m_euclideanDistanceMethodImp_aux2,
            m_euclideanDistanceMethodImp_currDist );
            
          if( m_euclideanDistanceMethodImp_currDist < m_euclideanDistanceMethodImp_dist1 )
          {
            m_euclideanDistanceMethodImp_dist1 = m_euclideanDistanceMethodImp_currDist;
          }
        }     
        
        m_euclideanDistanceMethodImp_dist2 = std::numeric_limits<double>::max();
        for( m_euclideanDistanceMethodImp_vecIdx = 0 ; 
          m_euclideanDistanceMethodImp_vecIdx < m_r2IntersectionSegmentsPointsSize ; 
          ++m_euclideanDistanceMethodImp_vecIdx )
        {
          
          getPerpendicularDistance( 
            col,
            line,
            m_r2IntersectionSegmentsPoints[ m_euclideanDistanceMethodImp_vecIdx ].first.x,
            m_r2IntersectionSegmentsPoints[ m_euclideanDistanceMethodImp_vecIdx ].first.y, 
            m_r2IntersectionSegmentsPoints[ m_euclideanDistanceMethodImp_vecIdx ].second.x,
            m_r2IntersectionSegmentsPoints[ m_euclideanDistanceMethodImp_vecIdx ].second.y,           
            m_euclideanDistanceMethodImp_aux1,
            m_euclideanDistanceMethodImp_aux2,
            m_euclideanDistanceMethodImp_currDist );
            
          if( m_euclideanDistanceMethodImp_currDist < m_euclideanDistanceMethodImp_dist2 )
          {
            m_euclideanDistanceMethodImp_dist2 = m_euclideanDistanceMethodImp_currDist;
          }
        } 
        
        // Finding the point over the second raster
        
        m_geomTransformationPtr->directMap( col, line, m_euclideanDistanceMethodImp_Point2Col,
          m_euclideanDistanceMethodImp_Point2Line );      
        
        // Blending values

        for( m_euclideanDistanceMethodImp_BandIdx = 0 ; m_euclideanDistanceMethodImp_BandIdx <
          m_raster1Bands.size() ; ++m_euclideanDistanceMethodImp_BandIdx )
        {
          m_interp1Ptr->getValue( col, line, m_euclideanDistanceMethodImp_cValue1, 
            m_raster1Bands[ m_euclideanDistanceMethodImp_BandIdx ] ); 
          m_interp2Ptr->getValue( m_euclideanDistanceMethodImp_Point2Col, 
            m_euclideanDistanceMethodImp_Point2Line, m_euclideanDistanceMethodImp_cValue2, 
            m_raster2Bands[ m_euclideanDistanceMethodImp_BandIdx ] );
      
          if( m_euclideanDistanceMethodImp_cValue1.real() == m_raster1NoDataValues[ m_euclideanDistanceMethodImp_BandIdx ] )
          {
            if( m_euclideanDistanceMethodImp_cValue2.real() == m_raster2NoDataValues[ m_euclideanDistanceMethodImp_BandIdx ] )
            {
              values[ m_euclideanDistanceMethodImp_BandIdx ] = m_outputNoDataValue;
            }
            else
            {
              values[ m_euclideanDistanceMethodImp_BandIdx ] = 
                ( 
                  m_euclideanDistanceMethodImp_cValue2.real() 
                  *
                  m_pixelScales2[ m_euclideanDistanceMethodImp_BandIdx ]
                )
                +
                m_pixelOffsets2[ m_euclideanDistanceMethodImp_BandIdx ];
            }
          }
          else
          {
            if( m_euclideanDistanceMethodImp_cValue2.real() == m_raster2NoDataValues[ m_euclideanDistanceMethodImp_BandIdx ] )
            {
              values[ m_euclideanDistanceMethodImp_BandIdx ] =
                ( 
                  m_euclideanDistanceMethodImp_cValue1.real()  
                  *
                  m_pixelScales1[ m_euclideanDistanceMethodImp_BandIdx ]
                  
                )
                +
                m_pixelOffsets1[ m_euclideanDistanceMethodImp_BandIdx ];
            }
            else
            {
              if( m_euclideanDistanceMethodImp_dist2 == 0.0 )
              {
                values[ m_euclideanDistanceMethodImp_BandIdx ] =
                  ( 
                    m_euclideanDistanceMethodImp_cValue1.real()  
                    *
                    m_pixelScales1[ m_euclideanDistanceMethodImp_BandIdx ]
                  )
                  +
                  m_pixelOffsets1[ m_euclideanDistanceMethodImp_BandIdx ]; 
              }
              else if( m_euclideanDistanceMethodImp_dist1 == 0.0 )
              {
                values[ m_euclideanDistanceMethodImp_BandIdx ] =  
                  ( 
                    m_euclideanDistanceMethodImp_cValue2.real()
                    *
                    m_pixelScales2[ m_euclideanDistanceMethodImp_BandIdx ]
                    
                  )
                  +
                  m_pixelOffsets2[ m_euclideanDistanceMethodImp_BandIdx ]; 
              }            
              else
              {
                values[ m_euclideanDistanceMethodImp_BandIdx ] =
                  (
                    (
                      (
                        (
                          m_euclideanDistanceMethodImp_cValue1.real()  
                          *
                          m_pixelScales1[ m_euclideanDistanceMethodImp_BandIdx ] 
                          
                        )
                        +
                        m_pixelOffsets1[ m_euclideanDistanceMethodImp_BandIdx ]
                      )
                      *
                      m_euclideanDistanceMethodImp_dist1
                    )
                    +
                    (
                      (
                        (
                          m_euclideanDistanceMethodImp_cValue2.real()
                          * 
                          m_pixelScales2[ m_euclideanDistanceMethodImp_BandIdx ] 
                        )
                        +
                        m_pixelOffsets2[ m_euclideanDistanceMethodImp_BandIdx ]
                      )
                      *
                      m_euclideanDistanceMethodImp_dist2
                    )
                  )
                  /
                  ( 
                    m_euclideanDistanceMethodImp_dist1 
                    +
                    m_euclideanDistanceMethodImp_dist2
                  );
              }
            }          
          }      
        }
      }
      else
      {
        noBlendMethodImp( line, col, values );
      }
    }    

    void Blender::sumMethodImp( const double& line, const double& col,
      double* const values )
    {
      TERP_DEBUG_TRUE_OR_THROW( m_intersectionPtr.get(), "Invalid intersection pointer" );
      TERP_DEBUG_TRUE_OR_THROW( m_r1IntersectionSegmentsPointsSize > 1, "Invalid intersection points" );
      TERP_DEBUG_TRUE_OR_THROW( m_r2IntersectionSegmentsPointsSize > 1, "Invalid intersection points" );
      
      // Checking if it is inside the intersection
      
      m_sumMethodImp_auxPoint.setX( col );
      m_sumMethodImp_auxPoint.setY( line );
      
      m_sumMethodImp_PointInsideIntersection = false;
      for( m_sumMethodImp_IntersectionTileIndexersIdx = 0;
        m_sumMethodImp_IntersectionTileIndexersIdx < m_intersectionTileIndexers.size() ;
        ++m_sumMethodImp_IntersectionTileIndexersIdx )
      {
        if( m_intersectionTileIndexers[ 
          m_sumMethodImp_IntersectionTileIndexersIdx ].within( 
          m_sumMethodImp_auxPoint ) )
        {
          m_sumMethodImp_PointInsideIntersection = true;
          break;
        }
      }
      
      // Blending if the point is inside the intersection      
      
      if( m_sumMethodImp_PointInsideIntersection )
      {
        // Finding the point over the second raster
        
        m_geomTransformationPtr->directMap( col, line, m_sumMethodImp_Point2Col,
          m_sumMethodImp_Point2Line );      
        
        // Blending values

        for( m_sumMethodImp_BandIdx = 0 ; m_sumMethodImp_BandIdx <
          m_raster1Bands.size() ; ++m_sumMethodImp_BandIdx )
        {
          m_interp1Ptr->getValue( col, line, m_sumMethodImp_cValue1, 
            m_raster1Bands[ m_sumMethodImp_BandIdx ] ); 
          m_interp2Ptr->getValue( m_sumMethodImp_Point2Col, 
            m_sumMethodImp_Point2Line, m_sumMethodImp_cValue2, 
            m_raster2Bands[ m_sumMethodImp_BandIdx ] );
      
          if( m_sumMethodImp_cValue1.real() == m_raster1NoDataValues[ m_sumMethodImp_BandIdx ] )
          {
            if( m_sumMethodImp_cValue2.real() == m_raster2NoDataValues[ m_sumMethodImp_BandIdx ] )
            {
              values[ m_sumMethodImp_BandIdx ] = m_outputNoDataValue;
            }
            else
            {
              values[ m_sumMethodImp_BandIdx ] = 
                ( 
                  m_sumMethodImp_cValue2.real()
                  *
                  m_pixelScales2[ m_sumMethodImp_BandIdx ]
                )
                +
                m_pixelOffsets2[ m_sumMethodImp_BandIdx ]; 
            }
          }
          else
          {
            if( m_sumMethodImp_cValue2.real() == m_raster2NoDataValues[ m_sumMethodImp_BandIdx ] )
            {
              values[ m_sumMethodImp_BandIdx ] =  
                ( 
                  m_sumMethodImp_cValue1.real()
                  
                  *
                  m_pixelScales1[ m_sumMethodImp_BandIdx ]
                )
                +
                m_pixelOffsets1[ m_sumMethodImp_BandIdx ]; 
            }
            else
            {
              values[ m_sumMethodImp_BandIdx ] =
                (
                  (
                    m_sumMethodImp_cValue1.real()  
                    * 
                    m_pixelScales1[ m_sumMethodImp_BandIdx ] 
                  )
                  +
                  m_pixelOffsets1[ m_sumMethodImp_BandIdx ]
                )
                +
                (
                  (
                    m_sumMethodImp_cValue2.real()
                    * 
                    m_pixelScales2[ m_sumMethodImp_BandIdx ]
                  )
                  +
                  m_pixelOffsets2[ m_sumMethodImp_BandIdx ]
                );
            }          
          }      
        }
      }
      else
      {
        noBlendMethodImp( line, col, values );
      }
    }    
    
        
    bool Blender::blendIntoRaster1()
    {
      TERP_TRUE_OR_RETURN_FALSE( m_raster1Ptr->getAccessPolicy() & 
        te::common::WAccess, "Invalid output raster access policy" );      
      
      // Locating raster2 over the raster1
      
      unsigned int firstOutputRasterCol = 0;
      unsigned int lastOutputRasterRow = 0;
      unsigned int lastOutputRasterCol = 0;
      unsigned int firstOutputRasterRow = 0;
      
      {
        const double raster2LastRowIdx = 
          (double)( m_raster2Ptr->getNumberOfRows() - 1 );
        const double raster2LastColIdx =
          (double)( m_raster2Ptr->getNumberOfColumns() - 1 );
        double raster2LLColOverRaster1 = 0;
        double raster2LLRowOverRaster1 = 0;
        double raster2LRColOverRaster1 = 0;
        double raster2LRRowOverRaster1 = 0;
        double raster2URColOverRaster1 = 0;
        double raster2URRowOverRaster1 = 0;
        double raster2ULColOverRaster1 = 0;
        double raster2ULRowOverRaster1 = 0;          

        m_geomTransformationPtr->inverseMap( 
          0.0,
          raster2LastRowIdx,
          raster2LLColOverRaster1,
          raster2LLRowOverRaster1);
        m_geomTransformationPtr->inverseMap( 
          raster2LastColIdx,
          raster2LastRowIdx,
          raster2LRColOverRaster1,
          raster2LRRowOverRaster1);                
        m_geomTransformationPtr->inverseMap( 
          raster2LastColIdx,
          0.0,
          raster2URColOverRaster1,
          raster2URRowOverRaster1); 
        m_geomTransformationPtr->inverseMap( 
          0.0,
          0.0,
          raster2ULColOverRaster1,
          raster2ULRowOverRaster1);  
        
        firstOutputRasterCol = (unsigned int)
          std::max( 0.0,
            std::min( (double)( m_raster1Ptr->getNumberOfColumns() - 1 ),              
              std::floor( 
                std::min( raster2LLColOverRaster1, 
                  std::min( raster2LRColOverRaster1, 
                    std::min( raster2URColOverRaster1, 
                                raster2ULColOverRaster1
                    ) 
                  ) 
                )
              )
            )
          );
          
        lastOutputRasterRow = (unsigned int)
          std::max( 0.0,
            std::min( (double)( m_raster1Ptr->getNumberOfRows() - 1 ),                
              std::ceil( 
                std::max( raster2LLRowOverRaster1, 
                  std::max( raster2LRRowOverRaster1, 
                    std::max( raster2URRowOverRaster1, 
                                raster2ULRowOverRaster1
                    ) 
                  ) 
                ) 
              )
            )
          );
          
        lastOutputRasterCol = (unsigned int)
          std::max( 0.0,
            std::min( (double)( m_raster1Ptr->getNumberOfColumns() - 1 ),                
              std::ceil( 
                std::max( raster2LLColOverRaster1, 
                  std::max( raster2LRColOverRaster1, 
                    std::max( raster2URColOverRaster1, 
                                raster2ULColOverRaster1
                    ) 
                  ) 
                )
              )
            )
          );
          
        firstOutputRasterRow = (unsigned int)
          std::max( 0.0,
            std::min( (double)( m_raster1Ptr->getNumberOfRows() - 1 ),               
              std::floor( 
                std::min( raster2LLRowOverRaster1, 
                  std::min( raster2LRRowOverRaster1, 
                    std::min( raster2URRowOverRaster1, 
                                raster2ULRowOverRaster1
                    ) 
                  ) 
                ) 
              )
            )
          );
          
        assert( firstOutputRasterCol >= 0 );
        assert( firstOutputRasterCol <= 
          ( m_raster1Ptr->getNumberOfColumns() - 1 ) );    
        assert( lastOutputRasterRow >= 0 );
        assert( lastOutputRasterRow <= 
          ( m_raster1Ptr->getNumberOfRows() - 1 ) );                 
        assert( lastOutputRasterCol >= 0 );
        assert( lastOutputRasterCol <= 
          ( m_raster1Ptr->getNumberOfColumns() - 1 ) ); 
        assert( firstOutputRasterRow >= 0 );
        assert( firstOutputRasterRow <= 
          ( m_raster1Ptr->getNumberOfRows() - 1 ) );
      }  
      
      // Discovering the raster 1 blocks we need to process
      
      std::vector< RasterBlockInfo > raster1BlocksInfos;
      bool allRaster1BandsWithSameBlocking = true;
      
      {
        const te::rst::Band& firstBand = *( m_raster1Ptr->getBand(
           m_raster1Bands[ 0 ] ) );        
        
        for( unsigned int raster1BandsIdx = 0 ; raster1BandsIdx < 
          m_raster1Bands.size() ; ++raster1BandsIdx )
        {
          const te::rst::Band& band = *( m_raster1Ptr->getBand(
            m_raster1Bands[ raster1BandsIdx ] ) ); 
          
          if(
              ( band.getProperty()->m_blkh != firstBand.getProperty()->m_blkh )
              ||
              ( band.getProperty()->m_blkw != firstBand.getProperty()->m_blkw )
              ||
              ( band.getProperty()->m_nblocksx != firstBand.getProperty()->m_nblocksx )
              ||
              ( band.getProperty()->m_nblocksy != firstBand.getProperty()->m_nblocksy )
            )
          {
            allRaster1BandsWithSameBlocking = false;
            break;
          }
        }
          
        unsigned int firstBlockX = firstOutputRasterCol / 
          firstBand.getProperty()->m_blkw;
        unsigned int lastBlockX = lastOutputRasterCol / 
          firstBand.getProperty()->m_blkw;
        unsigned int firstBlockY = firstOutputRasterRow / 
          firstBand.getProperty()->m_blkh;
        unsigned int lastBlockY = lastOutputRasterRow / 
          firstBand.getProperty()->m_blkh;                        
          
        for( unsigned int blkY = firstBlockY ; blkY <= lastBlockY ; ++blkY )
        {
          for( unsigned int blkX = firstBlockX ; blkX <= lastBlockX ; ++blkX )
          {
            raster1BlocksInfos.push_back( RasterBlockInfo() );
            
            RasterBlockInfo& rBInfo = raster1BlocksInfos.back();
            
            rBInfo.m_wasProcessed = false;            
            rBInfo.m_blkX = blkX;
            rBInfo.m_blkY = blkY;
            rBInfo.m_blkTotalPixelsNumber = firstBand.getProperty()->m_blkh *
              firstBand.getProperty()->m_blkw;
            
            rBInfo.m_firstRasterRow2Process = blkY * firstBand.getProperty()->m_blkh;
            rBInfo.m_firstRasterRow2Process = std::max( firstOutputRasterRow,
              rBInfo.m_firstRasterRow2Process );
            rBInfo.m_firstRasterRow2Process = std::min( lastOutputRasterRow,
              rBInfo.m_firstRasterRow2Process );
            
            rBInfo.m_rasterRows2ProcessBound = ( blkY + 1 ) * firstBand.getProperty()->m_blkh;
            rBInfo.m_rasterRows2ProcessBound = std::max( firstOutputRasterRow,
              rBInfo.m_rasterRows2ProcessBound );
            rBInfo.m_rasterRows2ProcessBound = std::min( lastOutputRasterRow,
              rBInfo.m_rasterRows2ProcessBound );              
            
            rBInfo.m_firstRasterCol2Process = blkX * firstBand.getProperty()->m_blkw;
            rBInfo.m_firstRasterCol2Process = std::max( firstOutputRasterCol,
              rBInfo.m_firstRasterCol2Process );
            rBInfo.m_firstRasterCol2Process = std::min( lastOutputRasterCol,
              rBInfo.m_firstRasterCol2Process );              
            
            rBInfo.m_rasterCols2ProcessBound = ( blkX + 1 ) * firstBand.getProperty()->m_blkw;          
            rBInfo.m_rasterCols2ProcessBound = std::max( firstOutputRasterCol,
              rBInfo.m_rasterCols2ProcessBound );
            rBInfo.m_rasterCols2ProcessBound = std::min( lastOutputRasterCol,
              rBInfo.m_rasterCols2ProcessBound );                
          }
        }
      }          
      
      // blending
      
      bool returnValue = true;
      
      {
        // Guessing memory resources
        
        const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
        const double usedVMem = (double)te::common::GetUsedVirtualMemory();
        const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() );
        const double maxVMem2Use = 0.75 * MIN( totalPhysMem, ( totalVMem - usedVMem ) );
        
        // Creating thread exec params
        
        bool abortValue = false;
        boost::mutex mutex;
        te::rst::RasterSynchronizer sync1( *m_raster1Ptr, te::common::RWAccess );
        te::rst::RasterSynchronizer sync2( *((te::rst::Raster*)m_raster2Ptr), te::common::RAccess );
        boost::mutex blockProcessedSignalMutex;
        boost::condition_variable blockProcessedSignal;
        unsigned int runningThreadsCounter = 0;
        
        BlendIntoRaster1ThreadParams auxThreadParams;
        auxThreadParams.m_returnValuePtr = &returnValue;
        auxThreadParams.m_abortValuePtr = &abortValue;
        auxThreadParams.m_sync1Ptr = &sync1;
        auxThreadParams.m_sync2Ptr = &sync2;
        auxThreadParams.m_raster1BlocksInfosPtr = &raster1BlocksInfos;
        auxThreadParams.m_mutexPtr = &mutex;
        auxThreadParams.m_blockProcessedSignalMutexPtr = &blockProcessedSignalMutex;
        auxThreadParams.m_blockProcessedSignalPtr = &blockProcessedSignal;
        auxThreadParams.m_runningThreadsCounterPtr = &runningThreadsCounter;
        auxThreadParams.m_raster1Bands = m_raster1Bands;
        auxThreadParams.m_raster2Bands = m_raster2Bands;
        auxThreadParams.m_blendMethod = m_blendMethod;
        auxThreadParams.m_interpMethod2 = m_interpMethod2;
        auxThreadParams.m_noDataValue = m_outputNoDataValue;
        auxThreadParams.m_forceRaster1NoDataValue = m_forceRaster1NoDataValue;
        auxThreadParams.m_forceRaster2NoDataValue = m_forceRaster2NoDataValue;
        auxThreadParams.m_pixelOffsets1 = m_pixelOffsets1;
        auxThreadParams.m_pixelScales1 = m_pixelScales1;
        auxThreadParams.m_pixelOffsets2 = m_pixelOffsets2;
        auxThreadParams.m_pixelScales2 = m_pixelScales2;          
        
        std::vector< BlendIntoRaster1ThreadParams > allThreadsParams( m_threadsNumber,
          auxThreadParams );
        
        // creating threads
        
        if( ( m_threadsNumber == 1 ) || (!allRaster1BandsWithSameBlocking) )
        {
          runningThreadsCounter = 1;
          if( m_r1ValidDataDelimiterPtr.get() )
          {
            allThreadsParams[ 0 ].m_r1ValidDataDelimiterPtr.reset( 
              (te::gm::MultiPolygon*)m_r1ValidDataDelimiterPtr->clone() );
          }
          if( m_r2ValidDataDelimiterPtr.get() )
          {
            allThreadsParams[ 0 ].m_r2ValidDataDelimiterPtr.reset( 
              (te::gm::MultiPolygon*)m_r2ValidDataDelimiterPtr->clone() );
          }
          allThreadsParams[ 0 ].m_geomTransformationPtr.reset( 
            m_geomTransformationPtr->clone() );
          allThreadsParams[ 0 ].m_maxRasterCachedBlocks = std::max( 1u, 
            ((unsigned int)maxVMem2Use)
            / ((unsigned int)m_raster2Ptr->getBand( m_raster1Bands[ 0 ] )->getBlockSize() ) );
          allThreadsParams[ 0 ].m_useProgress = m_enableProgressInterface;
          
          blendIntoRaster1Thread( &( allThreadsParams[ 0 ] ) );
        }
        else
        {
          boost::thread_group threads;
          runningThreadsCounter = m_threadsNumber;
          
          for( unsigned int threadIdx = 0 ; threadIdx < m_threadsNumber ;
            ++threadIdx )
          {
            if( m_r1ValidDataDelimiterPtr.get() )
            {
              allThreadsParams[ threadIdx ].m_r1ValidDataDelimiterPtr.reset( 
                (te::gm::MultiPolygon*)m_r1ValidDataDelimiterPtr->clone() );
            }
            if( m_r2ValidDataDelimiterPtr.get() )
            {
              allThreadsParams[ threadIdx ].m_r2ValidDataDelimiterPtr.reset( 
                (te::gm::MultiPolygon*)m_r2ValidDataDelimiterPtr->clone() );
            }
            allThreadsParams[ threadIdx ].m_geomTransformationPtr.reset( 
              m_geomTransformationPtr->clone() );
            
            allThreadsParams[ 0 ].m_maxRasterCachedBlocks = std::max( 1u,
              ((unsigned int)maxVMem2Use)
              / 
              (
                ((unsigned int)m_raster2Ptr->getBand( m_raster1Bands[ 0 ] )->getBlockSize() )
                *
                m_threadsNumber
              ) );
            
            allThreadsParams[ threadIdx ].m_useProgress = false;
            
            threads.add_thread( new boost::thread( blendIntoRaster1Thread, 
               &( allThreadsParams[ threadIdx ] ) ) );
          };    
          
          // progress stuff
          
          std::auto_ptr< te::common::TaskProgress > progressPtr;
          if( m_enableProgressInterface )
          {
            progressPtr.reset( new te::common::TaskProgress );
            progressPtr->setTotalSteps( raster1BlocksInfos.size() );
            progressPtr->setMessage( "Blending" );
          
            while( (!abortValue) && (runningThreadsCounter > 0 ) )
            {
              if( progressPtr->isActive() )
              {
                boost::unique_lock<boost::mutex> lock( blockProcessedSignalMutex );
                blockProcessedSignal.timed_wait( lock, 
                  boost::posix_time::seconds( 1 ) );
                  
                int processedBlocksNmb = 0;
                for( unsigned int raster1BlocksInfosIdx = 0 ; raster1BlocksInfosIdx <
                  raster1BlocksInfos.size() ; ++raster1BlocksInfosIdx )
                {
                  if( raster1BlocksInfos[ raster1BlocksInfosIdx ].m_wasProcessed )
                  {
                    ++processedBlocksNmb;
                  }
                }
                
                if( processedBlocksNmb != progressPtr->getCurrentStep() )
                {
                  progressPtr->pulse();
                }
              }
              else
              {
                mutex.lock();
                abortValue = true;
                mutex.unlock();
              }
            }
          }
          
          // Joining threads
          
          threads.join_all();
        }
      }
      
      return returnValue;
    }
    
    void Blender::blendIntoRaster1Thread( BlendIntoRaster1ThreadParams* paramsPtr )
    {
      // Instantiating the local rasters instance
      
      te::rst::SynchronizedRaster raster1( paramsPtr->m_raster1Bands.size(), 
        *( paramsPtr->m_sync1Ptr ) );
      te::rst::SynchronizedRaster raster2( paramsPtr->m_maxRasterCachedBlocks,
        *( paramsPtr->m_sync2Ptr ) );
      
      // Guessing the output raster channels ranges
      
      const std::vector< unsigned int >& raster1Bands = 
        paramsPtr->m_raster1Bands;
      const unsigned int raster1BandsSize = raster1Bands.size();      
      
      std::vector< double > raster1BandsRangeMin( raster1BandsSize, 0 );
      std::vector< double > raster1BandsRangeMax( raster1BandsSize, 0 ); 
      
      {
        for( unsigned int raster1BandsIdx = 0 ;  raster1BandsIdx < 
          raster1BandsSize ; ++raster1BandsIdx )
        {
          unsigned int bandIdx = raster1Bands[ raster1BandsIdx ];
          
          te::rst::GetDataTypeRanges( raster1.getBand( bandIdx )->getProperty()->m_type,
            raster1BandsRangeMin[ raster1BandsIdx ],
            raster1BandsRangeMax[ raster1BandsIdx ]);             
        }
      }      
      
      // instantiating the thread local blender instance
      
      paramsPtr->m_mutexPtr->lock();
      const unsigned int raster1BlocksInfosSize = paramsPtr->m_raster1BlocksInfosPtr->size();
      paramsPtr->m_mutexPtr->unlock();
      
      Blender blender;
      
      if( ! blender.initialize( 
        raster1,
        paramsPtr->m_raster1Bands,
        raster2,
        paramsPtr->m_raster2Bands,
        paramsPtr->m_blendMethod,
        te::rst::NearestNeighbor,
        paramsPtr->m_interpMethod2,
        paramsPtr->m_noDataValue,
        paramsPtr->m_forceRaster1NoDataValue,
        paramsPtr->m_forceRaster2NoDataValue,
        paramsPtr->m_pixelOffsets1,
        paramsPtr->m_pixelScales1,
        paramsPtr->m_pixelOffsets2,
        paramsPtr->m_pixelScales2,
        paramsPtr->m_r1ValidDataDelimiterPtr.get(),
        paramsPtr->m_r2ValidDataDelimiterPtr.get(),
        *( paramsPtr->m_geomTransformationPtr ),
        1,
        false ) )
      {
        paramsPtr->m_mutexPtr->lock();
        *(paramsPtr->m_abortValuePtr) = true;
        *(paramsPtr->m_returnValuePtr) = false;
        --( *(paramsPtr->m_runningThreadsCounterPtr) );
        paramsPtr->m_mutexPtr->unlock();
        return;
      }      
      
      // progress stuff
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      
      if( paramsPtr->m_useProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( raster1BlocksInfosSize );
        progressPtr->setMessage( "Blending" );
      }       
      
      // loocking for the next raster block to blend
      
      boost::scoped_array< double > blendedValuesHandler( new double[ raster1BandsSize ] );
      double* blendedValuesHandlerPtr = blendedValuesHandler.get();
      
      const double noDataValue = paramsPtr->m_noDataValue;
      
      for( unsigned int raster1BlocksInfosIdx = 0 ; raster1BlocksInfosIdx <
        raster1BlocksInfosSize ; ++raster1BlocksInfosIdx )
      {
        paramsPtr->m_mutexPtr->lock();
        
        if( paramsPtr->m_raster1BlocksInfosPtr->operator[]( raster1BlocksInfosIdx ).m_wasProcessed )
        {
          paramsPtr->m_mutexPtr->unlock();
        }
        else
        {
          RasterBlockInfo& rBInfo = paramsPtr->m_raster1BlocksInfosPtr->operator[]( 
            raster1BlocksInfosIdx );
          rBInfo.m_wasProcessed = true;
          paramsPtr->m_mutexPtr->unlock();
          
          //blending block data          
          
          unsigned int raster1Row = 0;
          unsigned int raster1Col = 0;
          unsigned int raster1BandsIdx = 0;
            
          for( raster1Row = rBInfo.m_firstRasterRow2Process ; raster1Row < rBInfo.m_rasterRows2ProcessBound ;
            ++raster1Row )
          {
            for( raster1Col = rBInfo.m_firstRasterCol2Process ; raster1Col < rBInfo.m_rasterCols2ProcessBound ;
              ++raster1Col )
            {
              blender.getBlendedValues( (double)raster1Row, (double)raster1Col,
                blendedValuesHandlerPtr );  
              
              for( raster1BandsIdx = 0 ; raster1BandsIdx < raster1BandsSize ; ++raster1BandsIdx )
              {          
                double& blendedValue = blendedValuesHandlerPtr[ raster1BandsIdx ];
                
                if( blendedValue != noDataValue )
                {
                  blendedValue = std::max( blendedValue , 
                    raster1BandsRangeMin[ raster1BandsIdx ] );
                  blendedValue = std::min( blendedValue , 
                    raster1BandsRangeMax[ raster1BandsIdx ] );

                  raster1.setValue( raster1Col, raster1Row, blendedValue,
                    raster1Bands[ raster1BandsIdx ] );
                }
              }
            }
          }
          
          // progress stuff
          
          if( paramsPtr->m_useProgress )
          {
            if( progressPtr->isActive() )
            {
              progressPtr->pulse();
            }
            else
            {
              paramsPtr->m_mutexPtr->lock();
              *(paramsPtr->m_abortValuePtr) = true;
              *(paramsPtr->m_returnValuePtr) = false;
              --( *(paramsPtr->m_runningThreadsCounterPtr) );
              paramsPtr->m_mutexPtr->unlock();
              return;
            }
          }
          else
          {
            // notifying the main thread with the block processed signal
            
            boost::lock_guard<boost::mutex> blockProcessedSignalLockGuard( 
              *( paramsPtr->m_blockProcessedSignalMutexPtr) );              
            
            paramsPtr->m_blockProcessedSignalPtr->notify_one();
          }
        }
        
        // do we must continue processing ?
        
        if( *(paramsPtr->m_abortValuePtr) )
        {
          break;
        }
      }
      
      paramsPtr->m_mutexPtr->lock();
      --( *(paramsPtr->m_runningThreadsCounterPtr) );
      paramsPtr->m_mutexPtr->unlock();
    }
    
    bool Blender::getSegments( te::gm::Geometry const * const geometryPtr, 
      std::vector< std::pair< te::gm::Coord2D, te::gm::Coord2D > >& segments ) const
    {
      if( dynamic_cast< te::gm::LineString const * >( geometryPtr ) )
      {
        te::gm::LineString const* castGeomPtr = 
          dynamic_cast< te::gm::LineString const * >( geometryPtr );
        
        std::size_t nPoints = castGeomPtr->size();
        te::gm::Coord2D const* coodsPtr = castGeomPtr->getCoordinates();
        
        for( std::size_t pIdx = 1 ; pIdx < nPoints ; ++pIdx )
        {
          segments.push_back( std::pair< te::gm::Coord2D, te::gm::Coord2D >(
            coodsPtr[ pIdx - 1 ], coodsPtr[ pIdx ] ) );
        }
      } 
      else if( dynamic_cast< te::gm::CurvePolygon const * >( geometryPtr ) )
      {
        te::gm::CurvePolygon const * castGeomPtr = 
          dynamic_cast< te::gm::CurvePolygon const * >( geometryPtr );
        
        std::size_t numGeoms = castGeomPtr->getNumRings();
        
        for( std::size_t gIdx = 0 ; gIdx < numGeoms ; ++gIdx )
        {
          if( ! ( getSegments( castGeomPtr->getRingN( gIdx ), segments ) ) )
          {
            return false;
          }
        }
      }              
      else if( dynamic_cast< te::gm::GeometryCollection const * >( geometryPtr ) )
      {
        te::gm::GeometryCollection const * castGeomPtr = 
          dynamic_cast< te::gm::GeometryCollection const * >( geometryPtr );
        
        std::size_t numGeoms = castGeomPtr->getNumGeometries();
        
        for( std::size_t gIdx = 0 ; gIdx < numGeoms ; ++gIdx )
        {
          if( ! ( getSegments( castGeomPtr->getGeometryN( gIdx ), segments ) ) )
          {
            return false;
          }
        }
      }     
      
      return true;
    }
    
    bool Blender::getTileIndexers( te::gm::Geometry const * const geometryPtr, 
      boost::ptr_vector< te::rst::TileIndexer >& tileIndexers ) const
    {
      if( dynamic_cast< te::gm::Polygon const * >( geometryPtr ) )
      {
        te::gm::Polygon const * castGeomPtr = 
          dynamic_cast< te::gm::Polygon const * >( geometryPtr );
          
        tileIndexers.push_back( new te::rst::TileIndexer( *castGeomPtr, 1.0 ) );
      }              
      else if( dynamic_cast< te::gm::GeometryCollection const * >( geometryPtr ) )
      {
        te::gm::GeometryCollection const * castGeomPtr = 
          dynamic_cast< te::gm::GeometryCollection const * >( geometryPtr );
        
        std::size_t numGeoms = castGeomPtr->getNumGeometries();
        
        for( std::size_t gIdx = 0 ; gIdx < numGeoms ; ++gIdx )
        {
          if( ! ( getTileIndexers( castGeomPtr->getGeometryN( gIdx ), tileIndexers ) ) )
          {
            return false;
          }
        }
      }     
      
      return true;
    }    
    
  } // end namespace rp
}   // end namespace te    

