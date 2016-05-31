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
#include <terralib/common/TerraLib.h>
#include <terralib/core/lib/Library.h>
#include <terralib/core/utils/Platform.h>
#include <terralib/plugin/PluginInfo.h>
#include <terralib/plugin/PluginManager.h>
#include <terralib/plugin/Utils.h>
#include <terralib/vm/core/VirtualMachine.h>
#include <terralib/vm/core/VirtualMachineManager.h>

int main(int argc, char *argv[])
{
  TerraLib::getInstance().initialize();

  std::string plgManifest = te::core::FindInTerraLibPath("share/terralib/plugins/te.vm.lua.teplg");

  std::string teLuaLib = te::core::FindInTerraLibPath("terralib_mod_binding_lua.so");

  te::core::Library lua(teLuaLib);

  te::plugin::PluginInfo* info;

  info = te::plugin::GetInstalledPlugin(plgManifest);

  te::plugin::PluginManager::getInstance().load(*info);

  std::string luaScript = te::core::FindInTerraLibPath("share/terralib/examples/lua/geometry.lua");

  te::vm::core::VirtualMachine* vm = te::vm::core::VirtualMachineManager::instance().get("lua");

  vm->build(luaScript);

  vm->execute();

  delete info;

  te::plugin::PluginManager::getInstance().unloadAll();

  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
