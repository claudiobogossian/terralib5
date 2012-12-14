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
      const te::gm::Polygon& r2ValidDataPolygon)
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
      m_r1ValidDataPolygon( r1ValidDataPolygon ),
      m_r2ValidDataPolygon( r2ValidDataPolygon )
    {
      switch( blendMethod )
      {
        default :
        {
          m_blendFuncPtr = &te::rp::Blender::noBlendMethodImp;
          break;
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

