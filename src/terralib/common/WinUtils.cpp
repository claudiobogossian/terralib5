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
  \file terralib/common/WinUtils.cpp

  \brief This file contains several utility functions when dealing with Microsoft Windows specific API.
*/

// TerraLib
#include "../Defines.h"
#include "WinUtils.h"

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
// Windows
#include <windows.h>

const std::string te::common::win::GetLastError()
{
  DWORD dwMessageId = ::GetLastError();

  LPVOID lpMsgBuf = 0;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dwMessageId,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf,
                0,
                NULL);

  if(lpMsgBuf == 0)
    return std::string("");

  std::string msg((char*)(lpMsgBuf));

  LocalFree(lpMsgBuf);

  return msg;
}

#endif  // TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

