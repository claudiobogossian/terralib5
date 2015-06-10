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
  \file QtInitializer.h

  \brief 
*/

#include <terralib/common/PlatformUtils.h>
#include <terralib/common/Exception.h>

// Qt
#include <QApplication>
#include <QIcon>
#include <qresource.h>

class QtInitializer
{
  public:

    QtInitializer() { }

    void begin() throw (te::common::Exception);

    void end();

};

inline void QtInitializer::begin() throw (te::common::Exception)
{
  char* argv[] = {"JAVA", NULL};
  int argc = 1;

  QApplication* app = new QApplication(argc, argv);

  std::string th_path = te::common::FindInTerraLibPath("share/terralib/icons");

  Q_INIT_RESOURCE(tlib);

  QStringList ithemes = QIcon::themeSearchPaths();

  ithemes.push_back(th_path.c_str());

  QIcon::setThemeSearchPaths(ithemes);

  QIcon::setThemeName("terralib");
}

inline void QtInitializer::end()
{
  delete qApp;
}
