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
  \file terralib/qt/plugins/datasource/gdal/GDALConnectorDialog.h

  \brief A dialog window for showing the GDAL connector widget.
*/

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_GDALCONNECTORDIALOG_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_GDALCONNECTORDIALOG_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class GDALConnectorDialogForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace gdal
      {
        /*!
          \class GDALConnectorDialog

          \brief A dialog window for showing the GDAL connector widget.
        */
        class GDALConnectorDialog : public QDialog
        {
          Q_OBJECT

          public:

            GDALConnectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~GDALConnectorDialog();

            const te::da::DataSourceInfoPtr& getDataSource() const;

            const te::da::DataSourcePtr& getDriver() const;

            void set(const te::da::DataSourceInfoPtr& ds);

          public slots:

            void openPushButtonPressed();

            void testPushButtonPressed();

            void searchDatasetToolButtonPressed();

          protected:

            void getConnectionInfo(std::map<std::string, std::string>& connInfo) const;

            void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

          private:

            std::auto_ptr<Ui::GDALConnectorDialogForm> m_ui;
            te::da::DataSourceInfoPtr m_datasource;
            te::da::DataSourcePtr m_driver;
        };

      } // end namespace gdal
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_GDALCONNECTORDIALOG_H

