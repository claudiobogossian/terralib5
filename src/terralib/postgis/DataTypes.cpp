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
  \file terralib/postgis/DataTypes.cpp
   
  \brief Utility data types for PostgreSQL.  
*/

// TerraLib
#include "DataTypes.h"

// STL
#include <cstdlib>

te::pgis::VersionInfo& operator<<(te::pgis::VersionInfo& vinfo, const char* v)
{
  while((*v != '\0') && (*v < '0' || *v > '9'))
    ++v;
  
  if(*v != '\0')
  {
    vinfo.m_major = atoi(v);

    ++v;
    
    while((*v != '\0') && (*v < '0' || *v > '9'))
      ++v;

    if(*v != '\0')
      vinfo.m_minor = atoi(v);

    ++v;

    while((*v != '\0') && (*v < '0' || *v > '9'))
      ++v;

    if(*v != '\0')
      vinfo.m_release = atoi(v);
  }

  return vinfo;
}

