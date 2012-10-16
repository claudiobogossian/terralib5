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
  \file terralib/qt/widgets/tools/AbstractTool.h

  \brief Abstract tool concept.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTTOOL_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTTOOL_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtCore/QObject>
#include <QtGui/QCursor>

// Forward declarations
class QMouseEvent;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class MapDisplay;

      /*!
        \class AbstractTool

        \brief This class defines an interface for objects that can receive application events and respond to
               them, according to specific tool behavior. Tools, in general, receives map display interaction events, like
               MouseClick, MouseMove, etc. Tools are created to do some GIS operation using the user interactions.
      */
      class TEQTWIDGETSEXPORT AbstractTool : public QObject
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a tool associated with the given map display and with the specified cursor.

            \param display The map display associated with the tool.
            \param parent The tool's parent.
            \param cursor The default tool cursor.

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          AbstractTool(MapDisplay* display, QObject* parent = 0, const QCursor& cursor = Qt::BlankCursor);

          /*! \brief Destructor. */
          virtual ~AbstractTool();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          /*! \brief It initializes the tool. */
          virtual void initialize() = 0;

          /*! \brief It finalizes the tool. */
          virtual void finalize() = 0;

          /*!
            \biref Filters events if this object has been installed as an event filter for the watched object.

            \param watched The watched object.
            \param event The last event sent to watched object.
            
            \note In your reimplementation of this function, if you want to filter the event out, 
                  i.e. stop it being handled further, return true; otherwise return false.

            \note Here, the default implementation looks for QMouseEvents and call the pure virutal methods
                  defined on AbstractTool class, e.g. mousePressEvent().
          */
          virtual bool eventFilter(QObject* watched, QEvent* e);

          /*!
            \biref This event handler can be reimplemented in a concrete tool class
                   to receive mouse press events for the watched object.
            
            \param e The mouse event.

            \return If you want to filter the event out,  i.e. stop it being handled further, return true; otherwise return false.
          */
          virtual bool mousePressEvent(QMouseEvent* e);

          /*!
            \biref This event handler can be reimplemented in a concrete tool class
                   to receive mouse move events for the watched object.
            
            \param e The mouse event.

            \return If you want to filter the event out,  i.e. stop it being handled further, return true; otherwise return false.
          */
          virtual bool mouseMoveEvent(QMouseEvent* e);

          /*!
            \biref This event handler can be reimplemented in a concrete tool class
                   to receive mouse release events for the watched object.

            \param e The mouse event.

            \return If you want to filter the event out,  i.e. stop it being handled further, return true; otherwise return false.
          */
          virtual bool mouseReleaseEvent(QMouseEvent* e);

          /*!
            \biref This event handler can be reimplemented in a concrete tool class
                   to receive mouse double click events for the watched object.

            \param e The mouse event.

            \return If you want to filter the event out,  i.e. stop it being handled further, return true; otherwise return false.

            \note The AbstractTool will also receive mouse press and mouse release events in addition to the double click event.
                  It is up to the developer to ensure that the application interprets these events correctly.
          */
          virtual bool mouseDoubleClickEvent(QMouseEvent* e);

          //@}

        private:

          /** @name Copy Constructor and Assignment Operator
           *  Copy constructor and assignment operator not allowed.
           */
          //@{

          /*!
            \brief Copy constructor not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.
          */
          AbstractTool(const AbstractTool& rhs);

          /*!
            \brief Assignment operator not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.

            \return A reference to this object.
          */
          AbstractTool& operator=(const AbstractTool& rhs);

          //@}

        protected:

          MapDisplay* m_display; //!< The map display associated with the tool.
          QCursor m_cursor;      //!< The default tool cursor.
          QCursor m_oldCursor;   //!< The cursor that has been used on map display before of this tool. Setted again on tool destruction.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTTOOL_H
