/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mysql/Module.h
   
  \brief The TerraLib MySQL driver is a plugin.
 */

#ifndef __TERRALIB_MYSQL_INTERNAL_MODULE_H
#define __TERRALIB_MYSQL_INTERNAL_MODULE_H

// TerraLib
#include "../plugin/Plugin.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace mysql
  {
    /*!
      \class Module

      \brief An utility class to initialize and terminate TerraLib MySQL driver support.

      \sa te::plugin::Plugin
    */
    class Module : public te::plugin::Plugin
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
          \brief It initializes the TerraLib MySQL driver support.       
           
          The initialization includes:
          <ul>
          <li>Multilanguage support</li>
          <li>MySQL capabilities</li>
          <li>MySQL query dialect</li>
          </ul>
           
          \exception Exception It may throws an exception.
         
          \warning Not thread safe!
        */
        void startup();

        /*!
          \brief It finalizes the TerraLib MySQL driver support.
           
          \exception Exception It may throws an exception.

          \warning Not thread safe!
        */
        void shutdown();
    };

  } // end namespace mysql
}   // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEMYSQLEXPORT);

#endif  // __TERRALIB_MYSQL_INTERNAL_MODULE_H

