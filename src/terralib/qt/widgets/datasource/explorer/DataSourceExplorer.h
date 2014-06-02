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
  \file terralib/qt/widgets/datasource/explorer/DataSourceExplorer.h

  \brief A widget designed to explore the data sources of an application.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCEEXPLORER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCEEXPLORER_H

// TerraLib
#include "../../Config.h"

// Qt
#include <QDockWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSourceToolBox;

      /*!
        \class DataSourceExplorer

        \brief A widget designed to explore the data sources of an application.

        \ingroup widgets

        \sa DataSourceToolBox, DataSourceExplorerTreeView, DataSourceExplorerTreeModel
      */
      class TEQTWIDGETSEXPORT DataSourceExplorer : public QDockWidget
      {
        Q_OBJECT

        public:

          DataSourceExplorer(QWidget* parent = 0, Qt::WindowFlags flags = 0);

          ~DataSourceExplorer();

          DataSourceToolBox* getToolBox() const;

        private:

          DataSourceToolBox* m_toolBox;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCEEXPLORER_H
