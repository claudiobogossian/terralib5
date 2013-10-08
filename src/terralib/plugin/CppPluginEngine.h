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
  \file terralib/plugin/CppPluginEngine.h
  
  \brief A plugin engine for plugins written in C++.
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINENGINE_H
#define __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINENGINE_H

// TerraLib
#include "AbstractPluginEngine.h"
#include "Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace plugin
  {     
    /*!
      \class CppPluginEngine
      
      \brief A plugin engine for plugins written in C++.

      \sa Plugin, PluginInfo, PluginEngineFactory, PluginEngine
    */
    class TEPLUGINEXPORT CppPluginEngine : public AbstractPluginEngine
    {
      public:

        /*! Default construtor. */
        CppPluginEngine();

        /*! \brief Destructor. */
        ~CppPluginEngine() { }

        AbstractPlugin* load(const PluginInfo& pInfo);

        void unload(AbstractPlugin* plugin);

        /*!
          \brief This method will help to form the library name wich contains the plugin.

          This method may append a prefix and suffix that are platform dependent
          and also the build mode: DEBUG or RELEASE.
        */
        static std::string getPluginFileName(const std::string& libName);
        
        /*!
          \brief It returns the default plugins libraries search directories.

          This method will generate directory paths in the following order:
          <ul>
          <li> The current execution directory.</li>
          <li> The directory "lib" under the current execution directory.</li>
          <li> The directory "lib" under the directory given by an environment variable defined by TERRALIB_DIR_ENVIRONMENT_VARIABLE)</li>
          <li> The directory defined by the macro TE_DEFAULT_PLUGINS_DIR</li>
          <li> The directory defined by the macro TE_DEFAULT_PLUGINS_DIR unde the directory defined by TERRALIB_DIR_ENVIRONMENT_VARIABLE</li>
          <li> Directories listed by the environmental variable LD_LIBRARY_PATH.</li>
          <li> Directories listed by the environmental variable PATH.</li>
          </ul>

          \param dirs The default plugin libraries search directories (the absolute path) or an empty string if none is found.
        */
        static void getDefaultDirs( std::vector< std::string >& dirs );  
        
      protected :
        
        std::vector< std::string > m_defaultSearchDirs; //!< The default plugin libraries search directories.
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINENGINE_H

