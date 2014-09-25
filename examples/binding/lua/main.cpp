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

// Lua
#include <lua.hpp>

// STL
#include <iostream>

extern "C"{
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

extern int luaopen_terralib_mod_binding_lua(lua_State* L); // declaring the wrapped module

int main(int /*argc*/, char** /*argv*/)
{
  int status, result;
  lua_State *L;

  /*
   * All Lua contexts are held in this structure. We work with it almost
   * all the time.
  */
  L = luaL_newstate();

  luaopen_base(L);

  luaL_openlibs(L); /* Load Lua libraries */

  /* Load the file containing the script we are going to run */
  std::string f = te::common::FindInTerraLibPath("share/terralib/examples/lua/geometry.lua");

  status = luaL_loadfile(L, f.c_str());
  if (status)
  {
    /* If something went wrong, error message is at the top of */
    /* the stack */
    std::cout <<std::endl <<"Couldn't load file: " <<lua_tostring(L, -1);
    return EXIT_FAILURE;
  }

  /* Ask Lua to run our little script */
  result = lua_pcall(L, 0, 0, 0);

  if (result)
  {
    std::cout <<std::endl <<"Failed to run script: " <<lua_tostring(L, -1);
    return EXIT_FAILURE;
  }

  lua_close(L);   /* Cya, Lua */

  return EXIT_SUCCESS;
}
