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
  \file terralib/qt/widgets/connector/ado/ADOConnectorDialog.h

  \brief ....
*/

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_ADO_INTERNAL_ADOCONNECTORDIALOG_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_ADO_INTERNAL_ADOCONNECTORDIALOG_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class ADOConnectorDialogForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace ado
      {
        /*!
          \class ADOConnectorDialog

          \brief ....
        */
        class ADOConnectorDialog : public QDialog
        {
          Q_OBJECT

          public:

            enum Operation
            {
              CONNECTION,
              UPDATE
            };

          public:

            ADOConnectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0, Operation op = CONNECTION);

            ~ADOConnectorDialog();

            const te::da::DataSourceInfoPtr& getDataSource() const;

            const te::da::DataSourcePtr& getDriver() const;

            void set(const te::da::DataSourceInfoPtr& ds);

          public slots:

            void openPushButtonPressed();

            void testPushButtonPressed();

            void helpPushButtonPressed();

            void searchDatabaseToolButtonPressed();

          protected:

            void getConnectionInfo(std::map<std::string, std::string>& connInfo) const;

            void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

          private:

            std::auto_ptr<Ui::ADOConnectorDialogForm> m_ui;
            te::da::DataSourceInfoPtr m_datasource;
            te::da::DataSourcePtr m_driver;
        }; 
      }   // end namespace ado
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_DATASOURCE_ADO_INTERNAL_ADOCONNECTORDIALOG_H

