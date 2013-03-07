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
  \file terralib/qt/af/BaseApplication.h

  \brief A QMainWindow to be used as the basis for TerraLib applications.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H
#define __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H

// TerraLib
#include "Config.h"

// STL
#include <map>

// Boost
#include <boost/noncopyable.hpp>

// Qt
#include <QtGui/QMainWindow>

// Forward declarations
namespace te
{
  namespace qt
  {
    namespace af
    {
// Forwar declarations
      class ApplicationController;
      class Event;
      class LayerExplorer;
      class MapDisplay;
      class Project;
      class TabularViewer;

      class TEQTAFEXPORT BaseApplication : public QMainWindow, public boost::noncopyable
      {
        Q_OBJECT

        public:

          BaseApplication(QWidget* parent = 0);

          virtual ~BaseApplication();

          virtual void init();

          virtual void init(const std::string& configFile);

        protected slots:

          void onApplicationTriggered(te::qt::af::Event* evt);

          void onAddDataSetLayerTriggered();

          void onPluginsManagerTriggered();

          void onPluginsBuilderTriggered();

//          void onHelpTriggered();

          /*!
            \brief

            \param
          */
          void onRecentProjectsTriggered(QAction* proj);


          /*!
            \brief
          */
          void onOpenProjectTriggered();

          /*!
            \brief
          */
          void onSaveProjectAsTriggered();

          void onToolsCustomizeTriggered();

          //void layerVisibilityChanged(const QModelIndex& idx);

          //void drawLayers();

          //void setPanTool(bool status);

          //void setZoomAreaTool(bool status);

          //void setAngleTool(bool status);

          //void setAreaMeasureTool(bool status);

          //void setDistanceTool(bool status);

          //void openPluginsManager();

          //void openDSrcManager();

          //void showProgressDock();

        protected:

          virtual void makeDialog();

          virtual void closeEvent(QCloseEvent* e);

          virtual void initAction(QAction*& act, const QString& icon, const QString& name,
                                  const QString& text, const QString& tooltip,
                                  bool iconVisibleInMenu, bool isCheckable, bool enabled);

          virtual void initActions();

          virtual void initMenus();

          virtual void initToolbars();

          virtual void initSlotsConnections();

          //! Qt components
          QAction *m_viewLayerExplorer;
          QAction *m_viewMapDisplay;
          QAction *m_viewDataTable;
          QAction *m_viewStyleExplorer;
          QAction *m_editUndo;
          QAction *m_editRedo;
          QAction *m_editCut;
          QAction *m_editCopy;
          QAction *m_editPaste;
          QAction *m_editSelectAll;
          QAction *m_editClear;
          QAction *m_editFind;
          QAction *m_editReplace;
          QAction *m_viewFullScreen;
          QAction *m_viewRefresh;
          QAction *m_toolsCustomize;
          QAction *m_pluginsManager;
          QAction *m_pluginsBuilder;
          QAction *m_helpContents;
          QAction *m_helpUpdate;
          QAction *m_viewToolBars;
          QAction *m_viewGrid;
          QAction *m_viewDataSourceExplorer;
          QAction *m_projectRemoveLayer;
          QAction *m_projectProperties;
          QAction *m_projectAddLayerDataset;
          QAction *m_projectAddLayerImage;
          QAction *m_projectAddLayerGraph;
          QAction *m_layerEdit;
          QAction *m_layerRename;
          QAction *m_layerExport;
          QAction *m_layerProperties;
          QAction *m_layerRaise;
          QAction *m_layerLower;
          QAction *m_layerToTop;
          QAction *m_layerToBottom;
          QAction *m_toolsDataSourceManagement;
          QAction *m_helpAbout;
          QAction *m_fileNewProject;
          QAction *m_fileSaveProject;
          QAction *m_fileSaveProjectAs;
          QAction *m_fileOpenProject;
          QAction *m_fileExit;
          QAction *m_filePrint;
          QAction *m_filePrintPreview;

          QWidget *m_centralwidget;
          
          QMenuBar *m_menubar;
          QMenu *m_editMenu;
          QMenu *m_viewMenu;
          QMenu *m_toolsMenu;
          QMenu *m_pluginsMenu;
          QMenu *m_helpMenu;
          QMenu *m_projectMenu;
          QMenu *m_projectAddLayerMenu;
          QMenu *m_layerMenu;
          QMenu *m_fileMenu;
          QMenu *m_recentProjectsMenu;

          QStatusBar *m_statusbar;
          QToolBar *m_fileToolBar;
          QToolBar *m_editToolBar;

// Well known Widgets
          LayerExplorer* m_explorer;  //!< A dockable tree view for the layers in the application project.
          MapDisplay* m_display;
          TabularViewer* m_viewer;

// Project
          Project* m_project;

          //QDockWidget* m_progressDock;                                    //!< Dock widget used to show progress information

          //te::qt::widgets::RasterVisualDockWidget* m_rasterVisualDock;    //!< Used to raster enhancement operations

          te::qt::af::ApplicationController* m_controller;
      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H
