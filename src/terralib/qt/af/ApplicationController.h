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
  \file terralib/qt/af/ApplicationController.h

  \brief The base API for controllers of TerraLib applications.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_APPLICATIONCONTROLLER_H
#define __TERRALIB_QT_AF_INTERNAL_APPLICATIONCONTROLLER_H

// Terralib
#include "Config.h"

// STL
#include <map>
#include <set>
#include <vector>

// TerraLib
#include <terralib/common/Singleton.h>

// Boost
//#include <boost/noncopyable.hpp>

// Qt
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtGui/QColor>

// Forward declarations
class QAction;
class QActionGroup;
class QMenu;
class QMenuBar;
class QToolBar;
class QWidget;

namespace te
{
  namespace qt
  {
    namespace af
    {
      class Project;

      namespace evt
      {
      // Forward declarations
        struct Event;
      }

      /*!
        \class ApplicationController

        \brief The base API for TerraLib applications.

        \note Temporally this class is a singleton!

        \ingroup af
      */
      class TEQTAFEXPORT ApplicationController : public QObject, public te::common::Singleton<ApplicationController>
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param parent The parent object.
          */
          ApplicationController(/*QObject* parent = 0*/);

          /*! 
            \brief Destructor. 
          */
          virtual ~ApplicationController();

          /*!
            \brief It gives access to the controller singleton.

            \return A pointer to the application singleton.

            \pre The application must have been initialized before calling this method.
          */
//          static ApplicationController& getInstance();

          /*!
            \brief Tells wich configuration file to be used by the controller during its initialization.

            \param configFileName The configuration file name with full path.
          */
          virtual void setConfigFile(const std::string& configFileName);

          /*!
            \brief Tells the widget to be used as the parent of messages showned in a QMessageBox.

            \param w The widget to be used as the parent of messages showned in a QMessageBox.
          */
          virtual void setMsgBoxParentWidget(QWidget* w);

          /*! 
            \name Menus and Menubars Management.

            \brief Register and recover menus and menu bars.

            The methods \a find will returns a valid object only if it exists in some menu, otherwise the return will be a NULL pointer.
            The methods \a get ALWAYS returns a valid pointer. If the requested menu or menubar does not exists these funtions will create,
            register and return the object.

            \note The identifiers of the menus are the texts presented by menus.
            \note The search for menus will occurs in ALL OF THE REGISTERED menus and menu bars, including submenus. The correct sintaxe
            for searchies submenus may be found in the documentation of the findMenu(QString mnuText, QMenu* mnu) method.
          */
          //@{

          /*!
            \brief Register the toolbar in the list of the known toolbars and dispatch an event.

            \param id  The toolbar identification string.
            \param bar The toolbar to be registered. The controller will not take the \a bar ownership.

            \pos An event will be broadcasted to indicate that the toolbar was appended.

            \exception Exception It throws an exception if a toolbar with the given \a id already exists.
          */
          void addToolBar(const QString& id, QToolBar* bar);

          /*!
            \brief Register the toolbar in the list of the known toolbars.

            \param id  The toolbar identification string.
            \param bar The toolbar to be registered. The controller will not take the \a bar ownership.

            \exception Exception It throws an exception if a toolbar with the given \a id already exists.

            \todo Rever se este metodo eh necessario ou nao! Talvez possamos viver somente com o addToolBar!
          */
          void registerToolBar(const QString& id, QToolBar* bar);

          /*!
            \brief Return the toolbar identified by \a id or \a NULL if none is found.

            \param id The toolbar identification string.

            \return A toolbar identified by \a id or \a NULL if none is found.
          */
          QToolBar* getToolBar(const QString& id) const;

          /*!
            \brief Return the list of registered toolbars.

            \return List of registered ToolBars.
          */
          std::vector<QToolBar*> getToolBars() const;

          /*!
            \brief Removes the toolbar identified by \a id.

            \param id Identifier of the tool bar.
          */
          void removeToolBar(const QString& id);

