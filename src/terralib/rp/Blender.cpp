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
#include "../raster/Grid.h"

namespace te
{
  namespace rp
  {
    Blender::Blender( const te::rst::Raster& raster1, 
      const std::vector< unsigned int >& raster1Bands, 
      const te::rst::Raster& raster2, 
      const std::vector< unsigned int >& raster2Bands,
      const BlendMethod& blendMethod, 
      const te::rst::Interpolator::Method& interpMethod,
      const te::gm::GeometricTransformation& geomTransformation,
      const double& noDataValue,
      const std::vector< double >& pixelOffsets1,
      const std::vector< double >& pixelScales1,
      const std::vector< double >& pixelOffsets2,
      const std::vector< double >& pixelScales2,
      const te::gm::Polygon& r1ValidDataPolygon,
      const te::gm::Polygon& r2ValidDataPolygon,
      const bool& ignoreRasterSRIDS )
    : m_raster1( raster1 ),
      m_raster2( raster2 ),
      m_raster1Bands( raster1Bands ),
      m_raster2Bands( raster2Bands ),
      m_blendMethod( blendMethod ),
      m_interpMethod( interpMethod ),
      m_geomTransformationPtr( geomTransformation.clone() ),
      m_noDataValue( noDataValue ),
      m_pixelOffsets1( pixelOffsets1 ),
      m_pixelScales1( pixelScales1 ),
      m_pixelOffsets2( pixelOffsets2 ),
      m_pixelScales2( pixelScales2 ),
      m_r1ValidDataPolygon( 0, te::gm::PolygonType, 0, 0 ),
      m_r2ValidDataPolygon( 0, te::gm::PolygonType, 0, 0 )
    {
      TERP_DEBUG_TRUE_OR_THROW( 
        raster1.getAccessPolicy() & te::common::RAccess, 
        "Invalid raster 1" );        
      TERP_DEBUG_TRUE_OR_THROW( 
        raster2.getAccessPolicy() & te::common::RAccess, 
        "Invalid raster 2" ); 
      TERP_DEBUG_TRUE_OR_THROW( geomTransformation.isValid(),
        "Invalid transformation" );
      TERP_DEBUG_TRUE_OR_THROW( r1ValidDataPolygon.getNumRings() > 0,
        "Invalid polygon 1" )
      TERP_DEBUG_TRUE_OR_THROW( r2ValidDataPolygon.getNumRings() > 0,
        "Invalid polygon 2" )
        
      switch( blendMethod )
      {
        default :
        {
          m_blendFuncPtr = &te::rp::Blender::noBlendMethodImp;
          break;
        }
      }
      
      if( ! ignoreRasterSRIDS )
      {
        m_convInstance.setSourceSRID( raster1.getSRID() );
        m_convInstance.setTargetSRID( raster2.getSRID() );        
      }
      
      // converting polygons from world cooods to indexed ones
      
      {
        const std::size_t nRings = r1ValidDataPolygon.getNumRings();
        te::gm::LinearRing const* oldRingPtr = 0;
        te::gm::LinearRing* newRingPtr = 0;
        std::size_t pointIdx = 0;
        std::size_t ringSize = 0;
        te::gm::Coord2D* newRingCoodsPtr = 0;
        te::gm::Coord2D const* oldRingCoodsPtr = 0;
        const te::rst::Grid& grid = (*raster1.getGrid());
        
        for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
        {
          oldRingPtr = dynamic_cast< te::gm::LinearRing* >( r1ValidDataPolygon[ ringIdx ] );
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
          
          m_r1ValidDataPolygon.add( newRingPtr );
        }
      }
      
      {
        const std::size_t nRings = r2ValidDataPolygon.getNumRings();
        te::gm::LinearRing const* oldRingPtr = 0;
        te::gm::LinearRing* newRingPtr = 0;
        std::size_t pointIdx = 0;
        std::size_t ringSize = 0;
        te::gm::Coord2D* newRingCoodsPtr = 0;
        te::gm::Coord2D const* oldRingCoodsPtr = 0;
        const te::rst::Grid& grid = (*raster2.getGrid());
        
        for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
        {
          oldRingPtr = dynamic_cast< te::gm::LinearRing* >( r2ValidDataPolygon[ ringIdx ] );
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
          
          m_r2ValidDataPolygon.add( newRingPtr );
        }
      }      
    }
    
    Blender::~Blender()
    {
      delete m_geomTransformationPtr;
    }
    
    void Blender::noBlendMethodImp( const double& line, const double& col,
      const unsigned int& rasterChannelsVecsIdx, double& value )
    {
      
    }

  } // end namespace rp
}   // end namespace te    

