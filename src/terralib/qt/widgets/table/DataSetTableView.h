/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QTableView>

namespace te
{
  namespace da { class DataSet; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataSetTableView

        \brief A tree view for the data sources of an application.

        \sa DataSetTableModel
      */
      class TEQTWIDGETSEXPORT DataSetTableView : public QTableView
      {
        Q_OBJECT

        public:

          DataSetTableView(QWidget* parent = 0);

          ~DataSetTableView();

          void set(te::da::DataSet* dataset);
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEVIEW_H
