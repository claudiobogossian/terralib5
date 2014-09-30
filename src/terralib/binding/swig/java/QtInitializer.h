/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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

// Qt
#include <QApplication>
#include <QIcon>

class QtInitializer
{
  public:

    QtInitializer() { }

    void begin();

    void end();

  protected:

    QApplication* m_app;

    int m_argc;

    char* m_argv[1];
};

inline void QtInitializer::begin()
{
  m_argc = 1;

  m_argv[0] = "Java";

  m_app = new QApplication(m_argc, m_argv);

  QStringList ithemes = QIcon::themeSearchPaths();

  std::string th_path = te::common::FindInTerraLibPath("share/terralib/icons");

  ithemes.push_back(th_path.c_str());

  QIcon::setThemeSearchPaths(ithemes);

  QIcon::setThemeName("terralib");
}

inline void QtInitializer::end()
{
  delete m_app;
}
