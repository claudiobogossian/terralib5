/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/core/Utils.h

  \brief Utilitary function for spatial analysis module.
*/

#ifndef __TERRALIB_SA_INTERNAL_UTILS_H
#define __TERRALIB_SA_INTERNAL_UTILS_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../../srs/Config.h"
#include "../Config.h"

// STL
#include <string>

namespace te
{
  // Forward declaration
  namespace da    { class DataSource; }
  namespace dt    { class AbstractData; }
  namespace graph { class AbstractGraph; }

  namespace sa
  {
    // Forward declaration
    class GeneralizedProximityMatrix;

    /*!
      \brief Function used to set a an attribute valeu from a dataset to the vertex objects from a gpm.

      \param gpm  Pointer to gpm that has the graph information.
      \param ds DataSource pointer that has the dataset information.
      \param dataSetName  Dataset name to get the attribute information.
      \param attrLink Attribute name used to link the vertex id to dataset.
      \param attr Attribute name that will be associated to the graph.
      \param dataType The type of the attribute that will be associated.
      \param srid If the new attribute was a geometry type
      
      \return Return the vertex attribute index.
    */
    TESAEXPORT int AssociateGPMVertexAttribute(te::sa::GeneralizedProximityMatrix* gpm, te::da::DataSource* ds, std::string dataSetName, std::string attrLink, std::string attr, int dataType, int srid = TE_UNKNOWN_SRS, int subType = te::gm::UnknownGeometryType);

    /*!
      \brief Function used to create the vertex attribute metadata in the graph of the gpm.

      \param graph  Pointer to the graph associated to the gpm.
      \param attrName Attribute name that will be created.
      \param dataType The data type of the new attribute.
      \param srid If the new attribute was a geometry type
      
      \return Return the vertex attribute index.
    */
    TESAEXPORT int AddGraphVertexAttribute(te::graph::AbstractGraph* graph, std::string attrName, int dataType, int srid = TE_UNKNOWN_SRS, int subType = te::gm::UnknownGeometryType);

    /*!
      \brief Function used to create the edge attribute metadata in the graph of the gpm.

      \param graph  Pointer to the graph associated to the gpm.
      \param attrName Attribute name that will be created.
      \param dataType The data type of the new attribute.
      
      \return Return the edge attribute index.
    */
    TESAEXPORT int AddGraphEdgeAttribute(te::graph::AbstractGraph* graph, std::string attrName, int dataType);

    /*!
      \brief Function used to get the vertex attribute index in the graph of the gpm.

      \param graph  Pointer to the graph associated to the gpm.
      \param attrName Attribute name to be searched.
      \param index The index of the attribute searched.
      
      \return Return true if the attribute was found and false in other case.
    */
    TESAEXPORT bool GetGraphVertexAttrIndex(te::graph::AbstractGraph* graph, std::string attrName, int& index);

    /*!
      \brief Function used to get the edge attribute index in the graph of the gpm.

      \param graph  Pointer to the graph associated to the gpm.
      \param attrName Attribute name to be searched.
      \param index The index of the attribute searched.
      
      \return Return true if the attribute was found and false in other case.
    */
    TESAEXPORT bool GetGraphEdgeAttrIndex(te::graph::AbstractGraph* graph, std::string attrName, int& index);

    /*!
      \brief Function used to get the numeric value from a gpm property.

      \param ad  Pointer to a abstract data that represents a property value from a gpm.
      
      \return Return a double value if its possible.
    */
    TESAEXPORT double GetDataValue(te::dt::AbstractData* ad);

    /*!
      \brief Function used to calculate the distance from a coord to the center of a geometry

      \param geom  Pointer to a geometry
      \param coord  Reference to a coord
      
      \return Return a double value with distance information
      
      \note It's only possible if the geom has a centroid.
    */
    TESAEXPORT double CalculateDistance(te::gm::Geometry* geom, te::gm::Coord2D& coord);

  } // end namespace sa
}   // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_UTILS_H

