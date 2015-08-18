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
\file terralib/edit/Utils.h

\brief Utility functions for TerraLib Edit module.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_UTILS_H
#define __TERRALIB_EDIT_INTERNAL_UTILS_H

// TerraLib
#include "../maptools/AbstractLayer.h"
#include "Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
  // Forward declarations
  namespace da
  {
    class ObjectId;
  }

  namespace gm
  {
    struct Coord2D;
    class Envelope;
    class Geometry;
    class GeometryCollection;
    class LineString;
    class Polygon;
  }

  namespace edit
  {
    // Forward declaration
    class Feature;

    struct VertexIndex
    {
      void setIndex(const std::size_t& line, const std::size_t& pos)
      {
        m_line = line;
        m_pos = pos;
      }

      void makeInvalid()
      {
        m_line = std::string::npos;
        m_pos = std::string::npos;
      }

      bool isValid() const
      {
        return m_line != std::string::npos && m_pos != std::string::npos;
      }

      std::size_t m_line;
      std::size_t m_pos;
    };

    /*!
    \enum OperationsType

    \brief Defines the operations (CUD - Create, Update ad Remove) to this tool.
    */
    enum OperationType
    {
      GEOMETRY_CREATE,       /*!< New Features        */
      GEOMETRY_UPDATE,       /*!< Features to update. */
      GEOMETRY_DELETE,       /*!< Features to delete. */
      NumberOfOperationTypes /*!< Size of Enum */
    };

    TEEDITEXPORT Feature* PickFeature(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& env, int srid, OperationType operation);

    TEEDITEXPORT void GetLines(te::gm::Geometry* geom, std::vector<te::gm::LineString*>& lines);

    TEEDITEXPORT void GetLines(te::gm::GeometryCollection* gc, std::vector<te::gm::LineString*>& lines);

    TEEDITEXPORT void GetLines(te::gm::Polygon* p, std::vector<te::gm::LineString*>& lines);

    TEEDITEXPORT void GetLines(te::gm::LineString* l, std::vector<te::gm::LineString*>& lines);

    TEEDITEXPORT void MoveVertex(std::vector<te::gm::LineString*>& lines, const VertexIndex& index, const double& x, const double& y);

    TEEDITEXPORT void RemoveVertex(std::vector<te::gm::LineString*>& lines, const VertexIndex& index);

    TEEDITEXPORT void AddVertex(std::vector<te::gm::LineString*>& lines, const double& x, const double& y, const te::gm::Envelope& env, int srid);

    TEEDITEXPORT VertexIndex FindSegment(std::vector<te::gm::LineString*>& lines, const te::gm::Envelope& env, int srid);

    TEEDITEXPORT void MoveGeometry(te::gm::Geometry* geom, const double& deltax, const double& deltay);

    TEEDITEXPORT bool IsSpecialRingVertex(te::gm::LineString* l, const VertexIndex& index);

    TEEDITEXPORT double GetDistance(const te::gm::Coord2D& c1, const te::gm::Coord2D& c2);

    TEEDITEXPORT void GetCoordinates(te::gm::Geometry* geom, std::vector<te::gm::Coord2D>& coords);

    TEEDITEXPORT void TrySnap(te::gm::Coord2D& coord, int srid);

    TEEDITEXPORT te::da::ObjectId* GenerateId();

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_UTILS_H
