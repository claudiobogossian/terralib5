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
\file terraview/TerraViewController.cpp

\brief The API for controller of TerraView application.
*/

// Boost
#include <boost/foreach.hpp> // Boost => don't change this include order, otherwise you may have compiling problems! 

// TerraLib
#include <terralib_config.h>
#include <terralib/common/SystemApplicationSettings.h>
#include <terralib/qt/af/SplashScreenManager.h>
#include <terralib/qt/widgets/utils/ScopedCursor.h>
#include "TerraViewController.h"
#include "Utils.h"

// Qt
#include <QApplication>
#if QT_VERSION < 0x050000
#include <QDesktopServices>
#endif
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>
#include <QResource>
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#endif
#include <QWidget>

// Boost
#include <boost/filesystem.hpp>

//STL
#include <algorithm>

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
//Log4cxx
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/logger.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/logstring.h>
#include <log4cxx/simplelayout.h>
#endif

TerraViewController::TerraViewController(te::qt::af::ApplicationController* app, std::string appConfigFile)
  : QObject(),
  m_app(app)
{
  te::common::SystemApplicationSettings::getInstance().load(appConfigFile);

  m_appProjectExtension = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.ProjectExtension"));
}

TerraViewController::~TerraViewController()
{
  m_appProjectExtension.clear();

  m_recentProjs.clear();

  m_recentProjsTitles.clear();
}

void TerraViewController::initializeProjectMenus()
{
  te::qt::af::SplashScreenManager::getInstance().showMessage("Loading recent projects...");

  try
  {
    QSettings user_settings(QSettings::IniFormat,
                            QSettings::UserScope,
                            QApplication::instance()->organizationName(),
                            QApplication::instance()->applicationName());

    QVariant projPath = user_settings.value("projects/most_recent/path", "");
    QVariant projTitle = user_settings.value("projects/most_recent/title", "");

    QMenu* mnu = m_app->getMenu("File.Recent Projects");

    if(!projPath.toString().isEmpty())
    {
      QAction* act = mnu->addAction(projPath.toString());
      act->setData(projPath);

      mnu->addSeparator();

      m_recentProjs.append(projPath.toString());
      m_recentProjsTitles.append(projTitle.toString());
    }
    
    user_settings.beginGroup("projects");
    
    int nrc = user_settings.beginReadArray("recents");
    
    for(int i = 0; i != nrc; ++i)
    {
      user_settings.setArrayIndex(i);
      QString npath = user_settings.value("projects/path").toString();
      QString ntitle = user_settings.value("projects/title").toString();
      
      
      QAction* act = mnu->addAction(npath);
      act->setData(npath);
      m_recentProjs.append(npath);
      m_recentProjsTitles.append(ntitle);
    }

    mnu->setEnabled(true);

    te::qt::af::SplashScreenManager::getInstance().showMessage("Recent projects loaded!");
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error loading the registered projects: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_app->getMainWindow(), m_app->getAppTitle(), msgErr);
  }
}

void TerraViewController::updateRecentProjects(const QString& prjFile, const QString& prjTitle)
{
  int pos = m_recentProjs.indexOf(prjFile);

  QString author;
  int maxSaved;

  GetProjectInformationsFromSettings(author, maxSaved);

  if(pos != 0)
  {
    if(pos < 0)
    {
      if(m_recentProjs.size() > maxSaved) // TODO: Size of the list must be configurable.
      {
        m_recentProjs.removeLast();
        m_recentProjsTitles.removeLast();
      }

      m_recentProjs.prepend(prjFile);
      m_recentProjsTitles.prepend(prjTitle);
    }
    else
    {
      m_recentProjs.move(pos, 0);
      m_recentProjsTitles.move(pos, 0);
    }

    if(m_recentProjs.isEmpty())
      return;

    QMenu* mnu = m_app->getMenu("File.Recent Projects");

    mnu->clear();

    mnu->setEnabled(true);

    QString recPrj = m_recentProjs.at(0);
    QAction* act = mnu->addAction(recPrj);
    act->setData(recPrj);

    mnu->addSeparator();

    if(m_recentProjs.size() > 1)
      for(int i=1; i<m_recentProjs.size(); i++)
      {
        recPrj = m_recentProjs.at(i);
        act = mnu->addAction(recPrj);
        act->setData(recPrj);
      }
  }

  QAction* act = m_app->findAction("File.Save Project As");

  if(act != 0)
    act->setEnabled(true);
}



const QString& TerraViewController::getAppProjectExtension() const
{
  return m_appProjectExtension;
}


QString TerraViewController::getMostRecentProject() const
{
  return m_recentProjs.isEmpty() ? QString("") : m_recentProjs.front();
}

QString TerraViewController::getExtensionFilter()
{
  QString appName = m_app->getAppName();
  QString appProjectExtension = m_appProjectExtension;
  QString extensionFilter = appName;
  extensionFilter += QString(" (*.");
  extensionFilter += appProjectExtension + ")";

  return extensionFilter;
}

