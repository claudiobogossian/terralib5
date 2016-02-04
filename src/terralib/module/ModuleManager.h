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
  \file terralib/module/ModuleManager.h

  \brief A singleton for managing modules.

  \author Gilberto Ribeiro de Queiroz
  \author Frederico Augusto Bede
 */

#ifndef __TERRALIB_MODULE_MODULEMANAGER_H__
#define __TERRALIB_MODULE_MODULEMANAGER_H__

// TerraLib
#include "Config.h"

// STL
#include <memory>
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace module
  {

// Forward declaration
    struct Module;

    //! A singleton for managing modules.
    /*!
      \note Methods in this class are not thread-safe.
     */
    class TEMODULEEXPORT ModuleManager : public boost::noncopyable
    {
      public:

        //! Return the list of module managed by ModuleManager.
        std::vector<std::string> listModules() const;

        //! Return the module identified by the given name.
        /*!
          \param name The module name.

          \return A module identified by the given name.

          \exception tws::item_not_found_error It throws an exception if there isn't a plugin with the given name in the manager.
         */
        std::shared_ptr<Module> getModule(const std::string& name) const;

        //! Tells if a given module is registered or not.
        bool exists(const std::string& module_name) const;

        //! Register the module.
        /*!
          \exception plugin_already_registered_error Throw an exception if a plugin with the same
                     name already exists and it is registered in the manager.
         */
        void insert(const std::shared_ptr<Module>& m);

        //! Un-register module from the manager.
        /*!
          This method removes the plugin from the manager.
          If the plugin was loaded, unload it and remove it from the manager.
          If it was unloaded or broked, just removes it from the correct list.
          Note that all its dependents will be moved to the broken list.

          \param plugin_name Name of the plugin to be removed.

          \exception tws::exception If the plugin can not be removed an exception is raised.

          \note Don't change the type of parameter to a const reference! (guess?)
         */
        void remove(const std::string& module_name);

        //! Access the singleton.
        static ModuleManager& instance();

      private:

        //! Constructor
        ModuleManager();

        //! Destructor.
        ~ModuleManager();

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  } // end namespace module
}   // end namespace te

#endif  // __TERRALIB_MODULE_MODULEMANAGER_H__
