/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/Globals.cpp
   
  \brief An static class with global definitions for the TerraLib SQLite Data Access driver.
*/

// TerraLib
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/datasource//DataSourceCapabilities.h"
#include "Globals.h"

const std::string te::sqlite::Globals::sm_driverIdentifier(TE_SQLITE_DRIVER_IDENTIFIER);
const std::string te::sqlite::Globals::sm_rliteIdentifier(TE_RASTERLITE_DRIVER_IDENTIFIER);
const std::string te::sqlite::Globals::sm_teliteIdentifier(TE_TERRALIBLITE_DRIVER_IDENTIFIER);

te::da::SQLDialect* te::sqlite::Globals::sm_spatialiteDialect(0);
te::da::SQLDialect* te::sqlite::Globals::sm_sqliteDialect(0);
te::da::DataSourceCapabilities* te::sqlite::Globals::sm_spatialiteCapabilities(0);
te::da::DataSourceCapabilities* te::sqlite::Globals::sm_sqliteCapabilities(0);
