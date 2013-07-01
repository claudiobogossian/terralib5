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
  \file terralib/maptools/AbstractLayerRenderer.h

  \brief It renders the objects associated to an abstract layer. i.e. a generic renderer.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYERRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYERRENDERER_H

// TerraLib
#include "AbstractRenderer.h"

// STL
#include <string>

namespace te
{
// Forward declaration
  namespace se
  {
    class CoverageStyle;
    class FeatureTypeStyle;
  }

  namespace map
  {
    /*!
      \class AbstractLayerRenderer

      \brief It renders the objects associated to an abstract layer. i.e. a generic renderer.

      \sa AbstractLayer, AbstractRenderer
    */
    class TEMAPEXPORT AbstractLayerRenderer : public AbstractRenderer
    {
      public:

        /*! Constructor */
        AbstractLayerRenderer();

        /*! \brief Destructor. */
        ~AbstractLayerRenderer();

        void draw(AbstractLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, int srid);

      private:

        /*!
          \brief It draws the abstract layer in the given canvas using the SRS informed.

          The informed bounding box (bbox) is used to constraint the layer objects to be drawn.
          Here, we assume that the given bbox was pre-processed. i.e. the bbox was clipped and contains the same projection of abstract layer.

          \param layer             The abstract layer that will be drawn.
          \param geomPropertyName  The geometry property name that will be drawn.
          \param style             The style that will be used.
          \param canvas            The canvas were the layer objects will be drawn.
          \param bbox              The interest area to render the map.
          \param srid              The SRS to be used to draw the layer objects.

          \note This method consider that the given layer contains vetorial data.
        */
        void drawLayerGeometries(AbstractLayer* layer,
                                 const std::string& geomPropertyName,
                                 te::se::FeatureTypeStyle* style,
                                 Canvas* canvas,
                                 const te::gm::Envelope& bbox,
                                 int srid);

        /*!
          \brief It draws the grouping of the abstract layer in the given canvas using the SRS informed.

          The informed bounding box (bbox) is used to constraint the layer objects to be drawn.
          Here, we assume that the given bbox was pre-processed. i.e. the bbox was clipped and contains the same projection of abstract layer.

          \param layer             The abstract layer that will be drawn.
          \param geomPropertyName  The geometry property name that will be drawn.
          \param canvas            The canvas were the layer objects will be drawn.
          \param bbox              The interest area to render the map.
          \param srid              The SRS to be used to draw the layer objects.

          \note This method consider that the given layer contains vetorial data.
        */
        void drawLayerGrouping(AbstractLayer* layer,
                               const std::string& geomPropertyName,
                               Canvas* canvas,
                               const te::gm::Envelope& bbox,
                               int srid);
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYERRENDERER_H

