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
  \file terralib/examples/vm/main.cpp

  \brief Examples for the Terralib Virtual Machine Module

  \author Frederico Augusto Bedê Teotônio
*/

// TerraLib
#include <terralib/common.h>
#include <terralib/core.h>
#include <terralib/plugin.h>
#include <terralib/vm.h>

// STL
#include <memory>
#include <string>

int main(int argc, char *argv[])
{
  TerraLib::getInstance().initialize();
  
  try
  {
// load Lua Virtual Machine
    std::string lvm_lua_manifest = te::core::FindInTerraLibPath("share/terralib/plugins/te.vm.lua.teplg");
    
    std::unique_ptr<te::plugin::PluginInfo> lvm_lua_info(te::plugin::GetInstalledPlugin(lvm_lua_manifest));

    te::plugin::PluginManager::getInstance().load(*lvm_lua_info);

    std::string lua_script = te::core::FindInTerraLibPath("examples/vm/vm_example.lua");

    te::vm::core::VirtualMachine* vm = te::vm::core::VirtualMachineManager::instance().get("lua");

    vm->build(lua_script);

    vm->execute();

    te::plugin::PluginManager::getInstance().unloadAll();
  }
  catch(const te::vm::core::Exception& e)
  {
    if(const std::string* d = boost::get_error_info<te::ErrorDescription>(e))
    {
      std::cerr << *d << std::endl;
    }
  }

  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
