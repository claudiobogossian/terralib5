/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSetTableView.h

  \brief A table view for a dataset.
*/


#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEVIEW_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEVIEW_H

#include "../Config.h"

// Qt
#include <QtGui/QTableView>

// Forward declarations
class TablePopupFilter;

namespace te
{
  // Forward declarations
  namespace da
  {
    class DataSet;
    class DataSetType;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declaration
      class DataSetTableModel;
      class HighlightDelegate;

      /*!
        \class

        \brief A customized table view for te::map::AbstractLayer objects. Uses a te::qt::widgets::DataSetModel as its model.

        \note We assume that the layer can return ALWAYS return a te::da::DataSet object with random access of it's values. 
      */
      class TEQTWIDGETSEXPORT DataSetTableView : public QTableView
      {
        Q_OBJECT

        public:
          /*!
            \brief Constructor.

            \param parent Qt widget parent.
          */
          DataSetTableView(QWidget* parent=0);

          /*!
            \brief Virtual destructor.
          */
          virtual ~DataSetTableView();

          /*!
            \brief Updates the data set being visualized.

            Note that this DataSet MUST HAVE random access. The view DOES NOT get the ownership of the pointer.

            \param dset The new data set to be visualized.
          */
          void setDataSet(te::da::DataSet* dset);

          /*!
            \brief Sets the schema of the data set. It is used to define the primary keys and create the ObjectIdSet.

            \param schema The DataSetType to extract keys.
          */
          virtual void setLayerSchema(const te::da::DataSetType* schema);

        public slots:
          
          /*!
            \brief Hides the column at position \a column

            \param column Column to be hidden.
          */
          void hideColumn(const int& column);

          /*!
            \brief Shows the hidden column.

            \param column Column to be presented.
          */
          void showColumn(const int& column);

          /*!
            \brief Shows all hidden columns.
          */
          void showAllColumns();

          /*!
            \brief Shows columns in the original order.
          */
          void resetColumnsOrder();

          /*!
            \brief Used to highlight the data when the mouse is clicked over a row in the table.

            \para row Row to be highlighted.
          */
          void highlightRow(const int& row, const QColor& color);

        protected:

          DataSetTableModel* m_model;       //!< The model to be used.
          TablePopupFilter*  m_popupFilter; //!< The menus popup filter.
          HighlightDelegate* m_delegate;    //!< Delegate used for rendering selected rows.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEVIEW_H
