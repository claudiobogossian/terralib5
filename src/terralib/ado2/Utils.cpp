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
  \file terralib/ado2/Utils.cpp
   
  \brief Utility functions for ADO.  
*/

// TerraLib
#include "Exception.h"
#include "Globals.h"
#include "Utils.h"

// Boost
#include <boost/lexical_cast.hpp>

std::string te::ado::MakeConnectionStr(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("PROVIDER");
  std::map<std::string, std::string>::const_iterator it_end = dsInfo.end();
  std::string connInfo;

  if(it != it_end)
    connInfo += " provider=" + it->second;

  it = dsInfo.find("DB_NAME");

  if(it != it_end)
    connInfo += " Data Source=" + it->second;

  it = dsInfo.find("USER_NAME");

  if(it != it_end)
    connInfo += " User Id=" + it->second;

  it = dsInfo.find("PASSWORD");

  if(it != it_end)
    connInfo += " Jet OLEDB:Database Password=" + it->second;

  return connInfo;
}