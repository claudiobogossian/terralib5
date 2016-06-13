/*
Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
\file terralib/examples/core/lib/main.cpp

\brief Examples for the TerraLib Library Module

\author Frederico Augusto Bedê
*/

// TerraLib
#include "MainWindow.h"

#include <terralib/core/utils/Platform.h>

// Qt
#include <QApplication>
#include <QIcon>


// Main program
int main(int argc, char *argv[])
{
  std::string iconThemePath = te::core::FindInTerraLibPath("share/terralib/icons");

  QStringList pts = QIcon::themeSearchPaths();

  pts <<QString::fromStdString(iconThemePath);

  QIcon::setThemeSearchPaths(pts);

  QIcon::setThemeName("terralib");

  QApplication app(argc, argv);

  MainWindow win;

  win.showMaximized();

  return app.exec();
}
