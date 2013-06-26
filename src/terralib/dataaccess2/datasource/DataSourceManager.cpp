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
  \file terralib/dataaccess/datasource/DataSourceManager.cpp

  \brief This is a singleton for managing all data source instances available in the system.
*/

// TerraLib
#include "DataSourceManager.h"

// STL
#include <algorithm>
#include <cassert>
#include <memory>

std::string te::da::DataSourceManager::open(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  return "";
}

void te::da::DataSourceManager::open(const std::string& id,
                                     const std::string& dsType,
                                     const std::map<std::string, std::string>& dsInfo)
{
}

bool te::da::DataSourceManager::isOpened(const std::string& id)
{
  return false;
}

te::da::DataSourcePtr te::da::DataSourceManager::get(const std::string& id)
{
  return DataSourcePtr();
}

void te::da::DataSourceManager::close(const std::string& id)
{
}


te::da::DataSourceManager::DataSourceManager()
{
}

te::da::DataSourceManager::~DataSourceManager()
{
}
