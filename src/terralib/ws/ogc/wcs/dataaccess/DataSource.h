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
  \file terralib/ws/ogc/wcs/dataaccess/DataSource.h

  \brief Data Source for WS OGC WCS

  \author Vinicius Campanha
*/

#ifndef __TERRALIB_WS_OGC_WCS_DATAACCESS_DATASOURCE_H
#define __TERRALIB_WS_OGC_WCS_DATAACCESS_DATASOURCE_H


// STL
#include <map>
#include <string>

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../client/WCS.h"
#include "Config.h"
#include "Exception.h"


namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wcs
      {
        namespace da
        {
          /*!
          \class DataSource

          \brief Implementation of the data source for the WCS driver.
          */
          class TEOGCWCSDATAACCESSEXPORT DataSource : public te::da::DataSource
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

          protected:

            void create(const std::map<std::string, std::string>& dsInfo);

            void drop(const std::map<std::string, std::string>& dsInfo);

            bool exists(const std::map<std::string, std::string>& dsInfo);

            std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

            std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo);

          private:

            void verifyConnectionInfo() const;

          private:

            te::ws::ogc::WCS wcs_;
            std::map<std::string, std::string> m_connectionInfo;
            bool m_isOpened;
            static te::da::DataSourceCapabilities sm_capabilities;
          };

        } // end namespace da
      } // end namespace wcs
    } // end namespace ogc
  } // end namespace ws
} // end namespace te


#endif // __TERRALIB_WS_OGC_WCS_DATAACCESS_DATASOURCE_H

