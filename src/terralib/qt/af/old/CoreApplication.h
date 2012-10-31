/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/CoreApplication.h

  \brief The application is a singleton.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_COREAPPLICATION_H
#define __TERRALIB_QT_AF_INTERNAL_COREAPPLICATION_H

// TerraLib
#include "Config.h"

// STL
#include <map>
#include <memory>

// Boost
#include <boost/noncopyable.hpp>

//Qt
//#include <QtCore/QString>
#include <QtGui/QMainWindow>

// Forward declaration
class QAction;
class QActionGroup;
class QSplashScreen;

namespace te
{
  namespace qt
  {
    namespace af
    {
// Forward declaration
      class Event;

      /*!
        \class CoreApplication

        \brief The application is a singleton.

        This is the main window for TerraLib applications based on this framework.
      */
      class TEQTAFEXPORT CoreApplication : public QMainWindow, public boost::noncopyable
      {
        Q_OBJECT

        public:

          /*! Constructor. */
          CoreApplication();

          /*! Virtual destructor. */
          virtual ~CoreApplication();

          /*!
            \brief It gives access to the application singleton.

            \return A pointer to the application singleton.

            \pre The application must have been initialized before calling this method.
          */
          static CoreApplication* getInstance();

          /*!
            \brief It adds a new menu to the application menu bar.

            \param name  A name to be used to identify the menu.
            \param title The new menu title.

            \return A pointer to the new created menu.

            \exception Exception It throws an exception if a menu with the given name already exists.
          */
          QMenu* addMenu(const QString& name, const QString& title);

          /*!
            \brief It searchs for a menu with the given name.

            You can use this method to search for sub-menus. In this case you
            can use a notation like: menu-name.submenu-name.

            All menus and submenus must be named in lower-case.

            \param name The name used to identify the menu in lower-case.

            \return A menu identified by the given name or NULL if none is found.
          */
          QMenu* getMenu(const QString& name) const;

          /*!
            \brief It adds a new toolbar to the application toolbar.

            \param name  A name to be used to identify the toolbar.
            \param title The new toolbar title.

            \return A pointer to the new created toolbar.

            \exception Exception It throws an exception if a toolbar with the given name already exists.
          */
          QToolBar* addToolBar(const QString& name, const QString& title);

          /*!
            \brief It searchs for a toolbar with the given name.

            \param name The name used to identify the toolbar.

            \return A toolbar identified by the given name or NULL if none is found.
          */
          QToolBar* getToolBar(const QString& name) const;

          /*!
            \brief It attaches a new item to the application.

            Use this method to publish widgets that may be shared/accessed by other system parts.

            The Application singleton will index the item by its id. The id may be
            a unique name or category.

            Several items can have the same id.
            For instance a data table can be added using the string "data_table"
            as its id and more than one opened table can have this same id.

            \param id       The application item id (a name or category).
            \param appItem  The application item to be registered in the system.

            \pos The Application singleton will connect its general signal triggered to a item slot called onApplicationTriggered.
                 If the item doesn't provide the slot the connection will not be automatically stablished.

            \exception Exception It thorws an exception if you try to add the same item twice under the same id.

            \note The Application will notify the system by emiting the appItemAdded signal. This signal
                  is emited only if the item was not previously registered.

            \note The Application singleton will not take the ownership of the application item.

            \note It may be possible to register the same item under different id.
          */
          void addItem(const QString& id, QObject* appItem);

          /*!
            \brief It returns the items registered in the system with the given name.

            \param id The application item id (a name or category).

            \return The items registered in the system with the given name.
          */
          QObjectList getAppItemList(const QString& id) const;

        signals:

          /*!
            \brief This is the common signal emited by the application framework when one of its components broadcast a message.

            \param e The event that is broadcasted by the application framework.

            \note Don't release the event pointer. In other words, the receiver can not take its ownership.
          */
          void triggered(Event* e);

