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
  \file terralib/ado/DataSource.h

  \brief Implementation of the data source class for the ADO driver.
*/

#ifndef __TERRALIB_ADO_INTERNAL_DATASOURCE_H
#define __TERRALIB_ADO_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"

// BOOST
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

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

        const std::map<std::string, std::string>& getGeomColumns() const;

        void registerGeometryColumn(const std::string& datasetName,
                                    const std::string& geomColName);

        bool isGeometryColumn(const std::string& datasetName,
                              const std::string& colName) const;

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

        std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo);

        void loadGeometryColumnsCache(_ConnectionPtr& adoConn);

      private:

        std::map<std::string, std::string> m_connInfo;    //!< Connection information.
        std::map<std::string, std::string> m_geomColumns; //!< The list of geometry columns.
        std::string m_currentSchema;                      //!< The default schema used when no one is provided.
        mutable boost::mutex m_mtx;
        bool m_isOpened;

        static te::da::DataSourceCapabilities sm_capabilities;  //!< ADO capabilities.
        static te::da::SQLDialect* sm_dialect; 
    };


  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_DATASOURCE_H
