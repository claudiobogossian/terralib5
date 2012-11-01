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

  namespace rst
  {
    class Grid;
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
          \brief It draws the layer raster objects in the given canvas using the informed SRS.

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

        /*!
          \brief Function used to verify if the drawing area has intersection with the layer objects.

          \param bbox  Area to be painted.

          \param raster  Raster object

          \param canvas  Pointer to canvas

          \param srid  SRID identifier

          \return True if has intersection and false in other case.
        */
        bool hasIntersection(const te::gm::Envelope& bbox, te::rst::Raster* raster, Canvas* canvas, int srid);

        /*!
          \brief Function used to create a GRID with canvas dimensions (width and height)

          \param canvas  Pointer to canvas

          \param bbox  Area to be painted.

          \param srid  SRID identifier
        */
        void buildRasterCanvas(Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        /*!
          \brief Create a default raster symbolizer to a raster object

          \param layer Pointer to a Raster Layer with the raster object
        */
        void createVisualDefault(RasterLayer* layer);

        /*!
          \brief Function used to apply the raster symbolizes associated to layer

          \param layer  Layer with the raster object

          \param canvas  Pointer to canvas

          \param srid  SRID identifier
        */
        void applyStyle(RasterLayer* layer, Canvas* canvas, int srid);

        /*!
          \brief Function used to apply the raster symbolizes associated to layer

          \param rMin  Define the minimum value in raster

          \param rMax  Define the maximum value in raster

          \param layer  Layer with the raster object
        */
        void getMinMaxValues(double& rMin, double& rMax, RasterLayer* layer);


      protected:

        te::rst::Grid* m_gridCanvas;           //!< This raster is used to paint canvas.

        //@}
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H

