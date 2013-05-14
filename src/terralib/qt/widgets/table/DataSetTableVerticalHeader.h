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
  \file DataSetTableVerticalHeader.h

  \brief
*/
#ifndef __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVERTICALHEADER_H
#define __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVERTICALHEADER_H

#include <QtGui/QHeaderView>

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

          void selectedRow(const int& row, const bool& add);

          void selectedRows(const int& initRow, const int& finalRow);

        protected:

          int m_initRow;

          DataSetTableView* m_view;
       };
    }
  }
}

#endif // __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEVERTICALHEADER_H

