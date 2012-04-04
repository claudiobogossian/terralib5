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
  \file Platform.h
   
  \brief A utility class to initialize and terminate TerraLib OGR driver support.  
 */

#ifndef __TERRALIB_OGR_INTERNAL_PLATFORM_H
#define __TERRALIB_OGR_INTERNAL_PLATFORM_H

// TerraLib
#include "../plugin/CppPlugin.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace ogr
  {
    /*!
      \class Platform

      \brief A utility class to initialize and terminate TerraLib OGR driver support.      
     */
    class TEOGREXPORT Platform : public te::plugin::CppPlugin
    {
      public:

        /** @name Initializer Methods
         *  Constructor/Destructor.
         */
        //@{

        /*! \brief It creates a new C++ module. */
        Platform(const te::plugin::PluginInfo& pluginInfo);

        /*! \brief destructor. */
        ~Platform();

        //@}

        /** @name Re-Implmentation from Plugin Interface
         *  Re-Implementation from Plugin Interface.
         */
        //@{

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

        //@}

        /** @name Static Initializer Methods
         *  Methods that can be used to initialize the OGR module.
         */
        //@{

        /*! \brief It initializes all TerraLib OGR driver support.       
           
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

        /*! \brief It finalizes all TerraLib OGR driver support.
           
            \note This function must be called once in your program. This must
                  be the last call to TerraLib OGR driver. As the Initialize function, this
                  can be called at the end of the main routine of your application.

            \warning Not thread safe!
         */
        static void finalize();

        /*!
          \brief It returns a reference to the driver identifier.

          \return A reference to the driver identifier.
         */
        static const std::string& getDriverID();

        //@}

        static std::string getDriverName(const std::string& name);

      public:

        static bool sm_ogrInitialized; //!< A flag that indicates if the OGR driver is initialized or not.
    };
  } // end namespace ogr
}   // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEOGREXPORT);

#endif  // __TERRALIB_OGR_INTERNAL_PLATFORM_H

