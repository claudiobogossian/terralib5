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
  \file terraview/TerraViewController.h

  \brief The API for controller of TerraView application.
*/

#ifndef __TERRAVIEW_INTERNAL_TERRAVIEWCONTROLLER_H
#define __TERRAVIEW_INTERNAL_TERRAVIEWCONTROLLER_H

// Terralib
#include <terralib/qt/af/ApplicationController.h>
#include "Config.h"

// STL
#include <map>
#include <set>
#include <vector>


// Qt
#include <QObject>
#include <QStringList>
#include <QSettings>
#include <QColor>

// Forward declarations
class QAction;
class QActionGroup;
class QMenu;
class QMenuBar;
class QToolBar;
class QWidget;

/*!
  \class TerraViewController

  \brief The API for controller of TerraView application.

*/
class TerraViewController : public QObject
{
  Q_OBJECT

  public:

    /*!
      \brief Constructor.

      \param parent The parent object.
    */
    TerraViewController(te::qt::af::ApplicationController* app, std::string appConfigFile);

    /*! 
      \brief Destructor. 
    */
    virtual ~TerraViewController();

    
    /*!
      \brief Initializes the menus for the most recent open projects.
    */
    virtual void initializeProjectMenus();

    /*!
      \brief Update the list of recent projects. This is commonly used when there's a new most recent project.

      \param prj_file Complete file name for the project file.

      \param prj_title Title of the project.
    */
    void updateRecentProjects(const QString& prjFile, const QString& prjTitle);

    /*!
      \brief Returns the application project extension.

      \return Application project extension.
    */
    const QString& getAppProjectExtension() const;

    /*!
    \brief Returns the most recent project.

    \return Application most recent opened project.
    */
    QString getMostRecentProject() const;

    /*!
    \brief Returns the project extension filter .

    \return Filter string.
    */
    QString getExtensionFilter();

  protected:

    te::qt::af::ApplicationController* m_app;

    QString m_appProjectExtension;              //!< Application project extension.
    QStringList m_recentProjs;                  //!< List of the recent projects.
    QStringList m_recentProjsTitles;            //!< List of the titles of the recent projects.
};

#endif // __TERRAVIEW_INTERNAL_TERRAVIEWCONTROLLER_H

