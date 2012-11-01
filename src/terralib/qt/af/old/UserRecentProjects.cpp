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
  \file terralib/qt/af/UserRecentProjects.cpp

  \brief A singleton for managing the recent projects of a specific user.
*/

// don't change this include order, otherwise
// some boost versions can punish you!
// see: (https://svn.boost.org/trac/boost/ticket/6455)!

// TerraLib
#include "CoreApplication.h"
#include "../../common/Translator.h"
#include "../../common/UserApplicationSettings.h"
#include "Exception.h"
#include "UserRecentProjects.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

// Qt
#include <QtCore/QList>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QMenu>

void te::qt::af::UserRecentProjects::load()
{
// lock singletons
  te::common::UserApplicationSettings::LockRead l1(te::common::UserApplicationSettings::getInstance());
  UserRecentProjects::LockRead l2(this);

// retrive most recent project
  std::string mostRecentProject = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.MostRecentProject.<xmlattr>.xlink:href");

// clear the recent projects from the application menu
  QMenu* recentProjects = CoreApplication::getInstance()->getMenu("file.recent_projects");
  recentProjects->clear();

// clear the action group of recent projects
  QActionGroup* projects = CoreApplication::getInstance()->findChild<QActionGroup*>("file.recent_projects");

  if(projects == 0)
    throw Exception(TR_QT_AF("Recent projects entry is invalid in the application!"));

  BOOST_FOREACH(QAction* a, projects->actions())
  {
    projects->removeAction(a);
  }

// get recent opened projects and add it to the application menu
  boost::property_tree::ptree& p = te::common::UserApplicationSettings::getInstance().getAllSettings();

  boost::property_tree::ptree& child = p.get_child("UserSettings.RecentProjects");

  boost::property_tree::ptree::iterator it = child.begin();
  boost::property_tree::ptree::iterator itend = child.end();

  while(it != itend)
  {
    const std::string& ptitle = it->second.get<std::string>("<xmlattr>.title");
    const std::string& pdir = it->second.get<std::string>("<xmlattr>.xlink:href");

    if(!boost::filesystem::exists(pdir))
    {
      //it = p.erase(it);
      it = child.erase(it);
      te::common::UserApplicationSettings::getInstance().changed();
      continue;
    }

    QAction* item = new QAction(ptitle.c_str(), projects);
    item->setToolTip(pdir.c_str());
    item->setData(QString(pdir.c_str()));
    item->setCheckable(true);

    //if(projects->checkedAction() == 0)
    //  item->setChecked(true);
    if(mostRecentProject == pdir)
      item->setChecked(true);

    ++it;
  }

  recentProjects->addActions(projects->actions());
}

te::qt::af::UserRecentProjects::UserRecentProjects()
{
}

te::qt::af::UserRecentProjects::~UserRecentProjects()
{
}
