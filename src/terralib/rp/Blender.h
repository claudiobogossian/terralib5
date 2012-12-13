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

#include <boost/noncopyable.hpp>

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
          NoBlendMethod = 1, //!< No blending performed.
          MeanBlendMethod = 2, //!< Mean of overlapped pixels method.
          EuclideanBlendMethod = 3 //!< Euclidean distance based blending method.
        };        
        
        Blender();
        
        ~Blender();
        
        /*!
          \brief Blend a pixel value using the current parameters.
          \param line Line (raster 1 reference).
          \param col Column (raster 1 reference).
          \param rasterChannelsVecsIdx Vector index (the index to search the correct band/channel for each input raster from raster1ChannelsVec and raster2ChannelsVec).
          \param value Blended value.
          \note The caller of this method must be aware that the returned blended value may be outside the original input rasters valid values range.
        *
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
          const double& col, const unsigned int& band, double& value );         

        BlendFunctPtr m_blendFuncPtr;

    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ALGORITHM_H

