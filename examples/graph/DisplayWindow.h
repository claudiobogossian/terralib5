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
  \file DisplayWindow.h

  \brief A simple main window to show example of TerraLib Graph.
*/

#ifndef __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_GRAPHDISPLAYWINDOW_H
#define __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_GRAPHDISPLAYWINDOW_H

// TerarLib
#include <terralib/geometry/Enums.h>
#include <terralib/maptools/AbstractLayer.h>

// Qt
#include <QtGui/QMainWindow>

// STL
#include <list>
#include <vector>

// Forward declarations
namespace te
{
  namespace da
  {
    class DataSource;
  }

  namespace graph
  {
    class AbstractGraph;
  }
  
  namespace se
  {
    class Style;
  }

  namespace qt
  {
    namespace widgets
    {
      class AbstractTool;
      class MapDisplay;
    }
  }
}

// Forward declarations
class QAction;
class QPointF;
class QToolBar;

/*!
  \class DisplayWindow

  \brief A simple main window to show example of TerraLib Qt Tools.
*/
class DisplayWindow : public QMainWindow
{
  Q_OBJECT

  public:

    /** @name Initializer Methods
      *  Methods related to instantiation and destruction.
      */
    //@{

    /*! \brief Constructor */
    DisplayWindow(QWidget* parent = 0, Qt::WindowFlags f = 0);

    /*! \brief Destructor. */
    ~DisplayWindow();

    //@}

    void addGraph(te::graph::AbstractGraph* graph, te::gm::Envelope extent, te::se::Style* s = 0);
    void addRasterLayer(std::string path, std::string name);
    void addVectorialLayer(std::string path, int srid);

    void setPNGPrefix(std::string prefix);

  private:

    void setupActions();

  private slots:

    void onPanTriggered();
    void onZoomAreaTriggered();
    void onPNGTriggered();
    void onCoordTracked(QPointF& coordinate);

  private:

    te::qt::widgets::MapDisplay* m_display;
    te::qt::widgets::AbstractTool* m_tool;

    std::list<te::map::AbstractLayerPtr> m_layers;

    QToolBar* m_toolBar;
    QAction* m_setPan;
    QAction* m_setZoomArea;
    QAction* m_png;

    static std::size_t ms_id;

    int m_count;
    std::string m_pngPrefix;
};

#endif  // __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_GRAPHDISPLAYWINDOW_H
