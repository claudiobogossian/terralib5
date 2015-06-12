/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/Library.cpp

  \brief A class for handling shared libraries.
*/

// TerraLib
#include "../Defines.h"
#include "Exception.h"
#include "Library.h"
#include "PlatformUtils.h"
#include "Translator.h"

// Boost
#include <boost/format.hpp>

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#include <windows.h>

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)
#include <cstdlib>
#include <dlfcn.h>
#else
  #error "Platform not supported! Please, contact the TerraLib team (terralib-team@dpi.inpe.br) for helping support this platform!"
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
static bool sg_addedSearchPath(false);  //!< This flag is very important for Windows because some routines in its API returns 0 with two meanings: error or empty, but there is no way to know which one is right!
#endif

class te::common::Library::Impl
{
  public:

    Impl(const std::string& fileName, bool delayLoad) throw(te::common::Exception)
      : m_fileName(fileName),
        m_module(0)
    {
      if(delayLoad)
        return;

      load();
    }

    ~Impl()
    {
      unload();
    }

    void load() throw(te::common::Exception)
    {
      if(m_module)
        return;

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      m_module = (void*)LoadLibraryA(m_fileName.c_str());

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)
      m_module = dlopen(m_fileName.c_str(), RTLD_NOW);  

#else
      #error "Platform not supported! Please, contact the TerraLib team (terralib-team@dpi.inpe.br) for helping support this platform!"
#endif

      if(m_module == 0)
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        throw te::common::Exception((boost::format(TE_TR("Could not load library: %1%, due to following error: %2%.")) % m_fileName % te::common::win::GetLastError()).str()); 
#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)
        throw te::common::Exception((boost::format(TE_TR("Could not load library: %1%, due to following error: %2%.")) % m_fileName % te::common::lx::GetDlError()).str());
#else
      #error "Platform not supported! Please, contact the TerraLib team (terralib-team@dpi.inpe.br) for helping support this platform!"
#endif
    }

    void unload() throw(te::common::Exception)
    {
      if(m_module == 0)
        return;

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      BOOL result = FreeLibrary((HMODULE)m_module);

      if(result == FALSE)
        throw te::common::Exception((boost::format(TE_TR("Could not unload library: %1%, due to following error: %2%.")) % m_fileName % te::common::win::GetLastError()).str()); 

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)
      if(dlclose(m_module))
        throw Exception((boost::format(TE_TR("Could not unload library: %1%, due to following error: %2%.")) % m_fileName % te::common::lx::GetDlError()).str());
#else
      #error "Platform not supported! Please, contact the TerraLib team (terralib-team@dpi.inpe.br) for helping support this platform!"
#endif

      m_module = 0;
    }

    bool isLoaded() const throw()
    {
      return (m_module != 0);
    }

    const std::string& getFileName() const throw()
    {
      return m_fileName;
    }

    void* getAddress(const std::string& symbol) const throw(te::common::Exception)
    {
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      void* f = GetProcAddress((HMODULE)m_module, symbol.c_str());
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
      void* f = dlsym(m_module, symbol.c_str());
#else
      #error "Platform not supported! Please, contact the TerraLib team for helping support this platform!"
#endif

      if(f == NULL)
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        throw Exception((boost::format(TE_TR("Could not find symbol: %1%, in the library %2%, due to the following error: %3%.")) % symbol % m_fileName % te::common::win::GetLastError()).str());
#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)
        throw Exception((boost::format(TE_TR("Could not find symbol: %1%, in the library %2%, due to the following error: %3%.")) % symbol % m_fileName % te::common::lx::GetDlError()).str());
#else
      #error "Platform not supported! Please, contact terralib-team@dpi.inpe.br for helping support this platform!"
#endif

      return f;
    }

  private:

    std::string m_fileName;    //!< The library file name with full path.
    void* m_module;            //!< The handle for DLLs, SO or a DyLib.
};

te::common::Library::Library(const std::string& fileName, bool delayLoad) throw(Exception)
  : m_pImpl(0)
{
  m_pImpl = new Impl(fileName, delayLoad);
}

te::common::Library::~Library()
{
  delete m_pImpl;
}

