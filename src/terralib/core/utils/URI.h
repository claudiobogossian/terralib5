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
  \file terralib/core/utils/URI.h

  \brief  This file contains utility functions used to manipulate data from a URI.

  \author Andre Gomes de Oliveira
  \author Gilberto Ribeiro de Queiroz
*/

#ifndef __TERRALIB_CORE_UTILS_URI_H__
#define __TERRALIB_CORE_UTILS_URI_H__

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace core
  {
    /*!
      \brief Split a query string into its components 

      \param query_str The query string that will have the data extracted.
          
      \exception Exception An exception can be thrown, if a parser error occurs during decomposition of the query string.
          
      \return A container with the key-value pairs extracted from the query string.
    */
    TECOREEXPORT std::map<std::string, std::string> Expand(const std::string& query_str);
  }
}  
#endif //__TERRALIB_CORE_UTILS_URI_H__
