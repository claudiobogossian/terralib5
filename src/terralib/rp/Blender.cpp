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
#include "../geometry/Envelope.h"
#include "../geometry/Enums.h"
#include "../raster/Raster.h"
#include "../raster/Grid.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"

#include <complex>
#include <limits>
#include <algorithm>

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
    Blender::Blender()
    {
      initState();
    };
    
    Blender::~Blender()
    {
      clear();
    }    
    
    bool Blender::initialize( const te::rst::Raster& raster1, 
      const std::vector< unsigned int >& raster1Bands, 
      const te::rst::Raster& raster2, 
      const std::vector< unsigned int >& raster2Bands,
      const BlendMethod& blendMethod, 
      const te::rst::Interpolator::Method& interpMethod1,
      const te::rst::Interpolator::Method& interpMethod2,
      const double& noDataValue,
      const bool forceInputNoDataValue,
      const std::vector< double >& pixelOffsets1,
      const std::vector< double >& pixelScales1,
      const std::vector< double >& pixelOffsets2,
      const std::vector< double >& pixelScales2,
      te::gm::Polygon const * const r1ValidDataDelimiterPtr,
      te::gm::Polygon const * const r2ValidDataDelimiterPtr,
      const te::gm::GeometricTransformation& geomTransformation )
    {
      TERP_TRUE_OR_RETURN_FALSE( 
        raster1.getAccessPolicy() & te::common::RAccess, 
        "Invalid raster 1" );        
      TERP_TRUE_OR_RETURN_FALSE( 
        raster2.getAccessPolicy() & te::common::RAccess, 
        "Invalid raster 2" ); 
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
        ( r1ValidDataDelimiterPtr->getNPoints() > 1 ) : true ),
        "Invalid polygon 1" )
      TERP_TRUE_OR_RETURN_FALSE( ( r2ValidDataDelimiterPtr ?
        ( r2ValidDataDelimiterPtr->getNPoints() > 1 ) : true ),
        "Invalid polygon 2" )
      TERP_TRUE_OR_RETURN_FALSE( geomTransformation.isValid(),
        "Invalid transformation" );
        
      initState();
      
      // defining the input rasters
      
      m_raster1Ptr = &raster1;
      m_raster2Ptr = &raster2;
      
      // Generating the valid data area points
      
      std::auto_ptr< te::gm::Polygon > indexedDelimiter1Ptr; // indexed under raster 1 lines/cols
      
      if( r1ValidDataDelimiterPtr )
      {
        const std::size_t nRings = r1ValidDataDelimiterPtr->getNumRings();
        const te::rst::Grid& grid = (*raster1.getGrid());
        
        indexedDelimiter1Ptr.reset( new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 ) );
        
        for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
        {
          te::gm::LinearRing const* inRingPtr = dynamic_cast< te::gm::LinearRing const* >( 
            r1ValidDataDelimiterPtr->getRingN( ringIdx ) );
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
          
          indexedDelimiter1Ptr->add( outRingPtr );
        }
      }
      else
      {
        indexedDelimiter1Ptr.reset( new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 ) );        
        
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
           
        indexedDelimiter1Ptr->add( outRingPtr );
      }
      
      std::auto_ptr< te::gm::Polygon > indexedDelimiter2Ptr; // indexed under raster 1 lines/cols
      
      if( r2ValidDataDelimiterPtr )
      {
        const std::size_t nRings = r2ValidDataDelimiterPtr->getNumRings();
        const te::rst::Grid& grid = (*raster2.getGrid());
        
        indexedDelimiter2Ptr.reset( new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 ) );
        
        for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
        {
          te::gm::LinearRing const* inRingPtr = dynamic_cast< te::gm::LinearRing const* >( 
            r2ValidDataDelimiterPtr->getRingN( ringIdx ) );
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
          
          indexedDelimiter1Ptr->add( outRingPtr );
        }        
      }
      else
      {
        indexedDelimiter2Ptr.reset( new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 ) );        
        
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
        
        indexedDelimiter2Ptr->add( outRingPtr );
      }
      
      {
        std::auto_ptr< te::gm::Geometry > segs1IndexedPtr;
        segs1IndexedPtr.reset( indexedDelimiter1Ptr->intersection( indexedDelimiter2Ptr->getRingN( 0 ) ) );
        
        if( segs1IndexedPtr.get() != 0 ) 
        {
          if( segs1IndexedPtr->getGeomTypeId() == te::gm::MultiLineStringType )
          {
            te::gm::MultiLineString const* segsIndexedNPtr = dynamic_cast< te::gm::MultiLineString const * >(
              segs1IndexedPtr.get() );
            assert( segsIndexedNPtr );
            
            std::size_t numGeoms = segsIndexedNPtr->getNumGeometries();
            
            for( std::size_t gIdx = 0 ; gIdx < numGeoms ; ++gIdx )
            {
              te::gm::LineString const* segIndexedNPtr = dynamic_cast< te::gm::LineString const * >(
                segsIndexedNPtr->getGeometryN( gIdx ) );
              assert( segIndexedNPtr );
              
              std::size_t nPoints = segIndexedNPtr->size();
              te::gm::Coord2D const* coodsPtr = segIndexedNPtr->getCoordinates();
              
              for( std::size_t pIdx = 1 ; pIdx < nPoints ; ++pIdx )
              {
                m_r2IntersectionSegmentsPoints.push_back( std::pair< te::gm::Coord2D, te::gm::Coord2D >(
                  coodsPtr[ pIdx - 1 ], coodsPtr[ pIdx ] ) );
              }
            }
          }
          else if( segs1IndexedPtr->getGeomTypeId() == te::gm::LineStringType )
          {
            te::gm::LineString const* segIndexedNPtr = dynamic_cast< te::gm::LineString const * >(
              segs1IndexedPtr.get() );
            assert( segIndexedNPtr );
            
            std::size_t nPoints = segIndexedNPtr->size();
            te::gm::Coord2D const* coodsPtr = segIndexedNPtr->getCoordinates();
            
            for( std::size_t pIdx = 1 ; pIdx < nPoints ; ++pIdx )
            {
              m_r2IntersectionSegmentsPoints.push_back( std::pair< te::gm::Coord2D, te::gm::Coord2D >(
                coodsPtr[ pIdx - 1 ], coodsPtr[ pIdx ] ) );
            }
          }
        }
        
/*        for( unsigned int idx = 0 ; idx < m_r2IntersectionSegmentsPoints.size() ; ++idx )
        {
          std::cout << std::endl << "m_r2IntersectionSegmentsPoints[" << idx << "]=" 
            << m_r2IntersectionSegmentsPoints[ idx ].first.x
            << " " << m_r2IntersectionSegmentsPoints[ idx ].first.y
            << " " << m_r2IntersectionSegmentsPoints[ idx ].second.x
            << " " << m_r2IntersectionSegmentsPoints[ idx ].second.y;            
        } */       
        
        std::auto_ptr< te::gm::Geometry > segs2IndexedPtr;
        segs2IndexedPtr.reset( indexedDelimiter2Ptr->intersection( indexedDelimiter1Ptr->getRingN( 0 ) ) );
        
        if( segs2IndexedPtr.get() != 0 )
        {
          if( segs2IndexedPtr->getGeomTypeId() == te::gm::MultiLineStringType )
          {
            te::gm::MultiLineString const* segsIndexedNPtr = dynamic_cast< te::gm::MultiLineString const * >(
              segs2IndexedPtr.get() );
            assert( segsIndexedNPtr );
            
            std::size_t numGeoms = segsIndexedNPtr->getNumGeometries();
            
            for( std::size_t gIdx = 0 ; gIdx < numGeoms ; ++gIdx )
            {
              te::gm::LineString const* segIndexedNPtr = dynamic_cast< te::gm::LineString const * >(
                segsIndexedNPtr->getGeometryN( gIdx ) );
              assert( segIndexedNPtr );
              
              std::size_t nPoints = segIndexedNPtr->size();
              te::gm::Coord2D const* coodsPtr = segIndexedNPtr->getCoordinates();
              
              for( std::size_t pIdx = 1 ; pIdx < nPoints ; ++pIdx )
              {
                m_r1IntersectionSegmentsPoints.push_back( std::pair< te::gm::Coord2D, te::gm::Coord2D >(
                  coodsPtr[ pIdx - 1 ], coodsPtr[ pIdx ] ) );
              }
            }
          }
          else if( segs2IndexedPtr->getGeomTypeId() == te::gm::LineStringType )
          {
            te::gm::LineString const* segIndexedNPtr = dynamic_cast< te::gm::LineString const * >(
              segs2IndexedPtr.get() );
            assert( segIndexedNPtr );
            
            std::size_t nPoints = segIndexedNPtr->size();
            te::gm::Coord2D const* coodsPtr = segIndexedNPtr->getCoordinates();
            
            for( std::size_t pIdx = 1 ; pIdx < nPoints ; ++pIdx )
            {
              m_r1IntersectionSegmentsPoints.push_back( std::pair< te::gm::Coord2D, te::gm::Coord2D >(
                coodsPtr[ pIdx - 1 ], coodsPtr[ pIdx ] ) );
            }
          }          
        }
        
/*        for( unsigned int idx = 0 ; idx < m_r1IntersectionSegmentsPoints.size() ; ++idx )
        {
          std::cout << std::endl << "m_r1IntersectionSegmentsPoints[" << idx << "]=" 
            << m_r1IntersectionSegmentsPoints[ idx ].first.x
            << " " << m_r1IntersectionSegmentsPoints[ idx ].first.y
            << " " << m_r1IntersectionSegmentsPoints[ idx ].second.x
            << " " << m_r1IntersectionSegmentsPoints[ idx ].second.y;            
        }  */         
          
        m_r2IntersectionSegmentsPointsSize = (unsigned int)m_r2IntersectionSegmentsPoints.size();
        m_r1IntersectionSegmentsPointsSize = (unsigned int)m_r1IntersectionSegmentsPoints.size();
      }
      
      // defining the blending function
      
      m_blendMethod = blendMethod;
        
      switch( blendMethod )
      {
        case NoBlendMethod :
        {
          m_blendFuncPtr = &te::rp::Blender::noBlendMethodImp;
          break;
        }
        case EuclideanDistanceMethod :
        {
          if( ( m_r1IntersectionSegmentsPointsSize > 0 ) &&
            ( m_r2IntersectionSegmentsPointsSize > 0 ) )
          {
            m_blendFuncPtr = &te::rp::Blender::euclideanDistanceMethodImp;
          }
          else
          {
            m_blendFuncPtr = &te::rp::Blender::noBlendMethodImp;
          }
          break;
        }        
        default :
        {
          return false;
          break;
        }
      }         
              
      // defining the geometric transformation  
        
      m_geomTransformationPtr = geomTransformation.clone();
      
      // defining the interpolators
      
      m_interp1 = new te::rst::Interpolator( &raster1, interpMethod1 );
      m_interp2 = new te::rst::Interpolator( &raster2, interpMethod2 );
        
      m_interpMethod1 = interpMethod1;
      m_interpMethod2 = interpMethod2;
      
      // defining dummy values
      
      for( std::vector< unsigned int >::size_type rasterBandsIdx = 0 ; 
        rasterBandsIdx < raster1Bands.size() ; ++rasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( raster1Bands[ rasterBandsIdx ] <
          raster1.getNumberOfBands(), "Invalid band" );
        TERP_TRUE_OR_RETURN_FALSE( raster2Bands[ rasterBandsIdx ] <
          raster2.getNumberOfBands(), "Invalid band" );            
        
        
        if( forceInputNoDataValue )
        {
          m_raster1NoDataValues.push_back( noDataValue );
          m_raster2NoDataValues.push_back( noDataValue );
        }
        else
        {
          m_raster1NoDataValues.push_back( raster1.getBand( raster1Bands[ 
            rasterBandsIdx ] )->getProperty()->m_noDataValue );
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
      
      return true;
    }
    
    void Blender::initState()
    {
      m_blendMethod = InvalidBlendMethod;
      m_blendFuncPtr = 0;
      m_raster1Ptr = 0;
      m_raster2Ptr = 0;
      m_r1IntersectionSegmentsPointsSize = 0;
      m_r2IntersectionSegmentsPointsSize = 0;
      m_geomTransformationPtr = 0;
      m_interpMethod1 = te::rst::Interpolator::NearestNeighbor;
      m_interpMethod2 = te::rst::Interpolator::NearestNeighbor;
      m_outputNoDataValue = 0;
      m_interp1 = 0;
      m_interp2 = 0;      
    };    
    
    void Blender::clear()
    {
      m_r1IntersectionSegmentsPoints.clear();
      m_r2IntersectionSegmentsPoints.clear();
      if( m_geomTransformationPtr ) delete m_geomTransformationPtr;
      if( m_interp1 ) delete m_interp1;
      if( m_interp2 ) delete m_interp2;
      m_raster1Bands.clear();
      m_raster2Bands.clear();
      m_pixelOffsets1.clear();
      m_pixelScales1.clear();
      m_pixelOffsets2.clear();
      m_pixelScales2.clear();
      m_raster1NoDataValues.clear();
      m_raster2NoDataValues.clear();

      initState();
    }
    
    void Blender::noBlendMethodImp( const double& line, const double& col,
      std::vector< double >& values )
    {
      TERP_DEBUG_TRUE_OR_THROW( values.size() == m_raster1Bands.size(), "Invalid values vector size" );
      
      // Finding the point over the second raster
      
      m_geomTransformationPtr->directMap( col, line, m_noBlendMethodImp_Point2Col,
        m_noBlendMethodImp_Point2Line );
      
      // Blending values

      for( m_noBlendMethodImp_BandIdx = 0 ; m_noBlendMethodImp_BandIdx <
        m_raster1Bands.size() ; ++m_noBlendMethodImp_BandIdx )
      {
        m_interp1->getValue( col, line, m_noBlendMethodImp_cValue, 
          m_raster1Bands[ m_noBlendMethodImp_BandIdx ] ); 
        m_noBlendMethodImp_Value = m_noBlendMethodImp_cValue.real();      
    
        if( m_noBlendMethodImp_Value == m_raster1NoDataValues[ m_noBlendMethodImp_BandIdx ] )
        {
          m_interp2->getValue( m_noBlendMethodImp_Point2Col, 
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
            values[ m_noBlendMethodImp_BandIdx ] = m_noBlendMethodImp_Value + 
              m_pixelOffsets2[ m_noBlendMethodImp_BandIdx ]; 
          }
        }
        else
        {
          m_noBlendMethodImp_Value *= m_pixelScales1[ m_noBlendMethodImp_BandIdx ];
          values[ m_noBlendMethodImp_BandIdx ] =  m_noBlendMethodImp_Value + 
            m_pixelOffsets1[ m_noBlendMethodImp_BandIdx ]; 
        }      
      }
    }
    
    void Blender::euclideanDistanceMethodImp( const double& line, const double& col,
      std::vector< double >& values )
    {
      TERP_DEBUG_TRUE_OR_THROW( values.size() == m_raster1Bands.size(), "Invalid values vector size" );
        
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
        m_interp1->getValue( col, line, m_euclideanDistanceMethodImp_cValue1, 
          m_raster1Bands[ m_euclideanDistanceMethodImp_BandIdx ] ); 
        m_interp2->getValue( m_euclideanDistanceMethodImp_Point2Col, 
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
              ( m_euclideanDistanceMethodImp_cValue2.real() * 
              m_pixelScales2[ m_euclideanDistanceMethodImp_BandIdx ] ) + 
              m_pixelOffsets2[ m_euclideanDistanceMethodImp_BandIdx ]; 
          }
        }
        else
        {
          if( m_euclideanDistanceMethodImp_cValue2.real() == m_raster2NoDataValues[ m_euclideanDistanceMethodImp_BandIdx ] )
          {
            values[ m_euclideanDistanceMethodImp_BandIdx ] =  
              ( m_euclideanDistanceMethodImp_cValue1.real()  * 
              m_pixelScales1[ m_euclideanDistanceMethodImp_BandIdx ] ) +
              m_pixelOffsets1[ m_euclideanDistanceMethodImp_BandIdx ]; 
          }
          else
          {
            if( m_euclideanDistanceMethodImp_dist1 == 0.0 )
            {
              values[ m_euclideanDistanceMethodImp_BandIdx ] =  
                ( m_euclideanDistanceMethodImp_cValue1.real()  * 
                m_pixelScales1[ m_euclideanDistanceMethodImp_BandIdx ] ) +
                m_pixelOffsets1[ m_euclideanDistanceMethodImp_BandIdx ]; 
            }
            else if( m_euclideanDistanceMethodImp_dist1 == 0.0 )
            {
              values[ m_euclideanDistanceMethodImp_BandIdx ] =  
                ( m_euclideanDistanceMethodImp_cValue2.real()  * 
                m_pixelScales2[ m_euclideanDistanceMethodImp_BandIdx ] ) +
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

  } // end namespace rp
}   // end namespace te    

