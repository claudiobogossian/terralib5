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
#include "TerraViewController.h"
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

    void startProject(const QString& projectFileName);

  protected:

    virtual void makeDialog();

    virtual void initActions();

    virtual void initMenus();

    virtual void initSlotsConnections();

    void addMenusActions();

    void addPopUpMenu();

    void initToolbars();

  protected slots:

    void showAboutDialog();


    void onApplicationTriggered(te::qt::af::evt::Event* e);


    void onRestartSystemTriggered();

    void onNewProjectTriggered();

    void onOpenProjectTriggered();

    void onSaveProjectTriggered();

    void onSaveProjectAsTriggered();


    void onHelpTriggered();


    void onLinkTriggered();

    void onLayerHistogramTriggered();

    void onLayerScatterTriggered();

    void onLayerChartTriggered();

    void onLayerGroupingTriggered();

    void onLayerCompositionModeTriggered();

    void onQueryLayerTriggered();


    void onMeasureDistanceToggled(bool checked);

    void onMeasureAreaToggled(bool checked);

    void onMeasureAngleToggled(bool checked);


    void onAddDataSetLayerTriggered();

    void onAddQueryLayerTriggered();

    void onAddTabularLayerTriggered();

    void onAddFolderLayerTriggered();

    void onProjectPropertiesTriggered();

    void onChangeLayerDataSourceTriggered();

    void onUpdateLayerDataSourceTriggered();

    void onRecentProjectsTriggered(QAction* proj);


    void onPluginsManagerTriggered();


    void onToolsCustomizeTriggered();

    void onToolsDataExchangerTriggered();

    void onToolsDataExchangerDirectTriggered();

    void onToolsDataExchangerDirectPopUpTriggered();

    void onToolsQueryDataSourceTriggered();

    void onToolsRasterMultiResolutionTriggered();

    void onDataSourceExplorerTriggered();


    void showProgressDockWidget();

    void onHighlightLayerObjects(const te::map::AbstractLayerPtr& layer, te::da::DataSet* dataset, const QColor& color);

    void onCreateNewLayer(te::map::AbstractLayerPtr layer);

  protected:

    void projectChanged();

    bool checkAndSaveProject();

    void openProject(const QString& prjFileName);

    void closeEvent(QCloseEvent * event);

    void addActions(const QString& name, const QString& category, const QList<QAction*>& acts);

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
   
    QAction* m_layerChartsHistogram;
    QAction* m_layerChartsScatter;
    QAction* m_layerChart;
    QAction* m_layerLinkTable;
    QAction* m_layerObjectGrouping;
    QAction* m_layerCompositionMode;
    QAction* m_layerQuery;

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
    QAction* m_projectProperties;

    QAction* m_toolsCustomize;
    QAction* m_toolsDataExchanger;
    QAction* m_toolsDataExchangerDirect;
    QAction* m_toolsDataExchangerDirectPopUp;
    QAction* m_toolsDataSourceExplorer;
    QAction* m_toolsDataSourceManagement;
    QAction* m_toolsQueryDataSource;
    QAction* m_toolsRasterMultiResolution;
    QAction* m_toolsFixGeometry;

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


    QDockWidget* m_progressDockWidget;       //!< Dock widget used to show progress information

    te::qt::widgets::HelpManagerImpl* m_helpManager;

    te::qt::af::InterfaceController* m_iController;

    te::qt::widgets::QueryDialog* m_queryDlg;

    te::qt::widgets::CompositionModeMenuWidget* m_compModeMenu;

    ProjectMetadata* m_project;

    TerraViewController* m_tvController;
};

#endif  // __TERRAVIEW_INTERNAL_TERRAVIEW_H
