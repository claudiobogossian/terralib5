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

  \brief Using the help button.
  */

#ifndef __TERRALIB_EXAMPLE_MAINWINDOW_H
#define __TERRALIB_EXAMPLE_MAINWINDOW_H

#include <QMainWindow>

//Forward declarations
namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class HelpManagerImpl;
    }
  }
}

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  MainWindow(QWidget* parent=0);

  ~MainWindow();

protected slots:

  void showTest1();

  void showTest2();

  void pluginTest();

protected:

  te::qt::widgets::HelpManagerImpl* m_impl;

private:
  Ui::MainWindow* m_ui;
};


#endif //__TERRALIB_EXAMPLE_MAINWINDOW_H