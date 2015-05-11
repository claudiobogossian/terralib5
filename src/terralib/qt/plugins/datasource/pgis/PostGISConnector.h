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
  \file terralib/qt/plugins/datasource/pgis/PostGISConnector.h

  \brief PostGIS connector implementation for the Qt data source widget.
*/

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_PGIS_INTERNAL_POSTGISCONNECTOR_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_PGIS_INTERNAL_POSTGISCONNECTOR_H

//! TerraLib
#include "../../../widgets/datasource/connector/AbstractDataSourceConnector.h"

//! Qt
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace pgis
      {
        /*!
          \class PostGISConnector

          \brief PostGIS connector implementation for the Qt data source widget.
        */
        class PostGISConnector : public te::qt::widgets::AbstractDataSourceConnector
        {
          public:

            PostGISConnector(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~PostGISConnector();

            void connect(std::list<te::da::DataSourceInfoPtr>& datasources);

            void create(std::list<te::da::DataSourceInfoPtr>& datasources);

            void update(std::list<te::da::DataSourceInfoPtr>& datasources);

            void remove(std::list<te::da::DataSourceInfoPtr>& datasources);
        }; 
      } // end namespace pgis
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_DATASOURCE_PGIS_INTERNAL_POSTGISCONNECTOR_H

