/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/gdal/DataSource.h

  \brief The implementation of a DataSource that consists of datasets that can be decoded by the GDAL Library.
*/

#ifndef __TERRALIB_GDAL_INTERNAL_DATASOURCE_H
#define __TERRALIB_GDAL_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"

namespace te
{
  namespace gdal
  {
    /*!
      \class DataSource

      \brief A driver to access raster data using the GDAL library.

      This class is an implementation of a DataSource that consists of raster that can be decoded 
      by the GDAL - Geospatial Data Abstraction Library (http://www.gdal.org/).
    */
    class TEGDALEXPORT DataSource : public te::da::DataSource
    {
      public:

        DataSource();
      
        ~DataSource();

        const std::string& getType() const;

        /*!
          \brief It sets information to be used when connecting to the data source.

          Examples of valid connection information strings are:
          <ul>
          <li>"uri=foo\bar\file.ext": refers to the single file "file.ext".</li>
          <li>"uri=foo\bar\": refers to all files located inside the directory "foo\bar".</li>
          <li>source=host='<host>' port:'<port>' dbname='<dbname>' user='<user>' password='<password>' [schema='<schema>'] [table='<raster_table>'] [where='<sql_where>'] [mode='<working_mode>']" : refers to a PostGIS WKT Raster.</li>
          </ul>

          \param connInfo Key-value-pairs (kvp) with the connection information.
        */
        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        const std::map<std::string, std::string>& getConnectionInfo() const;

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;

        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        te::da::DataSourceCatalog* getCatalog() const;

        te::da::DataSourceTransactor* getTransactor();

        void optimize(const std::map<std::string, std::string>& opInfo);

        bool isDirectory() const { return m_isDirectory; }

        /*!
          \brief It sets the capabilities document.
          
          \param capabilities The GDAL data source capabilities.

          \note GDAL driver extended method.
        */
        static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);
 
      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSources(const std::map<std::string, std::string>& info);

        std::vector<std::string> getEncodings(const std::map<std::string, std::string>& info);

      private:

        te::da::DataSourceCatalog* m_catalog;                 //!< The associated data source catalog.
        std::map<std::string, std::string> m_connectionInfo;  //!< The connection parameters.
        bool m_isOpened;                                      //!< Tells if the data source is opened.
        bool m_isDirectory;                                   //!< Tells if the data source refers to a directory.

        static te::da::DataSourceCapabilities sm_capabilities;  //!< GDAL capabilities.
    };

  } // end namespace gdal
} // end namespace te

#endif // __TERRALIB_GDAL_INTERNAL_DATASOURCE_H

