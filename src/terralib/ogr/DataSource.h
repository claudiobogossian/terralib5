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
  \file terralib/ogr/DataSource.h

  \brief A class for data providers of OGR.
*/

#ifndef __TERRALIB_OGR_INTERNAL_DATASOURCE_H
#define __TERRALIB_OGR_INTERNAL_DATASOURCE_H

// TerraLib
#include "Config.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "Exception.h"

// Forward declarations
class OGRDataSource;

namespace te
{
  namespace ogr
  {
    /*!
      \class DataSource

      \brief The OGR data source provider.
    */
    class TEOGREXPORT DataSource : public te::da::DataSource
    {
      public:

        /*! \brief Default constructor that can be called by subclasses. */
        DataSource();

        /*! \brief Virtual destructor. */
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
      
        void createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        static void setDialect(te::da::SQLDialect* dialect);

        OGRDataSource* getOGRDataSource();

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

        std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo);

      protected:

        std::map<std::string, std::string> m_connectionInfo;        //!< Connection information.
        OGRDataSource* m_ogrDS;                                     //!< A pointer to OGR Data Source.
        bool m_isValid;                                             //!< True if this is a valid datasource.
        te::da::DataSourceCapabilities m_capabilities;              //!< OGR capabilities.
        bool m_isInTransaction;                                     //!< Tells if there is a transaction in progress.

        static te::da::SQLDialect* sm_myDialect;                    //!< OGR SQL dialect.
    };

    DataSource* Build();
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_OGR_INTERNAL_DATASOURCE_H
