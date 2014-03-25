/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/plugin/CppPluginEngine.cpp
  
  \brief A plugin engine for plugins written in C++.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Library.h"
#include "../common/LibraryManager.h"
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../common/PlatformUtils.h"
#include "CppPluginEngine.h"
#include "CppPluginProxy.h"
#include "Plugin.h"
#include "PluginInfo.h"

// STL
#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>

// Boost
#include <boost/filesystem.hpp>

te::plugin::CppPluginEngine::CppPluginEngine()
{
  getDefaultDirs( m_defaultSearchDirs );
}

te::plugin::AbstractPlugin* te::plugin::CppPluginEngine::load(const PluginInfo& pInfo)
{
  PluginInfo internalPluginInfo = pInfo;
  
// get the plugin's shared library name in the resources list
  std::vector<PluginInfo::Resource>::const_iterator it = std::find_if(internalPluginInfo.m_resources.begin(),
                                                                      internalPluginInfo.m_resources.end(),
                                                                      PluginInfo::Finder1st("SharedLibraryName"));

  if(it == internalPluginInfo.m_resources.end())
  {
    std::string m  = TE_TR("Shared library name not informed for plugin: ");
                m += pInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

// get the adequate name depending on the platform and build type (release or debug)
  std::string libName = getPluginFileName(it->second);

// is there a library already loaded with this name?
  te::common::LibraryPtr slib(te::common::LibraryManager::getInstance().find(libName));

  if(slib.get() == 0)
  {
// if not loaded, load it!

    boost::filesystem::path pluginFile( internalPluginInfo.m_folder );
    pluginFile /= libName;
    
// the plugin library file may be in a special dir informed by internalPluginInfo.m_folder
    if( boost::filesystem::exists(pluginFile) && 
      boost::filesystem::is_regular_file(pluginFile) )
    {
      // create shared library entry but doesn't load it yet!
      slib.reset(new te::common::Library(pluginFile.string(), true));
    }
    else
    {
      for( std::vector< std::string >::size_type dirIdx = 0 ; dirIdx <
        m_defaultSearchDirs.size() ; ++dirIdx )
      {
        pluginFile = m_defaultSearchDirs[ dirIdx ];
        pluginFile /= libName;
        
        if(boost::filesystem::exists(pluginFile) &&
          boost::filesystem::is_regular_file(pluginFile) )
        {
          internalPluginInfo.m_folder = m_defaultSearchDirs[ dirIdx ];
          slib.reset(new te::common::Library(pluginFile.string(), true));
          break;
        }
      }
    }
  }
  
  if(slib.get() == 0)
  {
    std::string m  = TE_TR("Could not find load plugin: ");
                m += internalPluginInfo.m_name;
    throw te::common::Exception(m);
  }  
  
  if(!slib->isLoaded())
    slib->load();

// now we need to get the plugin constructor function address
  GetPluginFPtr getPluginFptr = (GetPluginFPtr) (slib->getAddress("CppPluginGetInstance"));

  if(getPluginFptr == NULL)
  {
    std::string m  = TE_TR("Could not find CppPluginGetInstance function into the plugin's code: ");
                m += internalPluginInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

  std::auto_ptr<Plugin> cppPlugin(getPluginFptr(internalPluginInfo));

  if(cppPlugin.get() == 0)
  {
    std::string m  = TE_TR("CppPluginGetInstance returned a null plugin instance: ");
                m += internalPluginInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

  CppPluginProxy* proxy = new CppPluginProxy(slib, cppPlugin.release());

  te::common::LibraryManager::getInstance().add(libName, slib);

  return proxy;
}

void te::plugin::CppPluginEngine::unload(AbstractPlugin* plugin)
{
  assert(plugin && (plugin->getInfo().m_engine == "C++"));

  delete plugin;
}

std::string te::plugin::CppPluginEngine::getPluginFileName(const std::string& libName)
{
#ifdef NDEBUG
  std::string nativeName = libName;
#else
  std::string nativeName = libName + "_d";  // add a suffix _d to the library name in debug mode
#endif
  nativeName = te::common::Library::getNativeName(nativeName);

  return nativeName;
}

void te::plugin::CppPluginEngine::getDefaultDirs( std::vector< std::string >& dirs )
{
  dirs.clear();
  
  dirs.push_back( "." );
  
  {
    boost::filesystem::path p("lib");
    
    if(boost::filesystem::is_directory(p))
      dirs.push_back( boost::filesystem::system_complete( p ).string() );   
  }

  {
    boost::filesystem::path p("win32");
    
    if(boost::filesystem::is_directory(p))
      dirs.push_back( boost::filesystem::system_complete( p ).string() );   
  }

  {
    boost::filesystem::path p("win64");
    
    if(boost::filesystem::is_directory(p))
      dirs.push_back( boost::filesystem::system_complete( p ).string() );   
  }
  
  #ifdef TE_DEFAULT_PLUGINS_DIR
    if(boost::filesystem::is_directory(TE_DEFAULT_PLUGINS_DIR))
    {
      dirs.push_back( boost::filesystem::system_complete(TE_DEFAULT_PLUGINS_DIR).string() );
    }
  #endif

  #ifdef TE_DIR_ENVIRONMENT_VARIABLE
    {
      char* e = getenv(TE_DIR_ENVIRONMENT_VARIABLE);

      if(e != 0)
      {
	      if(boost::filesystem::is_directory(e))
          dirs.push_back( boost::filesystem::system_complete(e).string() );

        {
          boost::filesystem::path p(e);
          p /= "lib";

          if(boost::filesystem::is_directory(p))
            dirs.push_back( boost::filesystem::system_complete(p).string() );
        }  

        {
          boost::filesystem::path p(e);
          p /= "bin32";

          if(boost::filesystem::is_directory(p))
            dirs.push_back( boost::filesystem::system_complete(p).string() );
        }

        {
          boost::filesystem::path p(e);
          p /= "bin64";

          if(boost::filesystem::is_directory(p))
            dirs.push_back( boost::filesystem::system_complete(p).string() );
        }
      
        #ifdef TE_DEFAULT_PLUGINS_DIR
          {
            boost::filesystem::path p(e);
            p /= TE_DEFAULT_PLUGINS_DIR;

            if(boost::filesystem::is_directory(p))
              dirs.push_back( boost::filesystem::system_complete(p).string() );
          }
        #endif
      }
    }
  #endif

  {
    char* e = getenv("TERRALIB_DIR");

    if(e != 0)
    {
	    if(boost::filesystem::is_directory(e))
        dirs.push_back( boost::filesystem::system_complete(e).string() );

      {
        boost::filesystem::path p(e);
        p /= "lib";

        if(boost::filesystem::is_directory(p))
          dirs.push_back( boost::filesystem::system_complete(p).string() );
      }  

      {
        boost::filesystem::path p(e);
        p /= "bin32";

        if(boost::filesystem::is_directory(p))
          dirs.push_back( boost::filesystem::system_complete(p).string() );
      }

      {
        boost::filesystem::path p(e);
        p /= "bin64";

        if(boost::filesystem::is_directory(p))
          dirs.push_back( boost::filesystem::system_complete(p).string() );
      }
      
      #ifdef TE_DEFAULT_PLUGINS_DIR
        {
          boost::filesystem::path p(e);
          p /= TE_DEFAULT_PLUGINS_DIR;

          if(boost::filesystem::is_directory(p))
            dirs.push_back( boost::filesystem::system_complete(p).string() );
        }
      #endif
    }
  }

  #ifdef TE_PLUGINS_PATH
    {
	    if(boost::filesystem::is_directory(TE_PLUGINS_PATH))
        dirs.push_back( boost::filesystem::system_complete(TE_PLUGINS_PATH).string() );

      {
        boost::filesystem::path p(TE_PLUGINS_PATH);
        p /= "lib";

        if(boost::filesystem::is_directory(p))
          dirs.push_back( boost::filesystem::system_complete(p).string() );
      }  

      {
        boost::filesystem::path p(TE_PLUGINS_PATH);
        p /= "bin32";

        if(boost::filesystem::is_directory(p))
          dirs.push_back( boost::filesystem::system_complete(p).string() );
      }

      {
        boost::filesystem::path p(TE_PLUGINS_PATH);
        p /= "bin64";

        if(boost::filesystem::is_directory(p))
          dirs.push_back( boost::filesystem::system_complete(p).string() );
      }
      
      #ifdef TE_DEFAULT_PLUGINS_DIR
        {
          boost::filesystem::path p(TE_PLUGINS_PATH);
          p /= TE_DEFAULT_PLUGINS_DIR;

          if(boost::filesystem::is_directory(p))
            dirs.push_back( boost::filesystem::system_complete(p).string() );
        }
      #endif
    }
  #endif
 
  std::vector< std::string > decPath;
  te::common::GetDecompostedPathEnvVar( decPath );
  for( std::vector< std::string >::size_type decPathIdx = 0 ; decPathIdx < decPath.size() ;
    ++decPathIdx )
  {
    dirs.push_back( decPath[ decPathIdx ] );
  }
  
  std::vector< std::string > decLDPath;
  te::common::GetDecompostedLDPathEnvVar( decLDPath );
  for( std::vector< std::string >::size_type decLDPathIdx = 0 ; decLDPathIdx < decLDPath.size() ;
    ++decLDPathIdx )
  {
    dirs.push_back( decLDPath[ decLDPathIdx ] );
  }
}

