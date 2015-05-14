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
  \file terralib/dataaccess/datasource/DataSourceCapabilities.h

  \brief A class that represents the known capabilities of a specific data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECAPABILITIES_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECAPABILITIES_H

// TerraLib
#include "../../common/Enums.h"
#include "../dataset/DataSetCapabilities.h"
#include "../dataset/DataSetTypeCapabilities.h"
#include "../query/QueryCapabilities.h"
#include "../Config.h"
#include "DataTypeCapabilities.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace da
  {
    /*!
      \class DataSourceCapabilities

      \brief A class that represents the known capabilities of a specific data source,
             i.e. this class informs all information about what the data source can perform.
             Here you will find if the data source implementation supports primary keys,
             foreign keys, if it can be used in a thread environment and much more information.

      \ingroup dataaccess
    */
    class TEDATAACCESSEXPORT DataSourceCapabilities
    {
      public:

        /*! \brief Constructor. */
        DataSourceCapabilities();

        /*! \brief Destructor. */
        ~DataSourceCapabilities();

      public:

        te::common::AccessPolicy getAccessPolicy() const;

        void setAccessPolicy(const te::common::AccessPolicy& accessPolicy);

        bool supportsTransactions() const;

        void setSupportTransactions(const bool& support);

        bool supportsDataSetPesistenceAPI() const;

        void setSupportDataSetPesistenceAPI(const bool& support);

        bool supportsDataSetTypePesistenceAPI() const;

        void setSupportDataSetTypePesistenceAPI(const bool& support);

        bool supportsPreparedQueryAPI() const;

        void setSupportPreparedQueryAPI(const bool& support);

        bool supportsBatchExecutorAPI() const;

        void setSupportBatchExecutorAPI(const bool& support);

        const DataTypeCapabilities& getDataTypeCapabilities() const;

        void setDataTypeCapabilities(const DataTypeCapabilities& capabilities);

        const DataSetTypeCapabilities& getDataSetTypeCapabilities() const;

        void setDataSetTypeCapabilities(const DataSetTypeCapabilities& capabilities);

        const DataSetCapabilities& getDataSetCapabilities() const;

        void setDataSetCapabilities(const DataSetCapabilities& capabilities);

        const QueryCapabilities& getQueryCapabilities() const;

        void setQueryCapabilities(const QueryCapabilities& capabilities);

        const std::map<std::string, std::string>& getSpecificCapabilities() const;

        void addSpecificCapability(const std::string& key, const std::string& value);

        void setSupportAll();

      private:

        te::common::AccessPolicy m_accessPolicy; //!< A value that indicates the data source access policy.
        bool m_supportTransactions;              //!< A flag that indicates if the data source supports ACID transactions.
        bool m_supportDataSetPesistenceAPI;      //!< A flag that indicates if the data source supports the DataSetPersistence API.
        bool m_supportDataSetTypePesistenceAPI;  //!< A flag that indicates if the data source supports the DataSetTypePersistence API.
        bool m_supportPreparedQueryAPI;          //!< A flag that indicates if the data source supports the PreparedQuery API.
        bool m_supportBatchExecutorAPI;          //!< A flag that indicates if the data source supports the BatchExecutor API.
        
        DataTypeCapabilities m_dataTypeCapabilities;       //!< The DataType Capabilities.
        DataSetTypeCapabilities m_dataSetTypeCapabilities; //!< The DataSetType Capabilities.
        DataSetCapabilities m_dataSetCapabilities;         //!< The DataSet Capabilities.
        QueryCapabilities m_queryCapabilities;             //!< The Query Capabilities.

        std::map<std::string, std::string> m_specificCapabilities; //!< A list of key-value-pairs that data source driver can supply with specifics capabilities.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECAPABILITIES_H
