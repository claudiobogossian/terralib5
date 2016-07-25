/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/xsd/ReadXSD.h

  \brief Functions for reading an XSD.
*/

#ifndef __TERRALIB_XSD_INTERNAL_READXSD_H
#define __TERRALIB_XSD_INTERNAL_READXSD_H

// TerraLib
#include "Config.h"

#if TE_XSD_BUILD_WITH_XML_PARSER
// STL
#include <string>

namespace te
{
  namespace xsd
  {
    class Schema;

    TEXSDEXPORT Schema* Read(const std::string& file_name);

  } // end namespace xsd
}   // end namespace te

#endif // TE_XSD_BUILD_WITH_XML_PARSER

#endif  // __TERRALIB_XSD_INTERNAL_READXSD_H
