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
  \file LayerExplorer.h

  \brief It can be used to explore the layers belonging to a project in a QTreeView.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_LAYEREXPLORER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_LAYEREXPLORER_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QTreeView>

class QAction;
class QDragMoveEvent;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class LayerExplorer

        \brief It can be used to explore the layers belonging to a project in a QTreeView.

        \sa LayerExplorerModel
       */
      class TEQTWIDGETSEXPORT LayerExplorer : public QTreeView
      {
        Q_OBJECT

        public:

          /*!
            \brief It constructs a tree view with the given parent widget.

            \param parent The parent object.
           */
          LayerExplorer(QWidget* parent = 0);

          /*! \brief Destructor. */
          ~LayerExplorer();

          /*!
            \brief It sets the list of actions to be performed on the context menu.

            \param actionsList The list of actions
          */
          void setActionsList(const QList<QAction*> actionsList);

         /*!
            \brief If the dragged item was the current one, the dropped item will be set to be the current item.

            \param dragIndex The index of the dragged item.
            \param dropIndex The index of the dropped item.
          */
          void updateCurrentIndex(const QModelIndex& dragIndex, const QModelIndex& dropIndex);

        protected:
         /*!
            \brief The event handler that receives widget context menu events.

            \param e The context menu event.
          */
          void contextMenuEvent(QContextMenuEvent* e);

         /*!
            \brief The event handler that receives mouse events.

            \param e The mouse event.
          */
          void mousePressEvent(QMouseEvent* e);

         /*!
            \brief The event handler that receives an event which is sent while a drag and drop action is in progress.

            \param e Event which is sent while a drag and drop action is in progress.
          */
          void dragMoveEvent(QDragMoveEvent* e);

        protected slots:
          /*!
            \brief This slot is called when a mouse button is pressed.
                   The item the mouse was pressed on is specified by index.
                   The slot is only called when the index is valid.

            \param index The item index.
          */
          void pressedItem(const QModelIndex& index);

          /*!
            \brief This slot is called when a mouse button is clicked.
                   The item the mouse was clicked on is specified by index.
                   The slot is only called when the index is valid.

            \param index The item index.
          */
          void clickedItem(const QModelIndex& index);

         /*!
            \brief If the dragged item was the current one, the dropped item will be set to be the current item.

            \param dragIndex The index of the dragged item.
            \param dropIndex The index of the dropped item.
          */
          void dragDropEnded(const QModelIndex& dragIndex, const QModelIndex& dropIndex);

        signals:
          void checkBoxWasClicked(const QModelIndex& index);
          void contextMenuPressed(const QModelIndex& index, const QPoint& pos);

        private:
          QModelIndex m_previousCurrentIndex;         //!< It stores the previous current index.
          bool m_leftMouseButtonWasPressed;           //!< It indicates that a item was pressed with the left mouse button.
          Qt::MouseButton m_mouseButton;              //!< It stores the pressed button.
          QPoint m_mousePos;                          //!< It stores the pressed position.
          QList<QAction*> m_actionsList;              //!< The list of actions associated to the context menu
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_LAYEREXPLORER_H
