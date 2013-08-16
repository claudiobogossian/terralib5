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
  \file terralib/maptools/Utils.h

  \brief Utility functions for MapTools module.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_UTILS_H
#define __TERRALIB_MAPTOOLS_INTERNAL_UTILS_H

// TerraLib
#include "../color/RGBAColor.h"
#include "../geometry/Envelope.h"
#include "AbstractLayer.h"
#include "Config.h"

// STL
#include <list>
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
    class DataSetType;
    class DataSourceTransactor;
  }

  namespace rst
  {
    class Raster;
    class RasterProperty;
  }

  namespace se
  {
    class CoverageStyle;
    class FeatureTypeStyle;
    class Fill;
    class ParameterValue;
    class RasterSymbolizer;
    class Stroke;
  }

  namespace map
  {
// Forward declaration
    class DataSetLayer;

    /*!
      \brief Gets the RGBA color from Stroke element.

      \param stroke The Symbology Enconding Stroke element.
      \param color  A pre-created color that will be filled with the values extracted from Stroke.

      \note The given color will be altered based on "stroke" and "stroke-opacity" attributes, if availables.
    */
    TEMAPEXPORT void GetColor(const te::se::Stroke* stroke, te::color::RGBAColor& color);

    /*!
      \brief Gets the RGBA color from Fill element.

      \param fill  The Symbology Enconding Fill element.
      \param color A pre-created color that will be filled with the values extracted from Fill.

      \note The given color will be altered based on "fill" and "fill-opacity" attributes, if availables.
    */
    TEMAPEXPORT void GetColor(const te::se::Fill* fill, te::color::RGBAColor& color);

    /*!
      \brief Gets the RGBA color from parameter values.

      \param color   The Symbology Enconding Parameter Value element that represents the color.
      \param opacity The Symbology Enconding Parameter Value element that represents the opacity.
      \param color A pre-created color that will be filled with the values extracted from color and opacity.

      \note The given color will be altered based on "color" and "opacity" parameters, if availables.
      \note Warning: It consider that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TEMAPEXPORT void GetColor(const te::se::ParameterValue* color, const te::se::ParameterValue* opacity, te::color::RGBAColor& rgba);

    /*!
      \brief Gets the parameter value as RGBA color.

      \param param The Symbology Enconding Parameter Value element.

      \return The RGBA color.

      \note Warning: It consider that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TEMAPEXPORT te::color::RGBAColor GetColor(const te::se::ParameterValue* param);

    /*!
      \brief Gets the parameter value as integer.

      \param param The Symbology Enconding Parameter Value element.

      \return The integer value.

      \note Warning: It consider that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TEMAPEXPORT int GetInt(const te::se::ParameterValue* param);

    /*!
      \brief Gets the parameter value as double.

      \param param The Symbology Enconding Parameter Value element.

      \return The double value.

      \note Warning: It consider that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TEMAPEXPORT double GetDouble(const te::se::ParameterValue* param);

    /*!
      \brief Gets the parameter value as string.

      \param param The Symbology Enconding Parameter Value element.

      \return The string value.

      \note Warning: It consider that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TEMAPEXPORT std::string GetString(const te::se::ParameterValue* param);

    /*!
      \brief Converts a dasharray pattern coded by a string to a vector of double.

      \param dasharray The dasharray pattern coded by a string.
      \param style     The vector of double that will be filled with the values extracted from dasharray string.

      \note The dasharray should be specified as an even number of positive values separated by blank space. i.e "10 5 3 4"
            Here, the values "10" and "3" (in uom) are dashes and the values "5" and "4" are spaces (in uom).
            The given vector style will be filled with the values: style[0] = 10; style[1] = 5; style[2] = 3; style[3] = 4.
    */
    TEMAPEXPORT void GetDashStyle(const std::string& dasharray, std::vector<double>& style);

    /*!
      \brief It gets the raster property referenced by the given data set layer.

      \param layer The data set layer that will be used to retrieve the raster property.

      \return The raster property referenced by the data set layer.

      \note The caller will take the ownership of the returned pointer.

      \exception Exception It will throw an exception if the raster property could not be retrieved.
    */
    TEMAPEXPORT te::rst::RasterProperty* GetRasterProperty(DataSetLayer* layer);

    /*!
      \brief It gets the raster referenced by the given data set layer.

      \param layer The data set layer that will be used to retrieve the raster.

      \return The raster referenced by the data set layer.

      \note The caller will take the ownership of the returned pointer.

      \exception Exception It will throw an exception if the raster could not be retrieved.
    */
    TEMAPEXPORT te::rst::Raster* GetRaster(DataSetLayer* layer);

    /*!
      \brief It gets the visible layers of the given layer list.

      \param layers        The layer list that will be consulted.
      \param visibleLayers A layer list that will be filled with the visible layers.
    */
    TEMAPEXPORT void GetVisibleLayers(const std::list<te::map::AbstractLayerPtr>& layers, std::list<te::map::AbstractLayerPtr>& visibleLayers);

    /*!
      \brief It gets the visible layers of the given layer.

      \param layer         The layer that will be consulted.
      \param visibleLayers A layer list that will be filled with the visible layers.
    */
    TEMAPEXPORT void GetVisibleLayers(const te::map::AbstractLayerPtr& layer, std::list<te::map::AbstractLayerPtr>& visibleLayers);

    /*!
      \brief It calculates the extent of the given layers in the given SRID.

      \param layers       The layer list that will be consulted.
      \param srid         The result extent SRID.
      \param onlyVisibles A flag that indicates if the method should consider only the visible layers.

      \return The extent of the given layers in the given SRID.

      \note This method considers the children of layers.
    */
    TEMAPEXPORT te::gm::Envelope GetExtent(const std::list<te::map::AbstractLayerPtr>& layers, int srid, bool onlyVisibles);

    /*!
      \brief It calculates the extent of the given layer in the given SRID.

      \param layer        The layer that will be consulted.
      \param srid         The result extent SRID.
      \param onlyVisibles A flag that indicates if the method should consider only the visible layers.

      \return The extent of the given layer in the given SRID.

      \note This method considers the children of layer.
    */
    TEMAPEXPORT te::gm::Envelope GetExtent(const  te::map::AbstractLayerPtr& layer, int srid, bool onlyVisibles);

    /*!
      \brief It creates a new In-Memory dataset with the items from the given dataset.

      \param dataset The dataset which will provide the items. Do not pass NULL!

      \return A new In-Memory dataset.

      \note The new dataset will not have a transactor associated to it.

      \note To copy all items the caller is responsible for informing the 
            dataset in the right position. i.e. at before begin.

      \note The caller will take the ownership of the returned pointer.
    */
    TEMAPEXPORT te::da::DataSet* DataSet2Memory(te::da::DataSet* dataset);

    /*!
      \brief It draws the data set geometries in the given canvas using the informed SRID and style.

      The informed bounding box (bbox) is used to constraint the data set to be drawn.
      Here, we assume that the given bbox was pre-processed. i.e. the bbox was clipped and contains the same projection of data set geometries.

      \param type        The data set type that describes the data set that will be drawn.
      \param transactor  A transactor that will be used to retrieve the data set objects.
      \param canvas      The canvas were the data set geometries will be drawn.
      \param bbox        The interest area to render the geometries.
      \param bboxSRID    The SRID of interest area.
      \param srid        The SRID to be used to draw the data set geometries.
      \param style       The style that will be used.
    */
    TEMAPEXPORT void DrawGeometries(te::da::DataSetType* type, te::da::DataSourceTransactor* transactor, Canvas* canvas,
                                    const te::gm::Envelope& bbox, int bboxSRID,
                                    int srid, te::se::FeatureTypeStyle* style);

    /*!
      \brief It draws the data set geometries in the given canvas using the informed SRS.

      \param dataset     The data set that will be drawn.
      \param gpos        The geometry property position that will be drawn.
      \param canvas      The canvas were the data set geometries will be drawn.
      \param fromSRID    The SRID of data set geometries.
      \param srid        The SRID to be used to draw the data set geometries.
      \param task        An optional task that can be used cancel the draw process.
    */
    TEMAPEXPORT void DrawGeometries(te::da::DataSet* dataset, const std::size_t& gpos,
                                    Canvas* canvas, int fromSRID, int toSRID, te::common::TaskProgress* task = 0);

    TEMAPEXPORT void DrawRaster(te::da::DataSetType* type, te::da::DataSourceTransactor* transactor, Canvas* canvas,
                                const te::gm::Envelope& bbox, int bboxSRID, const te::gm::Envelope& visibleArea, int srid, te::se::CoverageStyle* style);

    TEMAPEXPORT void DrawRaster(te::rst::Raster* raster, Canvas* canvas, const te::gm::Envelope& bbox, int bboxSRID,
                                const te::gm::Envelope& visibleArea, int srid, te::se::CoverageStyle* style);

  } // end namespace map
}   // end namespace te

#endif // __TERRALIB_MAPTOOLS_INTERNAL_UTILS_H
