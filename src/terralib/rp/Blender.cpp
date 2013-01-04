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
#include "../raster/Raster.h"
#include "../raster/Grid.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"

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
      const bool forceNoDataValue,
      const std::vector< double >& pixelOffsets1,
      const std::vector< double >& pixelScales1,
      const std::vector< double >& pixelOffsets2,
      const std::vector< double >& pixelScales2,
      te::gm::Polygon const * const r1ValidDataPolygonPtr,
      te::gm::Polygon const * const r2ValidDataPolygonPtr,
      te::gm::GeometricTransformation const * const geomTransformationPtr )
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
      TERP_TRUE_OR_RETURN_FALSE( ( r1ValidDataPolygonPtr ?
        ( r1ValidDataPolygonPtr->getNumRings() > 0 ) : true ),
        "Invalid polygon 1" )
      TERP_TRUE_OR_RETURN_FALSE( ( r2ValidDataPolygonPtr ?
        ( r2ValidDataPolygonPtr->getNumRings() > 0 ) : true ),
        "Invalid polygon 2" )
      TERP_TRUE_OR_RETURN_FALSE( ( geomTransformationPtr ? 
        geomTransformationPtr->isValid() : true ),
        "Invalid transformation" );
        
      clear();
      
      // defining the blending method
      
      m_blendMethod = blendMethod;
        
      switch( blendMethod )
      {
        case NoBlendMethod :
        {
          m_blendFuncPtr = &te::rp::Blender::noBlendMethodImp;
          break;
        }
        default :
        {
          return false;
          break;
        }
      }        
      
      // defining the input rasters
        
      m_raster1Ptr = &raster1;
      m_raster2Ptr = &raster2;
      
      // converting polygons from world cooods to indexed ones
      
      if( r1ValidDataPolygonPtr )
      {
        m_r1ValidDataPolygonPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );
        
        const std::size_t nRings = r1ValidDataPolygonPtr->getNumRings();
        te::gm::LinearRing const* oldRingPtr = 0;
        te::gm::LinearRing* newRingPtr = 0;
        std::size_t pointIdx = 0;
        std::size_t ringSize = 0;
        te::gm::Coord2D* newRingCoodsPtr = 0;
        te::gm::Coord2D const* oldRingCoodsPtr = 0;
        const te::rst::Grid& grid = (*raster1.getGrid());
        
        for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
        {
          oldRingPtr = dynamic_cast< te::gm::LinearRing* >( r1ValidDataPolygonPtr->operator[]( ringIdx ) );
          TERP_DEBUG_TRUE_OR_THROW( oldRingPtr != 0, "Invalid ring" )
          
          ringSize = oldRingPtr->getNPoints();
          
          newRingPtr = new te::gm::LinearRing( ringSize, te::gm::LineStringType, 0, 0 );
          newRingCoodsPtr = newRingPtr->getCoordinates();
          
          oldRingCoodsPtr = oldRingPtr->getCoordinates();
          
          for( pointIdx = 0 ; pointIdx < ringSize ; ++pointIdx )
          {
            grid.geoToGrid( oldRingCoodsPtr->x, oldRingCoodsPtr->y, 
              newRingCoodsPtr->x, newRingCoodsPtr->y );
          }
          
          m_r1ValidDataPolygonPtr->add( newRingPtr );
        }
      }
      
      if( r2ValidDataPolygonPtr )
      {
        m_r2ValidDataPolygonPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );
        
        const std::size_t nRings = r2ValidDataPolygonPtr->getNumRings();
        te::gm::LinearRing const* oldRingPtr = 0;
        te::gm::LinearRing* newRingPtr = 0;
        std::size_t pointIdx = 0;
        std::size_t ringSize = 0;
        te::gm::Coord2D* newRingCoodsPtr = 0;
        te::gm::Coord2D const* oldRingCoodsPtr = 0;
        const te::rst::Grid& grid = (*raster2.getGrid());
        
        for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
        {
          oldRingPtr = dynamic_cast< te::gm::LinearRing* >( r2ValidDataPolygonPtr->operator[]( ringIdx ) );
          TERP_DEBUG_TRUE_OR_THROW( oldRingPtr != 0, "Invalid ring" )
          
          ringSize = oldRingPtr->getNPoints();
          
          newRingPtr = new te::gm::LinearRing( ringSize, te::gm::LineStringType, 0, 0 );
          newRingCoodsPtr = newRingPtr->getCoordinates();
          
          oldRingCoodsPtr = oldRingPtr->getCoordinates();
          
          for( pointIdx = 0 ; pointIdx < ringSize ; ++pointIdx )
          {
            grid.geoToGrid( oldRingCoodsPtr->x, oldRingCoodsPtr->y, 
              newRingCoodsPtr->x, newRingCoodsPtr->y );
          }
          
          m_r2ValidDataPolygonPtr->add( newRingPtr );
        }
      }
        
      // defining the geometric transformation  
        
      if( geomTransformationPtr )
        m_geomTransformationPtr = geomTransformationPtr->clone();
      
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
        m_raster1NoDataValues.push_back( raster1.getBand( raster1Bands[ 
          rasterBandsIdx ] )->getProperty()->m_noDataValue );
          
        TERP_TRUE_OR_RETURN_FALSE( raster2Bands[ rasterBandsIdx ] <
          raster2.getNumberOfBands(), "Invalid band" );            
        m_raster2NoDataValues.push_back( raster2.getBand( raster2Bands[ 
          rasterBandsIdx ] )->getProperty()->m_noDataValue );            
      }
      
      m_noDataValue = noDataValue;
      m_forceNoDataValue = forceNoDataValue;
      
      // defining raster bands
      
      m_raster1Bands = raster1Bands;
      m_raster2Bands = raster2Bands;
      
      // defining pixel offsets
      
      m_pixelOffsets1 = pixelOffsets1;
      m_pixelScales1 = pixelScales1;
      
      m_pixelOffsets2 = pixelOffsets2;
      m_pixelScales2 = pixelScales2;
      
      // initializing the converter
      
      if( geomTransformationPtr == 0 )
      {
        TERP_TRUE_OR_RETURN_FALSE( raster1.getSRID() >= 0,
          "Invalid raster 1 SRID" )
        m_convInstance.setSourceSRID( raster1.getSRID() );
        
        TERP_TRUE_OR_RETURN_FALSE( raster2.getSRID() >= 0,
          "Invalid raster 2 SRID" )
        m_convInstance.setTargetSRID( raster2.getSRID() );        
      }
      
      return true;
    }
    
    void Blender::initState()
    {
      m_blendMethod = InvalidBlendMethod;
      m_blendFuncPtr = 0;
      m_raster1Ptr = 0;
      m_raster2Ptr = 0;
      m_r1ValidDataPolygonPtr = 0;
      m_r2ValidDataPolygonPtr = 0;
      m_geomTransformationPtr = 0;
      m_interpMethod1 = te::rst::Interpolator::NearestNeighbor;
      m_interpMethod2 = te::rst::Interpolator::NearestNeighbor;
      m_noDataValue = 0;
      m_forceNoDataValue = false;
      m_interp1 = 0;
      m_interp2 = 0;      
    };    
    
    void Blender::clear()
    {
      if( m_r1ValidDataPolygonPtr ) delete m_r1ValidDataPolygonPtr;
      if( m_r2ValidDataPolygonPtr ) delete m_r2ValidDataPolygonPtr;
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
    
    void Blender::getBlendedValue( const double& line, const double& col, 
      const unsigned int& rasterChannelsVecsIdx , double& value )
    {
      TERP_DEBUG_TRUE_OR_THROW( m_blendFuncPtr, "Invalid blend function pointer" )
      
      // Verifying if the point is inside the valid raster 1 area
      
      if( m_r1ValidDataPolygonPtr )
      {
        m_getBlendedValue_Point1Indexed.setX( line );
        m_getBlendedValue_Point1Indexed.setY( col );
        if( ! m_getBlendedValue_Point1Indexed.within( m_r1ValidDataPolygonPtr ) )
        {
          value = m_noDataValue;
          return;
        }
      }
      
      // Finding the point over the second raster
      
      if( m_geomTransformationPtr )
      {
        m_geomTransformationPtr->directMap( col, line, m_getBlendedValue_Point2Col,
          m_getBlendedValue_Point2Line );
      }
      else
      {
        m_raster1Ptr->getGrid()->gridToGeo( col, line, m_getBlendedValue_Point1XProj1,
          m_getBlendedValue_Point1YProj1 );
          
        m_convInstance.convert( m_getBlendedValue_Point1XProj1,
          m_getBlendedValue_Point1YProj1, m_getBlendedValue_Point1XProj2,
          m_getBlendedValue_Point1YProj2 );
          
        m_raster2Ptr->getGrid()->geoToGrid( m_getBlendedValue_Point1XProj2,
          m_getBlendedValue_Point1YProj2, m_getBlendedValue_Point2Col,
          m_getBlendedValue_Point2Line );
      }
        
      // Verifying if the point is inside the valid raster 2 area
      
      if( m_r2ValidDataPolygonPtr )
      {
        m_getBlendedValue_Point2Indexed.setX( m_getBlendedValue_Point2Col );
        m_getBlendedValue_Point2Indexed.setY( m_getBlendedValue_Point2Line );
        if( ! m_getBlendedValue_Point2Indexed.within( m_r2ValidDataPolygonPtr ) )
        {
          value = m_noDataValue;
          return;
        }
      }       
      
      return (this->*m_blendFuncPtr)( line, col, m_getBlendedValue_Point2Line,
        m_getBlendedValue_Point2Col, rasterChannelsVecsIdx, value );
    }    
    
    void Blender::noBlendMethodImp( const double& line1, const double& col1,
      const double& line2, const double& col2,
      const unsigned int& rasterChannelsVecsIdx, double& value )
    {
      m_interp1->getValue( col1, line1, m_noBlendMethodImp_cValue, 
        m_raster1Bands[ rasterChannelsVecsIdx ] );
      value =  m_noBlendMethodImp_cValue.real();
        
      if( m_forceNoDataValue )
      {
        if( value == m_noDataValue )
        {
          m_interp2->getValue( col2, line2, m_noBlendMethodImp_cValue, 
            m_raster2Bands[ rasterChannelsVecsIdx ] );
          value =  m_noBlendMethodImp_cValue.real();          
        }
      }
      else
      {
        if( value == m_raster1NoDataValues[ rasterChannelsVecsIdx ] )
        {
          m_interp2->getValue( col2, line2, m_noBlendMethodImp_cValue, 
            m_raster2Bands[ rasterChannelsVecsIdx ] );
          value =  m_noBlendMethodImp_cValue.real();
          
          if( value == m_raster2NoDataValues[ rasterChannelsVecsIdx ] )
          {
            value = m_noDataValue;            
          }
        }
      }
    }

  } // end namespace rp
}   // end namespace te    

