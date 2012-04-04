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

  \brief It is used to paint the rows and columns that are selected.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATAGRIDITEMDELEGATE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATAGRIDITEMDELEGATE_H

// TerraLib
#include "../Config.h"

//Qt
#include <QtGui/QItemDelegate>

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
      /*!
        \class DataGridItemDelegate

        \brief It is used to paint the rows and columns that are selected.

        \sa DataGrid, DataGridView, DataGridModel
       */
      class TEQTWIDGETSEXPORT DataGridItemDelegate : public QItemDelegate
      {
        public:
          /*!
            \brief It constructs a item delegate for the view.

            \parent The parent object.
           */
          DataGridItemDelegate(QObject* parent = 0);
          
          /*! Destructor. */
          ~DataGridItemDelegate();

          /*!
            \brief It sets the data grid operation for the item delegate.

            \parent dataGridOp The data grid operation.
           */
          void setDataGridOperation(te::map::DataGridOperation* dataGridOp);

          /*!
            \brief It renders the delegate using the given painter and style option for the item specified by index.

            \param painter The QPainter object.
            \param option The style option.
            \param index The item index.
           */
          void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

        private:

          te::map::DataGridOperation* m_dataGridOp;  // The data grid associated to this item delegate.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif   // __TERRALIB_QT_WIDGETS_INTERNAL_DATAGRIDITEMDELEGATE_H
