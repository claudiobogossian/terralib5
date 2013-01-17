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

// Qt
#include <QtGui/QMainWindow>

// Forward declarations
namespace te
{
  namespace map
  {
    class AbstractLayer;
  }
}

namespace Ui { class BaseApplicationForm; }

class QModelIndex;

namespace te
{
  namespace qt
  {
    namespace af
    {
// Forwar declarations
      class Event;
      class LayerExplorer;
      class MapDisplay;
      class Project;
      class TabularViewer;

      class TEQTAFEXPORT BaseApplication : public QMainWindow
      {
        Q_OBJECT

        public:

          BaseApplication(QWidget* parent = 0);

          virtual ~BaseApplication();

          virtual void init();

          virtual void init(const std::string& configFile);

        protected slots:

          void onApplicationTriggered(te::qt::af::Event* evt);

          void onAddDataSetLayer();

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

        protected:

          LayerExplorer* m_explorer;
          MapDisplay* m_display;
          TabularViewer* m_viewer;
          Project* m_project;

          //QDockWidget* m_progressDock;                                    //!< Dock widget used to show progress information

          //te::qt::widgets::RasterVisualDockWidget* m_rasterVisualDock;    //!< Used to raster enhancement operations

        private:

          Ui::BaseApplicationForm* m_ui;
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H
