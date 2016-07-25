/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/lib/LibraryManager.h

  \brief A singleton that can be used to observe the available shared libraries in the system.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_LIB_LIBRARYMANAGER_H__
#define __TERRALIB_CORE_LIB_LIBRARYMANAGER_H__

// TerraLib
#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace core
  {
    /*!
      \typedef void (*StartupFptr)(void);

      \brief This is the type for call back functions that makes the startup of a module.
     */
    typedef boost::function0<void> StartupFnct;

    /*!
      \typedef void (*CleanupFptr)(void);

      \brief This is the type for call back functions that makes the cleanup of a module.
     */
    typedef boost::function0<void> CleanupFnct;


    struct LibraryEntry
    {
      std::string m_name;
      StartupFnct m_startFptr;
      CleanupFnct m_cleanupFptr;
      bool m_initialized;
    };

    /*!
      \class LibraryManager

      \brief A singleton that can be used to observe the available libraries in the system.

      This singleton doesn't control the libraries lifetime, it just make smart
      references to them. These references will be automatically removed
      when a library goes out of scope (or been destroyed). Actually it works
      like an observer of known libraries.
     */
    class TECOREEXPORT LibraryManager
    {
      public:

        /*!
          \brief It inserts a LibraryEntry to the manager.

          \param entry LibraryEntry to be managed.

          \exception te::InvalidArgumentException If a LibraryEntry with the given name is already registered.
         */
        void insert(const LibraryEntry& entry);

        /*!
          \brief It removes a LibraryEntry from the manager.

          \param name The name of the LibraryEntry.

          \exception te::OutOfRangeException If a LibraryEntry with the given name is not registered.,
          \exception te::Exception If trying to remove a initialized library.
         */
        void remove(const std::string& name);

        /*!
          \brief Return a null pointer if a library doesnt't exist.

          \param name The LibraryEntry name.

          \return a const reference to the LibraryEntry

          \exception te::OutOfRangeException If a LibraryEntry with the given name is not registered.
        */
        const LibraryEntry& get(const std::string& name);

        /*!
          \brief The current state of the LibraryEntry.

          \param name The name of the LibraryEntry.

          \return true if the LibraryEntry is initialized or false if isn't.

          \exception te::OutOfRangeException If a LibraryEntry with the given name is not registered.
         */
        bool isInitialized(const std::string& name);

        /*!
          \brief Checks if a LibraryEntry exists from a given name

          \param name The name of the LibraryEntry

          \return true if the LibraryEntry exist or false if doesn't.
         */
        bool exists(const std::string& name);

        /*!
          \brief It returns a reference to the singleton instance.

          \return A reference to the singleton instance.
         */
        static LibraryManager& instance();

      protected:

        /*! \brief Singleton constructor must be private or protected. */
        LibraryManager();

        /*! \brief Singleton destructor must be private or protected. */
        ~LibraryManager();

// no copy allowed
        LibraryManager(const LibraryManager&);
        LibraryManager& operator=(const LibraryManager&);

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  }  // end namespace core
}    // end namespace te

#define TERRALIB_LIBRARY_BEGIN(library_name) \
class library_ ## library_name \
{ \
  public: \
\
    library_ ## library_name() \
    { \
      LibraryEntry le = {#library_name, \
                         startup, \
                         shutdown \
                        }; \
\
       te::core::LibraryManager::instance().insert(le); \
    } \
\
    ~library_ ## library_name() \
    {\
      if(te::core::LibraryManager::instance().exists(#library_name))\
      {\
        te::core::LibraryManager::instance().get(#library_name).shutdown();\
        te::core::LibraryManager::instance().remove(#library_name);\
      }\
    }


#define TERRALIB_LIBRARY_STARTUP \
    void startup()

#define TERRALIB_LIBRARY_SHUTDOWN \
    void shutdown()

#define TERRALIB_LIBRARY_END(library_name) \
}; \
\
static library_ ## library_name s_lib;

#endif  // __TERRALIB_CORE_LIB_LIBRARYMANAGER_H__
