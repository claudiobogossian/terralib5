/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ado2/Utils.h
   
  \brief Utility functions for ADO.  
*/

#ifndef __TERRALIB_ADO_INTERNAL_UTILS_H
#define __TERRALIB_ADO_INTERNAL_UTILS_H

// TerraLib
#include "../common/StringUtils.h"
#include "Config.h"

// STL
#include <cstdlib>
#include <cstring>
#include <vector>

// ADO
#import "msado15.dll" \
  no_namespace rename("EOF", "EndOfFile")

namespace te
{
  namespace ado
  {

    std::string MakeConnectionStr(const std::map<std::string, std::string>& dsInfo);

  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_UTILS_H
