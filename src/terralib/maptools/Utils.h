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
  \file terralib/maptools/Utils.h

  \brief Utility functions for MapTools module.

  \ingroup map
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_UTILS_H
#define __TERRALIB_MAPTOOLS_INTERNAL_UTILS_H

// TerraLib
#include "../color/RGBAColor.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../geometry/Enums.h"
#include "../geometry/Envelope.h"
#include "../raster/Raster.h"
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
  }

  namespace rst
  {
    class RasterProperty;
  }

  namespace se
  {
    class CoverageStyle;
    class FeatureTypeStyle;
  }

  namespace map
  {
// Forward declaration
    class DataSetLayer;

    /*!
      \brief It calculates the extent of selected objects of the given layers in the given SRID.

      \param layers       The layer list that will be consulted.
      \param srid         The result extent SRID.
      \param onlyVisibles A flag that indicates if the method should consider only the visible layers.

      \return The extent of selected objects of the given layers in the given SRID.

      \note This method considers the children of layers.
    */
    TEMAPEXPORT te::gm::Envelope GetSelectedExtent(const std::list<te::map::AbstractLayerPtr> layers, int srid, bool onlyVisibles);

    /*!
      \brief It calculates the extent of selected objects of the given layer in the given SRID.

      \param layer        The layer that will be consulted.
      \param srid         The result extent SRID.
      \param onlyVisibles A flag that indicates if the method should consider only the visible layers.

      \return The extent of the selected objects of the given layer in the given SRID.

      \note This method considers the children of layers.
    */
    TEMAPEXPORT te::gm::Envelope GetSelectedExtent(const te::map::AbstractLayerPtr layer, int srid, bool onlyVisibles);

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

    TEMAPEXPORT void GetVisibleLayers(const std::list<te::map::AbstractLayer*>& layers, std::list<te::map::AbstractLayer*>& visibleLayers);

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
      \param ds          A datasource that will be used to retrieve the data set objects.
      \param canvas      The canvas were the data set geometries will be drawn.
      \param bbox        The interest area to render the geometries.
      \param bboxSRID    The SRID of interest area.
      \param srid        The SRID to be used to draw the data set geometries.
      \param style       The style that will be used.
    */
    TEMAPEXPORT void DrawGeometries(te::da::DataSetType* type, te::da::DataSourcePtr ds, Canvas* canvas,
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

    TEMAPEXPORT void DrawRaster(te::da::DataSetType* type, te::da::DataSourcePtr ds, Canvas* canvas,
                                const te::gm::Envelope& bbox, int bboxSRID, const te::gm::Envelope& visibleArea, int srid, te::se::CoverageStyle* style);

    TEMAPEXPORT void DrawRaster(te::rst::Raster* raster, Canvas* canvas, const te::gm::Envelope& bbox, int bboxSRID,
                                const te::gm::Envelope& visibleArea, int srid, te::se::CoverageStyle* style);

    TEMAPEXPORT te::rst::Raster* GetExtentRaster(te::rst::Raster* raster, int w, int h, const te::gm::Envelope& bbox, int bboxSRID,
                                const te::gm::Envelope& visibleArea, int srid);

     /*!
      \brief It gets the geometry type of the given layer.

      \param layer The layer that will be consulted.
    */
    TEMAPEXPORT te::gm::GeomType GetGeomType(const  te::map::AbstractLayerPtr& layer);


    /*!
     \brief It gets the requested envelope on a UTM planar projection

     \param worldBox WordBox to be transformed to planar
     \param srid The WorldBox SRID to transform from
   */
    TEMAPEXPORT te::gm::Envelope GetWorldBoxInPlanar(const te::gm::Envelope& worldBox, int srid);

    /*!
      \brief Returns proj4 string with UTM projection in the specified zone (Only working for south).
      \param UTM Zone
      \return proj4 string
    */
    TEMAPEXPORT std::string GetUTMProj4FromZone(int zone);

    /*!
      \brief Calculates the UTM zone from a Geographic envelope.

      \param latLongBox Geographic envelope
      \return UTM zone
    */
    TEMAPEXPORT int CalculatePlanarZone(te::gm::Envelope latLongBox);

  } // end namespace map
}   // end namespace te

#endif // __TERRALIB_MAPTOOLS_INTERNAL_UTILS_H
