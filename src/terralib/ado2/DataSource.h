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
  \file terralib/ado2/DataSource.h

  \brief Implementation of the data source for the ADO driver.
*/

#ifndef __TERRALIB_ADO_INTERNAL_DATASOURCE_H
#define __TERRALIB_ADO_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess2/datasource/DataSource.h"
#include "../dataaccess2/datasource/DataSourceCapabilities.h"
#include "Config.h"

// ADO
#import "msado15.dll" \
  no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"


namespace te
{
  namespace gm  { class GeometryProperty; }

  namespace ado
  {
    // Forward declaration
    class ConnectionPool;
    struct VersionInfo;

    /*!
      \class DataSource

      \brief The ADO driver.

      \sa te::da::DataSource, te::da::DataSourceFactory, te::da::DataSourceManager, DataSourceFactory
    */
    class TEADOEXPORT DataSource : public te::da::DataSource
    {
      public:

        /** @name Initializer Methods
         *  Methods related to the instantiation and destruction.
         */
        //@{

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

        const te::da::SQLDialect* getDialect() const;

        void cancel();

        boost::int64_t getLastGeneratedId();

        static std::vector<std::string> getDataSourceNames(const std::string& dsType, const std::map<std::string, std::string>& info);

        static std::vector<std::string> getEncodings(const std::string& dsType, const std::map<std::string, std::string>& info);

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getEncodings(const std::map<std::string, std::string>& dsInfo);

        void getIndexes(te::da::DataSetTypePtr& dt);

        void getProperties(te::da::DataSetTypePtr& dt);

      private:

        te::da::DataSourceCatalog* m_catalog;             //!< The main system catalog.
        std::map<std::string, std::string> m_connInfo;    //!< Connection information.
        Connection* m_conn;                               //!< The Connection.
        std::string m_currentSchema;                      //!< The default schema used when no one is provided.
        bool m_isInTransaction;                           //!< It indicates if there is a transaction in progress.

        static te::da::DataSourceCapabilities sm_capabilities;  //!< ADO capabilities.
        static te::da::SQLDialect* sm_dialect; 
    };

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_DATASOURCE_H
