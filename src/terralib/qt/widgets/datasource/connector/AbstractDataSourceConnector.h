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
  \file terralib/qt/widgets/datasource/connector/AbstractDataSourceConnector.h

  \brief A base class for widgets that allows application to create, edit or remove data sources.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_ABSTRACTDATASOURCECONNECTOR_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_ABSTRACTDATASOURCECONNECTOR_H

// TerraLib
#include "../../Config.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"

// Qt
#include <QWidget>

// STL
#include <list>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractDataSourceConnector

        \brief A base class for widgets that allows application to create, edit or remove data sources.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT AbstractDataSourceConnector : public QWidget
      {
        public:

          AbstractDataSourceConnector(QWidget* parent = 0, Qt::WindowFlags f = 0);

          virtual ~AbstractDataSourceConnector();

          virtual void connect(std::list<te::da::DataSourceInfoPtr>& datasources) = 0;

          virtual void create(std::list<te::da::DataSourceInfoPtr>& datasources) = 0;

          virtual void update(std::list<te::da::DataSourceInfoPtr>& datasources) = 0;

          virtual void remove(std::list<te::da::DataSourceInfoPtr>& datasources) = 0;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_ABSTRACTDATASOURCECONNECTOR_H

