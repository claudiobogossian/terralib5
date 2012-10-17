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
  \file terralib/qt/widgets/connector/geofile/GeoFileConnectorDialog.h

  \brief ....
*/

#ifndef __TERRALIB_QT_WIDGETS_CONNECTOR_GEOFILE_INTERNAL_GEOFILECONNECTORDIALOG_H
#define __TERRALIB_QT_WIDGETS_CONNECTOR_GEOFILE_INTERNAL_GEOFILECONNECTORDIALOG_H

// TerraLib
#include "../../../../../dataaccess/datasource/DataSource.h"
#include "../../core/DataSource.h"

// STL
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class GeoFileConnectorDialogForm; }

class QListWidgetItem;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class GeoFileConnectorDialog

        \brief ....
      */
      class TEQTWIDGETSEXPORT GeoFileConnectorDialog : public QDialog
      {
        Q_OBJECT

        public:

          GeoFileConnectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~GeoFileConnectorDialog();

          const std::list<DataSourcePtr>& getDataSources() const;

          void set(const std::list<DataSourcePtr>& datasources);

        protected slots:

          void openPushButtonPressed();

          void testPushButtonPressed();

          void helpPushButtonPressed();

          void fileSearchToolButtonPressed();

          void addDatasourceToolButtonPressed();

          void removeDatasourceToolButtonPressed();

          void updateDatasourceToolButtonPressed();

          void dataSourcePressed(QListWidgetItem* item);

        private:

          te::da::DataSourcePtr test();

        private:

          struct FindById
          {
            std::string m_id;

            FindById(const std::string& id)
              : m_id(id)
            {
            }

            bool operator()(const DataSourcePtr& ds) const; 
          };

          std::auto_ptr<Ui::GeoFileConnectorDialogForm> m_ui;
          std::list<DataSourcePtr> m_datasources;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CONNECTOR_GEOFILE_INTERNAL_GEOFILECONNECTORDIALOG_H

