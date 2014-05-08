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
  \file terralib/qt/plugins/datasource/wfs/WFSConnectorDialog.h

  \brief A dialog window for showing the WFS connector widget.
*/

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_WFS_INTERNAL_WFSCONNECTORDIALOG_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_WFS_INTERNAL_WFSCONNECTORDIALOG_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"

// STL
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class WFSConnectorDialogForm; }

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace wfs
      {
        /*!
          \class WFSConnectorDialog

          \brief A dialog window for showing the WFS connector widget.
        */
        class WFSConnectorDialog : public QDialog
        {
          Q_OBJECT

          public:

            WFSConnectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

            ~WFSConnectorDialog();

            const te::da::DataSourceInfoPtr& getDataSource() const;

            const te::da::DataSourcePtr& getDriver() const;

            void set(const te::da::DataSourceInfoPtr& ds);

          public slots:

            void openPushButtonPressed();

            void testPushButtonPressed();

            void helpPushButtonPressed();

          private:

            void getConnectionInfo(std::map<std::string, std::string>& connInfo) const;

          private:

            std::auto_ptr<Ui::WFSConnectorDialogForm> m_ui;
            te::da::DataSourceInfoPtr m_datasource;
            te::da::DataSourcePtr m_driver;
        };

      } // end namespace wfs
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_PLUGINS_DATASOURCE_WFS_INTERNAL_WFSCONNECTORDIALOG_H

