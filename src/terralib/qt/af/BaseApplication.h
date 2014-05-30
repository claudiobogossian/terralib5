/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "Config.h"

// STL
#include <map>

// Boost
#include <boost/noncopyable.hpp>

// Qt
#include <QColor>
#include <QLineEdit>
#include <QMainWindow>

class QLabel;

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSet;
  }

  namespace qt
  {
    namespace widgets
    {
      class QueryDialog;
    }

    namespace af
    {
// Forward declarations
      class InterfaceController;
      class LayerExplorer;
      class MapDisplay;
      class Project;
      class DataSetTableDockWidget;
      class StyleExplorer;

      namespace evt
      {
        struct Event;
      }

      /*!
        \class BaseApplication

        \brief A QMainWindow to be used as the basis for TerraLib applications.

        \ingroup af
      */
      class TEQTAFEXPORT BaseApplication : public QMainWindow, public boost::noncopyable
      {
        Q_OBJECT

        public:

          BaseApplication(QWidget* parent = 0);

          virtual ~BaseApplication();

          virtual void init();

          virtual void init(const std::string& configFile);

          MapDisplay* getDisplay();

          virtual void resetState();

          void resetTerraLib(const bool& status);

        protected slots:

          void onApplicationTriggered(te::qt::af::evt::Event* evt);

          void onAddDataSetLayerTriggered();

          void onAddQueryLayerTriggered();

          void onAddTabularLayerTriggered();

          void onRemoveLayerTriggered();

          void onLayerRemoveItemTriggered();

          void onRenameLayerTriggered();

          void onPluginsManagerTriggered();

          void onRecentProjectsTriggered(QAction* proj);

          void onNewProjectTriggered();

          void onOpenProjectTriggered();

          void onSaveProjectTriggered();

          void onSaveProjectAsTriggered();

          void onRestartSystemTriggered();

          void onToolsCustomizeTriggered();

          void onToolsDataExchangerTriggered();

          void onToolsDataExchangerDirectTriggered();

          void onToolsDataExchangerDirectPopUpTriggered();

          void onProjectPropertiesTriggered();

          void onAddFolderLayerTriggered();

          void onLayerPropertiesTriggered();

          void onLayerRemoveSelectionTriggered();

          void onLayerSRSTriggered();

          void onLayerShowTableTriggered();

          void onLayerHistogramTriggered();

          void onLayerScatterTriggered();

          void onLayerChartTriggered();

          void onLayerGroupingTriggered();

          void onLayerFitOnMapDisplayTriggered();

          void onLayerFitSelectedOnMapDisplayTriggered();

          void onLayerPanToSelectedOnMapDisplayTriggered();

          void onQueryLayerTriggered();

          void onMapSRIDTriggered();

          void onMapSetUnknwonSRIDTriggered();

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

          void onMeasureDistanceToggled(bool checked);

          void onMeasureAreaToggled(bool checked);

          void onMeasureAngleToggled(bool checked);

          void onStopDrawTriggered();

          void showProgressDockWidget();

          void onLayerTableClose(te::qt::af::DataSetTableDockWidget* wid);

          void onFullScreenToggled(bool checked);

          void onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr& layer);

          void onHighlightLayerObjects(const te::map::AbstractLayerPtr& layer, te::da::DataSet* dataset, const QColor& color);

          void onLayerExplorerVisibilityChanged(bool visible);

          void onDisplayVisibilityChanged(bool visible);

          void onDisplayDataTableChanged(bool visible);

          void onStyleExplorerVisibilityChanged(bool visible);

          void onDataSourceExplorerTriggered();

          //void onTrajectoryAnimationTriggered(); // Lauro

        protected:

          virtual void openProject(const QString& projectFileName);

          virtual void checkProjectSave();

          virtual void newProject();

          virtual void makeDialog();

          virtual void closeEvent(QCloseEvent* e);

          virtual void initAction(QAction*& act, const QString& icon, const QString& name,
                                  const QString& text, const QString& tooltip,
                                  bool iconVisibleInMenu, bool isCheckable, bool enabled, QObject* parent);

          virtual void initActions();

          virtual void initMenus();

          virtual void initToolbars();

          virtual void initStatusBar();

          virtual void initSlotsConnections();

        signals:
          void applicationClose();


        protected:

          //! Qt components
          QAction* m_viewLayerExplorer;
          QAction* m_viewMapDisplay;
          QAction* m_viewDataTable;
          QAction* m_viewStyleExplorer;
          //QAction* m_editUndo;
          //QAction* m_editRedo;
          //QAction* m_editCut;
          //QAction* m_editCopy;
          //QAction* m_editPaste;
          //QAction* m_editSelectAll;
          //QAction* m_editClear;
          //QAction* m_editFind;
          //QAction* m_editReplace;
          QAction* m_viewFullScreen;
          QAction* m_viewRefresh;
          QAction* m_toolsCustomize;
          QAction* m_toolsDataExchanger;
          QAction* m_toolsDataExchangerDirect;
          QAction* m_toolsDataExchangerDirectPopUp;
          QAction* m_toolsDataSourceExplorer;
          QAction* m_pluginsManager;
          QAction* m_helpContents;
          QAction* m_helpUpdate;
          QAction* m_viewGrid;
          QAction* m_projectAddLayerDataset;
          QAction *m_projectAddLayerQueryDataSet;
          QAction *m_projectAddLayerTabularDataSet;
          QAction* m_projectAddLayerGraph;
          QAction* m_projectAddFolderLayer;
          QAction* m_projectRemoveLayer;
          QAction* m_projectRenameLayer;
          QAction* m_projectProperties;
          QAction* m_layerEdit;
          QAction* m_layerRemoveItem;
          QAction* m_layerExport;
          QAction* m_layerProperties;
          QAction* m_layerSRS;
          QAction* m_layerShowTable;
          QAction* m_layerRaise;
          QAction* m_layerLower;
          QAction* m_layerToTop;
          QAction* m_layerToBottom;
          QAction* m_layerChartsHistogram;
          QAction* m_layerChartsScatter;
          QAction* m_layerChart;
          QAction* m_layerObjectGrouping;
          QAction* m_layerFitOnMapDisplay;
          QAction* m_layerRemoveObjectSelection;
          QAction* m_layerFitSelectedOnMapDisplay;
          QAction* m_layerPanToSelectedOnMapDisplay;
          QAction* m_queryLayer;
          QAction* m_toolsDataSourceManagement;
          QAction* m_helpAbout;
          QAction* m_fileNewProject;
          QAction* m_fileSaveProject;
          QAction* m_fileSaveProjectAs;
          QAction* m_fileOpenProject;
          QAction* m_fileExit;
          QAction* m_filePrint;
          QAction* m_filePrintPreview;
          QAction* m_fileRestartSystem;
          QAction* m_mapSRID;
          QAction* m_mapUnknownSRID;
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
          QAction* m_mapMeasureDistance;
          QAction* m_mapMeasureArea;
          QAction* m_mapMeasureAngle;
          QAction* m_mapStopDrawing;
          QSize m_mapCursorSize;

          QWidget* m_centralwidget;

          QLineEdit* m_mapSRIDLineEdit;
          QLineEdit* m_coordinateLineEdit;

          QMenuBar* m_menubar;
          //QMenu* m_editMenu;
          QMenu* m_viewMenu;
          QMenu* m_viewToolBarsMenu;
          QMenu* m_toolsMenu;
          QMenu* m_toolsExchangerMenu;
          QMenu* m_pluginsMenu;
          QMenu* m_helpMenu;
          QMenu* m_projectMenu;
          QMenu* m_projectAddLayerMenu;
          QMenu* m_layerMenu;
          QMenu* m_fileMenu;
          QMenu* m_recentProjectsMenu;
          QMenu* m_mapMenu;

          QLabel* m_selected;

          QStatusBar* m_statusbar;
          QToolBar* m_fileToolBar;
          //QToolBar* m_editToolBar;
          QToolBar* m_mapToolBar;

          // Well known Widgets
          InterfaceController* m_iController;
          LayerExplorer* m_explorer;  //!< A dockable tree view for the layers in the application project.
          MapDisplay* m_display;
          StyleExplorer* m_styleExplorer;

          std::vector<DataSetTableDockWidget*> m_tableDocks;

          //non modal intefaces
          te::qt::widgets::QueryDialog* m_queryDlg;

          // Project
          Project* m_project;

          QDockWidget* m_progressDockWidget;       //!< Dock widget used to show progress information
          QDockWidget* m_zoomInDisplaysDockWidget; //!< Dock widget used to show zoom in display
          QDockWidget* m_eyeBirdDisplaysDockWidget; //!< Dock widget used to show eye bird display

          bool m_restartTerraLib;
      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H
