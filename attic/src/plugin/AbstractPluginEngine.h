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
  \file terralib/plugin/AbstractPluginEngine.h
  
  \brief An abstract class for plugin engines.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_ABSTRACTPLUGINENGINE_H
#define __TERRALIB_PLUGIN_INTERNAL_ABSTRACTPLUGINENGINE_H

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace plugin
  {
// Forward declaration
    class AbstractPlugin;

    struct PluginInfo;

    /*!
      \class CppPluginEngine
      
      \brief An abstract class for plugin engines.

      For each kind of programming language there will be
      an specific engine capable of loading and preparing the
      plugin before the startup method is called.

      \ingroup plugin,

      \sa Plugin, PluginInfo, PluginEngineFactory, CppPluginEngine
     */
    class AbstractPluginEngine : public boost::noncopyable
    {
      public:

        /*! Default construtor. */
        AbstractPluginEngine() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractPluginEngine() { }

        /*!
          \brief It try to create and load the informed plugin.

          \param pInfo The information needed to create and load the plugin.

          \return It returns a loaded plugin. The caller will take the ownership of the returned plugin.

          \exception Exception It throws an exception if the plugin can not be loaded.
          
          \note If the parameter PluginInfo::m_folder does not contains the required plugin it may be searched and loaded using a set of default paths.
        */
        virtual AbstractPlugin* load(const PluginInfo& pInfo) = 0;

        /*!
          \brief It try to unload the informed plugin.

          On success the informed plugin will be also destroyed
          and its pointer will be invalidated.

          \param plugin The plugin to be unloaded.

          \exception Exception It throws an exception if the plugin can not be unloaded.
        */
        virtual void unload(AbstractPlugin* plugin) = 0;
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_ABSTRACTPLUGINENGINE_H