void te::common::Library::load() throw(Exception)
{
  m_pImpl->load();
}

void te::common::Library::unload() throw(Exception)
{
  m_pImpl->unload();
}

bool te::common::Library::isLoaded() const throw()
{
  return m_pImpl->isLoaded();
}

const std::string& te::common::Library::getFileName() const throw()
{
  return m_pImpl->getFileName();
}

void* te::common::Library::getAddress(const std::string& symbol) const throw(Exception)
{
  return m_pImpl->getAddress(symbol);
}

std::string te::common::Library::getNativeName(const std::string& name) throw()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  std::string nativeName = name + ".dll";

#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
  std::string nativeName = "lib" + name + ".so";

#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
  std::string nativeName = "lib" + name + ".dylib";

#else
  #error "Platform not supported! Please, contact the TerraLib team (terralib-team@dpi.inpe.br) for helping support this platform!"
#endif

  return nativeName;
}

void te::common::Library::addSearchDir(const std::string& d) throw(Exception)
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  if(d.length() > (MAX_PATH - 2))
    throw Exception((boost::format(TE_TR("The DLL lookup path is too long: %1%.")) % d).str());

// add dir to the path of LoadLibrary
  BOOL retval = SetDllDirectory(d.c_str());

  if(retval == FALSE)
    throw Exception((boost::format(TE_TR("The informed dir \"%1%\" couldn't be added to the application dll lookup path due to the following error: %2%.")) % d % te::common::win::GetLastError()).str());

  sg_addedSearchPath = true;

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  const char* ldLibraryPath = getenv("LD_LIBRARY_PATH");

  if(ldLibraryPath == 0)
  {
// LD_LIBRARY_PATH doesn't exist
    int result = setenv("LD_LIBRARY_PATH", d.c_str(), 1);

    if(result)
      throw Exception((boost::format(TE_TR("Could not create LD_LIBRARY_PATH for the application. It is not pointing to the informed dir \"%1%\".")) % d).str());
  }
  else
  {
// LD_LIBRARY_PATH may be empty or may have any other value, we need to concatenate
    std::string newLdLibraryPath(ldLibraryPath);
    newLdLibraryPath += ";";
    newLdLibraryPath += d;

    int result = setenv("LD_LIBRARY_PATH", newLdLibraryPath.c_str(), 1);

    if(result)
      throw Exception((boost::format(TE_TR("Couldn't add the informed dir \"%1%\" to the application's environment variable LD_LIBRARY_PATH.")) % d).str());
  }

#else
  throw Exception(TE_TR("method: void te::common::Library::addSearchDir(const std::string& d) throw(Exception) not implemented for this platform! Contact terralib-team@dpi.inpe.br"));
#endif
}

void te::common::Library::resetSearchPath() throw(Exception)
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
// come back with default Windows path
  BOOL retval = SetDllDirectory("");

  if(retval == FALSE)
    throw Exception((boost::format(TE_TR("Couldn't come back with default Windows DLL lookup path due to the following error: %1%.")) % te::common::win::GetLastError()).str());

  sg_addedSearchPath = false;

#else
  throw Exception(TE_TR("method: void te::common::Library::resetSearchPath() throw(Exception) not implemented for this platform! Contact terralib-team@dpi.inpe.br"));
#endif
}

std::string te::common::Library::getSearchPath() throw(Exception)
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  const DWORD buffSize = 32768;

  char buff[buffSize];

  DWORD length = GetDllDirectory(buffSize, buff);

  if(length == 0 && sg_addedSearchPath)
    throw Exception((boost::format(TE_TR("Couldn't get Windows DLL lookup path due to the following error: %1%!")) % te::common::win::GetLastError()).str());

  if(length <= buffSize)
    return std::string(buff, length);

  throw Exception(TE_TR("Windows DLL lookup path too long!"));

#elif (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

  const char* ldLibraryPath = getenv("LD_LIBRARY_PATH");

  if(ldLibraryPath == 0)
    return std::string("");
  else
    return std::string(ldLibraryPath);

#else
  #warning "Verify the method: std::string te::common::Library::getSearchPath() throw(std::exception) for this platform!"
#endif
}

