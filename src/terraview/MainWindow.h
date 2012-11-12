/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

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
  \file MainWindow.h
  \brief 
  \details 
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */

#ifndef __TERRAVIEW_MAINWINDOW_H
#define __TERRAVIEW_MAINWINDOW_H

//! Qt include files
#include <QMainWindow>

//! STL include files
#include <map>

//! Forward declarations
namespace te
{
  namespace map
  {
    class AbstractLayer;
  }

  namespace qt
  {
    namespace af
    {
      class Event;
      class LayerExplorer;
      class MapDisplay;
      class TabularViewer;
    }
  }
}

namespace Ui
{
  class MainWindow;
}
class QModelIndex;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent=0);

  ~MainWindow();

protected slots:
  
  void onApplicationTriggered(te::qt::af::Event* evt);

  void addOGRLayer();

  void layerVisibilityChanged(const QModelIndex& idx);

  void drawLayers();

  void setPanTool(bool status);

  void setZoomAreaTool(bool status);

  void setAngleTool(bool status);

  void setAreaMeasureTool(bool status);

  void setDistanceTool(bool status);

  void openPluginsManager();

protected:

  void makeDialog();

  void updateIcons();

  te::qt::af::LayerExplorer* m_explorer;
  te::qt::af::MapDisplay* m_display;
  te::qt::af::TabularViewer* m_viewer;
  std::map<int, te::map::AbstractLayer*> m_layers;

private:
  Ui::MainWindow* m_ui;
};

#endif //__TERRAVIEW_MAINWINDOW_H
