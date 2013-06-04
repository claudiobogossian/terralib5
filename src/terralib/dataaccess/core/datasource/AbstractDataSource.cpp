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
  \file terralib/dataaccess/core/datasource/AbstractDataSource.cpp

  \brief An abstract class for data providers like a DBMS, Web Services or a regular file.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../Exception.h"
#include "AbstractDataSource.h"
#include "DataSourceFactory.h"


std::auto_ptr<te::da::core::AbstractDataSource>
te::da::core::AbstractDataSource::create(const std::string& dsType,
                                         const std::map<std::string, std::string>& dsInfo) throw(Exception)
{
  std::auto_ptr<AbstractDataSource> ds(DataSourceFactory::make(dsType));

  ds->create(dsInfo);

  return ds;
}

void
te::da::core::AbstractDataSource::drop(const std::string& dsType,
                                       const std::map<std::string, std::string>& dsInfo) throw(Exception)
{
  std::auto_ptr<AbstractDataSource> ds(DataSourceFactory::make(dsType));

  ds->drop(dsInfo);
}

bool
te::da::core::AbstractDataSource::exists(const std::string& dsType,
                                         const std::map<std::string, std::string>& dsInfo) throw(Exception)
{
  std::auto_ptr<AbstractDataSource> ds(DataSourceFactory::make(dsType));

  return ds->exists(dsInfo);
}

std::vector<std::string>
te::da::core::AbstractDataSource::getDataSourceNames(const std::string& dsType,
                                                     const std::map<std::string, std::string>& info) throw(Exception)
{
  std::auto_ptr<AbstractDataSource> ds(DataSourceFactory::make(dsType));

  return ds->getDataSourceNames(info);
}
