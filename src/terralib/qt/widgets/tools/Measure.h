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
  \file terralib/qt/widgets/tools/Measure.h

  \brief This class implements a concrete tool to measure operation (distance, area, and angle).
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_MEASURE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_MEASURE_H

// TerraLib
#include "../../../geometry/Coord2D.h"
#include "../Config.h"
#include "AbstractTool.h"

// Qt
#include <QFont>
#include <QPainterPath>
#include <QPen>

// STL
#include <vector>
#include <string>

namespace te
{
  namespace gm
  {
    class LineString;
    class Point;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class Canvas;
    class MapDisplay;

      /*!
        \class Measure

        \brief This class implements a concrete tool to measure operation (distance, area, and angle).

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT Measure : public AbstractTool
      {
        Q_OBJECT

        public:

          /*!
            \enum MeasureType

            \brief Defines the measure type measured by this tool.
          */
          enum MeasureType
          {
            Distance,  /*!< Distance. */
            Area,      /*!< Area.     */
            Angle      /*!< Angle.    */
          };

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a measure tool associated with the given map display.

            \param display The map display associated with the tool.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
          */
          Measure(MapDisplay* display, const MeasureType& measureType, QObject* parent = 0);

          /*! \brief Destructor. */
          ~Measure();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mousePressEvent(QMouseEvent* e);

          bool mouseMoveEvent(QMouseEvent* e);

          bool mouseReleaseEvent(QMouseEvent* e);

          bool mouseDoubleClickEvent(QMouseEvent* e);

          //@}

        private:

          void drawGeometry();

          void drawLine(Canvas& canvas);

          void drawPolygon(Canvas& canvas);

          void drawText(Canvas& canvas, const std::string& text, te::gm::Point* p);

          void clear();

          double calculateLength(te::gm::LineString* line) const;

          double calculateAngle(te::gm::LineString* line) const;

        private slots:

          void onExtentChanged();

        protected:

          std::vector<te::gm::Coord2D> m_coords;  //!< The coord list managed by the measure tool.
          te::gm::Coord2D m_lastPos;              //!< The last position captured on mouse move event.
          MeasureType m_measureType;              //!< The measure type.
          QPen m_pen;                             //!< The pen used to draw the path.
          QBrush m_brush;                         //!< The brush used to draw the path.
          bool m_isFinished;                      //!< A flag that indicates if the operations was finished.
          std::string m_unit;                     //!< The unit symbol for the measure.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_MEASURE_H
