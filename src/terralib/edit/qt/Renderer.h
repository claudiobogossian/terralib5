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
\file terralib/edit/qt/Renderer.h

\brief This is a singleton for rendering geometries and features.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_RENDERER_H
#define __TERRALIB_EDIT_QT_INTERNAL_RENDERER_H

// TerraLib
#include "../../common/Singleton.h"
#include "../../geometry/Enums.h"
#include "Config.h"

// Qt
#include <QColor>
#include <QPaintDevice>
#include <QString>

// STL
#include <string>
#include <vector>

namespace te
{
  namespace gm
  {
    class Envelope;
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
    /*!
    \class Renderer

    \brief This is a singleton for rendering geometries and features.
    */
    class TEEDITQTEXPORT Renderer : public te::common::Singleton<Renderer>
    {
      friend class te::common::Singleton<Renderer>;

    public:

      void begin(QPaintDevice* device, const te::gm::Envelope& e, int srid);

      void drawRepositories(const te::gm::Envelope& e, int srid);

      void drawRepository(const std::string& source, const te::gm::Envelope& e, int srid);

      void prepare(te::gm::GeomType type);

      void draw(te::gm::Geometry* geom, bool showVertexes = false);

      void drawVertexes(te::gm::Geometry* geom);

      void drawVertexes(const std::vector<te::gm::LineString*>& lines);

      void drawVertexes(te::gm::LineString* line);

      void end();

      void setPolygonStyle(const QColor& fillColor, const QColor& contourColor, const std::size_t& contourWidth);

      void setPointStyle(const QString& mark, const QColor& fillColor, const QColor& contourColor,
        const std::size_t& contourWidth, const std::size_t& size);

    protected:

      /*! \brief It initializes the singleton instance of the renderer. */
      Renderer();

      /*! \brief Singleton destructor. */
      ~Renderer();

    private:

      void setupDefaultStyle();

    private:

      te::qt::widgets::Canvas* m_canvas;
      int m_srid;

      QColor m_polygonFillColor;
      QColor m_polygonContourColor;
      std::size_t m_polygonContourWidth;

      QColor m_lineColor;
      std::size_t m_lineWidth;

      QString m_pointMark;
      QColor m_pointFillColor;
      QColor m_pointContourColor;
      std::size_t m_pointContourWidth;
      std::size_t m_pointSize;

      te::gm::GeomType m_currentGeomType;
      bool m_styleChanged;
    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_RENDERER_H
