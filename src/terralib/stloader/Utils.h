/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Utils.h
   
  \brief Utility functions for ST Loader module.  
 */

#ifndef __TERRALIB_STLOADER_INTERNAL_UTILS_H
#define __TERRALIB_STLOADER_INTERNAL_UTILS_H

// ST Loader
#include "Config.h"

// Forward declarations
namespace te { namespace xml { class Reader; } }

namespace te
{
  namespace stloader
  {
    /*!
      TO DO: passar essas funções para o módulo XML 
    */
    TESTLOADEREXPORT bool GetNextValueNode(te::xml::Reader* xmlReader, const std::string& nodeName, std::string& nodeValue);

    TESTLOADEREXPORT bool GetNextStartNode(te::xml::Reader* xmlReader, const std::string& nodeName);    

  } // end namespace stloader
}   // end namespace te

#endif  // __TERRALIB_STLOADER_INTERNAL_UTILS_H
