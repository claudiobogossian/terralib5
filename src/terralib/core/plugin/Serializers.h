/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/plugin/Serializers.h

  \brief General utilities for serializing plugin information.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_SERIALIZERS_H__
#define __TERRALIB_CORE_PLUGIN_SERIALIZERS_H__

// TerraLib
#include "../Config.h"
#include "PluginInfo.h"

namespace te
{
  namespace core
  {

    /*! A plugin finder that search for plugins in some special directories defined by compile time macros. */
    TECOREEXPORT PluginInfo JSONPluginInfoSerializer(const std::string& file_name);

  } // end namespace core
}   // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_SERIALIZERS_H__
