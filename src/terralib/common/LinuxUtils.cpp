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
  \file terralib/common/LinuxUtils.cpp

  \brief This file contains several utility functions when dealing with Linux specific API.
*/

// TerraLib
#include "../Defines.h"
#include "LinuxUtils.h"

#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

// Linux
#include <dlfcn.h>

const std::string te::common::lx::GetDlError()
{
  const char* errMsg = dlerror();

  if(errMsg == 0)
    return std::string("");
  else
    return std::string(errMsg);
}

#endif  // (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

