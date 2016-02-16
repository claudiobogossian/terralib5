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
  \file terralib/module/Library.cpp

  \brief A class for handling shared libraries.

  \author Gilberto Ribeiro de Queiroz
 */

// TerraLib
#include "Library.h"
#include "../Defines.h"
#include "Exception.h"

#ifndef TE_PLATFORM
#error "Could not determine platform! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"
#endif

// STL
#include <cassert>

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

struct te::module::Library::Impl
{
    std::string slib_file_name;  //!< The shared library file name.
    void* module;                //!< The handle for a DLLs, SO or a DyLib.

#if defined(TE_PLATFORM) && (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)
    static bool added_search_path_;  //!< This flag is very important for Windows because some routines in its API returns 0 with two meanings: error or empty, but there is no way to know which one is right!
#endif

    Impl(const std::string& slib_file_name);
};

#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)
bool te::module::Library::Impl::added_search_path_(false);
#endif

te::module::Library::Impl::Impl(const std::string& slib_file_name)
  : slib_file_name(slib_file_name),
    module(false)
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
    return std::string("Shared Library: could not determine the Operational System report error!");

  std::string msg((char*)(lp_msg_buf));

  LocalFree(lp_msg_buf);

  return msg;

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  const char* err_msg = dlerror();

  return (err_msg == false) ? std::string("Shared Library: could not determine the Operational System report error!") : std::string(err_msg);

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif
}

te::module::Library::Library(const std::string& slib_file_name, bool delay_load)
  : m_pimpl(false)
{
  m_pimpl = new Impl(slib_file_name);

  if(delay_load)
    return;

  load();
}

te::module::Library::~Library()
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
te::module::Library::load()
{
  if(m_pimpl->module != false)
    return;

#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  m_pimpl->module = (void*)LoadLibraryA(m_pimpl->slib_file_name.c_str());

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  m_pimpl->module = dlopen(m_pimpl->slib_file_name.c_str(), RTLD_NOW);

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif

  if(m_pimpl->module == false)
  {
    boost::format err_msg("Could not load library: %1%, due to following error: %2%.");

    throw LibraryLoadException() << te::ErrorDescription((err_msg % m_pimpl->slib_file_name % te_get_os_error()).str());
  }
}

void
te::module::Library::unload()
{
  if(m_pimpl->module == false)
    return;

#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  BOOL result = FreeLibrary((HMODULE)m_pimpl->module);

  if(result == FALSE)
  {
    boost::format err_msg("Could not unload library: %1%, due to following error: %2%.");

    throw LibraryUnloadException() << te::ErrorDescription((err_msg % m_pimpl->slib_file_name % te_get_os_error()).str());
  }

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  if(dlclose(m_pimpl->module))
  {
    boost::format err_msg("Could not unload library: %1%, due to following error: %2%.");

    throw LibraryUnloadException() << te::ErrorDescription((err_msg % m_pimpl->slib_file_name % te_get_os_error()).str());
  }

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif

  m_pimpl->module = false;
}

bool
te::module::Library::isLoaded() const
{
  return (m_pimpl->module != false);
}

const std::string&
te::module::Library::getFileName() const
{
  return m_pimpl->slib_file_name;
}

void*
te::module::Library::getAddress(const char* symbol) const
{
  assert(symbol);

#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  void* f = GetProcAddress((HMODULE)m_module, symbol.c_str());

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  void* f = dlsym(m_pimpl->module, symbol);

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif

  if(f == false)
  {
    boost::format err_msg("Could not find symbol: %1%, in the library %2%, due to the following error: %3%.");

    throw LibrarySymbolNotFoundException() << te::ErrorDescription((err_msg % symbol % m_pimpl->slib_file_name % te_get_os_error()).str());
  }

  return f;
}

std::string
te::module::Library::getNativeName(const std::string& name)
{
#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  std::string nativeName = name + ".dll";

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX)

  std::string nativeName = "lib" + name + ".so";

#elif (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  std::string nativeName = "lib" + name + ".dylib";

#else

  #error "Platform not supported! Please, contact TerraLib team (gribeiro@dpi.inpe.br) for helping support this platform!"

#endif

  return nativeName;
}

