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
#include "../geometry/Envelope.h"
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
      const bool forceInputNoDataValue,
      const std::vector< double >& pixelOffsets1,
      const std::vector< double >& pixelScales1,
      const std::vector< double >& pixelOffsets2,
      const std::vector< double >& pixelScales2,
      te::gm::Polygon const * const r1ValidDataPolygonPtr,
      te::gm::Polygon const * const r2ValidDataPolygonPtr,
      const te::gm::GeometricTransformation& geomTransformation,
      const bool raster1HasPrecedence )
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
      TERP_TRUE_OR_RETURN_FALSE( geomTransformation.isValid(),
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
      
      m_raster1HasPrecedence = raster1HasPrecedence;
        
      m_raster1Ptr = &raster1;
      m_raster2Ptr = &raster2;
      
      // converting polygons from world cooods to indexed ones
      
      if( r1ValidDataPolygonPtr )
      {
        m_r1ValidDataPolygonPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );
        
        const std::size_t nRings = r1ValidDataPolygonPtr->getNumRings();
        te::gm::LinearRing const* oldRingPtr = 0;
        std::auto_ptr< te::gm::LinearRing > newRingPtr;
        std::size_t pointIdx = 0;
        std::size_t ringElementsBound = 0;
        const te::rst::Grid& grid = (*raster1.getGrid());
        double newXCoord = 0;
        double newYCoord = 0;
        
        for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
        {
          oldRingPtr = dynamic_cast< te::gm::LinearRing const* >( r1ValidDataPolygonPtr->operator[]( ringIdx ) );
          TERP_DEBUG_TRUE_OR_THROW( oldRingPtr != 0, "Invalid ring" )
          
          TERP_DEBUG_TRUE_OR_THROW( oldRingPtr->getNPoints(), "Invalid ring size" );
          ringElementsBound = oldRingPtr->getNPoints() - 1;
          
          newRingPtr.reset( new te::gm::LinearRing( oldRingPtr->getNPoints(), te::gm::LineStringType, 0, 0 ) );
          
          for( pointIdx = 0 ; pointIdx < ringElementsBound ; ++pointIdx )
          {
            grid.geoToGrid( oldRingPtr->getX( pointIdx ), oldRingPtr->getY( pointIdx ), 
              newXCoord, newYCoord );
            newRingPtr->setX( pointIdx, newXCoord );
            newRingPtr->setY( pointIdx, newYCoord );
          }
          newRingPtr->setX( ringElementsBound, newRingPtr->getX( 0 ) );
          newRingPtr->setY( ringElementsBound, newRingPtr->getY( 0 ) );          
          
          m_r1ValidDataPolygonPtr->add( newRingPtr.release() );
        }
      }
      else
      {
        te::gm::LinearRing* auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
        auxLinearRingPtr->setPoint( 0, -0.5, -0.5 );
        auxLinearRingPtr->setPoint( 1, ((double)raster1.getNumberOfColumns()) - 0.5, -0.5 );
        auxLinearRingPtr->setPoint( 2, ((double)raster1.getNumberOfColumns()) - 0.5, 
          ((double)raster1.getNumberOfRows()) - 0.5 );
        auxLinearRingPtr->setPoint( 3, -0.5, ((double)raster1.getNumberOfRows()) - 0.5 );
        auxLinearRingPtr->setPoint( 4, -0.5, -0.5 );
        
        m_r1ValidDataPolygonPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );
        m_r1ValidDataPolygonPtr->push_back( auxLinearRingPtr );        
      }
      
      if( r2ValidDataPolygonPtr )
      {
        m_r2ValidDataPolygonPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );
        
        const std::size_t nRings = r2ValidDataPolygonPtr->getNumRings();
        te::gm::LinearRing const* oldRingPtr = 0;
        std::auto_ptr< te::gm::LinearRing > newRingPtr;
        std::size_t pointIdx = 0;
        std::size_t ringElementsBound = 0;
        const te::rst::Grid& grid = (*raster2.getGrid());
        double newXCoord = 0;
        double newYCoord = 0;        
        
        for( std::size_t ringIdx = 0 ; ringIdx < nRings ; ++ringIdx )
        {
          oldRingPtr = dynamic_cast< te::gm::LinearRing* >( r2ValidDataPolygonPtr->operator[]( ringIdx ) );
          TERP_DEBUG_TRUE_OR_THROW( oldRingPtr != 0, "Invalid ring" )
          
          TERP_DEBUG_TRUE_OR_THROW( oldRingPtr->getNPoints(), "Invalid ring size" );
          ringElementsBound = oldRingPtr->getNPoints() - 1;
          
          newRingPtr.reset( new te::gm::LinearRing( oldRingPtr->getNPoints(), te::gm::LineStringType, 0, 0 ) );
          
          for( pointIdx = 0 ; pointIdx < ringElementsBound ; ++pointIdx )
          {
            grid.geoToGrid(  oldRingPtr->getX( pointIdx ), oldRingPtr->getY( pointIdx ), 
              newXCoord, newYCoord );
            newRingPtr->setX( pointIdx, newXCoord );
            newRingPtr->setY( pointIdx, newYCoord );              
          }
          newRingPtr->setX( ringElementsBound, newRingPtr->getX( 0 ) );
          newRingPtr->setY( ringElementsBound, newRingPtr->getY( 0 ) ); 
          
          m_r2ValidDataPolygonPtr->add( newRingPtr.release() );
        }
      }
      else
      {
        te::gm::LinearRing* auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
        auxLinearRingPtr->setPoint( 0, -0.5, -0.5 );
        auxLinearRingPtr->setPoint( 1, ((double)raster2.getNumberOfColumns()) - 0.5, -0.5 );
        auxLinearRingPtr->setPoint( 2, ((double)raster2.getNumberOfColumns()) - 0.5, 
          ((double)raster2.getNumberOfRows()) - 0.5 );
        auxLinearRingPtr->setPoint( 3, -0.5, ((double)raster2.getNumberOfRows()) - 0.5 );
        auxLinearRingPtr->setPoint( 4, -0.5, -0.5 );       
        
        m_r2ValidDataPolygonPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 0, 0 );
        m_r2ValidDataPolygonPtr->push_back( auxLinearRingPtr );        
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
      m_r1ValidDataPolygonPtr = 0;
      m_r2ValidDataPolygonPtr = 0;
      m_geomTransformationPtr = 0;
      m_interpMethod1 = te::rst::Interpolator::NearestNeighbor;
      m_interpMethod2 = te::rst::Interpolator::NearestNeighbor;
      m_outputNoDataValue = 0;
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
    
    void Blender::noBlendMethodImp( const double& line, const double& col,
      std::vector< double >& values )
    {
      TERP_DEBUG_TRUE_OR_THROW( values.size() == m_raster1Bands.size(), "Invalid values vector size" );
      
      // Finding the point over the second raster
      
      m_geomTransformationPtr->directMap( col, line, m_noBlendMethodImp_Point2Col,
        m_noBlendMethodImp_Point2Line );
      
      // Blending values
      
      if( m_raster1HasPrecedence )
      {
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
      else
      {
        for( m_noBlendMethodImp_BandIdx = 0 ; m_noBlendMethodImp_BandIdx <
          m_raster1Bands.size() ; ++m_noBlendMethodImp_BandIdx )
        {
          m_interp2->getValue( m_noBlendMethodImp_Point2Col, 
            m_noBlendMethodImp_Point2Line, m_noBlendMethodImp_cValue, 
            m_raster2Bands[ m_noBlendMethodImp_BandIdx ] );          
          m_noBlendMethodImp_Value = m_noBlendMethodImp_cValue.real();      
      
          if( m_noBlendMethodImp_Value == m_raster2NoDataValues[ m_noBlendMethodImp_BandIdx ] )
          {
            m_interp1->getValue( col, line, m_noBlendMethodImp_cValue, 
              m_raster1Bands[ m_noBlendMethodImp_BandIdx ] ); 
            m_noBlendMethodImp_Value =  m_noBlendMethodImp_cValue.real();          
            
            if( m_noBlendMethodImp_Value == m_raster1NoDataValues[ m_noBlendMethodImp_BandIdx ] )
            {
              values[ m_noBlendMethodImp_BandIdx ] = m_outputNoDataValue;
            }
            else
            {
              m_noBlendMethodImp_Value *= m_pixelScales1[ m_noBlendMethodImp_BandIdx ];
              values[ m_noBlendMethodImp_BandIdx ] = m_noBlendMethodImp_Value + 
                m_pixelOffsets1[ m_noBlendMethodImp_BandIdx ]; 
            }
          }
          else
          {
            m_noBlendMethodImp_Value *= m_pixelScales2[ m_noBlendMethodImp_BandIdx ];
            values[ m_noBlendMethodImp_BandIdx ] =  m_noBlendMethodImp_Value + 
              m_pixelOffsets2[ m_noBlendMethodImp_BandIdx ]; 
          }      
        }
      }
    }

  } // end namespace rp
}   // end namespace te    

