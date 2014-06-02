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
  \file MainWindow.h

  \brief A simple main window to show example of TerraLib Qt Datasource components.
*/

#ifndef __TERRALIB_EXAMPLES_QT_DATASOURCE_INTERNAL_H
#define __TERRALIB_EXAMPLES_QT_DATASOURCE_INTERNAL_H


// Qt
#include <QMainWindow>

// STL
#include <list>
#include <vector>

// Forward declarations
class QAction;
class QPointF;
class QToolBar;

/*!
  \class MainWindow

  \brief A simple main window to show example of TerraLib Qt Tools.
*/
class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:


    /*! \brief Constructor */
    MainWindow(QWidget* parent = 0, Qt::WindowFlags f = 0);

    /*! \brief Destructor. */
    ~MainWindow();

  private:

    void setupActions();

  private slots:

    void onOpenGDALTriggered();
    void onOpenOGRTriggered();
    void onOpenPostGISTriggered();

  private:

    QToolBar* m_toolBar;
    QAction* m_openGDAL;
    QAction* m_openOGR;
    QAction* m_openPostGIS;
};

#endif  // __TERRALIB_EXAMPLES_QT_DATASOURCE_INTERNAL_H
