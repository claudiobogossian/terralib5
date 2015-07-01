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
  \file terralib/qt/widgets/tools/RubberBand.h

  \brief This is a utility class tool that provides a rectangle that can indicate a boundary.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_RUBBERBAND_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_RUBBERBAND_H

// TerraLib
#include "../Config.h"
#include "AbstractTool.h"

// Qt
#include <QPen>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class MapDisplay;

      /*!
        \class RubberBand

        \brief This class provides a rectangle that can indicate a boundary.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT RubberBand : public AbstractTool
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a rubber band associated with the given map display and with the specified cursor.

            \param display The map display associated with the tool.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          RubberBand(MapDisplay* display, QObject* parent = 0);

          /*! \brief Destructor. */
          virtual ~RubberBand();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          virtual bool mousePressEvent(QMouseEvent* e);

          virtual bool mouseMoveEvent(QMouseEvent* e);

          virtual bool mouseReleaseEvent(QMouseEvent* e);

          //@}

        protected:

          QPoint m_origin;  //!< Origin point on mouse pressed.
          QRectF m_rect;    //!< The boundary rectangle managed by the rubber band.
          QPen m_pen;       //!< The pen used to draw the rubber band shape.
          QBrush m_brush;   //!< The brush used to draw the rubber band shape.

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_RUBBERBAND_H
