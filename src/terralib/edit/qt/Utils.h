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
  \file terralib/edit/qt/Utils.h
   
  \brief Utility Qt functions for TerraLib Edit module.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_QTUTILS_H
#define __TERRALIB_EDIT_INTERNAL_QTUTILS_H

// TerraLib
#include "../../geometry/Coord2D.h"
#include "../Config.h"

// Qt
#include <QMouseEvent>
#include <QPointF>

// STL
#include <vector>

namespace te
{
// Forward declarations
  namespace gm
  {
    class Geometry;
    class LineString;
  }

  namespace qt
  {
    namespace widgets
    {
      class Canvas;
    }
  }

  namespace edit
  {
    TEEDITEXPORT QPointF GetPosition(QMouseEvent* e);

    TEEDITEXPORT void DrawGeometry(te::qt::widgets::Canvas* canvas, te::gm::Geometry* geom, int srid);

    TEEDITEXPORT void DrawVertexes(te::qt::widgets::Canvas* canvas, const std::vector<te::gm::Geometry*>& geom, int srid);

    TEEDITEXPORT void DrawVertexes(te::qt::widgets::Canvas* canvas, te::gm::Geometry* geom, int srid);

    TEEDITEXPORT void DrawVertexes(te::qt::widgets::Canvas* canvas, const std::vector<te::gm::LineString*>& lines, int srid);

    TEEDITEXPORT void DrawVertexes(te::qt::widgets::Canvas* canvas, te::gm::LineString* line, int srid);

    TEEDITEXPORT void DrawVertexes(te::qt::widgets::Canvas* canvas, const std::vector<te::gm::Coord2D>& coords, int fromSRID, int toSRID);

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_QTUTILS_H
