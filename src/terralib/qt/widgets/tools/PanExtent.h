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
  \file terralib/qt/widgets/tools/PanExtent.h

  \brief This class implements a concrete tool to pan a user defined extent.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_PANEXTENT_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_PANEXTENT_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../Config.h"
#include "AbstractTool.h"

// Qt
#include <QPainterPath>
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
        \class PanExtent

        \brief This class implements a concrete tool to pan a user defined extent.
      */
      class TEQTWIDGETSEXPORT PanExtent : public AbstractTool
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a pan tool associated with the given map display and with the specified cursors.

            \param display The map display associated with the tool.
            \param cursor The default tool cursor.
            \param actionCursor An optional cursor to be used during the pan user action. If Qt::BlankCursor, it will be NOT modified.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          PanExtent(MapDisplay* display, const QCursor& cursor, const QCursor& actionCursor = Qt::BlankCursor, QObject* parent = 0);

          /*! \brief Destructor. */
          ~PanExtent();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mousePressEvent(QMouseEvent* e);

          bool mouseMoveEvent(QMouseEvent* e);

          bool mouseReleaseEvent(QMouseEvent* e);

          void setCurrentExtent(const te::gm::Envelope& e);

        protected:

          void drawCurrentExtent();

        signals:

          void extentMoved(te::gm::Envelope e);

          //@}

        protected:

          bool m_panStarted;                      //!< Flag that indicates if pan operation was started.

          QCursor m_actionCursor;                 //!< An optional cursor to be used during the pan user action.
          QPen m_pen;                             //!< The pen used to draw the path.
          QBrush m_brush;                         //!< The brush used to draw the path.

          te::gm::Envelope m_currentExtent;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_PANEXTENT_H
