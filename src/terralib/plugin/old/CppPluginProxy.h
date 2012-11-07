/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file CppPluginProxy.h
  
  \brief A Proxy ....
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXY_H
#define __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXY_H

// TerraLib
#include "../common/Config.h"
#include "Plugin.h"

// Windows
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#include <windows.h>
#endif

namespace te
{
  namespace plugin
  {     
    class CppPlugin;

    /*!
      \class CppPluginProxy
      
      \brief An abstract interface for Plugins written in C++.

      Remember to implement the startup and shutdown methods from the base
      class: Plugin.

      \sa Plugin, PluginManager
     */
    class TEPLUGINEXPORT CppPluginProxy : public Plugin
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It creates a new C++ plugin.

          \param plugin The real C++ plugin. The CppPluginProxy will take the ownership of the given plugin.
         */
        CppPluginProxy(CppPlugin* plugin);

        /*! \brief Destructor. */
        ~CppPluginProxy();

        //@}

        /** @name Re-Implmentation from Plugin Interface
         *  Re-Implmentation from Plugin Interface.
         */
        //@{

        /*!
          \brief This method will be called by TerraLib to startup some plugin's functionality.
                  
          \exception Exception It throws and exception if the plugin can not be started.
         */
        void startup();

        /*!
          \brief This method will be called by TerraLib to shutdown plugin's functionality.
          
          \exception Exception It throws and exception if the plugin can not be shutdown.
         */
        void shutdown();

        /*!
          \brief It return the information associated to the plugin.

          \return The information associated to the plugin.
         */
        const PluginInfo& getInfo() const;

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other instance.
         */
        CppPluginProxy(const CppPluginProxy& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other instance.

          \return A reference for this instance.
         */
        CppPluginProxy& operator=(const CppPluginProxy& rhs);

        //@}

      private:

        CppPlugin* m_plugin;  //!< The real C++ plugin.

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        HMODULE m_module;  //!< Windows plugin's instance.
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
        void* m_module; //!< Linux plugin's instance
#endif
        friend class CppPluginProxyFactory;
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXY_H

