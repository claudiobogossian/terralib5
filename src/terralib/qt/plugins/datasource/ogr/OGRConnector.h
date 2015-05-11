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
  \file terralib/qt/plugins/datasource/ogr/OGRConnector.h

  \brief OGR connector implementation for the Qt data source widget.
*/

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_OGR_INTERNAL_OGRCONNECTOR_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_OGR_INTERNAL_OGRCONNECTOR_H

// TerraLib include files
#include "../../../widgets/datasource/connector/AbstractDataSourceConnector.h"

// Qt include files
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace ogr
      {
        /*!
          \class OGRConnector

          \brief OGR connector implementation for the Qt data source widget.
        */
        class OGRConnector : public te::qt::widgets::AbstractDataSourceConnector
        {
          public:

            OGRConnector(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~OGRConnector();

            void connect(std::list<te::da::DataSourceInfoPtr>& datasources);

            void create(std::list<te::da::DataSourceInfoPtr>& datasources);

            void update(std::list<te::da::DataSourceInfoPtr>& datasources);

            void remove(std::list<te::da::DataSourceInfoPtr>& datasources);
        }; 
      } // end namespace ogr
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_DATASOURCE_OGR_INTERNAL_OGRCONNECTOR_H

