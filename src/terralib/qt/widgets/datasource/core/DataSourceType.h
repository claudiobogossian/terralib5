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
  \file terralib/qt/widgets/datasource/connector/core/DataSourceType.h

  \brief A base class for the suppported types of data source.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCETYPE_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCETYPE_H

// TerraLib
#include "../../Config.h"

// STL
#include <map>
#include <string>

// Qt
#include <QIcon>
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataSourceType

        \brief A base class for the suppported types of data source.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT DataSourceType
      {
        public:

          enum
          {
            WIDGET_DATASOURCE_CONNECTOR,
            WIDGET_LAYER_SELECTOR,
          };

          enum
          {
            ICON_DATASOURCE_SMALL,
            ICON_DATASOURCE_CONNECTOR
          };

          DataSourceType();

          virtual ~DataSourceType();

          virtual bool hasDatabaseSupport() const = 0;

          virtual bool hasFileSupport() const = 0;

          virtual bool hasRasterSupport() const = 0;

          virtual bool hasVectorialSupport() const = 0;

          virtual std::string getName() const = 0;

          virtual std::string getTitle() const = 0;

          virtual std::string getDescription() const = 0;

          virtual QWidget* getWidget(int widgetType, QWidget* parent = 0, Qt::WindowFlags f = 0) const = 0;

          virtual QIcon getIcon(int iconType ) const = 0;

          bool operator<(const DataSourceType& rhs) const;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCETYPE_H
