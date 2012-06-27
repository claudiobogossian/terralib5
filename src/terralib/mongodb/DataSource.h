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
  \file terralib/mongodb/DataSource.h

  \brief Implements the DataSource class for MongoDB.
*/

#ifndef __TERRALIB_MONGODB_INTERNAL_DATASOURCE_H
#define __TERRALIB_MONGODB_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "Config.h"

// Forward declarations
namespace mongo { class DBClientConnection; }

namespace te
{
  namespace mongodb
  {
    /*!
      \class DataSource

      \brief Implements the DataSource class for MongoDB.

      \sa te::da::DataSource
    */
    class TEMONGODBEXPORT DataSource : public te::da::DataSource
    {
      public:

        DataSource();

        ~DataSource();

        const std::string& getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        void getCapabilities(std::map<std::string, std::string>& capabilities) const;

        const te::da::SQLDialect* getDialect() const;

        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        te::da::DataSourceCatalog* getCatalog() const;

        te::da::DataSourceTransactor* getTransactor();

        void optimize(const std::map<std::string, std::string>& opInfo);

        /*!
          \note MongoDB driver extended method.
        */
        const std::string& getDB() const;

        /*!
          \note MongoDB driver extended method.
        */
        mongo::DBClientConnection* getConn() const;

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

      private:

        std::map<std::string, std::string> m_dsInfo;
        std::string m_db;
        mongo::DBClientConnection* m_conn;
    };

    inline const std::string& DataSource::getDB() const
    {
      return m_db;
    }

    inline mongo::DBClientConnection* DataSource::getConn() const
    {
      return m_conn;
    }

  } // end namespace mongodb
}   // end namespace te

#endif  // __TERRALIB_MONGODB_INTERNAL_DATASOURCE_H

