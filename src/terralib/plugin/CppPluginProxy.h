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
  \file terralib/plugin/CppPluginProxy.h
  
  \brief A proxy class for C++ plugins.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXY_H
#define __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXY_H

// TerraLib
#include "../common/Library.h"
#include "AbstractPlugin.h"

// STL
#include <memory>

namespace te
{
  namespace plugin
  {     
// Forward declaration
    class Plugin;

    /*!
      \class CppPluginProxy
      
      \brief A proxy class for C++ plugins.

      \ingroup plugin

      \sa AbstractPlugin, Plugin, CppPlugin
     */
    class TEPLUGINEXPORT CppPluginProxy : public AbstractPlugin
    {
      public:

        /*!
          \brief Construct a proxy for the real C++ plugin.

          \param lib    A pointer to the library that contains the plugin.
          \param plugin The real C++ plugin. The CppPluginProxy will take the ownership of the given plugin.
        */
        CppPluginProxy(const te::common::LibraryPtr& lib, Plugin* plugin);

        /*! \brief Destructor. */
        ~CppPluginProxy();

        /*! \brief Forward the info retrieval to the real plugin. */
        const PluginInfo& getInfo() const;

        /*! \brief Forward the info retrieval to the real plugin. */
        bool isStarted() const;

        /*! \brief Forward the startup to the real plugin . */
        void startup();

        /*! \brief Forward the shutdown to the real plugin. */
        void shutdown();

      private:

        te::common::LibraryPtr m_lib;    //!< The library that contains the real plugin.
        std::auto_ptr<Plugin> m_plugin;  //!< The real C++ plugin.
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXY_H

