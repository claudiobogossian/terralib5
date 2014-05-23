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

// Forward declaration
class TablePopupFilter;

namespace te
{
  // Forward declarations
  namespace da
  {
    class DataSet;
    class DataSetType;
    class ObjectIdSet;
    class DataSourceCapabilities;
  }

  namespace gm
  {
    class Envelope;
  }

  namespace map
  {
    class AbstractLayer;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declaration
      class DataSetTableModel;
      class HighlightDelegate;

      /*!
        \class DataSetTableView

        \brief A customized table view for te::map::AbstractLayer objects. Uses a te::qt::widgets::DataSetModel as its model.

        \note We assume that the layer can return ALWAYS return a te::da::DataSet object with random access of it's values.

        \ingroup widgets
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
            \brief Sets the layer to be presented.
          
            \param layer Pointer to the layer to be presented.
          */
          void setLayer(const te::map::AbstractLayer* layer);

          /*!
            \brief Updates the data set being visualized.

            Note that this DataSet MUST HAVE random access. The view DOES TAKE the ownership of the pointer.

            \param dset The new data set to be visualized.
          */
          void setDataSet(te::da::DataSet* dset);

          /*!
            \brief Sets the schema of the data set. It is used to define the primary keys and create the ObjectIdSet.

            \param schema The DataSetType to extract keys.
          */
          virtual void setLayerSchema(const te::da::DataSetType* schema);


          /*!
            \brief Highlights the objects identified by \a oids

            \param oids The identifiers of rows to be highlighted.
          */
          void highlightOIds(const te::da::ObjectIdSet* oids);

          /*!
            \brief Update the color to be used.

            \param color The new color;
          */
          void setHighlightColor(const QColor& color);

        public slots:
          
          /*!
            \name Table slot methods.

            \brief Methods to handle user interaction with table.
          */

          //@{

          /*!
            \brief Creates a new histogram based on the data at position \a column

            \param column Column that provides the data for the histogram.
          */
          void createHistogram(const int& column);

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
            \brief Rename a column of the table.
            \param column Index of the column to be renamed.
          */
          void renameColumn(const int& column);

          /*!
            \brief Changes teh type of a column in the table.
            \param column Index of the column to be changed.
          */
          void retypeColumn(const int& column);

          /*!
            \brief Shows the change column data dialog.
            \param column Index of the column to be updated.
          */
          void changeColumnData(const int& column);

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

            \param row Row to be highlighted.

            \param add True to add to selection, false to new selection.
          */
          void highlightRow(const int& row, const bool& add);

          /*!
            \brief Select all rows from \a initRow to \a finalRow.

            \param initRow the begin row.

            \param finalRow the final row.

            \note It does not matter if \a initRow is less than \a finalRow.
          */
          void highlightRows(const int& initRow, const int& finalRow);

          /*!
            \brief Promotes the highlighted rows.

            The rows highlighted are presented in the begining of the table.

            \param If \a true, scrolls to the top of the table.
          */
          void promote(const bool& scroll=false);

          /*!
            \brief Sort by the selected columns.

            \param asc True for ascendent order, false for descendent.
          */
          void sortByColumns(const bool& asc);

          //@}

          /*!
            \brief Shows or hides the icon sinalizing the columns that identify each row.

            \param visible True for icon visible, false otherwise.
          */
          void setOIdsColumnsVisible(const bool& visible);

          /*!
            \brief Add column to the table.
          */
          void addColumn();

          /*
            \brief Removes a column from the table.
            
            \param column Position of the column to be removed.
          */
          void removeColumn(const int& column);

          /*!
            \brief Enable / disable auto-scroll.

            \param enable True for enable auto-scroll, false for disable it.
          */
          void setAutoScrollEnabled(const bool& enable);

          /*!
            \brief Enable / disable promotion.

            If enabled all selected rows will be moved to te top of the table.

            \param enable \a True to enable promotion, \a false to disable it
          */
          void setPromotionEnabled(const bool& enable);

          /*!
           \brief Saves all editions to the dataset.
           */
          void saveEditions();

        signals:

          /*!
            \brief Emmite when objects was selected.
          */
          void selectOIds(te::da::ObjectIdSet*, const bool&, te::gm::Envelope*);

          void deselectOIds(te::da::ObjectIdSet*);

        protected:

          void removeSelection(const int& initRow, const int& finalRow);

          DataSetTableModel* m_model;       //!< The model to be used.
          TablePopupFilter*  m_popupFilter; //!< The menus popup filter.
          HighlightDelegate* m_delegate;    //!< Delegate used for rendering selected rows.
          const te::map::AbstractLayer* m_layer;  //!< Pointer to the layer being presented.
          bool m_autoScrollEnabled;         //!< Auto scroll enabling.
          bool m_doScroll;                  //!< Flag to force or not scrolling.
          bool m_promotionEnabled;          //!< Promotion enabled.
          te::da::DataSet* m_dset;          //!< Data set being used.
          std::vector<std::string> m_orderby;          //!< Order by columns.
          bool m_orderAsc;                       //!< Flag that sinalizes if the it is sorted in ascending sorting.
          bool m_resetOrder;                //!< Flag that sinalizes if there's is no sort.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEVIEW_H
