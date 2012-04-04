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
  \file PluginInfo.h
  
  \brief Information about a given Plugin.
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_PLUGININFO_H
#define __TERRALIB_PLUGIN_INTERNAL_PLUGININFO_H

// STL
#include <string>

namespace te
{
  namespace plugin
  {
    /*!
      \struct PluginInfo
      
      \brief Information about a given Plugin.

      \sa Plugin, PluginFactory
     */
    struct PluginInfo
    {
      std::string m_name;
      std::string m_displayName;
      std::string m_description;
      std::string m_mainFile;
      std::string m_type;
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_PLUGININFO_H

