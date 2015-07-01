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
  \file DataSetTableVerticalHeader.h

  \brief Defines a vertical header for a dataset table view.
*/
#ifndef __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVERTICALHEADER_H
#define __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVERTICALHEADER_H

#include <QHeaderView>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class DataSetTableView;

      /*!
        \class DataSetTableVerticalHeader

        \brief A vertical header used for selecting rows at a table view.
      */
      class DataSetTableVerticalHeader : public QHeaderView
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.
          */
          DataSetTableVerticalHeader(DataSetTableView* view);

          /*!
            \brief Destructor.
          */
          ~DataSetTableVerticalHeader();

          void mousePressEvent(QMouseEvent* evt);

        signals:

          /*!
            \brief Emitted when user clicks mouse on a row.
          */
          void selectedRow(const int& row, const bool& add);

          /*!
            \brief Emitted when mouse clicks on row with SHIFT key button pressed.
          */
          void selectedRows(const int& initRow, const int& finalRow);

        protected:

          int m_initRow;              //!< Init row.

          DataSetTableView* m_view;   //!< Table view.
       };
    }
  }
}

#endif // __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVERTICALHEADER_H

