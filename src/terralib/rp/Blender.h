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
#include "../geometry/Point.h"
#include "../srs/Converter.h"

#include <boost/noncopyable.hpp>

#include <vector>
#include <complex>

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
          InvalidBlendMethod = 0, //!< Invalid blending method.
          NoBlendMethod = 1 //!< No blending performed.
        };        
        
        /*! Default constructor. */
        Blender();
        
        /*! Default destructor. */
        ~Blender();
        
        /*!
          \brief Inititate the blender instance.
          \param raster1 Input raster 1.
          \param raster1Bands Input raster 1 band indexes to use.
          \param raster2 Input raster 2.
          \param raster2Bands Input raster 2 band indexes to use (this vector has the same size as raster1Bands).
          \param blendMethod The blend method to apply.
          \param interpMethod1 The interpolation method to use when reading raster 1 data.
          \param interpMethod2 The interpolation method to use when reading raster 2 data.
          \param noDataValue The value returned where there is no pixel data bo blend.
          \param forceNoDataValue Use noDataValue as the input rasters no-data value (The original rasters no-data values will be ignored) 
          \param pixelOffsets1 The values offset to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelScales1 The values scale to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelOffsets2 The values offset to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelScales2 The values scale to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param r1ValidDataPolygonPtr A pointer to a polygon (raster 1 world/projected coords) delimiting the raster region with valid data, or null if all raster data area is valid.
          \param r2ValidDataPolygon A pointer to a polygon (raster 2 world/projected coords) delimiting the raster region with valid data, or null if all raster data area is valid.
          \param geomTransformation A pointer transformation mapping raster 1 pixels ( te::gm::GTParameters::TiePoint::first ) to raster 2 ( te::gm::GTParameters::TiePoint::second ) (Note: all coords are indexed by lines/columns), or a null value if only the spatial reference systems must be used.
          \return true if ok, false on errors
        */
        bool initialize( 
          const te::rst::Raster& raster1, 
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
          te::gm::GeometricTransformation const * const geomTransformationPtr );
        
        
        /*!
          \brief Blend a pixel value using the current parameters.
          \param line Line (raster 1 reference).
          \param col Column (raster 1 reference).
          \param rasterChannelsVecsIdx Vector index (the index to search the correct band/channel for each input raster from raster1ChannelsVec and raster2ChannelsVec).
          \param value Blended value.
          \note The caller of this method must be aware that the returned blended value may be outside the original input rasters valid values range.
        */
        void getBlendedValue( const double& line, const double& col, 
          const unsigned int& rasterChannelsVecsIdx , double& value );

      protected:
        
        /*!
          \brief Type definition for the a bleding function pointer.
          \param line1 Raster 1 Line.
          \param col1 Raster 1 Column.
          \param line1 Raster 2 Line.
          \param col1 Raster 2 Column.
          \param band Band.
          \param value Interpolated value.
        */      
        typedef void (Blender::*BlendFunctPtr)( const double& line1, 
          const double& col1, const double& line2, const double& col2,
          const unsigned int& rasterChannelsVecsIdx, double& value );         

        BlendMethod m_blendMethod; //!< The blend method to apply.
        BlendFunctPtr m_blendFuncPtr; //!< The current blend function.
        te::rst::Raster const* m_raster1Ptr; //!< Input raster 1.
        te::rst::Raster const* m_raster2Ptr; //!< Input raster 2.
        te::gm::Polygon* m_r1ValidDataPolygonPtr; //!< null or a polygon (raster 1 indexed coords).
        te::gm::Polygon* m_r2ValidDataPolygonPtr; //!< null or a polygon (raster 2 indexed coords).
        te::gm::GeometricTransformation* m_geomTransformationPtr; //!< A transformation mapping raster 1 pixels ( te::gm::GTParameters::TiePoint::first ) to raster 2 ( te::gm::GTParameters::TiePoint::second ) (Note: all coords are indexed by lines/columns).
        te::rst::Interpolator::Method m_interpMethod1; //!< The interpolation method to use when reading raster 1 data.
        te::rst::Interpolator::Method m_interpMethod2; //!< The interpolation method to use when reading raster 2 data.
        double m_noDataValue; //!< The value returned where there is no pixel data bo blend.
        bool m_forceNoDataValue; //!< Use noDataValue as the input rasters no-data value (The original rasters no-data values will be ignored) 
        te::rst::Interpolator* m_interp1; //!< Raster 1 interpolator instance pointer.
        te::rst::Interpolator* m_interp2; //!< Raster 2 interpolator instance pointer.        
        std::vector< unsigned int > m_raster1Bands; //!< Input raster 1 band indexes to use.
        std::vector< unsigned int > m_raster2Bands; //!< Input raster 2 band indexes to use.
        std::vector< double > m_pixelOffsets1; //!< The values offset to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
        std::vector< double > m_pixelScales1; //!< The values scale to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
        std::vector< double > m_pixelOffsets2; //!< The values offset to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
        std::vector< double > m_pixelScales2; //!< The values scale to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
        std::vector< double > m_raster1NoDataValues; //! Raster 1 no-data values (on value per band).
        std::vector< double > m_raster2NoDataValues; //! Raster 2 no-data values (on value per band).
        te::srs::Converter m_convInstance; //!< A SRS converter instance to be used when the rasters SRIDs are not the same.
        
        // variables used by the getBlendedValue method
        double m_getBlendedValue_Point1XProj1;
        double m_getBlendedValue_Point1YProj1;
        double m_getBlendedValue_Point1XProj2;
        double m_getBlendedValue_Point1YProj2;
        double m_getBlendedValue_Point2Line;
        double m_getBlendedValue_Point2Col;
        te::gm::Point m_getBlendedValue_Point1Indexed;
        te::gm::Point m_getBlendedValue_Point2Indexed;
        
        // variables used by the noBlendMethodImp method
        std::complex< double > m_noBlendMethodImp_cValue;
        
        /*! \brief Reset the instance to its initial default state. */
        void initState();
        
        /*! \brief Clear all internal allocated resources. */
        void clear();
        
        /*!
          \brief Implementation for NoBlendMethod.
          \param line1 Raster 1 Line.
          \param col1 Raster 1 Column.
          \param line1 Raster 2 Line.
          \param col1 Raster 2 Column.
          \param rasterChannelsVecsIdx Vector index (the index to search the correct band/channel for each input raster from raster1ChannelsVec and raster2ChannelsVec).
          \param value Blended value.
        */
        void noBlendMethodImp( const double& line1, const double& col1,
          const double& line2, const double& col2,
          const unsigned int& rasterChannelsVecsIdx, double& value );        

    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ALGORITHM_H

