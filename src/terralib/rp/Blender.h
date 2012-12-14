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
  \file terralib/rp/Blender.h
  \brief Blended pixel value calculation for two overlaped rasters.
 */

#ifndef __TERRALIB_RP_INTERNAL_BLENDER_H
#define __TERRALIB_RP_INTERNAL_BLENDER_H

#include "Config.h"
#include "Macros.h"
#include "../raster/Raster.h"
#include "../raster/Interpolator.h"
#include "../geometry/GeometricTransformation.h"
#include "../geometry/Polygon.h"

#include <boost/noncopyable.hpp>

#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class Blender
      \brief Blended pixel value calculation for two overlaped rasters.
      \details The overlap between the two images is modeled by the given geometric transformation direct mapping input raster 1 indexed coords (line, column) to input raster 2 indexed coords.
      \ingroup RPModule
     */
    class TERPEXPORT Blender : public boost::noncopyable
    {
      public:
        
        /*! \enum BlendMethod Pixel Blend methods. */      
        enum BlendMethod 
        {
          NoBlendMethod = 1 //!< No blending performed.
        };        

        /*!
          \brief Default constructor.
          \param raster1 Input raster 1.
          \param raster1Bands Input raster 1 band indexes to use.
          \param raster2 Input raster 2.
          \param raster2Bands Input raster 2 band indexes to use (this vector has the same size as raster1Bands).
          \param blendMethod The blend method to apply.
          \param interpMethod The interpolation method to use.
          \param geomTransformation A transformation mapping raster 1 pixels ( te::gm::GTParameters::TiePoint::first ) to raster 2 ( te::gm::GTParameters::TiePoint::first ) (Note: all coords are indexed by lines/columns).
          \param noDataValue The value returned where there is no pixel data bo blend.
          \param pixelOffsets1 The values offset to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelScales1 The values scale to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelOffsets2 The values offset to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelScales2 The values scale to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param r1ValidDataPolygon A polygon (raster 1 indexed coords - line, col) delimiting the raster region with valid data.
          \param r2ValidDataPolygon A polygon (raster 2 indexed coords - line, col) delimiting the raster region with valid data.
        */
        Blender( 
          const te::rst::Raster& raster1, 
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
          const te::gm::Polygon& r2ValidDataPolygon );
        
        ~Blender();
        
        /*!
          \brief Blend a pixel value using the current parameters.
          \param line Line (raster 1 reference).
          \param col Column (raster 1 reference).
          \param rasterChannelsVecsIdx Vector index (the index to search the correct band/channel for each input raster from raster1ChannelsVec and raster2ChannelsVec).
          \param value Blended value.
          \note The caller of this method must be aware that the returned blended value may be outside the original input rasters valid values range.
        */
        inline void getBlendedValue( const double& line, const double& col, 
          const unsigned int& rasterChannelsVecsIdx , double& value )
        {
          TERP_DEBUG_TRUE_OR_THROW( m_blendFuncPtr, "Invalid blend function pointer" )
          return (this->*m_blendFuncPtr)( line, col, rasterChannelsVecsIdx, 
            value );
        };        

      protected:
        
        /*!
          \brief Type definition for the a bleding function pointer.
          \param line Line.
          \param col Column.
          \param band Band.
          \param value Interpolated value.
        */      
        typedef void (Blender::*BlendFunctPtr)( const double& line, 
          const double& col, const unsigned int& rasterChannelsVecsIdx, double& value );         

        BlendFunctPtr m_blendFuncPtr;
        const te::rst::Raster& m_raster1;
        const te::rst::Raster& m_raster2;
        const std::vector< unsigned int > m_raster1Bands;
        const std::vector< unsigned int > m_raster2Bands;
        const BlendMethod m_blendMethod;
        const te::rst::Interpolator::Method m_interpMethod;
        const te::gm::GeometricTransformation* m_geomTransformationPtr;
        const double m_noDataValue;
        const std::vector< double > m_pixelOffsets1;
        const std::vector< double > m_pixelScales1;
        const std::vector< double > m_pixelOffsets2;
        const std::vector< double > m_pixelScales2;
        const te::gm::Polygon m_r1ValidDataPolygon;
        const te::gm::Polygon m_r2ValidDataPolygon;        
        
        /*!
          \brief Implementation for NoBlendMethod.
          \param line Line (raster 1 reference).
          \param col Column (raster 1 reference).
          \param rasterChannelsVecsIdx Vector index (the index to search the correct band/channel for each input raster from raster1ChannelsVec and raster2ChannelsVec).
          \param value Blended value.
        */
        void noBlendMethodImp( const double& line, const double& col,
          const unsigned int& rasterChannelsVecsIdx, double& value );        

    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ALGORITHM_H