          /*!
            \brief Register the \a mnu.

            \param mnu Pointer to the menu to be registered.
          */
          void registerMenu(QMenu* mnu);

          /*!
            \brief Returns the menu registered with key \a id

            \param id Indentifier of the menu.

            \return Menu registered or NULL with there is not one registered with the key \a id.
          */
          QMenu* findMenu(const QString& id) const;

          /*!
            \brief Returns a menu registered with key \a id.

            \param id Identifier of menu to be searched.

            \return The menu registered with the \a id identifier.

            \note If the menu does not exists, then this function will create it.
          */
          QMenu* getMenu(const QString& id);

          /*!
            \brief Register the \a bar.

            \param bar Pointer to the menu bar to be registered.
          */
          void registerMenuBar(QMenuBar* bar);

          /*!
            \brief Returns the menu bar registered with key \a id

            \param id Indentifier of the menu bar.

            \return Menu bar registered or NULL with there is not one registered with the key \a id.
          */
          QMenuBar* findMenuBar(const QString& id) const;

          /*!
            \brief Returns a menu bar registered with key \a id.

            \param id Identifier of menu bar to be searched.

            \return The menu bar registered with the \a id identifier.

            \note If the menu bar does not exists, then this function will create it.
          */
          QMenuBar* getMenuBar(const QString& id) const;

          /*!
            \brief Returns the action identified by \a id or NULL if there's not an action identified by \a id.

            \param id Identifier if the action. 

            \return QAction identified or NULL if could not find one.

            \note There's a rule to determine \a id. The text of menus and submenus separated by '.' character.
          */
          QAction* findAction(const QString& id) const;

          /*!
            \brief Returns the action group identified by \a id or NULL if there's not an action group identified by \a id.

            \param id Identifier of the action group.

            \return QActionGroup identified or NULL if could not find one.
          */
          QActionGroup* findActionGroup(const QString& id) const;

          //@}

          /*!
            \brief Insert an application item that will listen to framework events.

            \param obj The application item that will listen to framework events.

            \note The \a obj MUST HAVE AN IMPLEMENTATION of the slot \a onApplicationTriggered(Event*). If it hasn't, a warning will be sent at run-time.

            \note If the \a obj was previously registered, nothing will be done.
          */
          void addListener(QObject* obj);

          /*!
            \brief Remove the \a obj from the list of event listeners.

            \param obj The application item to be removed.

            \note If the \a obj was not previously registered, nothing will be done.
          */
          void removeListener(QObject* obj);

          /*!
            \brief Initializes the application framework.

            The initialization will read the following configurations files (in order):
            <ul>
            <li>A main config file, generally called config.xml</li>
            <li>A user settings file, generally called user_settings.xml<li>
            </ul>

            It will start by initializing the available TerraLib modules.
            This means that applications that uses the application framework, 
            does not need to call TerraLib initialization (TerraLib::getInstance().initialize()) and 
            finalization methods (TerraLib::getInstance().finalize()).

            \exception Exception It throws an exception if the initialization fails.

            \note Make sure that the application calls this method BEFORE it uses any TerraLib modules or functions.
          */
          virtual void initialize();

          /*!
            \brief Load the plugin list and initialize the plugins enabled by the user.

            The plugin initialization will read a file containing a list of plugins with their addresses.
            This file is generally called application_plugins.xml.

            \pre The application must assure that any resource needed by their plugins are ready for use. This
                 means that initialize should be called before initializing the plugins.
          */
          virtual void initializePlugins();

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
            \brief Set the current project.

            \param prj Pointer to a current project.

          */
          void set(te::qt::af::Project* prj);

          /*!
            \brief Get the current project.

            \return Pointer to a current project if defined or a null pointer in other case.

          */
          te::qt::af::Project* getProject();

          /*!
            \brief Finalize the application framework.

            \details This method will unload all TerraLib modules, plugins, and dynamic libraries that are loaded.

            \note Make sure that the application calls this method BEFORE it finishes.
          */
          virtual void finalize();

