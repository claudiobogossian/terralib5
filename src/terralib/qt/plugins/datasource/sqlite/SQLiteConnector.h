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
  \file terralib/qt/widgets/datasource/connector/sqlite/SQLiteConnector.h

  \brief ...
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_SQLITE_INTERNAL_SQLITECONNECTOR_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_SQLITE_INTERNAL_SQLITECONNECTOR_H

// TerraLib
#include "../core/AbstractDataSourceConnector.h"

// Qt
#include <QtGui/QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class SQLiteConnector

        \brief ....
      */
      class TEQTWIDGETSEXPORT SQLiteConnector : public AbstractDataSourceConnector
      {
        public:

          SQLiteConnector(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~SQLiteConnector();

          void connect(std::list<te::da::DataSourceInfoPtr>& datasources);

          void create(std::list<te::da::DataSourceInfoPtr>& datasources);

          void update(std::list<te::da::DataSourceInfoPtr>& datasources);

          void remove(std::list<te::da::DataSourceInfoPtr>& datasources);
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_SQLITE_INTERNAL_SQLITECONNECTOR_H

