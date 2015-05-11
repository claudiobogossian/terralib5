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
  \file terralib/wfs/DataSource.h

  \brief Implementation of the data source for the WFS driver.
*/

#ifndef __TERRALIB_WFS_INTERNAL_DATASOURCE_H
#define __TERRALIB_WFS_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"
#include "Exception.h"
#include "WFSLayerInfo.h"

// STL
#include <string>
#include <vector>

// Forward declarations
class OGRDataSource;

namespace te
{
  namespace wfs
  {
    /*!
      \class DataSource
     
      \brief Implementation of the data source for the WFS driver.
    */
    class TEWFSEXPORT DataSource : public te::da::DataSource
    {
      public:

        DataSource();

        ~DataSource();

        std::string getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        std::auto_ptr<te::da::DataSourceTransactor> getTransactor();

        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        const te::da::DataSourceCapabilities& getCapabilities() const;

        static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);

        const te::da::SQLDialect* getDialect() const;

        static void setDialect(te::da::SQLDialect* dialect);

        OGRDataSource* getOGRDataSource();

        const std::vector<WFSLayerInfo>& getLayersInfo();

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

        std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo);

      private:

        void verifyConnectionInfo() const;

        void buildLayersInfo();

      private:

        std::map<std::string, std::string> m_connectionInfo;
        OGRDataSource* m_ogrDS;
        bool m_isOpened;
        std::vector<WFSLayerInfo> m_layersInfo;
        static te::da::DataSourceCapabilities sm_capabilities;
        static te::da::SQLDialect* sm_dialect;
    };

  } // end namespace wfs
}   // end namespace te

#endif // __TERRALIB_WFS_INTERNAL_DATASOURCE_H
