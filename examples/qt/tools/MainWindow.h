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
  \file MainWindow.h

  \brief A simple main window to show example of TerraLib Qt Tools.
*/

#ifndef __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_TOOLS_H
#define __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_TOOLS_H

// TerarLib
#include <terralib/geometry/Enums.h>
#include <terralib/maptools/AbstractLayer.h>

// Qt
#include <QMainWindow>

// STL
#include <list>
#include <vector>

// Forward declarations
namespace te
{
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
class QContextMenuEvent;
class QPointF;
class QMenu;
class QToolBar;

/*!
  \class MainWindow

  \brief A simple main window to show example of TerraLib Qt Tools.
*/
class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:

    /** @name Initializer Methods
      *  Methods related to instantiation and destruction.
      */
    //@{

    /*! \brief Constructor */
    MainWindow(QWidget* parent = 0, Qt::WindowFlags f = 0);

    /*! \brief Destructor. */
    ~MainWindow();

    //@}

  private:

    void setupActions();

    void addDataSetLayer(const QString& path, const std::string& driver);

    void contextMenuEvent(QContextMenuEvent* e);

  private slots:

    void onAddVectorDataTriggered();

    void onAddRasterDataTriggered();

    void onPanTriggered();

    void onZoomInTriggered();

    void onZoomOutTriggered();

    void onZoomAreaTriggered();

    void onDistanceTriggered();

    void onAreaTriggered();

    void onAngleTriggered();

    void onSelectionTriggered();

    void onStopAllTriggered();

    void onCoordTracked(QPointF& coordinate);

  private:

    te::qt::widgets::MapDisplay* m_display;
    te::qt::widgets::AbstractTool* m_tool;

    std::list<te::map::AbstractLayerPtr> m_layers;

    QToolBar* m_toolBar;
    QMenu* m_menu;

    static std::size_t ms_id;
};

#endif  // __TERRALIB_EXAMPLES_QT_TOOLS_INTERNAL_TOOLS_H
