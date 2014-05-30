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
  \file DataSourceToolBox.h

  \brief A tool box to help explorer data sources.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCETOOLBOX_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCETOOLBOX_H

// TerraLib
#include "../../Config.h"

// STL
#include <memory>

// Qt
#include <QToolBox>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class AbstractDataSourceTreeItem;

      /*!
        \class DataSourceTreeView

        \brief A tool box to help explorer data sources.

        \sa DataSourceExplorer, DataSourceTreeView
      */
      class TEQTWIDGETSEXPORT DataSourceToolBox : public QToolBox
      {
        Q_OBJECT

        public:

          DataSourceToolBox(QWidget * parent = 0, Qt::WindowFlags f = 0);

          ~DataSourceToolBox();

// Nota: o namespace traz varios problemas para os sinais e slots do Qt!
        signals:

          void activated(te::qt::widgets::AbstractDataSourceTreeItem* item);
          
          void clicked(te::qt::widgets::AbstractDataSourceTreeItem* item);

          void doubleClicked(te::qt::widgets::AbstractDataSourceTreeItem* item);

          void entered(te::qt::widgets::AbstractDataSourceTreeItem* item);
          
          void pressed(te::qt::widgets::AbstractDataSourceTreeItem* item);

        protected slots:

          void itemActivated(AbstractDataSourceTreeItem* item);

          void itemClicked(AbstractDataSourceTreeItem* item);

          void itemDoubleClicked(AbstractDataSourceTreeItem* item);

          void itemEntered(AbstractDataSourceTreeItem* item);

          void itemPressed(AbstractDataSourceTreeItem* item);
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATASOURCETOOLBOX_H
