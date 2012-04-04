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
  \file DataGridView.h

  \brief It is used to display the data contents of a dataset in a table view.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATAGRIDVIEW_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATAGRIDVIEW_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QTableView>

// Forward declarations
class QHeaderView;
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
      class HeaderView;

      /*!
        \class DataGridView

        \brief It is used to display the data contents of a dataset in a data view.

        \sa DataGridModel
       */
      class TEQTWIDGETSEXPORT DataGridView : public QTableView
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a data grid view with the given parent widget.

            \param parent The parent object.
           */
          DataGridView(QWidget* parent = 0);

          /*! \brief Destructor. */
          ~DataGridView();

          //@}

          /** @name Accessor methods
           *  Methods used to get or set the class attributes.
           */
          //@{

          /*!
            \brief It gets the horizontal header view of this view.

            \return The horizontal header view of this view.
           */
          te::qt::widgets::HeaderView* getHorizontalHeaderView();

          /*!
            \brief It gets the vertical header view of this view.

            \return The vertical header view of this view.
           */
          te::qt::widgets::HeaderView* getVerticalHeaderView();

          /*!
            \brief It sets the menu associated to the viewport header of this view.

            \param menu The menu.
           */
          void setViewportMenu(QMenu* menu);

          //@}

          /** @name General methods
           *  Methods that are related to the data grid view operations.
           */
          //@{

          /*!
            \brief It sets the model for the view.
                   This function will create and set a new selection model,
                   replacing any model that was previously set with setSelectionModel().
                   However, the old selection model will not be deleted as it may be shared between several views.
                   It is recommended to delete the old selection model, if it is no longer required.

            \param model The model to be set as the model for the view.

            \note This method is a reimplementation from QTableView::setModel()
           */
          void setModel(QAbstractItemModel* model);

          /*!
            \brief It updates the selection status of the table view according to the selection status
                   of the rows and columns in memory.
           */
          void updateTableViewSelectionStatus();

          //@}

        protected slots:

          /*!
            \brief This slot is called when the vertical section of the table view 
                   is clicked with the left mouse button.

            \param clickedVisualRow The visual row clicked.
           */
          virtual void rowClicked(int clickedVisualRow);

          /*!
            \brief This slot is called when the horizontal section of the table view
                   is clicked with the left mouse button.

            \param clickedVisualColumn The column clicked.
           */
          void columnClicked(int clickedVisualColumn);

          /*! \brief This slot makes the visual rows that are pointed to take place of the first rows of this view. */
          void promotePointedRows();

          /*! \brief This slot makes the visual rows that are queried to take place of the first rows of this view. */
          void promoteQueriedRows();

          /*! \brief This slot removes all the selections in the viewport of the table view. */
          virtual void removeAllSelections();

          /*! \brief This slot sorts the selected columns in ascendent order. */
          void sortColumnsInAscendentOrder();

          /*! \brief This slot sorts the selected columns in descendent order. */
          void sortColumnsInDescendentOrder();

          /*!
            \brief This slot is called when a visual column is moved to another position on the table view. 

            \param fromPos The original column position.
            \param toPos   The destination column position.
           */
          void columnMoved(int fromPos, int toPos);

        protected:

          /*!
            \brief It sets the sections having the same selection status (deselected or not)
                   in a ordered sequence of sections.

            \param sections The rows or columns (deselected or not) to be put in sequence.

            \return seqPairs The ordered sequence of sections is returned. 
           */
          void getSequencePairs(std::vector<int>& sections, std::vector<std::pair<int, int> >& seqPairs);

          /*!
            \brief This event handler was reimplemented in this class to receive mouse release events for this view.

            \param e The mouse event.
           */
          void mouseReleaseEvent(QMouseEvent* e);

          void sortColumns(std::string order);

        private:
          te::qt::widgets::HeaderView* m_horizHeaderView;  //!< The horizontal header view of this table view.
          te::qt::widgets::HeaderView* m_vertHeaderView;   //!< The horizontal header view of this table view.

          te::map::DataGridOperation* m_dataGridOp;        //!< The data grid associated to this table view.

          te::qt::widgets::DataGridItemDelegate* m_itemDelegate;  //!< The item delegate for this table view.

          QMenu* m_viewportMenu;    //!< The viewport menu of this table view.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATAGRIDVIEW_H
