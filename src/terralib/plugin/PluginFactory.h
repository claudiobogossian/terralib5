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
  \file PluginFactory.h

  \brief This is the abstract factory for Plugins.
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_PLUGINFACTORY_H
#define __TERRALIB_PLUGIN_INTERNAL_PLUGINFACTORY_H

// TerraLib
#include "../common/ParameterizedAbstractFactory.h"
#include "Plugin.h"
#include "PluginInfo.h"

namespace te
{
  namespace plugin
  {
    /*!
      \class PluginFactory

      \brief This is the abstract factory for Plugins.

      If you want to build a plugin, use a command like:
      <code>
      te::plugin::Plugin* p1 = te::plugin::PluginFactory::make("dll", mapOverlayInfo);
      </code>

      \sa Plugin, ParameterizedAbstractFactory
     */
    class TEPLUGINEXPORT PluginFactory : public te::common::ParameterizedAbstractFactory<Plugin, std::string, PluginInfo>
    {
      public:

        /** @name Static Members
         *  Static Members.
         */
        //@{

        /*!
          \brief It makes a new Plugin from the specified file.

          \param fileName The file extension must be a valid one (like ".php", ".dll", ".js").

          \return A Plugin that has not been initialized yet.
         */
        //static Plugin* make(const std::string& fileName);

        //@}

        /** @name Public Destructor
         *  Destructor.
         */
        //@{

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        virtual ~PluginFactory();

        //@} 

      protected:

        /** @name Protected Initializers
         *  Initializers.
         */
        //@{

        /*!
          \brief It creates the factory.

          The key of a PluginFactory is a string.

          \param factoryKey The key that identifies the factory.
         */
        PluginFactory(const std::string& factoryKey);        

        //@}        

    };

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEFACTORY_H


