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
  \file terralib/qt/af/BaseApplication.h

  \brief A QMainWindow to be used as the basis for TerraLib applications.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H
#define __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H

#include "Config.h"

// TerraLib
#include "../../maptools/AbstractLayer.h"


#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QStatusBar>


//#include <ui_BaseApplicationForm.h>

namespace Ui {
  class BaseApplicationForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ChartDisplayWidget;
      class LayerItemView;
      class MapDisplay;
    }

    namespace af
    {      
      namespace evt
      {
        // Forward declaration
        struct Event;
      }

      class ApplicationController;
      class DataSetTableDockWidget;
      class MapDisplay;
      class LayerExplorer;
      class StyleExplorer;

      class TEQTAFEXPORT BaseApplication : public QMainWindow
      {
        Q_OBJECT

      public:

        BaseApplication(QWidget* parent = 0);

        virtual ~BaseApplication();

        virtual void init(const QString& cfgFile);

        te::qt::widgets::LayerItemView* getLayerExplorer();

        te::qt::widgets::MapDisplay* getMapDisplay();

        te::qt::af::DataSetTableDockWidget* getLayerDock(const te::map::AbstractLayer* layer, const std::vector<te::qt::af::DataSetTableDockWidget*>& docs);

        QDockWidget* getLayerExplorerDock();

      public slots:

        virtual void onApplicationTriggered(te::qt::af::evt::Event* e);

        void onDrawTriggered();

        void onZoomInToggled(bool checked);

        void onZoomOutToggled(bool checked);

        void onPreviousExtentTriggered();

        void onNextExtentTriggered();

        void onPanToggled(bool checked);

        void onZoomExtentTriggered();

        void onInfoToggled(bool checked);

        void onMapRemoveSelectionTriggered();

        void onSelectionToggled(bool checked);

        void onMapSRIDTriggered();

        void onMapSetUnknwonSRIDTriggered();

        void onStopDrawTriggered();

        void onLayerRemoveTriggered();

        void onLayerRenameTriggered();

        void onLayerPropertiesTriggered();

        void onLayerRemoveSelectionTriggered();

        void onLayerSRSTriggered();

        void onLayerRemoveItemTriggered();

        void onLayerFitOnMapDisplayTriggered();

        void onLayerFitSelectedOnMapDisplayTriggered();

        void onLayerPanToSelectedOnMapDisplayTriggered();

        void onFullScreenToggled(bool checked);

        void onLayerExplorerVisibilityChanged(bool visible);

        void onStyleExplorerVisibilityChanged(bool visible);

        void onDisplayDataTableChanged(bool visible);

        void onLayerShowTableTriggered();

        void onLayerTableClose(te::qt::af::DataSetTableDockWidget* wid);

        void onChartDisplayCreated(te::qt::widgets::ChartDisplayWidget* chartDisplay, te::map::AbstractLayer* layer);

      protected slots:

        void onLayerSelectionChanged(const te::map::AbstractLayerPtr& layer);

        void onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr& layer);

      signals:

        void triggered(te::qt::af::evt::Event* e);

      protected:

        virtual void makeDialog();

        virtual void initFramework(const QString& cfgFile);

        virtual void initStatusBar();

        virtual void initActions();

        virtual void initMenus();

        virtual void initSlotsConnections();

        virtual void initAction(QAction*& act, const QString& icon, const QString& name,
          const QString& text, const QString& tooltip,
          bool iconVisibleInMenu, bool isCheckable, bool enabled, QObject* parent);

        QMenuBar* m_menubar;

        QSize m_mapCursorSize;

        //default actions
        QAction* m_mapDraw;
        QAction* m_mapZoomIn;
        QAction* m_mapZoomOut;
        QAction* m_mapZoomArea;
        QAction* m_mapPan;
        QAction* m_mapZoomExtent;
        QAction* m_mapPreviousExtent;
        QAction* m_mapNextExtent;
        QAction* m_mapInfo;
        QAction* m_mapRemoveSelection;
        QAction* m_mapSelection;

        QAction* m_layerShowTable;
        QAction* m_layerRemove;
        QAction* m_layerRename;
        QAction* m_layerRemoveItem;
        QAction* m_layerRemoveObjectSelection;
        QAction* m_layerProperties;
        QAction* m_layerSRS;
        QAction* m_layerFitOnMapDisplay;
        QAction* m_layerFitSelectedOnMapDisplay;
        QAction* m_layerPanToSelectedOnMapDisplay;

        QAction* m_viewDataTable;
        QAction* m_viewLayerExplorer;
        QAction* m_viewStyleExplorer;
        QAction* m_viewFullScreen;

        //main widgets
        ApplicationController* m_app;
        LayerExplorer* m_layerExplorer;
        MapDisplay* m_display;
        StyleExplorer* m_styleExplorer;
        std::vector<te::qt::af::DataSetTableDockWidget*> m_tables;

        //status bar widgets
        QStatusBar* m_statusbar;
        QLabel* m_selected;
        QAction* m_mapSRID;
        QAction* m_mapUnknownSRID;
        QAction* m_mapStopDrawing;
        QLineEdit* m_mapSRIDLineEdit;
        QLineEdit* m_coordinateLineEdit;

      private:
        Ui::BaseApplicationForm* m_ui;
      };
    }
  }
}

#endif // __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H
