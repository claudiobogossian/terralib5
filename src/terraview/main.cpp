/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

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
  \file terraview/main.cpp

  \brief It contains the main routine of TerraView.
*/

#include "TerraViewConfig.h"

// TerraLib
#include "../terralib/qt/af/BaseApplication.h"
#include "../terralib/qt/af/SplashScreenManager.h"

// STL
#include <cstdlib>
#include <exception>

// Qt
#include <QtCore/QResource>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  //QResource::registerResource(TERRAVIEW_RESOURCE_FILE);

  int waitVal = EXIT_FAILURE;

  try
  {
    QPixmap pixmap(TVIEW_SPLASH_SCREEN_PIXMAP);

    QSplashScreen* splash(new QSplashScreen(pixmap, Qt::WindowStaysOnTopHint));

    splash->setAttribute(Qt::WA_DeleteOnClose, true);

    splash->setStyleSheet("QWidget { font-size: 12px; font-weight: bold }");

    te::qt::af::SplashScreenManager::getInstance().set(splash, Qt::AlignBottom | Qt::AlignHCenter, Qt::white);

//    splash->show();

    te::qt::af::BaseApplication tview;

    tview.init(TVIEW_CONFIG_FILE);

    splash->finish(&tview);

    tview.showMaximized();

    waitVal = app.exec();
  }
  catch(const std::exception& /*e*/)
  {
    return EXIT_FAILURE;
  }
  catch(...)
  {
    return EXIT_FAILURE;
  }

  return waitVal;
}

