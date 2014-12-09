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
  \file main.cpp

  \brief An example of usage of terralib binding lua module.
 */

// TerraLib
#include <terralib/common/PlatformUtils.h>

// STL
#include <iostream>

// Lua
#include <lua.hpp>

int main(int /*argc*/, char** /*argv*/)
{
  lua_State *L = luaL_newstate();

  luaopen_base(L);

  luaL_openlibs(L); /* Load Lua libraries */

  /* Load the file containing the script we are going to run */
  std::string f = te::common::FindInTerraLibPath("share/terralib/examples/lua/dataset.lua");

  int status = luaL_loadfile(L, f.c_str());
  if (status)
  {

    std::cout <<std::endl <<"Couldn't load file: " <<lua_tostring(L, -1);
    return EXIT_FAILURE;
  }

  /* Ask Lua to run our little script */
  int result = lua_pcall(L, 0, 0, 0);

  if (result)
  {
    std::cout <<std::endl <<"Failed to run script: " <<lua_tostring(L, -1);
    return EXIT_FAILURE;
  }

  lua_close(L);

  getchar();

  return EXIT_SUCCESS;
}
