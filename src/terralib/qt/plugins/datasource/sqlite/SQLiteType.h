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
  \file terralib/qt/widgets/datasource/connector/sqlite/SQLiteType.h

  \brief SQLite data source type.
*/

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_SQLITE_INTERNAL_SQLITETYPE_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_SQLITE_INTERNAL_SQLITETYPE_H

// TerraLib
#include "../../../widgets/datasource/core/DataSourceType.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace sqlite
      {
        class SQLiteType : public te::qt::widgets::DataSourceType
        {
          public:

            SQLiteType();

            ~SQLiteType();

            bool hasDatabaseSupport() const { return false; }

            bool hasFileSupport() const { return false; }

            bool hasRasterSupport() const { return false; }

            bool hasVectorialSupport() const { return false; }

            std::string getName() const;

            std::string getTitle() const;

            std::string getDescription() const;

            QWidget* getWidget(int widgetType, QWidget* parent = 0, Qt::WindowFlags f = 0) const;

            QIcon getIcon(int iconType) const;
        };
      }   // end namespace sqlite
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_DATASOURCE_SQLITE_INTERNAL_SQLITETYPE_H
