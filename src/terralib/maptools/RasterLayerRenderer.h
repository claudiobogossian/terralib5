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
  \file RasterLayerRenderer.h

  \brief It renders the raster objects associated to a Layer.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H

// TerraLib
#include "AbstractRenderer.h"

namespace te
{
// Forward declarations
  namespace se
  {
    class CoverageStyle;
    class FeatureTypeStyle;
  }

  namespace map
  {    
    /*!
      \class RasterLayerRenderer

      \brief It renders the raster objects associated to a Layer.
    
      \sa RasterLayer, AbstractRenderer, Canvas
     */
    class TEMAPEXPORT RasterLayerRenderer : public AbstractRenderer
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! Constructor */
        RasterLayerRenderer();

        /*! \brief Destructor. */
        ~RasterLayerRenderer();

        //@}      

        /** @name Implementation of Pure Virtual Methods from AbstractLayer
         *  Every subclass must implent this interface.
         */
        //@{

        /*!
          \brief It draws the layer geographic objects in the given canvas using the informed SRS.

          The informed bounding box (bbox) is used to constraint the Layer objects to be drawn.
          The bbox coordinates must be in the Spatial Reference System given by srid.

          \param layer  The layer to be rendered.
          \param canvas The canvas were the layer objects will be drawn.
          \param bbox   The interest area to render the map.
          \param srid   The SRS to be used to draw the layer objects.
         */
        virtual void draw(AbstractLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        //@}

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        RasterLayerRenderer(const RasterLayerRenderer& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        RasterLayerRenderer& operator=(const RasterLayerRenderer& rhs);



        bool hasIntersection(const te::gm::Envelope& bbox, te::rst::Raster* raster, Canvas* canvas, int srid);

        void getResampledRaster(te::rst::Raster* raster, Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        void buildRasterCanvas(te::rst::Raster* raster, Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        /*!
          \brief Create a default raster symbolizer to a raster object

          \param layer Pointer to a Raster Layer with the raster object
        */
        void createVisualDefault(RasterLayer* layer);


      protected:

        te::rst::Raster* _rasterCanvas;     //!< This raster is used to paint canvas.
        te::rst::Raster* _resampledRaster;  //!< This raster is the resampled grid for a given scale

        //@}
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H

