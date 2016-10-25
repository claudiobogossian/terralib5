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
  \file terralib/qt/widgets/tools/PanMiddleClick.h

  \brief This class implements a concrete tool to geographic pan operation with mouse middle button.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_PANMIDDLECLICK_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_PANMIDDLECLICK_H

// TerraLib
#include "../Config.h"
#include "AbstractTool.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class MapDisplay;

      /*!
        \class PanMiddleClick

        \brief This class implements a concrete tool to geographic pan operation with mouse middle click.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT PanMiddleClick : public AbstractTool
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a pan tool, with mouse middle button, associated with the given map display and with the specified cursors.

            \param display The map display associated with the tool.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          PanMiddleClick(MapDisplay* display, QObject* parent = 0);

          /*! \brief Destructor. */
          ~PanMiddleClick();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mousePressEvent(QMouseEvent* e);

          bool mouseMoveEvent(QMouseEvent* e);

          bool mouseReleaseEvent(QMouseEvent* e);

          //@}

        protected:

          bool m_panStarted;      //!< Flag that indicates if pan operation was started.
          QPoint m_origin;        //!< Origin point on mouse pressed.
          QPoint m_delta;         //!< Difference between pressed point and destination point on mouse move.
          QCursor m_currentCursor; //!< Current cursor to be used in the end of the pan user action.
          QPointF m_referencePoint;   //!< The reference origin point, in World Coordinate System.

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_PANMIDDLECLICK_H
