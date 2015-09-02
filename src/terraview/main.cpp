/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
    along with TerraView. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terraview/main.cpp

  \brief It contains the main routine of TerraView.
*/

// TerraView
#include "Config.h"
#include "../terralib/Defines.h"
#include "TerraView.h"

// TerraLib
#include <terralib/common/PlatformUtils.h>
#include <terralib/qt/af/Utils.h>
#include <terralib/qt/af/SplashScreenManager.h>

// STL
#include <cstdlib>
#include <exception>
#include <locale>

// Qt
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QSplashScreen>
#include <QTextCodec>
#include <QTranslator>


#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif


int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QDir dir(QLibraryInfo::location(QLibraryInfo::TranslationsPath));

  QStringList filters;
  filters <<"*" + QLocale::system().name().toLower() + ".qm";

  QFileInfoList lst = dir.entryInfoList(filters, QDir::Files);

  for(int i=0; i<lst.size(); ++i)
  {
    QTranslator* trans = new QTranslator;
    trans->load(lst.at(i).baseName(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(trans);
  }

  setlocale(LC_ALL,"C"); // This force to use "." as decimal separator.

#if QT_VERSION >= 0x050000
  QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
#else
  QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
#endif

  int waitVal = EXIT_FAILURE;

  const int RESTART_CODE = 1000;

  try
  {
    do 
    {
      std::string splash_pix = te::common::FindInTerraLibPath(TVIEW_SPLASH_SCREEN_PIXMAP);

      QPixmap pixmap(splash_pix.c_str());

      QSplashScreen* splash(new QSplashScreen(pixmap/*, Qt::WindowStaysOnTopHint*/));

      splash->setAttribute(Qt::WA_DeleteOnClose, true);

      splash->setStyleSheet("QWidget { font-size: 12px; font-weight: bold }");

      te::qt::af::SplashScreenManager::getInstance().set(splash, Qt::AlignBottom | Qt::AlignHCenter, Qt::white);

      splash->show();

      TerraView tview;

      tview.resetTerraLib(waitVal != RESTART_CODE);
      
#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
      CFBundleRef mainBundle = CFBundleGetMainBundle();
      CFURLRef execPath = CFBundleCopyBundleURL(mainBundle);
        
      char path[PATH_MAX];
      
      if (!CFURLGetFileSystemRepresentation(execPath, TRUE, (UInt8 *)path, PATH_MAX))
        throw; // error!
        
      CFRelease(execPath);
        
      QDir dPath(path);
        
      dPath.cd("Contents");
        
      chdir(dPath.path().toStdString().c_str());
#endif

      tview.init();

      splash->finish(&tview);

      tview.showMaximized();

      tview.resetState();

      // Start TerraView from project file(.tview)
      if(argc > 1)
      {
        std::string projPath = argv[1];

        if(!projPath.empty())
          tview.startProject(projPath.c_str());
      }

      waitVal = app.exec();

      tview.resetTerraLib(waitVal != RESTART_CODE);

    } while(waitVal == RESTART_CODE);
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
