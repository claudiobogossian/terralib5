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
  \file terralib/qt/plugins/datasource/pgis/PostGISConnectorDialog.h

  \brief A dialog window for showing the PostGIS connector widget.
*/

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_PGIS_INTERNAL_POSTGISCONNECTORDIALOG_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_PGIS_INTERNAL_POSTGISCONNECTORDIALOG_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class PostGISConnectorDialogForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace pgis
      {
        /*!
          \class PostGISConnectorDialog

          \brief A dialog window for showing the PostGIS connector widget.
        */
        class PostGISConnectorDialog : public QDialog
        {
          Q_OBJECT

          public:

            PostGISConnectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~PostGISConnectorDialog();

            const te::da::DataSourceInfoPtr& getDataSource() const;

            const te::da::DataSourcePtr& getDriver() const;

            void set(const te::da::DataSourceInfoPtr& ds);

          public slots:

            void openPushButtonPressed();

            void testPushButtonPressed();

            void advancedConnectionOptionsCheckBoxToggled(bool t);

            void passwordLineEditEditingFinished();

          protected:

            std::map<std::string, std::string> getConnectionInfo(bool getPrivateKeys) const;

            void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

            void setDatabasesNames(std::vector<std::string> names);

          private:

            std::auto_ptr<Ui::PostGISConnectorDialogForm> m_ui;
            te::da::DataSourceInfoPtr m_datasource;
            te::da::DataSourcePtr m_driver;
        };

      } // end namespace pgis
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_DATASOURCE_PGIS_INTERNAL_POSTGISCONNECTORDIALOG_H

