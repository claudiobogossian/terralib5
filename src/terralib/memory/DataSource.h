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
  \file terralib/memory/DataSource.h

  \brief 
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H
#define __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "Config.h"

// Boost
#include <boost/thread.hpp>

namespace te
{
  namespace da
  {
    class ObjectIdSet;
  }
  namespace mem
  {
    /*!
      \class DataSource

      \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.

      \sa te::da::DataSource
    */
    class TEMEMORYEXPORT DataSource : public te::da::DataSource
    {
      public:

        DataSource();

        ~DataSource();

        /*!
          \brief It returns a map relating the dataset names and their contents.

          \return The map relating the dataset names and their contents..
        */
        const std::map<std::string, te::da::DataSetPtr>& getDataSets() const;

        /*!
          \brief It returns a map relating the dataset names and their schemas.

          \return The map relating the dataset names and their schemas.
        */
        const std::map<std::string, te::da::DataSetTypePtr> getSchemas() const;

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

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        std::vector<std::string> getDataSetNames();

        std::size_t getNumberOfDataSets();

        std::auto_ptr<te::da::DataSetType> getDataSetType(const std::string& datasetName);

        boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& name);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos);

        std::vector<std::string> getPropertyNames(const std::string& datasetName);

        std::size_t getNumberOfProperties(const std::string& datasetName);

        bool propertyExists(const std::string& datasetName, const std::string& name);

        void addProperty(const std::string& datasetName, te::dt::Property* p);

        void dropProperty(const std::string& datasetName, const std::string& propertyName);

        void renameProperty(const std::string& datasetName, const std::string& name, const std::string& newName);

        std::size_t getNumberOfItems(const std::string& datasetName);

        bool hasDataSets();

        bool dataSetExists(const std::string& name);

        void createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        void cloneDataSet(const std::string& name, const std::string& cloneName,
                          const std::map<std::string, std::string>& options);

        void dropDataSet(const std::string& name);

        void renameDataSet(const std::string& name, const std::string& newName);

        void add(const std::string& datasetName, te::da::DataSet* d,
                 const std::map<std::string, std::string>& options, std::size_t limit);

        void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids);

        void update(const std::string& datasetName,
                    te::da::DataSet* dataset,
                    const std::vector<std::size_t>& properties,
                    const te::da::ObjectIdSet* oids,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit = 0);

        /*!
          \brief It sets the capabilities document.
          
          \param capabilities The memory data source capabilities.

          \note The memory data source will take the ownership of the given capabilities object.

          \note Memory driver extended method.
        */
        static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

        std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo);

      private:

        std::map<std::string, std::string> m_connInfo;              //!< DataSource information.
        std::map<std::string, te::da::DataSetPtr> m_datasets;       //!< The set of datasets stored in memory.
        std::map<std::string, te::da::DataSetTypePtr> m_schemas;    //!< The set of dataset schemas.
        mutable boost::recursive_mutex m_mtx;                       //!< The internal mutex.
        std::size_t m_numDatasets;                                  //!< The number of datasets kept in the data source.
        std::size_t m_maxNumDatasets;                               //!< The maximum number of datasets to be handled by the data source.
        bool m_isOpened;                                            //!< A flag to control the state of the data source.
        bool m_deepCopy;                                            //!< If true, each dataset is cloned in the getDataSet method.

        static te::da::DataSourceCapabilities sm_capabilities;      //!< The Memory data source capabilities.
        static const te::da::SQLDialect sm_dialect;                 //!< A dummy dialect.
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H
