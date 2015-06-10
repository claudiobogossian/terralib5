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
  \file CppPlugin.h
  
  \brief An abstract interface for Plugins written in C++.
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_CPPPLUGIN_H
#define __TERRALIB_PLUGIN_INTERNAL_CPPPLUGIN_H

// TerraLib
#include "Plugin.h"

namespace te
{
  namespace plugin
  {     
    /*!
      \class CppPlugin
      
      \brief An abstract interface for Plugins written in C++.

      Remember to implement the startup and shutdown methods from the base
      class: Plugin.

      \ingroup plugin

      \sa Plugin, PluginManager
     */
    class TEPLUGINEXPORT CppPlugin : public Plugin
    {
      public:

        /*!
          \brief It creates a new C++ plugin.

          \param info Information about a C++ plugin.
        */
        CppPlugin(const PluginInfo& info);

        /*! \brief Virtual destructor. */
        virtual ~CppPlugin();

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other instance.
        */
        CppPlugin(const CppPlugin& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other instance.

          \return A reference for this instance.
        */
        CppPlugin& operator=(const CppPlugin& rhs);

        //@}
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_CPPPLUGIN_H

