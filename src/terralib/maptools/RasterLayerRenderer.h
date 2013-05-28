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
  \file terralib/maptools/RasterLayerRenderer.h

  \brief It renders the objects associated to a dataset layer.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H

// TerraLib
#include "AbstractRenderer.h"

namespace te
{
  namespace map
  {
// Forward declarations
    class RasterLayer;

    /*!
      \class RasterLayerRenderer

      \brief It renders the objects associated to a raster layer.

      \sa AbstractLayer, AbstractRenderer, CoverageStyleRenderer, FeatureTypeStyleRenderer, QueryLayerRenderer, Canvas, te::se::Style

    */
    class TEMAPEXPORT RasterLayerRenderer : public AbstractRenderer
    {
      public:

        /*! Constructor */
        RasterLayerRenderer();

        /*! \brief Destructor. */
        ~RasterLayerRenderer();

        void draw(AbstractLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, int srid);

      private:

        /*!
          \brief It draws the raster layer in the given canvas using the SRS informed.

          The informed bounding box (bbox) is used to constraint the data set layer objects to be drawn.
          Here, we assume that the given bbox was pre-processed. i.e. the bbox was clipped and contains the same projection of data set layer.

          \param layer       The data set layer that will be drawn.
          \param canvas      The canvas were the data set layer objects will be drawn.
          \param bbox        The interest area to render the map.
          \param visibleArea The original area requested to render the map.
          \param srid        The SRS to be used to draw the data set layer objects.

          \note The renderer will NOT take the owership of the given pointers.

          \note This method consider that the referenced data set contains raster data.
        */
        void drawRaster(RasterLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, const te::gm::Envelope& visibleArea, int srid);
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H

