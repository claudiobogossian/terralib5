/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/Version.cpp

  \brief Utility class for system versioning.
*/

// TerraLib
#include "Version.h"

// STL
#include <cassert>

int te::common::Version::majorNumber()
{
  return TERRALIB_MAJOR_VERSION;
}

int te::common::Version::minorNumber()
{
  return TERRALIB_MINOR_VERSION;
}

int te::common::Version::patchNumber()
{
  return TERRALIB_PATCH_VERSION;
}

std::string te::common::Version::releaseStatus()
{
  assert(TERRALIB_RELEASE_STATUS);
  return std::string(TERRALIB_RELEASE_STATUS);
}

std::string te::common::Version::buildDate()
{
  assert(__DATE__ " " __TIME__);
  return std::string(__DATE__ " " __TIME__);
}

std::string te::common::Version::asString()
{
  assert(TERRALIB_STRING_VERSION);
  return std::string(TERRALIB_STRING_VERSION);
}

int te::common::Version::asInt()
{
  return TERRALIB_INT_VERSION;
}

