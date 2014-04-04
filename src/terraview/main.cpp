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

// TerraView
#include "Config.h"
#include "TerraView.h"

// TerraLib
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
#include <QMessageBox>
#include <QResource>
#include <QSplashScreen>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  setlocale(LC_ALL,"C");// This force to use "." as decimal separator.

  //QResource::registerResource(TERRAVIEW_RESOURCE_FILE);

  int waitVal = EXIT_FAILURE;

  const int RESTART_CODE = 1000;

  try
  {
    do 
    {
      const char* te_env = getenv("TERRALIB_DIR");

      if(te_env == 0)
      {
        QMessageBox::critical(0, QObject::tr("Execution Failure"), QObject::tr("Environment variable \"TERRALIB_DIR\" not found.\nTry to set it before run the application."));
        throw std::exception();
      }

      std::string splash_pix(te_env);
      splash_pix += "/resources/images/png/terraview-splashscreen.png";

      QPixmap pixmap(splash_pix.c_str());

      QSplashScreen* splash(new QSplashScreen(pixmap/*, Qt::WindowStaysOnTopHint*/));

      splash->setAttribute(Qt::WA_DeleteOnClose, true);

      splash->setStyleSheet("QWidget { font-size: 12px; font-weight: bold }");

      te::qt::af::SplashScreenManager::getInstance().set(splash, Qt::AlignBottom | Qt::AlignHCenter, Qt::white);

      splash->show();

      TerraView tview;

      QString cFile = te::qt::af::GetConfigFileName();
      QFileInfo info(cFile);

      if(cFile.isEmpty() || !info.exists())
      {
        cFile = te::qt::af::GetDefaultConfigFileOutputDir() + "/config.xml";
        te::qt::af::WriteConfigFile(cFile, "TerraView", "TerraView");
      }

      // Copying JSON files

      QDir out_dir = QFileInfo(cFile).absoluteDir();
      info.setFile(out_dir.absolutePath() + "/resources/json/srs.json");

      if(!info.exists())
      {
        out_dir.mkpath("resources/json");

        QString origin = te_env + QString("/resources/json");

        QStringList files = QDir(origin).entryList(QDir::Files);

        QFile cf;

        foreach (QString f, files)
        {
          cf.setFileName(origin + "/" + f);
          cf.copy(out_dir.absolutePath() + "/resources/json/" + f);
        }
      }

      tview.resetTerraLib(waitVal != RESTART_CODE);

      tview.init(cFile.toStdString());

      splash->finish(&tview);

      tview.showMaximized();

      tview.resetState();

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

