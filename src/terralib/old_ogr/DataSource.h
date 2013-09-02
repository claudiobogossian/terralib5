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
  \file terralib/ogr/DataSource.h

  \brief The OGR data source provider.  
 */

#ifndef __TERRALIB_OGR_INTERNAL_DATASOURCE_H
#define __TERRALIB_OGR_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"

// Forward declarations
class OGRDataSource;

namespace te
{
  namespace ogr
  {
    /*!
      \class DataSource

      \brief The OGR data source provider.

      \sa te::da::DataSource, te::da::DataSourceFactory, te::da::DataSourceManager, Transactor, DataSourceFactory
     */
    class TEOGREXPORT DataSource : public te::da::DataSource
    {
      public:        

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new OGR data source object. */
        DataSource();

        /*!
          \brief Destructor.

          \note It will automatically release any used resources.
         */
        ~DataSource();

        //@}

        const std::string& getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;

        static void setDialect(te::da::SQLDialect* myDialect);

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

        std::vector<std::string> getDataSources(const std::map<std::string, std::string>& info);

        std::vector<std::string> getEncodings(const std::map<std::string, std::string>& info);

      private:

        std::map<std::string, std::string> m_connectionInfo;        //!< Connection information.
        te::da::DataSourceCatalog* m_catalog;                       //!< The main system catalog.
        OGRDataSource* m_ogrDS;                                     //!< A pointer to OGR Data Source.
        bool m_isValid;                                             //!< True if this is a valid datasource.
        te::da::DataSourceCapabilities m_capabilities;              //!< OGR capabilities.

        static te::da::SQLDialect* sm_myDialect;                //!< OGR SQL dialect.
    };

  } // end namespace ogr
}   // end namespace te


#endif  // __TERRALIB_OGR_INTERNAL_DATASOURCE_H


