/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/OSSettingsDir.cpp

  \brief A singleton class for discovering Operational System settings dir.
*/

// TerraLib
#include "OSSettingsDir.h"

// STL
#include <cstdlib>

// Boost
#include <boost/filesystem.hpp>

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
// Windows
#include <Shlobj.h>
#endif

const std::string& te::common::OSSettingsDir::getUserSettingsPath() const
{
  return m_userSettingsPath;
}

const std::string& te::common::OSSettingsDir::getSystemSettingsPath() const
{
  return m_systemSettingsPath;
}

te::common::OSSettingsDir::OSSettingsDir()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  char userSettingsPath[MAX_PATH];

//there is also a tag: CSIDL_APPDATA -> maybe older versions of XP may depends on it!
  HRESULT result = SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, userSettingsPath);

  if((result == S_OK) && (userSettingsPath != 0) && boost::filesystem::is_directory(userSettingsPath))
    m_userSettingsPath = userSettingsPath;

  char systemSettingsPath[MAX_PATH];

  result = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, systemSettingsPath);
  
  if((result == S_OK) && (systemSettingsPath != 0) && boost::filesystem::is_directory(systemSettingsPath))
    m_systemSettingsPath = systemSettingsPath;

#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
  const char* userSettingsPath = std::getenv("$HOME");

  if((userSettingsPath != 0) && boost::filesystem::is_directory(userSettingsPath))
  {
    boost::filesystem::path p(userSettingsPath);
    p /= ".config";

    m_userSettingsPath = p.string();
  }

  if(boost::filesystem::is_directory("/etc/xdg"))
    m_userSettingsPath = "/etc/xdg";

#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
  const char* userSettingsPath = std::getenv("$HOME");

  if((userSettingsPath != 0) && boost::filesystem::is_directory(userSettingsPath))
  {
    boost::filesystem::path p(userSettingsPath);
    p /= ".config";

    m_userSettingsPath = p.string();
  }

  if(boost::filesystem::is_directory("/etc/xdg"))
    m_userSettingsPath = "/etc/xdg";

#else
  #error "Platform not supported! Contact TerraLib Team"
#endif

}

te::common::OSSettingsDir::~OSSettingsDir()
{
}

