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
  \file terralib/ws/ogc/wms/qt/WMSLayerSelectorDialog.h

  \brief A dialog window to show GetCapabilities data from a WMS Server.
*/

#ifndef __TERRALIB_WS_OGC_WMS_QT_WMSLAYERSELECTORDIALOG_H
#define __TERRALIB_WS_OGC_WMS_QT_WMSLAYERSELECTORDIALOG_H


//TerraLib
#include "Config.h"

#include "../client/DataTypes.h"
#include "../../../../qt/widgets/dataset/explorer/DataSetTreeView.h"
#include "../client/WMSClient.h"
#include "WMSLayerDisplay.h"
#include "WMSLayerItem.h"

#ifndef Q_MOC_RUN
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#endif

//QT
#include <QDialog>
#include <QTreeWidgetItem>

// STL
#include <memory>

namespace Ui { class  WMSLayerSelectorDialogForm; }

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {
        namespace qt
        {


        /*!
          \class WMSLayerSelectorDialog

          \brief A dialog window to show GetCapabilities data from a WMS Server.
        */
        class TEOGCWMSQTEXPORT WMSLayerSelectorDialog : public QDialog
        {
          Q_OBJECT

        public:

          WMSLayerSelectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~WMSLayerSelectorDialog();


          void set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems = false);

          std::vector<te::ws::ogc::wms::WMSGetMapRequest> getCheckedRequests();

        public slots:

          void currentLayerChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

          void okPushButtonPressed();

          void imgFormatIndexChanged(const int& index);

          void timeDimIndexChanged(const int& index);

          void styleIndexChanged(const int& index);

          void crsIndexChanged(const int& index);

          void transparencyStateChanged(const int& state);

          void checkLayerItemChanged(QTreeWidgetItem* item, int column);

        private:

          void refreshLayerInfoTree(const te::ws::ogc::wms::Layer& currentLayer);

          void refreshLayerProperties();

          void refreshMapPreview();

          std::vector<WMSLayerItem*> getCheckedLayerItens();

        private:

          std::unique_ptr<Ui::WMSLayerSelectorDialogForm>   m_ui;
          te::ws::ogc::wms::Layer                           m_rootLayer;
          std::unique_ptr<te::ws::ogc::WMSClient>           m_client;
          te::da::DataSourceInfoPtr                         m_datasourceInfo;
          te::da::DataSourcePtr                             m_datasource;
          WMSLayerItem*                                     m_currentLayerItem;
          std::unique_ptr<WMSLayerDisplay>                  m_mapPreview;
          bool                                              m_refreshingProperties;

        };

        }
      }
    }
  }
}
#endif // __TERRALIB_WS_OGC_WMS_QT_WMSLAYERSELECTORDIALOG_H
