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
  \file DataSource.h

  \brief It implements the DataSource class for ADO.
*/

#ifndef __TERRALIB_ADO_INTERNAL_DATASOURCE_H
#define __TERRALIB_ADO_INTERNAL_DATASOURCE_H

// TerraLib
#include "../common/ThreadingPolicies.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/query/SQLDialect.h"
#include "Config.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

#include <stdio.h>
#include "icrsint.h"

// Boost
#include <boost/ptr_container/ptr_map.hpp>

namespace te
{
  namespace da
  {
    class DataSourceCatalog;
  }

  namespace ado
  {
    /*!
      \class DataSource

      \brief It implements the DataSource class for ADO.

      \sa te::da::DataSource
    */
    class TEADOEXPORT DataSource : public te::da::DataSource
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

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

      private:

        _ConnectionPtr m_conn;                                      //!< ADO connection
        te::da::DataSourceCatalog* m_catalog;                       //!< The main system catalog.
        _bstr_t m_strCnn;                                           //!< Connection String
        std::map<std::string, std::string> m_connectionInfo;        //!< Connection information.

        static te::da::SQLDialect* sm_myDialect;                    //!< ADO SQL dialect.
    };

  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_DATASOURCE_H

