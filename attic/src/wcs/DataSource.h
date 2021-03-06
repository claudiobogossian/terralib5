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
  \file terralib/wcs/DataSource.h

  \brief Implementation of the data source for the WCS driver.
*/

#ifndef __TERRALIB_WCS_INTERNAL_DATASOURCE_H
#define __TERRALIB_WCS_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"
#include "Exception.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace core { class URI; }
  namespace wcs
  {
    /*!
      \class DataSource
     
      \brief Implementation of the data source for the WCS driver.
    */
    class TEWCSEXPORT DataSource : public te::da::DataSource
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

        static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);

        const te::da::SQLDialect* getDialect() const;

      protected:

        void create(const std::string& connInfo);

        void drop(const std::string& connInfo);

        bool exists(const std::string& connInfo);

        std::vector<std::string> getDataSourceNames(const std::string& connInfo);

        std::vector<te::core::EncodingType> getEncodings(const std::string& connInfo);

      private:

        void verifyConnectionInfo() const;

      private:

        bool m_isOpened;
        static te::da::DataSourceCapabilities sm_capabilities;
    };

  } // end namespace wcs
}   // end namespace te

#endif // __TERRALIB_WCS_INTERNAL_DATASOURCE_H
