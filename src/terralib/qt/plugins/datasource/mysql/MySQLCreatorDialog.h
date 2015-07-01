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
  \file terralib/qt/widgets/connector/mysql/MySQLCreatorDialog.h

  \brief ....
*/

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_MYSQL_INTERNAL_MYSQLCREATORDIALOG_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_MYSQL_INTERNAL_MYSQLCREATORDIALOG_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"

// STL
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class MySQLCreatorDialogForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace mysql
      {
        /*!
          \class MySQLCreationDialog

          \brief ....
        */
        class MySQLCreatorDialog : public QDialog
        {
          Q_OBJECT

          public:

            MySQLCreatorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~MySQLCreatorDialog();

            const te::da::DataSourceInfoPtr& getDataSource() const;

            const te::da::DataSourcePtr& getDriver() const;

          public slots:

            void advancedCreationOptionsCheckBoxToggled(bool t);

            void applyPushButtonPressed();

            void closePushButtonPressed();

            void helpPushButtonPressed();

            void passwordLineEditEditingFinished();

          protected:

            void getConnectionInfo(std::map<std::string, std::string>& connInfo) const;

          private:

            std::auto_ptr<Ui::MySQLCreatorDialogForm> m_ui;
            te::da::DataSourceInfoPtr m_datasource;
            te::da::DataSourcePtr m_driver;
        }; 
      }   // end namespace mysql
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_DATASOURCE_MYSQL_INTERNAL_MYSQLCREATORDIALOG_H

