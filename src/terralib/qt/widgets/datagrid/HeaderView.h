/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file HeaderView.h

  \brief It is used to display the data contents of a dataset.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HEADERVIEW_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HEADERVIEW_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QHeaderView>

// Forward declarations
class QMenu;

namespace te
{
// Forward declarations
  namespace map
  {
    class DataGridOperation;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class DataGridItemDelegate;

      /*!
        \class HeaderView

        \brief It is a reimplementation of the horizontal or vertical QHeaderView of a QTableView.

        \sa DataGridView, DataGridModel, te::map::DataGrid
       */
      class TEQTWIDGETSEXPORT HeaderView : public QHeaderView
      {
        Q_OBJECT

        public:

          /*!
            \brief It constructs a header view with the given parent widget.

            \param orientation (horizontal or vertical)
            \param parent The parent object.
           */
          HeaderView(Qt::Orientation orientation, QWidget* parent = 0);

          /*! \brief Destructor. */
          ~HeaderView();

          /*!
            \brief It sets the data grid associated to the table view contents.

            \param dataGrid The data grid.
           */
          void setDataGridOperation(te::map::DataGridOperation* dataGridOp);

          /*!
            \brief It gets the visual column when the right mouse button is clicked in a horizontal header view.

            \return The column number clicked.
           */
          int getContextVisualColumnClicked() const;

          /*!
            \brief It sets the menu associated to the horizontal header.

            \param menu The menu.
            \note The class takes the ownership of the given menu.
           */
          void setHorizontalHeaderMenu(QMenu* menu);

          /*!
            \brief It sets the menu associated to the vertical header.

            \param menu The menu. 
            \note The class takes the ownership of the given menu.
          */
          void setVerticalHeaderMenu(QMenu* menu);

        signals:
          /*!
            \brief This signal is emitted when a column is moved to another position.

            \param fromPos The column original position. 
            \param toPos   The column destination position. 
          */
          void columnMoved(int fromPos, int toPos);

        private slots:

         /*!
            \brief This slot is called when a section is resized.
           */
          void sectionResized(int, int, int);

        protected:

          /*!
            \brief This is a event handler for mouse press events.

            \param e The mouse event. 
           */
          void mousePressEvent(QMouseEvent* e);

          /*!
            \brief This is a event handler for mouse release events.

            \param e The mouse event. 
           */
          void mouseReleaseEvent(QMouseEvent* e);

          /*!
            \brief This is a event handler for context menu events.

            \param e The mouse event. 
           */
          void contextMenuEvent(QContextMenuEvent* e);

        private:

          te::map::DataGridOperation* m_dataGridOp;    //!< The data grid associated to the table view contents.

          bool m_resized;                   //!< True when a header section is resized.

          int m_visualColumnPressed;        //!< The column number when the mouse is pressed.
          int m_contextVisualColumnClicked; //!< The column clicked when the right mouse button is released on the vertical header.

          QMenu* m_horizHeaderMenu;         //!< Horizontal header menu
          QMenu* m_vertHeaderMenu;          //!< Vertical header menu
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HEADERVIEW_H
