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
  \file terralib/se/RasterSymbolizer.h

  \brief The RasterSymbolizer describes how to render raster/matrix-coverage data (e.g., satellite photos, DEMs).
*/

#ifndef __TERRALIB_SE_INTERNAL_RASTERSYMBOLIZER_H
#define __TERRALIB_SE_INTERNAL_RASTERSYMBOLIZER_H

// TerraLib
#include "Symbolizer.h"

namespace te
{
  namespace fe { class PropertyName; }

  namespace se
  {
// Forward declarations
    class ChannelSelection;
    class ColorMap;
    class ContrastEnhancement;
    class ImageOutline;
    class ParameterValue;
    class ShadedRelief;

    /*!
      \class RasterSymbolizer
      
      \brief The RasterSymbolizer describes how to render raster/matrix-coverage data (e.g., satellite photos, DEMs).

      A RasterSymbolizer is used to specify the rendering of
      raster/matrix-coverage data (e.g., satellite images, DEMs).
      <br>
      Geometry-type transformations are also system-dependent and
      it is assumed that this capability will be little used.

      \sa Symbolizer, te::fe::PropertyName, ParameterValue, ChannelSelection, ColorMap, ContrastEnhancement, ShadedRelief, ImageOutline
    */
    class TESEEXPORT RasterSymbolizer : public Symbolizer
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \enum OverlapBehavior

          \brief OverlapBehavior tells a system how to behave when multiple
                 raster images in a layer overlap each other, for example with
                 satellite-image scenes.
        */
        enum OverlapBehavior
        {
          LATEST_ON_TOP,      /*!< It refers to the time the scene was captured.       */
          EARLIEST_ON_TOP,    /*!< It refers to the time the scene was captured.       */
          AVERAGE,            /*!< It means to average multiple scenes together. This
                                   can produce blurry results if the source images
                                   are not perfectly aligned in their geo-referencing. */
          RANDOM,             /*!< It means to select an image (or piece thereof)
                                   randomly and place it on top.                       */
          NO_BEHAVIOR
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new RasterSymbolizer. */
        RasterSymbolizer();

        /*!
          \brief Copy constructor.

          \param rhs The other raster symbolizer.
        */
        RasterSymbolizer(const RasterSymbolizer& rhs);

        /*! \brief Destructor. */
        ~RasterSymbolizer();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setGeometry(te::fe::PropertyName* g);

        void setOpacity(ParameterValue* p);

        ParameterValue* getOpacity() { return m_opacity; }

        void setChannelSelection(ChannelSelection* c);

        te::se::ChannelSelection* getChannelSelection() { return m_channelSelection; }

        void setOverlapBehavior(OverlapBehavior b);

        void setColorMap(ColorMap* c);

        te::se::ColorMap* getColorMap() { return m_colorMap; }

        void setContrastEnhancement(ContrastEnhancement* c);

        te::se::ContrastEnhancement* getContrastEnhancement() { return m_contrastEnhancement; }

        void setShadedRelief(ShadedRelief* s);

        void setImageOutline(ImageOutline* i);

        //@}

        /*! \brief It creates a new copy of this object. */
        Symbolizer* clone() const;

      private:

        te::fe::PropertyName* m_geometry;           //!< The interpretation of Geometry is system-dependent, as raster data may be organized differently from feature data, though omitting this element selects the default raster-data source. (Optional)
        ParameterValue* m_opacity;                  //!< An Opacity of 0.0 can be selected for the main raster to avoid rendering the main-raster pixels, or an opacity can be used for a PolygonSymbolizer Fill to allow the main-raster data be visible through the fill. (Optional)
        ChannelSelection* m_channelSelection;       //!< It specifies the false-color channel selection for a multi-spectral raster source (such as a multi-band satellite-imagery source). (Optional)
        OverlapBehavior m_overlapBehavior;          //!< It tells a system how to behave when multiple raster images in a layer overlap each other, for example with satellite-image scenes. (Optional)
        ColorMap* m_colorMap;                       //!< It defines either the colors of a pallette-type raster source or the mapping of numeric pixel values to colors. (Optional)
        ContrastEnhancement* m_contrastEnhancement; //!< It defines contrast enhancement for a channel of a false-color image or for a color image. (Optional)
        ShadedRelief* m_shadedRelief;               //!< It selects the application of relief shading (or "hill shading") to an image for a three-dimensional visual effect. (Optional)
        ImageOutline* m_imageOutline;               //!< It specifies how individual source rasters in a multi-raster set (such as a set of satellite-image scenes) should be outlined to make the individual-image locations visible. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_RASTERSYMBOLIZER_H
