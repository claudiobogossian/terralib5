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
  \file terralib/qt/af/Utils.h

  \brief Utility routines for the TerraLib Application Framework module.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_UTILS_H
#define __TERRALIB_QT_AF_INTERNAL_UTILS_H

// TerraLib
#include "../../common/Exception.h"
#include "Config.h"

// Forward declarations
class QStringList;
class QToolBar;

namespace te
{

// Forward declaration
  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace qt
  {
    namespace af
    {
      class Project;

      /*!
        \brief Reads and return a te::qt::af::Project from the file.

        \param uri Location of the te::qt::af::Project file.

        \return A pointer to the te::qt::af::Project defined in the \a uri file.
      */
      TEQTAFEXPORT Project* ReadProject(const std::string& uri);

      /*!
        \brief Reads and return a te::qt::af::Project using \a reader XML reader.

        \param reader The XML reader to be used.

        \return A pointer to the te::qt::af::Project.
      */
      TEQTAFEXPORT Project* ReadProject(te::xml::Reader& reader);

      /*!
        \brief Saves the informations of the project in the \a uri file.

        \param project The te::qt::af::Project to be saved.

        \param uri File location.
      */
      TEQTAFEXPORT void Save(const Project& project, const std::string& uri);

      /*!
        \brief Saves the informations of the te::qt::af::Project using \a writer XML writer.

        \param project The te::qt::af::Project to be saved.

        \param writer The XML writer to be used.
      */
      TEQTAFEXPORT void Save(const Project& project, te::xml::Writer& writer);

      /*!
        \brief Updates user settings file section about information of the projects.

        \param prjFiles List of the files containing projects.

        \param prjTitles List of the titles of the projects.

        \param userConfigFile Name of the user configuration file.
      */
      TEQTAFEXPORT void UpdateUserSettings(const QStringList& prjFiles, const QStringList& prjTitles, const std::string& userConfigFile);

      /*!
        \brief Saves data sources file.
      */
      TEQTAFEXPORT void SaveDataSourcesFile();

      /*!
        \brief Update plugins file.
      */
      TEQTAFEXPORT void UpdateApplicationPlugins();

      /*!
        \brief Update settings with a new tool bar.
      */
      TEQTAFEXPORT void AddToolBarToSettings(QToolBar* bar);

      /*!
        \brief Removes a tool bar from the settings.
      */
      TEQTAFEXPORT void RemoveToolBarFromSettings(QToolBar* bar);

      /*!
        \brief Returns a vector of tool bars registered in the QSettings.
      */
      TEQTAFEXPORT std::vector<QToolBar*> ReadToolBarsFromSettings(QWidget* barsParent=0);

      /*!
        \brief Creates a default QSettings.
      */
      TEQTAFEXPORT void CreateDefaultSettings();
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_UTILS_H

