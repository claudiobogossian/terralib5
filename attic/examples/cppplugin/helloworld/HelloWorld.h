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
  \file HelloWorld.h
  
  \brief Example...
 */

#ifndef __TERRALIB_EXAMPLE_CPPPLUGIN_INTERNAL_HELLOWORLD_H
#define __TERRALIB_EXAMPLE_CPPPLUGIN_INTERNAL_HELLOWORLD_H

#ifdef WIN32
  #pragma warning( disable : 4251 )
//#pragma warning( disable : 4275 )

  #ifdef TECPPPLUGINHELLOWORLDDLL
    #define TECPPPLUGINHELLOWORLDEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TECPPPLUGINHELLOWORLDEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TECPPPLUGINHELLOWORLDEXPORT
#endif

// TerraLib
#include <terralib/plugin/CppPlugin.h>

/*!
  \class HelloWorldPlugin
  
  \brief Example....

  \sa CppPlugin, Plugin, PluginManager
 */
class /*TECPPPLUGINHELLOWORLDEXPORT*/ HelloWorldPlugin : public te::plugin::CppPlugin
{
  public:

    /** @name Initializer Methods
     *  Methods related to instantiation and destruction.
     */
    //@{

    /*! \brief It creates a new C++ plugin. */
    HelloWorldPlugin(const te::plugin::PluginInfo& pluginInfo);

    /*! \brief destructor. */
    ~HelloWorldPlugin();

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

    //@}
};   

PLUGIN_CALL_BACK_DECLARATION(TECPPPLUGINHELLOWORLDEXPORT);

#endif  // __TERRALIB_EXAMPLE_CPPPLUGIN_INTERNAL_HELLOWORLD_H