          /*!
            \brief Return the QSettings of the application. This can be used to add settings from external sources.

            \return QSettings being used.
          */
          QSettings& getSettings(); 

        public slots:

          /*!
            \brief Send events in broadcast for all registered components.

            \param evt Event to be sent.
          */
          void broadcast(te::qt::af::evt::Event* evt);

        signals:

          /*!
            \brief Signal emmited to the aplication framework listeners.
          */
          void triggered(te::qt::af::evt::Event*);

        public:

          /*!
            \brief Returns the application name.

            \return Application name.
          */
          const QString& getAppName() const;

          /*!
            \brief Returns the application title.

            \return Application title.
          */
          const QString& getAppTitle() const;

          /*!
            \brief Returns the application project extension.

            \return Application project extension.
          */
          const QString& getAppProjectExtension() const;

          const QString& getAboutLogo() const;

          const QString& getTlibLogo() const;

          /*!
            \brief Returns the application icon.

            \return Application icon.
          */
          const QString& getAppIconName() const;

          /*!
          \brief Returns the plugins file path of application.

          \return A path of plugins file.
          */
          const QString& getAppPluginsPath() const;

          /*!
            \brief Returns the most recent project.

            \return Application most recent opened project.
          */
          QString getMostRecentProject() const;

          /*!
            \brief Returns the application default SRID value.

            \return Application default SRID value.
          */
          int getDefaultSRID() const;

          /*!
            \brief Returns the application selection color.

            \return Application selection color.
          */
          QColor getSelectionColor() const;

          /*! \brief Sets the application selection color. */
          void setSelectionColor(const QColor& c);

          /*!
            \brief Returns main window.

            \return The main window pointer.
          */
          QWidget* getMainWindow() const;

          void setResetTerraLibFlag(const bool& status);
        
          const QString& getUserDataDir() const;

        protected:

          std::map<QString, QToolBar*> m_toolbars;    //!< Toolbars registered.
          std::vector<QMenu*> m_menus;                //!< Menus registered.
          std::vector<QMenuBar*> m_menuBars;          //!< Menu bars registered.
          std::set<QObject*> m_applicationItems;      //!< The list of registered application items.
          QWidget* m_msgBoxParentWidget;              //!< Parent used to show message boxes.
          std::string m_appConfigFile;                //!< The application framework configuration file.
          QString m_userDataDir;                      //!< The data dir used to store data files.
          QString m_appOrganization;                  //!< Organization name.
          QString m_appName;                          //!< Application name.
          QString m_appTitle;                         //!< Application title.
          QString m_appProjectExtension;              //!< Application project extension.
          QString m_appIconName;                      //!< Icon used in the application.
          QString m_aboutLogo;
          QString m_tLibLogo;
          QStringList m_recentProjs;                  //!< List of the recent projects.
          QStringList m_recentProjsTitles;            //!< List of the titles of the recent projects.
          std::string m_appUserSettingsFile;          //!< Name of the user settings file.
          QString m_appPluginsPath;                   //!< Name of the plugins path.
          QString m_appHelpFile;                      //!< Name of the help file.
          QString m_appIconThemeDir;                  //!< Directory of the application icon theme.
          QString m_appDefaultIconTheme;              //!< Name of the icon theme to be used.
          QString m_appToolBarDefaultIconSize;        //!< Size of the tool buttons.
          std::string m_appDatasourcesFile;           //!< Name of the file containing datasources used.
          int m_defaultSRID;                          //!< Default SRID value.
          QColor m_selectionColor;                    //!< Default selection color.

          bool m_initialized;                         //!< A flag indicating if the controller is initialized.

          Project* m_project;                         //!< Pointer to current project.

          QSettings m_appSettings;

          bool m_resetTerralib;

// Singleton instance
//          static ApplicationController* sm_instance;  //!< There can be only one object of class Application.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_AF_INTERNAL_APPLICATIONCONTROLLER_H

