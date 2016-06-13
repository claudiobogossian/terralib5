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
  \file terralib/core/lib/Library.cpp

  \brief A class for handling shared libraries.

  \author Gilberto Ribeiro de Queiroz
 */

// TerraLib
#include "Library.h"
#include "../../Defines.h"
#include "Exception.h"
#include "../translator/Translator.h"

#ifndef TE_PLATFORM
#error "Could not determine platform! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"
#endif

// STL
#include <cassert>
#include <algorithm>
#include <cctype>

// Boost
#include <boost/format.hpp>

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#include <windows.h>
#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)
#include <cstdlib>
#include <dlfcn.h>
#else
#error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"
#endif

struct te::core::Library::Impl
{
    std::string slib_file_name;  //!< The shared library file name.
    void* module;                //!< The handle for a DLLs, SO or a DyLib.

#if defined(TE_PLATFORM) && (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)
    static bool added_search_path_;  //!< This flag is very important for Windows because some routines in its API returns 0 with two meanings: error or empty, but there is no way to know which one is right!
#endif

    Impl(const std::string& slib_file_name);
};

#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)
bool te::core::Library::Impl::added_search_path_(false);
#endif

te::core::Library::Impl::Impl(const std::string& slib_file_name)
  : slib_file_name(slib_file_name),
    module(nullptr)
{
}

static std::string te_get_os_error()
{
#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  DWORD dw_message_id = ::GetLastError();

  LPVOID lp_msg_buf = 0;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw_message_id,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lp_msg_buf,
                0,
                NULL);

  if(lp_msg_buf == 0)
    return std::string(TE_TR("Shared Library: could not determine the Operational System report error!"));

  std::string msg((char*)(lp_msg_buf));

  LocalFree(lp_msg_buf);

  return msg;

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  const char* err_msg = dlerror();

  return (err_msg == nullptr) ? std::string(TE_TR("Shared Library: could not determine the Operational System report error!")) : std::string(err_msg);

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif
}

te::core::Library::Library(const std::string& slib_file_name, const bool& delay_load)
  : m_pimpl(nullptr)
{
  if(slib_file_name.empty() || std::all_of(slib_file_name.begin(), slib_file_name.end(), isspace))
  {
    boost::format err_msg(TE_TR("Library name cannot be empty."));

    throw LibraryNameException() << te::ErrorDescription(err_msg.str());
  }

  m_pimpl = new Impl(slib_file_name);

  if(delay_load)
    return;

  load();
}

te::core::Library::~Library()
{
#ifdef NDEBUG
  try
  {
#endif
    unload();
#ifdef NDEBUG
  }
  catch(...)
  {
  }
#endif

  delete m_pimpl;
}

void
te::core::Library::load()
{
  if(isLoaded())
    return;

#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

    m_pimpl->module = (void*)LoadLibraryA(m_pimpl->slib_file_name.c_str());

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  m_pimpl->module = dlopen(m_pimpl->slib_file_name.c_str(), RTLD_NOW);

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif

  if(!isLoaded())
  {
    boost::format err_msg(TE_TR("Could not load library: %1%, due to following error: %2%."));

    throw LibraryLoadException() << te::ErrorDescription((err_msg % m_pimpl->slib_file_name % te_get_os_error()).str());
  }
}

void
te::core::Library::unload()
{
  if(!isLoaded())
    return;

#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  BOOL result = FreeLibrary((HMODULE)m_pimpl->module);

  if(result == FALSE)
  {
    boost::format err_msg(TE_TR("Could not unload library: %1%, due to following error: %2%."));

    throw LibraryUnloadException() << te::ErrorDescription((err_msg % m_pimpl->slib_file_name % te_get_os_error()).str());
  }

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  if(dlclose(m_pimpl->module))
  {
    boost::format err_msg(TE_TR("Could not unload library: %1%, due to following error: %2%."));

    throw LibraryUnloadException() << te::ErrorDescription((err_msg % m_pimpl->slib_file_name % te_get_os_error()).str());
  }

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif

  m_pimpl->module = nullptr;
}

bool
te::core::Library::isLoaded() const
{
  return (m_pimpl != nullptr) && (m_pimpl->module != nullptr);
}

const std::string&
te::core::Library::getFileName() const
{
  return m_pimpl->slib_file_name;
}

void*
te::core::Library::getAddress(const char* symbol) const
{
  assert(symbol);

#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  void* f = GetProcAddress((HINSTANCE)m_pimpl->module, symbol);

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  void* f = dlsym(m_pimpl->module, symbol);

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif

  if(f == nullptr)
  {
    boost::format err_msg(TE_TR("Could not find symbol: %1%, in the library %2%, due to the following error: %3%."));

    throw LibrarySymbolNotFoundException() << te::ErrorDescription((err_msg % symbol % m_pimpl->slib_file_name % te_get_os_error()).str());
  }

  return f;
}

std::string
te::core::Library::getNativeName(const std::string& name)
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
  std::string nativeName = name + ".dll";
#else
  std::string nativeName = name + "d.dll";
#endif

#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
  std::string nativeName = "lib" + name + ".so";

#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
  std::string nativeName = "lib" + name + ".dylib";

#else
#error "Platform not supported! Please, contact the TerraLib team (terralib-team@dpi.inpe.br) for helping support this platform!"
#endif

  return nativeName;
}

void
te::core::Library::addSearchDir(const std::string& dir_name)
{
#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  if(dir_name.length() > (MAX_PATH - 2))
  {
    boost::format err_msg(TE_TR("The DLL lookup path is too long: %1%."));

    throw LibraryInvalidSearchPathException() << te::ErrorDescription((err_msg % dir_name).str());
  }

// add dir to the path of LoadLibrary
  BOOL retval = SetDllDirectory(dir_name.c_str());

  if(retval == FALSE)
  {
    boost::format err_msg(TE_TR("The informed dir \"%1%\" couldn't be added to the application dll lookup path due to the following error: \"%2%\"."));

    throw te::core::LibraryInvalidSearchPathException() << te::ErrorDescription((err_msg % dir_name % te_get_os_error()).str());
  }

  te::core::Library::Impl::added_search_path_ = true;
#endif
}

void
te::core::Library::resetSearchPath()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

// come back with default Windows path
  BOOL retval = SetDllDirectory("");

  if(retval == FALSE)
  {
    boost::format err_msg(TE_TR("Couldn't come back with default Windows DLL lookup path due to the following error: %1%."));

    throw LibraryResetSearchPathException() << te::ErrorDescription((err_msg % te_get_os_error()).str());
  }

  te::core::Library::Impl::added_search_path_ = false;
#endif
}

std::string
te::core::Library::getSearchPath()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

  const DWORD buff_size = 32768;

  char buff[buff_size];

  DWORD length = GetDllDirectory(buff_size, buff);

  if(length == 0 && te::core::Library::Impl::added_search_path_)
  {
    boost::format err_msg(TE_TR("Couldn't get Windows DLL lookup path due to the following error: %1%!"));

    throw LibrarySearchPathException() << te::ErrorDescription((err_msg % te_get_os_error()).str());
  }

  if(length <= buff_size)
  {
    return std::string(buff, length);
  }
  else
  {
    throw LibrarySearchPathException() << te::ErrorDescription(TE_TR("Windows DLL lookup path too long!"));
  }

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
  const char* ldLibraryPath = getenv("LD_LIBRARY_PATH");
#else
  const char* ldLibraryPath = getenv("DYLD_LIBRARY_PATH");
#endif

  return (ldLibraryPath == nullptr) ? std::string("") : std::string(ldLibraryPath);

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif
}

