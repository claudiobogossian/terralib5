/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terraview/TerraView.h

  \brief The main class of TerraView.
*/

#ifndef __TERRAVIEW_INTERNAL_TERRAVIEW_H
#define __TERRAVIEW_INTERNAL_TERRAVIEW_H

// TerraLib
#include <terralib/qt/af/BaseApplication.h>

// STL
#include <string>

// Forward declarations
class QWidget;
class QMenu;

struct ProjectMetadata;

namespace te
{
  namespace qt
  {
    namespace af
    {
      class DataSetTableDockWidget;
      class InterfaceController;
    }
    namespace widgets
    {
      class ChartDisplayWidget;
      class CompositionModeMenuWidget;
      class HelpManagerImpl;
      class QueryDialog;
    }
  }
}

/*!
  \brief The main class of TerraView.

  \sa te::qt::af::BaseApplication
*/
class TerraView : public te::qt::af::BaseApplication
{
  Q_OBJECT

  public:

    TerraView(QWidget* parent = 0);

    ~TerraView();

    void init();

//    void init(const std::string& configFile);

    void startProject(const QString& projectFileName);

  protected:

    virtual void makeDialog();

    virtual void initActions();

    virtual void initMenus();

    virtual void initSlotsConnections();

    void addMenusActions();

    void initToolbars();

  protected slots:

    void showAboutDialog();

    void onHelpTriggered();

    void onApplicationTriggered(te::qt::af::evt::Event* e);

    void onShowTableTriggered();

    void onLayerTableClose(te::qt::af::DataSetTableDockWidget* wid);

    void onSaveProjectTriggered();

    void onOpenProjectTriggered();

    void onAddDataSetLayerTriggered();

    void onAddQueryLayerTriggered();

    void onAddTabularLayerTriggered();

    void onChartDisplayCreated(te::qt::widgets::ChartDisplayWidget* chartDisplay, te::map::AbstractLayer* layer);

    void onRemoveLayerTriggered();

    void onChangeLayerDataSourceTriggered();

    void onUpdateLayerDataSourceTriggered();

    void onLayerRemoveItemTriggered();

    void onRenameLayerTriggered();

    void onPluginsManagerTriggered();

    void onRecentProjectsTriggered(QAction* proj);

    void onNewProjectTriggered();

    void onSaveProjectAsTriggered();

    void onRestartSystemTriggered();

    void onToolsCustomizeTriggered();

    void onToolsDataExchangerTriggered();

    void onToolsDataExchangerDirectTriggered();

    void onToolsDataExchangerDirectPopUpTriggered();

    void onToolsQueryDataSourceTriggered();

    void onToolsRasterMultiResolutionTriggered();

    void onToolsFixGeometryTriggered();

    void onProjectPropertiesTriggered();

    void onAddFolderLayerTriggered();

    void onLayerPropertiesTriggered();

    void onLayerRemoveSelectionTriggered();

    void onLayerSRSTriggered();

    void onLayerShowTableTriggered();

    void onLayerHistogramTriggered();

    void onLinkTriggered();

    void onLayerScatterTriggered();

    void onLayerChartTriggered();

    void onLayerGroupingTriggered();

    void onLayerFitOnMapDisplayTriggered();

    void onLayerFitSelectedOnMapDisplayTriggered();

    void onLayerPanToSelectedOnMapDisplayTriggered();

    void onLayerCompositionModeTriggered();

    void onQueryLayerTriggered();

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

    void showProgressDockWidget();

    void onFullScreenToggled(bool checked);

    void onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr& layer);

    void onHighlightLayerObjects(const te::map::AbstractLayerPtr& layer, te::da::DataSet* dataset, const QColor& color);

    void onLayerExplorerVisibilityChanged(bool visible);

    void onDisplayDataTableChanged(bool visible);

    void onStyleExplorerVisibilityChanged(bool visible);

    void onDataSourceExplorerTriggered();

    void onCreateNewLayer(te::map::AbstractLayerPtr layer);

  protected:

    void projectChanged();

    void checkAndSaveProject();

    void openProject(const QString& prjFileName);

    void closeEvent(QCloseEvent * event);

  protected:

    QAction* m_fileNewProject;
    QAction* m_fileSaveProject;
    QAction* m_fileSaveProjectAs;
    QAction* m_fileOpenProject;
    QAction* m_fileExit;
    QAction* m_filePrint;
    QAction* m_filePrintPreview;
    QAction* m_fileRestartSystem;
    QAction* m_helpAbout;
    QAction* m_helpContents;
    QAction* m_helpUpdate;
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
    QAction* m_layerLinkTable;
    QAction* m_layerObjectGrouping;
    QAction* m_layerFitOnMapDisplay;
    QAction* m_layerRemoveObjectSelection;
    QAction* m_layerFitSelectedOnMapDisplay;
    QAction* m_layerPanToSelectedOnMapDisplay;
    QAction* m_layerCompositionMode;
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
    QAction* m_pluginsManager;
    QAction* m_projectAddLayerDataset;
    QAction* m_projectAddLayerQueryDataSet;
    QAction* m_projectAddLayerTabularDataSet;
    QAction* m_projectAddLayerGraph;
    QAction* m_projectAddFolderLayer;
    QAction* m_projectChangeLayerDataSource;
    QAction* m_projectUpdateLayerDataSource;
    QAction* m_projectRemoveLayer;
    QAction* m_projectRenameLayer;
    QAction* m_projectProperties;
    QAction* m_queryLayer;
    QAction* m_toolsCustomize;
    QAction* m_toolsDataExchanger;
    QAction* m_toolsDataExchangerDirect;
    QAction* m_toolsDataExchangerDirectPopUp;
    QAction* m_toolsDataSourceExplorer;
    QAction* m_toolsDataSourceManagement;
    QAction* m_toolsQueryDataSource;
    QAction* m_toolsRasterMultiResolution;
    QAction* m_toolsFixGeometry;
    QAction* m_viewDataTable;
    QAction* m_viewGrid;
    QAction* m_viewLayerExplorer;
    QAction* m_viewStyleExplorer;
    QAction* m_viewFullScreen;
    QAction* m_viewRefresh;

    QMenu* m_fileMenu;
    QMenu* m_helpMenu;
    QMenu* m_layerMenu;
    QMenu* m_mapMenu;
    QMenu* m_pluginsMenu;
    QMenu* m_projectMenu;
    QMenu* m_projectAddLayerMenu;
    QMenu* m_recentProjectsMenu;
    QMenu* m_toolsMenu;
    QMenu* m_toolsExchangerMenu;
    QMenu* m_viewMenu;
    QMenu* m_viewToolBarsMenu;

    QSize m_mapCursorSize;

    QDockWidget* m_progressDockWidget;       //!< Dock widget used to show progress information

    te::qt::widgets::HelpManagerImpl* m_helpManager;

    te::qt::af::InterfaceController* m_iController;

    te::qt::widgets::QueryDialog* m_queryDlg;

    te::qt::widgets::CompositionModeMenuWidget* m_compModeMenu;

    std::vector<te::qt::af::DataSetTableDockWidget*> m_tables;

    ProjectMetadata* m_project;
};

#endif  // __TERRAVIEW_INTERNAL_TERRAVIEW_H
