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
  \file terralib/qt/af/BaseApplicationController.h

  \brief The base API for controllers of TerraLib applications.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATIONCONTROLLER_H
#define __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATIONCONTROLLER_H

// Terralib
#include "Config.h"

// STL
#include <map>
#include <set>
#include <vector>

// Qt
#include <QObject>

// Forward declarations
class QToolBar;
class QMenu;

namespace te
{
  namespace qt
  {
    namespace af
    {
// Forward declarations
      class Event;

      /*!
        \class BaseApplicationController

        \brief The base API for TerraLib applications.
      */
      class TEQTAFEXPORT BaseApplicationController : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param parent The parent object.
          */
          BaseApplicationController(QObject* parent = 0);

          /*! \brief Destructor. */
          virtual ~BaseApplicationController();

          virtual void setMainConfig(const std::string& configFileName);

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
            - ???
            - ???
            - ???

            It will start by initializing the available TerraLib modules.
            This means that applications that uses the application framework, 
            does not need to call TerraLib initialization (TerraLib::getInstance().initialize()) and 
            finalization methods (TerraLib::getInstance().finalize()).

            \exception Exception It throws an exception if the initialization fails.

            \pre The application must assure that any resource needed by their plugins are ready for use.

            \note Make sure that the application calls this method BEFORE it uses any TerraLib modules or functions.
          */
          virtual void initialize();

          virtual void initializeMainModules();

          virtual void initializePlugins();

          /*!
            \brief Finalize the application framework.

            \details This method will unload all TerraLib modules, plugins, and dynamic libraries that are loaded.

            \note Make sure that the application calls this method BEFORE it finishes.
          */
          virtual void finalize();

        public slots:

          /*!
            \brief Send events in broadcast for all registered components.

            \param evt Event to be sent.
          */
          void broadcast(te::qt::af::Event* evt);

        signals:

          /*!
            \brief Signal emmited for the aplication framework listeners.
          */
          void triggered(te::qt::af::Event*);

        protected:

          std::map<QString, QToolBar*> m_toolbars;    //!< Toolbars registered.
          std::set<QObject*> m_applicationItems;      //!< The list of registered application items.
          std::string m_configFile;                   //!< The application framework configuration file.
          bool m_initialized;                         //!< A flag indicating if the controller is initialized.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATIONCONTROLLER_H

