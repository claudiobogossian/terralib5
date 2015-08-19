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

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACTLAYOUT_TOOL_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACTLAYOUT_TOOL_H

// TerraLib
#include "../../../core/Config.h"

// Boost
#include <boost/noncopyable.hpp>

// Qt
#include <QtCore/QObject>
#include <QtGui/QCursor>

// Forward declarations
class QMouseEvent;
class QKeyEvent;

namespace te
{
  namespace layout
  {
    class View;

      class TELAYOUTEXPORT AbstractLayoutTool : public QObject, public boost::noncopyable
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

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          AbstractLayoutTool(View* view, QObject* parent = 0);

          /*! \brief Destructor. */
          virtual ~AbstractLayoutTool();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          /*!
            \brief Filters events if this object has been installed as an event filter for the watched object.

            \param watched The watched object.
            \param event The last event sent to watched object.
            
            \note In your reimplementation of this function, if you want to filter the event out, 
                  i.e. stop it being handled further, return true; otherwise return false.

            \note Here, the default implementation looks for QMouseEvents and call the pure virutal methods
                  defined on AbstractTool class, e.g. mousePressEvent().
          */
          virtual bool eventFilter(QObject* watched, QEvent* e);

          /*!
            \brief This event handler can be reimplemented in a concrete tool class
                   to receive mouse press events for the watched object.
            
            \param e The mouse event.

            \return If you want to filter the event out,  i.e. stop it being handled further, return true; otherwise return false.
          */
          virtual bool mousePressEvent(QMouseEvent* e);

          /*!
            \brief This event handler can be reimplemented in a concrete tool class
                   to receive mouse move events for the watched object.
            
            \param e The mouse event.

            \return If you want to filter the event out,  i.e. stop it being handled further, return true; otherwise return false.
          */
          virtual bool mouseMoveEvent(QMouseEvent* e);

          /*!
            \brief This event handler can be reimplemented in a concrete tool class
                   to receive mouse release events for the watched object.

            \param e The mouse event.

            \return If you want to filter the event out,  i.e. stop it being handled further, return true; otherwise return false.
          */
          virtual bool mouseReleaseEvent(QMouseEvent* e);

          /*!
            \brief This event handler can be reimplemented in a concrete tool class
                   to receive mouse double click events for the watched object.

            \param e The mouse event.

            \return If you want to filter the event out,  i.e. stop it being handled further, return true; otherwise return false.

            \note The AbstractTool will also receive mouse press and mouse release events in addition to the double click event.
                  It is up to the developer to ensure that the application interprets these events correctly.
          */
          virtual bool mouseDoubleClickEvent(QMouseEvent* e);

          virtual bool keyPressEvent(QKeyEvent* e);

          //@}

          /*!
            \brief It sets the tool cursor.

            \param cursor The cursor that will be used during the tool operation.
          */
          virtual void setCursor(const QCursor& cursor);

        protected:

					virtual QCursor createCursor(std::string pathIcon);

          View* m_view; //!< The QGraphicsView associated with the tool.
          QCursor m_cursor;      //!< The default tool cursor.
      };

  }   // end namespace layout
}     // end namespace te

#endif  // __TERRALIB_LAYOUT_INTERNAL_ABSTRACTLAYOUT_TOOL_H
