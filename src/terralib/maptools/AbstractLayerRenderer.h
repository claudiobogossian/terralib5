/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
#include "../color/RGBAColor.h"
#include "../geometry/Coord2D.h"
#include "../sam/rtree/Index.h"
#include "AbstractRenderer.h"
#include "WorldDeviceTransformer.h"

// STL
#include <string>
#include <vector>

namespace te
{
// Forward declarations
  namespace common
  {
    class TaskProgress;
  }

  namespace da
  {
    class DataSet;
  }

  namespace gm
  {
    class Geometry;
  }

  namespace se
  {
    class FeatureTypeStyle;
  }

  namespace map
  {
// Forward declaration
    class Chart;

    /*!
      \class AbstractLayerRenderer

      \brief It renders the objects associated to an abstract layer. i.e. a generic renderer.

      \ingroup map

      \sa AbstractLayer, AbstractRenderer
    */
    class TEMAPEXPORT AbstractLayerRenderer : public AbstractRenderer
    {
      public:

        /*! Constructor */
        AbstractLayerRenderer();

        /*! \brief Destructor. */
        ~AbstractLayerRenderer();

        void draw(AbstractLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, int srid, const double& scale, bool* cancel);

      protected:

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
          \param scale             The scale used to draw the layer.

          \note This method consider that the given layer contains vetorial data.
        */
        void drawLayerGeometries(AbstractLayer* layer,
                                 const std::string& geomPropertyName,
                                 te::se::FeatureTypeStyle* style,
                                 Canvas* canvas,
                                 const te::gm::Envelope& bbox,
                                 int srid,
                                 const double& scale, bool* cancel);

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
                               int srid, bool* cancel);

        /*!
          \brief It draws the grouping of the abstract layer in the given canvas using the SRS informed.

          The informed bounding box (bbox) is used to constraint the layer objects to be drawn.
          Here, we assume that the given bbox was pre-processed. i.e. the bbox was clipped and contains the same projection of abstract layer.

          \param layer             The abstract layer that will be drawn.
          \param geomPropertyName  The geometry property name that will be drawn.
          \param canvas            The canvas were the layer objects will be drawn.
          \param bbox              The interest area to render the map.
          \param srid              The SRS to be used to draw the layer objects.
          \param scale             The scale used to draw the layer.

          \note This method consider that the given layer contains vetorial data.

          \note This method retrieves the layer data using only spatial extent restriction and performs the grouping in memory.
        */
        void drawLayerGroupingMem(AbstractLayer* layer,
                                  const std::string& geomPropertyName,
                                  Canvas* canvas,
                                  const te::gm::Envelope& bbox,
                                  int srid,
                                  const double& scale, bool* cancel);

        /*!
          \brief It draws the data set geometries in the given canvas using the informed SRS.

          \param dataset     The data set that will be drawn.
          \param gpos        The geometry property position that will be drawn.
          \param canvas      The canvas were the data set geometries will be drawn.
          \param fromSRID    The SRID of data set geometries.
          \param srid        The SRID to be used to draw the data set geometries.
          \param chart       Chart informations that can be used.
          \param task        An optional task that can be used cancel the draw process.
        */
        void drawDatSetGeometries(te::da::DataSet* dataset, const std::size_t& gpos,
                                  Canvas* canvas, int fromSRID, int toSRID, Chart* chart, bool* cancel, te::common::TaskProgress* task = 0);

        void buildChart(Chart* chart, te::da::DataSet* dataset, te::gm::Geometry* geom);

        void reset();

      protected:

        WorldDeviceTransformer m_transformer;              // World Device Transformer.
        te::sam::rtree::Index<std::size_t, 8> m_rtree;     // r-Tree that can be used to avoid conflicts (charts, texts, etc.).
        std::size_t m_index;                               // Unsigned int used as r-Tree index.
        std::vector<te::color::RGBAColor**> m_chartImages; // The generated chart images.
        std::vector<te::gm::Coord2D> m_chartCoordinates;   // The generated chart coordinates.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYERRENDERER_H
