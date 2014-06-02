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
  \file DataSourceTreeView.h

  \brief A tree view for the data sources of an application.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCETREEVIEW_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCETREEVIEW_H

// TerraLib
#include "../../Config.h"

// Qt
#include <QTreeView>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class AbstractDataSourceTreeItem;

      /*!
        \class DataSourceTreeView

        \brief A tree view for the data sources of an application.

        \sa DataSourceTreeViewModel
      */
      class TEQTWIDGETSEXPORT DataSourceTreeView : public QTreeView
      {
        Q_OBJECT

        public:

          DataSourceTreeView(QWidget* parent = 0);

          ~DataSourceTreeView();

        signals:

          void activated(AbstractDataSourceTreeItem* item);

          void clicked(AbstractDataSourceTreeItem* item);

          void doubleClicked(AbstractDataSourceTreeItem* item);

          void entered(AbstractDataSourceTreeItem* item);

          void pressed(AbstractDataSourceTreeItem* item);

          void toggled(AbstractDataSourceTreeItem* item, bool checked);

        protected slots:

          void itemActivated(const QModelIndex & index);

          void itemClicked(const QModelIndex & index);

          void itemDoubleClicked(const QModelIndex & index);

          void itemEntered(const QModelIndex & index);

          void itemPressed(const QModelIndex & index);
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCETREEVIEW_H
