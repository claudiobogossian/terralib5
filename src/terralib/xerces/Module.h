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
  \file Module.h
   
  \brief This TerraLib Xerces driver as a plugin.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_MODULE_H
#define __TERRALIB_XERCES_INTERNAL_MODULE_H

// TerraLib
#include "../plugin/CppPlugin.h"
#include "Config.h"

namespace te
{
  namespace xerces
  {
    /*!
      \class Module

      \brief The TerraLib Xerces module is a plugin.
    */
    class Module : public te::plugin::CppPlugin
    { 
    public:
      
      /*! 
       \brief Plugin constructor.
       
       \param pInfo Basic information needed to initialize the plugin.
       */
      Module(const te::plugin::PluginInfo& pInfo);
      
      /* \brief Destructor. */
      ~Module();        
      
      /*!
       \brief This method will be called by TerraLib to startup some plugin's functionality.
       
       \exception Exception It throws an exception if the plugin can not be started.
       */
      void startup();
      
      /*!
       \brief This method will be called by TerraLib to shutdown plugin's functionality.
       
       \exception Exception It throws an exception if the plugin can not be shutdown.
       */
      void shutdown();
      
      /*! \brief It initializes all TerraLib XERCES driver support.       
       
       The initialization includes:
       <ul>
       <li>Multilanguage support;</li>
       <li>Logger initialization.</li>
       </ul>
       
       \note This function must be called once in your program. I think that the best
       place to call it, is inside the main routine of your application.
       
       \warning Not thread safe!
       */
      static void initialize();
      
      /*! \brief It finalizes all TerraLib XERCES driver support.
       
       \note This function must be called once in your program. This must
       be the last call to TerraLib XERCES driver. As the Initialize function, this
       can be called at the end of the main routine of your application.
       
       \warning Not thread safe!
       */
      static void finalize();
      
    public:
      
      static bool m_initialized;      
    };
  } // end namespace xerces
}   // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEXERCESEXPORT)

#endif  // __TERRALIB_XERCES_INTERNAL_MODULE_H

