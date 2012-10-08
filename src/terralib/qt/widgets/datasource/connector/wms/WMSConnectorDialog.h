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
  \file terralib/qt/widgets/connector/wms/WMSConnectorDialog.h

  \brief ....
*/

#ifndef __TERRALIB_QT_WIDGETS_CONNECTOR_WMS_INTERNAL_WMSCONNECTORDIALOG_H
#define __TERRALIB_QT_WIDGETS_CONNECTOR_WMS_INTERNAL_WMSCONNECTORDIALOG_H

// TerraLib
#include "../../core/DataSource.h"

// STL
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class WMSConnectorDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class WMSConnectorDialog

        \brief ....
      */
      class TEQTWIDGETSEXPORT WMSConnectorDialog : public QDialog
      {
        Q_OBJECT

        public:

          WMSConnectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~WMSConnectorDialog();

          const DataSourcePtr& getDataSource() const;

          void set(const DataSourcePtr& ds);

        public slots:

          void openPushButtonPressed();

          void testPushButtonPressed();

          void helpPushButtonPressed();

        private:

          std::auto_ptr<Ui::WMSConnectorDialogForm> m_ui;
          DataSourcePtr m_datasource;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CONNECTOR_WMS_INTERNAL_WMSCONNECTORDIALOG_H

