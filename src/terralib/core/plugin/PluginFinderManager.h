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
  \file terralib/core/plugin/PluginFinderManager.h

  \brief A singleton that can be used to register plugin finders.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */
 
#ifndef __TERRALIB_CORE_PLUGIN_PLUGINFINDERMANAGER_H__
#define __TERRALIB_CORE_PLUGIN_PLUGINFINDERMANAGER_H__

// TerraLib
#include "../Config.h"
#include "PluginInfo.h"

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace core
  {

    //! Finders allow applications to extend how to search for plugins.
    typedef boost::function0<std::vector<PluginInfo> > PluginFinderFnct;

    //! A singleton that can be used to register plugin finders.
    class TECOREEXPORT PluginFinderManager
    {
      public:

        //! Return the list of registered plugin finders.
        std::vector<std::string> getFinders() const;

        //! Return a specific plugin finder.
        /*!
          \exception te::item_not_found_error If a finder with the given name is not found.
         */
        PluginFinderFnct& get(const std::string& finder_name);

        //! Registers a new plugin finder
        /*!
         \exception plugin_finder_already_registered_error Throw an exception if
                    a finder with the same name already exists.
         */
        void insert(const std::string& finder_name,
                    PluginFinderFnct finder);

        //! Remove a plugin finder from the manager.
        /*!
          \exception te::item_not_found_error If a finder with the given name is not found.
         */
        void remove(const std::string& finder_name);

        //! Return true if a finder with the given name is registered.
        bool exists(const std::string& finder_name) const;

        //! Access the singleton.
        static PluginFinderManager& instance();

      protected:

        /*! \brief Consructor. */
        PluginFinderManager();

        /*! \brief Destructor. */
        ~PluginFinderManager();

      private:

// no copy allowed
        PluginFinderManager(const PluginFinderManager&);
        PluginFinderManager& operator=(const PluginFinderManager&);

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  } // end namespace core
}   // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_PLUGINFINDERMANAGER_H__
