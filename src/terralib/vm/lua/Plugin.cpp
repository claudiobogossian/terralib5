/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib - a Framework for building GIS enabled
  applications.

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
  \file terralib/vm/lua/Plugin.cpp

  \brief Add a plugin interface for dynamic loading of Lua Virtual Machine for
  TerraLib.

  \author Gilberto Ribeiro de Queiroz
 */

// TerraLib
#include "../../Defines.h"
#include "../../core/plugin/CppPlugin.h"
#include "../core/VirtualMachineManager.h"
#include "VirtualMachine.h"

// STL
#include <memory>

namespace te
{
  namespace vm
  {
    namespace lua
    {
      TERRALIB_CPP_PLUGIN_BEGIN(Plugin)

      TERRALIB_CPP_PLUGIN_STARTUP
      {
        if(m_initialized)
          return;

        te::vm::core::VirtualMachineManager::instance().insert(
            "lua", std::unique_ptr<te::vm::core::VirtualMachine>(
                       new te::vm::lua::VirtualMachine()));

        m_initialized = true;
      }

      TERRALIB_CPP_PLUGIN_SHUTDOWN
      {
        if(!m_initialized)
          return;

        te::vm::core::VirtualMachineManager::instance().erase("lua");

        m_initialized = false;
      }

      TERRALIB_CPP_PLUGIN_END(te::vm::lua::Plugin)
    }
  }
}
