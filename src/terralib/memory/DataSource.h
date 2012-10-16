/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/DataSource.h

  \brief Implements the DataSource class for the TerraLib In-Memory driver.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H
#define __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H

// TerraLib
#include "../common/ThreadingPolicies.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/query/SQLDialect.h"
#include "Config.h"

// Boost
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// STL
#include <vector>

namespace te
{
  namespace da
  {
    class DataSetType;
  }

  namespace mem
  {
// Forward declaration
    class DataSet;

    /*!
      \class DataSource

      \brief Implements the DataSource class for the TerraLib In-Memory driver.

      \sa te::da::DataSource
    */
    class TEMEMORYEXPORT DataSource : public te::da::DataSource, public te::common::ObjectLevelLockable<DataSource,
                                                                                                        ::boost::recursive_mutex,
                                                                                                        ::boost::lock_guard< ::boost::recursive_mutex>,
                                                                                                        ::boost::lock_guard< ::boost::recursive_mutex> >
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
          \brief It adds a dataset to be managed by the data source.

          \exception Exception It throws an excpetion if a dataset with the given name already exists or if the data source has reached the maximum size.

          \note The data source will take the ownership of the dataset.

          \note Thread-safe.

          \note In-Memory driver extended method.
        */
        void add(const std::string& name, DataSet* dataset);

        /*!
          \brief It returns a dataset (a deep copy or a shared copy).

          \return A dataset or NULL if none is found.

          \note The caller will take the ownership of the returned dataset.

          \note Thread-safe.

          \note In-Memory driver extended method.
        */
        DataSet* getDataSet(const std::string& name);

        /*!
          \brief It returns the internal dataset pointer.

          \return A dataset or NULL if none is found.

          \note The caller will NOT take the ownership of the returned dataset.

          \note Thread-safe.

          \note In-Memory driver extended method.
        */
        DataSet* getDataSetRef(const std::string& name);

        /*!
          \brief It returns a dataset type.

          \return A dataset type or NULL if none is found.

          \note The caller will take the ownership of the returned dataset type.

          \note Thread-safe.

          \note In-Memory driver extended method.
        */
        te::da::DataSetType* getDataSetType(const std::string& name);

        /*!
          \brief It returns the list of datasets managed by the data source.

          \note The caller will take the ownership of the returned list.

          \note In-Memory driver extended method.
        */
        boost::ptr_vector<std::string> getDataSets() const;

        /*!
          \brief It checks if a given dataset exists.

          \return True if a dataset with the given name is stored in the data source.

          \note In-Memory driver extended method.
        */
        bool datasetExists(const std::string& name) const;

        /*!
          \brief It removes a dataset.

          \param datasetName The name of the dataset to be removed.

          \exception Exception It throws an excpetion if a dataset with the given name doesn't exist.

          \note In-Memory driver extended method.
        */
        void remove(const std::string& datasetName);
        
        /*!
          \brief It renames a dataset

          \param oldName The name of the dataset to be renamed.
          \param newName The dataset new name.

          \exception Exception It throws an excpetion if a dataset with the old-name doesn't exist or if one with new-name already exist.

          \note In-Memory driver extended method.
        */
        void rename(const std::string& oldName, std::string newName);
      
        /*!
         \brief It return true if the data source has any dataset.
         
         \return True if the data source has datasets otherwise, false.
         */      
        bool hasDataSets();      

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

      private:

        std::map<std::string, std::string> m_connInfo;      //!< DataSource information.
        boost::ptr_map<std::string, DataSet> m_datasets;    //!< The set of datasets stored in memory.
        std::auto_ptr<te::da::DataSourceCatalog> m_catalog; //!< The main system catalog.
        std::size_t m_ndatasets;                            //!< The number of datasets kept in the data source.
        std::size_t m_maxdatasets;                          //!< The maximum number of datasets to be handled by the data source.
        bool m_isOpened;                                    //!< A flag to control the state of the data source.
        bool m_deepCopy;                                    //!< If true each dataset is cloned in the getDataSet method.

        static const te::da::SQLDialect sm_dialect;         //!< A dummy dialect.
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H

