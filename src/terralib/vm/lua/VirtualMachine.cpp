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
  \file terralib/vm/core/VirtualMachine.h

  \brief Implementation of a virtual machine for the Lua Programming Language.

  \author Frederico Augusto Bedê
  \author Gilberto Ribeiro de Queiroz
*/

// TerraLib
#include "VirtualMachine.h"

// STL
#include <cassert>

// Lua
#include <lua.hpp>

struct te::vm::lua::VirtualMachine::Impl
{
  lua_State* state;   //!< The Lua environment (or state).
  
  Impl()
    : state(nullptr)
  {
    //state = luaL_newstate();  // creates a new Lua environment (or state) without any pre-defined functions

    //if(state == nullptr)
    //  throw Exception(TR_LUA_VM("Could not start a new Lua environment: not enough memory!"));

    //luaL_openlibs(state);   // opens all standard libraries

    //te::lua::i::RegisterLuaBinding(m_state);
  }
  
  ~Impl()
  {
    //if(state)
      //lua_close(state);
  }
};

te::vm::lua::VirtualMachine::VirtualMachine()
  : pimpl_(nullptr)
{
  //pimpl_ = new Impl;
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
  return "TerraLib Virtual Machine for the Lua Programming Language";
}

std::string
te::vm::lua::VirtualMachine::getDescription() const
{
  return "This Virtual Machine allows applications to execute lua scripts."
         "\nAll the TerraLib API will be available for TerraLib programmers "
         "in the Lua environment.";
}

void
te::vm::lua::VirtualMachine::build(const std::string& file)
{
// just load the lua file as a chunk... doesn't execute it!
  //int result = luaL_loadfile(pimpl_->state, file.c_str());

  //if(result)  // 0 = SUCCESS
  //{
    //const char* errmsg = lua_tostring(m_state, -1);

    //throw Exception((boost::format(TR_LUA_VM("Error parsing Lua script:\n %1%.")) % errmsg).str());
  //}
}

void te::vm::lua::VirtualMachine::execute()
{
// run the loaded chunk
  //int result = lua_pcall(pimpl_->state, 0, 0, 0);

  //if(result)  // 0 = SUCCESS
  //{
  //  const char* errmsg = lua_tostring(pimpl_->state, -1);

  //  throw Exception((boost::format(TR_LUA_VM("Error executing Lua script: %1%.")) % errmsg).str());
  //}
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