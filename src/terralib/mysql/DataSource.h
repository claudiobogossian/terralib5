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
  \file terralib/mysql/DataSource.h

  \brief Implements the DataSource class for MySQL data access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_DATASOURCE_H
#define __TERRALIB_MYSQL_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"

// STL
#include <memory>
#include <set>

namespace te
{
  namespace da
  {
    class DataSetType;
  }

  namespace mysql
  {
// Forwars declaration
    class ConnectionPool;
    class DataSourceTransactor;

    /*!
      \class DataSource

      \brief Implements the DataSource class for MySQL data access driver.

      \sa te::da::DataSource
    */
    class TEMYSQLEXPORT DataSource : public te::da::DataSource
    {
      public:

        DataSource();

        ~DataSource();

        const std::string& getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;

        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        te::da::DataSourceCatalog* getCatalog() const;

        te::da::DataSourceTransactor* getTransactor();

        void optimize(const std::map<std::string, std::string>& opInfo);

        /*!
          \note MySQL driver extended method.
        */
        DataSourceTransactor* getMyTransactor();

        /*!
          \note MySQL driver extended method.
        */        
        ConnectionPool* getPool() const;

        /*!
          \note MySQL driver extended method.
        */
        bool isRaster(const std::string& datasetName) const;

        /*!
          \note MySQL driver extended method.
        */
        const std::map<std::string, te::da::DataSetType*>& getRasters() const { return m_rasters; }

        /*!
          \note MySQL driver extended method.
        */
        std::map<std::string, te::da::DataSetType*>& getRasters() { return m_rasters; }

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        /*!
          \note MySQL driver extended method.
        */
        void createMetadata();

        /*!
          \note MySQL driver extended method.
        */
        void loadRasters();

      private:

        std::auto_ptr<ConnectionPool> m_pool;
        std::auto_ptr<te::da::DataSourceCatalog> m_catalog;

        std::map<std::string, std::string> m_dsInfo;

        std::map<std::string, te::da::DataSetType*> m_rasters;

        //static const te::da::SQLDialect* sm_dialect;              //!< The Query dialect of the MySQL driver (note: the data source doesn't have the ownership of this pointer).
        static te::da::DataSourceCapabilities sm_capabilities;    //!< MySQL capabilities.
    };

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_DATASOURCE_H

