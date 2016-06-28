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
  \file terralib/vm/lua/VirtualMachine.cpp

  \brief Implementation of a virtual machine for the Lua Programming Language.

  \author Frederico Augusto Bedê
  \author Gilberto Ribeiro de Queiroz
  */

// TerraLib
#include "VirtualMachine.h"
#include "../../core/translator/Translator.h"
#include "../core/Exception.h"

// STL
#include <iostream>

// Boost
#include <boost/format.hpp>

// Lua
#include <lua.hpp>

//extern "C"
//{
//#include <lua.h>
//#include <lualib.h>
//#include <lauxlib.h>
//}
//
static int l_print(lua_State* L)
{
  int nargs = lua_gettop(L);

  for(int i = 1; i <= nargs; i++)
  {
    int t = lua_type(L, i);
    switch(t)
    {
      case LUA_TSTRING: { /* strings */
                          std::cout << lua_tostring(L, i);
                          break;
      }
      case LUA_TBOOLEAN: { /* booleans */
                           std::cout << (lua_toboolean(L, i) ? "true" : "false");
                           break;
      }
      case LUA_TNUMBER: { /* numbers */
                          std::cout << lua_tonumber(L, i);
                          break;
      }
      default: { /* other values */
                 std::cout << lua_typename(L, t);
                 break;
      }
    }
    if(i != nargs)
    {
      std::cout << "\t";
    }
  }

  std::cout << std::endl;

  return 0;
}

struct te::vm::lua::VirtualMachine::Impl
{
  lua_State* state;   //!< The Lua environment (or state).

  Impl()
    : state(nullptr)
  {
    // creates a new Lua environment (or state) without any pre-defined functions
    state = luaL_newstate();

    if(state == nullptr)
      throw te::vm::core::CreationException() << te::ErrorDescription(TE_TR("Could not start a new Lua environment: not enough memory!"));

    // opens all standard libraries
    luaL_openlibs(state);
  }

  ~Impl()
  {
    lua_close(state);
  }
};

te::vm::lua::VirtualMachine::VirtualMachine()
: pimpl_(nullptr)
{
  pimpl_ = new Impl;
}

te::vm::lua::VirtualMachine::~VirtualMachine()
{
  delete pimpl_;
}

std::string
te::vm::lua::VirtualMachine::getName() const
{
  return "Lua";
}

std::string
te::vm::lua::VirtualMachine::getTitle() const
{
  std::string msg(TE_TR("TerraLib Virtual Machine for the Lua Programming Language"));

  return msg;
}

std::string
te::vm::lua::VirtualMachine::getDescription() const
{
  std::string msg(TE_TR("This Virtual Machine allows applications to execute lua scripts."
    "\nAll the TerraLib API will be available for TerraLib programmers "
    "in the Lua environment"));

  return msg;
}

void
te::vm::lua::VirtualMachine::build(const std::string& file)
{
  // TODO: checar se o arquivo existe!

  // just load the lua file as a chunk: doesn't execute it!
  int result = luaL_loadfile(pimpl_->state, file.c_str());

  lua_pushcfunction(pimpl_->state, l_print);
  lua_setglobal(pimpl_->state, "print");

  if(result != LUA_OK)
  {
    const char* lerrmsg = lua_tostring(pimpl_->state, -1);

    boost::format errmsg(TE_TR("Error parsing Lua script '%1%': %2%."));

    throw te::vm::core::BuildException() << te::ErrorDescription((errmsg % file % lerrmsg).str());
  }
}

void te::vm::lua::VirtualMachine::execute()
{
  // run the loaded chunk
  int result = lua_pcall(pimpl_->state, 0, 0, 0);

  if(result != LUA_OK)
  {
    const char* lerrmsg = lua_tostring(pimpl_->state, -1);

    boost::format errmsg(TE_TR("Error executing Lua script: %1%."));

    throw te::vm::core::RunException() << te::ErrorDescription((errmsg % lerrmsg).str());
  }
}

void
te::vm::lua::VirtualMachine::pause()
{
}

void
te::vm::lua::VirtualMachine::stop()
{
}

void
te::vm::lua::VirtualMachine::reset()
{
}
