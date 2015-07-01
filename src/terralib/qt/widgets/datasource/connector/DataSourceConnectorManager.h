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
  \file terralib/qt/widgets/datasource/connector/DataSourceConnectorManager.h

  \brief A widget that groups datasource identificatin and connection information.
*/

#ifndef __TERRALIB_QT_WIDGETS_CONNECTOR_INTERNAL_DATASOURCECONNECTORMANAGER_H
#define __TERRALIB_QT_WIDGETS_CONNECTOR_INTERNAL_DATASOURCECONNECTORMANAGER_H

// TerraLib
#include "../../Config.h"

// STL
#include <map>
#include <string>

// Qt
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataSourceConnectorManager

        \brief A widget that groups datasource identificatin and connection information.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT DataSourceConnectorManager : public QWidget
      {
        public:

          DataSourceConnectorManager(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DataSourceConnectorManager();

      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CONNECTOR_INTERNAL_DATASOURCECONNECTORMANAGER_H

