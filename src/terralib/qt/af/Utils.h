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
  \file terralib/qt/af/Utils.h

  \brief Utility routines for the TerraLib Application Framework module.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_UTILS_H
#define __TERRALIB_QT_AF_INTERNAL_UTILS_H

// TerraLib
#include "../../common/Exception.h"
#include "Config.h"

// Qt
//#include <QtCore/QString>
//#include <QtGui/QColor>
    
// Forward declarations
class QString;
class QColor;
class QAction;
class QMainWindow;
class QStringList;
class QToolBar;
class QWidget;

namespace te
{

// Forward declaration
  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace qt
  {
    namespace af
    {
//      class Project;

      /*!
        \brief Reads and return a te::qt::af::Project from the file.

        \param uri Location of the te::qt::af::Project file.

        \return A pointer to the te::qt::af::Project defined in the \a uri file.
      */
//      TEQTAFEXPORT Project* ReadProject(const std::string& uri);

      /*!
        \brief Reads and return a te::qt::af::Project using \a reader XML reader.

        \param reader The XML reader to be used.

        \return A pointer to the te::qt::af::Project.
      */
//      TEQTAFEXPORT Project* ReadProject(te::xml::Reader& reader);

      /*!
        \brief Saves the informations of the project in the \a uri file.

        \param project The te::qt::af::Project to be saved.

        \param uri File location.
      */
//      TEQTAFEXPORT void Save(const Project& project, const std::string& uri);

      /*!
        \brief Saves the informations of the te::qt::af::Project using \a writer XML writer.

        \param project The te::qt::af::Project to be saved.

        \param writer The XML writer to be used.
      */
//      TEQTAFEXPORT void Save(const Project& project, te::xml::AbstractWriter& writer);

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
        \brief Unsaved star
      */
      TEQTAFEXPORT QString UnsavedStar(const QString windowTitle, bool isUnsaved);

      /*!
        \brief Update plugins file.
      */
      //TEQTAFEXPORT void UpdateApplicationPlugins();

      /*!
        \brief Update the existing tool bars

        \param bars Set with the existing tool bars.
      */
      TEQTAFEXPORT void UpdateToolBarsInTheSettings();

      /*!
        \brief Update settings with a new tool bar.

        \param bar Bar to be added.
      */
      TEQTAFEXPORT void AddToolBarToSettings(QToolBar* bar);

      /*!
        \brief Removes a tool bar from the settings.

        \param bar Bar to be removed.
      */
      TEQTAFEXPORT void RemoveToolBarFromSettings(QToolBar* bar);

      /*!
        \brief Returns a vector of tool bars registered in the QSettings.

        \param barsParent
      */
      TEQTAFEXPORT std::vector<QToolBar*> ReadToolBarsFromSettings(QWidget* barsParent=0);

      /*
        \brief

        \param
      */
      TEQTAFEXPORT void SaveState(QMainWindow* mainWindow);

      /*
        \brief

        \param
      */
      TEQTAFEXPORT void RestoreState(QMainWindow* mainWindow);

      /*
        \brief

        \param[out]

        \param[out]
      */
      TEQTAFEXPORT void GetProjectInformationsFromSettings(QString& defaultAuthor, int& maxSaved);

      /*
        \brief

        \param

        \param
      */
      TEQTAFEXPORT void SaveProjectInformationsOnSettings(const QString& defaultAuthor, const int& maxSaved);

      /*!
        \brief

        \param
      */
      TEQTAFEXPORT void SaveLastDatasourceOnSettings(const QString& dsType);

      /*!
        \brief

        \param
      */
      TEQTAFEXPORT void SaveOpenLastProjectOnSettings(bool openLast);

      /*!
        \brief

        \return
      */
      TEQTAFEXPORT QString GetLastDatasourceFromSettings();

      /*!
        \brief

        \return
      */
      TEQTAFEXPORT bool GetOpenLastProjectFromSettings();

      /*!
        \brief

        \return
      */
      TEQTAFEXPORT QColor GetDefaultDisplayColorFromSettings();

      /*!
        \brief

        \return
      */
      TEQTAFEXPORT QString GetStyleSheetFromColors(QColor primaryColor, QColor secondaryColor);

      /*!
        \brief

        \return
      */
      TEQTAFEXPORT QString GetStyleSheetFromSettings();

      /*!
        \brief

        \return
      */
      TEQTAFEXPORT bool GetAlternateRowColorsFromSettings();

      /*!
        \brief Creates a default QSettings.
      */
      TEQTAFEXPORT void CreateDefaultSettings();

      /*!
        \brief Check QSettings for existance of \a act and adds it if necessary.

        \param act Action to be inserted.
      */
      TEQTAFEXPORT void AddActionToCustomToolbars(QAction* act);

      /*!
        \brief Returns the complete path of the configuration file or an empty value if it not exists.
      */
      //TEQTAFEXPORT QString GetConfigFileName();

      /*!
        \brief Setts the name of the application configuration file.

        \param fileName Complete path to the application configuration file.
      */
      TEQTAFEXPORT void SetConfigFileName(const QString& fileName);

      /*!
        \brief 
      */
      TEQTAFEXPORT QString GetDateTime();

      /*!
        \brief 
        \param 
      */
      TEQTAFEXPORT void SetDateTime(const QString& dateTime);


      /*!
        \brief Returns the default path for output of configuration file.
      */
      TEQTAFEXPORT QString GetDefaultConfigFileOutputDir();

      TEQTAFEXPORT std::vector<std::string> GetPluginsFiles();

      TEQTAFEXPORT std::vector<std::string> GetDefaultPluginsNames();
      
      TEQTAFEXPORT std::vector<std::string> GetPluginsNames(const std::vector<std::string>& plgFiles);

      /*!
        \brief Changes the user settings file location.

        \param fileName The new file to be generated.

        \param removeOlder Flag information the remove older settings file. If true remove the older file, otherwise maintain it.
      */
      TEQTAFEXPORT void UpdateUserSettingsFile(const QString& fileName, const bool& removeOlder = true);

      /*!
        \brief Changes the application plugins file location.

        \param fileName The new file to be generated.

        \param removeOlder Flag information the remove older application plugins file. If true remove the older file, otherwise maintain it.
      */
      TEQTAFEXPORT void UpdateAppPluginsFile(const QString& fileName, const bool& removeOlder = true);
      
      /*!
        \brief Writes the configuration file. It updates the application settings.

        \param fileName Complete path to the configuration file. 

        \param appName Name of the application.

        \param appTitle Title to be presented on the main window.
      */
      //TEQTAFEXPORT void WriteConfigFile(const QString& fileName, const QString& appName, const QString& appTitle, const bool& writeOnlyConfig = false);

      /*!
        \brief Writes the user settings file.
      */
      //TEQTAFEXPORT void WriteUserSettingsFile(const QString& fileName);

      /*!
        \brief Writes the application plugins file.
      */
      //TEQTAFEXPORT void WriteAppPluginsFile(const QString& fileName);

      /*!
        \brief Writes the default project file.
      */
      TEQTAFEXPORT void WriteDefaultProjectFile(const QString& fileName);

      /*!
        \brief Return a QString with the new window title based on the project informations.
      */
//      TEQTAFEXPORT QString GetWindowTitle(const Project& project);

      /*!
        \brief Returns the date and time of generated binary.
      */
      TEQTAFEXPORT QString GetGenerationDate();

      /*!
        \brief Return extension filter string
      */
      TEQTAFEXPORT QString GetExtensionFilter();
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_UTILS_H

