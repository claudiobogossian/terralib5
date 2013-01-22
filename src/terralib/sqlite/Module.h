/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/Module.h
   
  \brief The TerraLib SQLite Data Access driver implements the plugin interface.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_MODULE_H
#define __TERRALIB_SQLITE_INTERNAL_MODULE_H

// TerraLib
#include "../plugin/Plugin.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace sqlite
  {
    /*!
      \class Module

      \brief This is only an utility class to implement the plugin API for the SQLite Data Access driver.

      \sa te::plugin::Plugin
    */
    class TESQLITEEXPORT Module : public te::plugin::Plugin
    {
      public:

        /*!
          \brief Plugin constructor.
            
          \param pInfo Basic information provided to initialize this module through the plugin API.
        */
        Module(const te::plugin::PluginInfo& pluginInfo);

        /*! \brief Destructor. */
        ~Module();

        /*! 
          \brief It initializes the TerraLib SQLite driver support.
           
          The initialization includes:
          <ul>
          <li>Multilanguage support</li>
          <li>SQLite capabilities</li>
          <li>SQLite query dialect</li>
          </ul>
           
          \exception Exception It may throws an exception.
         
          \warning Not thread safe!
        */
        void startup();

        /*!
          \brief It finalizes the TerraLib SQLite driver support.
           
          \exception Exception It may throws an exception.

          \warning Not thread safe!
        */
        void shutdown();
    };

  } // end namespace sqlite
}   // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TESQLITEEXPORT);

#endif  // __TERRALIB_SQLITE_INTERNAL_MODULE_H

