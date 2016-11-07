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
//class OGRDataSource;
class GDALDataset;

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

        DataSource(const std::string& connInfo);

        DataSource(const te::core::URI& uri);

        /*! \brief Virtual destructor. */
        ~DataSource();

        std::string getType() const;

        std::auto_ptr<te::da::DataSourceTransactor> getTransactor();

        void open();
        
        void close();

        bool isOpened() const;

        bool isValid() const;

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;
      
        void createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        static void setDialect(te::da::SQLDialect* dialect);

        GDALDataset* getOGRDataSource();

        void createOGRDataSource();

        te::core::EncodingType getEncoding();

        void setEncoding(const te::core::EncodingType& et);
        

      protected:

        void create(const std::string& connInfo);

        void drop(const std::string& connInfo);

        bool exists(const std::string& connInfo);

        std::vector<std::string> getDataSourceNames(const std::string& connInfo);

      protected:

        GDALDataset* m_ogrDS;                                       //!< A pointer to OGR Data Source.
        bool m_isValid;                                             //!< True if this is a valid datasource.
        te::da::DataSourceCapabilities m_capabilities;              //!< OGR capabilities.
        bool m_isInTransaction;                                     //!< Tells if there is a transaction in progress.
        static te::da::SQLDialect* sm_myDialect;                    //!< OGR SQL dialect.
        te::core::EncodingType m_encoding;                          //!< The data source encoding type.
    };

    te::da::DataSource* Build(const te::core::URI& uri);
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_OGR_INTERNAL_DATASOURCE_H
