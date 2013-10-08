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
  \file terralib/st/Module.h
   
  \brief The TerraLib ST module is a plugin.
*/

#ifndef __TERRALIB_ST_INTERNAL_MODULE_H
#define __TERRALIB_ST_INTERNAL_MODULE_H

// TerraLib
#include "../plugin/Plugin.h"
#include "Config.h"

namespace te
{
  namespace st
  {
    /*!
      \class Module

      \brief The TerraLib ST module is a plugin.    
    */
    class Module : public te::plugin::Plugin
    {
      public:

        /*!
          \brief Plugin constructor.
            
          \param pInfo Basic information provided to initialize this module through the plugin API.
        */
        Module(const te::plugin::PluginInfo& pInfo);
        
        /* \brief Destructor. */
        ~Module();

         /*!
          \brief It initializes the TerraLib st module support.       
           
          The initialization includes:
          <ul>
          <li>Multilanguage support</li>
          <li>STDataLoader factory</li>
          </ul>
           
          \exception Exception It may throws an exception.
         
          \note This function must be called at least once in your program. The best
                place to call it is inside the main routine of your application.

          \warning Not thread safe!

          \warning If this method throws an exception we recommend you to
                    quit the program, don't try to resume it because you can have intermittent errors!
        */
        void startup();

        /*!
          \brief It finalizes the TerraLib st module support.    
           
          \exception Exception It may throws an exception.
           
          \note This function must be called in your program at least the same times you have called
                initialize. In general this must be the last call to TerraLib API. As in case of the initialize method,
                this routine can be called at the end of the main routine of your application.

          \warning Not thread safe!

          \warning If this method throws an exception we recommend you to
                   quit the program, don't try to resume it because you can have intermittent errors!
        */
        void shutdown();     
    };

  } // end namespace st
}   // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TESTEXPORT);

#endif  // __TERRALIB_ST_INTERNAL_MODULE_H