          /*!
            \brief This signal is emited when a an appliocation item is registered in the system.

            \param id      The id used to register the application item.
            \param appItem The item being registered.
          */
          void appItemAdded(const QString& id, QObject* appItem);

          /*! \brief This signal is emited whenever a user clicks the refresh menu. */
          void refresh();

        protected slots:

          void fileNewProject();

          void fileOpenProject();

          void fileOpenProject(QAction* action);

          void fileOpenProject(const QString& fileName);

          void fileSaveProject();

          void fileSaveAsProject();

          void fileExit();

          void editUndo();

          void editRedo();

          void editCut();

          void editCopy();

          void editPaste();

          void editSelectAll();

          void editClear();

          void editFind();

          void editReplace();

          void viewFullScreen(bool checked);

          void viewRefresh();

          void pluginManagement();

          void pluginBuilder();

        protected:

          /*! \brief Sub-classes may re-implement this method in order to have a different initialization process. */
          virtual void initialize();

          /*! \brief Sub-classes may re-implement this method in order to have a different termination process. */
          virtual void finalize();

          /*! \brief It creates the common application actions. */
          virtual void createActions();

          /*! \brief It creates the common application menus. */
          virtual void createMenus();

          /*! \brief It creates the common application toolbars. */
          virtual void createToolBars();

          /*! \brief It creates the status bar. */
          virtual void createStatusBar();

          virtual void createIconThemesActions();

          virtual void createIconThemesMenu();

          virtual void createPluginCategoriesMenu();

// Events overload
          virtual void closeEvent(QCloseEvent* e);

        protected:

// Application items
          std::map<QString, QObjectList> m_appItems;  //!< A map from application item category/name -> application item

// Toolbars index
          std::map<QString, QToolBar*> m_toolbars;  //!< A map from toolbar name -> toolbar

// Menu - File
          QMenu* m_fileMenu;

          QToolBar* m_fileToolBar;

// Menu - File -> Project
          QAction* m_fileNewProject;
          QAction* m_fileOpenProject;
          QAction* m_fileSaveProject;
          QAction* m_fileSaveAsProject;
          QAction* m_fileImportProject;
          QAction* m_fileExportProject;
          QAction* m_filePrint;
          QAction* m_filePrintPreview;

          QMenu* m_fileMenuRecentProjects;

          QActionGroup* m_fileRecentProjects;

// Menu - File -> Exit
          QAction* m_fileExit;

// Menu - Edit
          QMenu* m_editMenu;

          QToolBar* m_editToolBar;

          QAction* m_editUndo;
          QAction* m_editRedo;
          QAction* m_editCut;
          QAction* m_editCopy;
          QAction* m_editPaste;
          QAction* m_editSelectAll;
          QAction* m_editClear;
          QAction* m_editFind;
          QAction* m_editReplace;

// Menu - View
          QMenu* m_viewMenu;
          QMenu* m_viewToolBarsMenu;

          QToolBar* m_viewToolBar;
          QToolBar* m_viewFullScreenToolBar;

          //QAction* m_viewLayerExplorer;
          //QAction* m_viewMapDisplay;
          //QAction* m_viewZoomIn;
          //QAction* m_viewZoomOut;
          //QAction* m_viewZoomSelect;
          //QAction* m_viewZoomFit;
          QAction* m_viewFullScreen;
          QAction* m_viewRefresh;

// Menu - Settings
          QMenu* m_settingsMenu;

// Menu - Settings -> IconTheme
          QActionGroup* m_iconThemes;

          QMenu* m_iconThemesMenu;

// Menu - Settings -> preferences
          QAction* m_settingsPreferences;

// Menu - Plugins
          QMenu* m_pluginsMenu;

          QAction* m_pluginManager;
          QAction* m_pluginBuilder;

// Menu - Help
          QMenu* m_helpMenu;

          QAction* m_helpContents;
          QAction* m_helpUpdate;

// Singleton instance
          static CoreApplication* sm_instance;  //!< There can be only one object of class Application.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_COREAPPLICATION_H

