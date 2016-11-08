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
  \file terralib/terralib4/DataSource.h

  \brief Implements the DataSource class for the TerraLib 4.x Data Access Driver.
*/

#ifndef __TERRALIB_TERRALIB4_INTERNAL_DATASOURCE_H
#define __TERRALIB_TERRALIB4_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"

class TeDatabase;
class TeTheme;

namespace terralib4
{
  struct ThemeInfo;

  class TETERRALIB4EXPORT DataSource : public te::da::DataSource
  {
    public:

      DataSource(const std::string& connInfo);

      DataSource(const te::core::URI& uri);

      ~DataSource();

      std::string getType() const;

      std::auto_ptr<te::da::DataSourceTransactor> getTransactor();

      void open();

      void close();

      bool isOpened() const;

      bool isValid() const;

      const te::da::DataSourceCapabilities& getCapabilities() const;

      const te::da::SQLDialect* getDialect() const;

      TeDatabase* getTerralib4Db();

      std::vector<std::string> getTL4Layers();

      std::vector<std::string> getTL4Tables();

      std::vector<std::string> getTL4Rasters();

      std::vector<std::pair<std::string, std::string> > getTL4RasterFiles();

      std::vector<::terralib4::ThemeInfo> getTL4Themes();

      TeTheme* getTL4Theme(const ::terralib4::ThemeInfo& theme);

      int getLayerSRID(const std::string & layerName);

    protected:

      void create(const std::string& dsInfo);

      void drop(const std::string& dsInfo);

      bool exists(const std::string& dsInfo);

      std::vector<std::string> getDataSourceNames(const std::string& dsInfo);

    private:

      TeDatabase* m_db;

      static te::da::DataSourceCapabilities sm_capabilities;
      static te::da::SQLDialect* sm_dialect;
  };

}

#endif  // __TERRALIB_TERRALIB4_INTERNAL_DATASOURCE_H