void
te::module::Library::addSearchDir(const std::string& dir_name)
{
#if (TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS)

  if(dir_name.length() > (MAX_PATH - 2))
  {
    boost::format err_msg("The DLL lookup path is too long: %1%.");

    throw LibraryInvalidSearchPathException() << te::ErrorDescription((err_msg % dir_name).str());
  }

// add dir to the path of LoadLibrary
  BOOL retval = SetDllDirectory(d.c_str());

  if(retval == FALSE)
  {
    boost::format err_msg("The informed dir \"%1%\" couldn't be added to the application dll lookup path due to the following error: %2%."");

    throw library_invalid_search_path_error() << error_description((err_msg % dir_name % te_get_os_error()).str());
  }

  te::module::Library::Impl::added_search_path_ = true;

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
  const char* ldLibraryPath = getenv("LD_LIBRARY_PATH");
#else
  const char* ldLibraryPath = getenv("DYLD_LIBRARY_PATH");
#endif

  if(ldLibraryPath == false)
  {
// XX_LIBRARY_PATH doesn't exist
#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
    int result = setenv("LD_LIBRARY_PATH", dir_name.c_str(), 1);
#else
    int result = setenv("DYLD_LIBRARY_PATH", dir_name.c_str(), 1);
#endif

    if(result)
    {
#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
      boost::format err_msg("Could not create LD_LIBRARY_PATH for the application. It is not pointing to the informed dir \"%1%\".");
#else
      boost::format err_msg("Could not create DYLD_LIBRARY_PATH for the application. It is not pointing to the informed dir \"%1%\".");
#endif

      throw LibraryInvalidSearchPathException() << te::ErrorDescription((err_msg % dir_name).str());
    }
  }
  else
  {
// XX_LIBRARY_PATH may be empty or may have any other value, we need to concatenate

    std::string newLdLibraryPath(ldLibraryPath);
    newLdLibraryPath += ";";
    newLdLibraryPath += dir_name;

#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
    int result = setenv("LD_LIBRARY_PATH", newLdLibraryPath.c_str(), 1);
#else
    int result = setenv("DYLD_LIBRARY_PATH", newLdLibraryPath.c_str(), 1);
#endif

    if(result)
    {
#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
      boost::format err_msg("Couldn't add the informed dir \"%1%\" to the application's environment variable LD_LIBRARY_PATH.");
#else
      boost::format err_msg("Couldn't add the informed dir \"%1%\" to the application's environment variable DYLD_LIBRARY_PATH.");
#endif

      throw LibraryInvalidSearchPathException() << te::ErrorDescription((err_msg % dir_name).str());
    }
  }

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif
}

void
te::module::Library::resetSearchPath()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

// come back with default Windows path
  BOOL retval = SetDllDirectory("");

  if(retval == FALSE)
  {
    boost::format err_msg("Couldn't come back with default Windows DLL lookup path due to the following error: %1%.");

    throw LibraryResetSearchPathException() << te::ErrorDescription((err_msg % te_get_os_error()).str());
  }

  te::module::Library::Impl::added_search_path_ = false;

#else

  throw LibraryResetSearchPathException() << te::ErrorDescription("method: void te::common::Library::resetSearchPath() throw(Exception) not implemented for this platform! Contact gribeiro@dpi.inpe.br");

#endif
}

std::string
te::module::Library::getSearchPath()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

  const DWORD buff_size = 32768;

  char buff[buff_size];

  DWORD length = GetDllDirectory(buff_size, buff);

  if(length == 0 && te::module::Library::Impl::added_search_path_)
  {
    boost::format err_msg("Couldn't get Windows DLL lookup path due to the following error: %1%!");

    throw LibrarySearchPathException() << te::ErrorDescription((err_msg % te_get_os_error()).str());
  }

  if(length <= buffSize)
  {
    return std::string(buff, length);
  }
  else
  {
    throw LibrarySearchPathException() << te::ErrorDescription("Windows DLL lookup path too long!");
  }

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
  const char* ldLibraryPath = getenv("LD_LIBRARY_PATH");
#else
  const char* ldLibraryPath = getenv("DYLD_LIBRARY_PATH");
#endif

  return (ldLibraryPath == false) ? std::string("") : std::string(ldLibraryPath);

#else

  #error "Platform not supported! Please, contact TerraLib team (terralib-team@terralib.org) for helping support this platform!"

#endif
}
