/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/datasource/DataSourceTransactor.h

  \brief A DataSourceTransactor can be viewed as a connection to the data source for reading/writing things into it.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H

// TerraLib
#include "../../common/Enums.h"
#include "../../geometry/Enums.h"
#include "../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
// Forward declarations
  namespace dt { class Property; }
  namespace gm { class Envelope; class Geometry; }

  namespace da
  {
// Forward declarations    
    class BatchExecutor;
    class DataSet;
    class DataSetPersistence;
    class DataSetTypePersistence;
    class DataSource;
    class DataSourceCatalogLoader;
    class PreparedQuery;
    class Query;
    class Select;
   
    /*!
      \class DataSourceTransactor

      \brief A DataSourceTransactor can be viewed as a connection to the data source for reading/writing things into it.

      A transactor can be used to create a prepared query or a batch command executor.

      If you are planning a multi-thread application, it is better not to share
      the same transactor between threads because its methods are not thread-safe. Instead,
      use one transactor per thread.

      \sa DataSource, DataSourceCatalog, DataSet, Query, PreparedQuery, BatchExecutor, DataSetPersistence, DataSetTypePersistence

      \warning Don't delete the transactor if you have opened datasets with the transactor, this may
               cause errors in future access to the dataset objects.
    */
    class TEDATAACCESSEXPORT DataSourceTransactor : public boost::noncopyable 
    {
      public:

        /** @name Transaction
         *  Methods for dealing with transactions.
         */
        //@{

        /*!
          \brief It starts a new transaction.
          
          \note Not thread-safe!
        */         
        virtual void begin() = 0;

        /*!
          \brief It commits the transaction.

          After commiting or rolling back, you can start another transaction.

          \note Not thread-safe!
        */
        virtual void commit() = 0;

        /*!
          \brief It aborts the transaction. Any changes will be rolled-back.

          After commiting or rolling back, you can start another transaction.

          \note Not thread-safe!
        */
        virtual void rollBack() = 0;

        /*!
          \brief It returns true if a transaction is in progress, otherwise, false.

          \return True if a transaction is in progress, otherwise, false.
        */
        virtual bool isInTransaction() const = 0;

        //@}        

        /** @name DataSet Retrieval
         *  Methods for retrieving data from the DataSource.
         */
        //@{

        /*!
          \brief It gets the dataset identified by the given name.

          \param name     The name of the dataset. It must be the same name as the DataSetType name in the DataSource catalog.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned dataset.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Not thread-safe!
        */
        virtual DataSet* getDataSet(const std::string& name, 
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole = te::common::RAccess) = 0;

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the default geometric property.

          \param name     The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
          \param e        A rectangle to be used as a spatial filter when retrieving datasets.
          \param r        The spatial relation to be used during the filter.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned DataSet.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Transactor will not take the ownership of the given envelope.

          \note The envelope coordinates should be in the same coordinate system as the dataset.

          \note This first attempt of this method will be to retrieve the default geometric column in
                the data source catalog. If it can not retrieve that information it may implicitly
                use the data source catalog loader to query the data source and retrieve the geometric
                property information.

          \note Not thread-safe!
        */
        virtual DataSet* getDataSet(const std::string& name,
                                    const te::gm::Envelope* e,
                                    te::gm::SpatialRelation r,
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole = te::common::RAccess);

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the specified property.

          \param name     The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
          \param p        The geometric property in order to apply the spatial filter.
          \param e        A rectangle to be used as a spatial filter when retrieving datasets.
          \param r        The spatial relation to be used during the filter.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned DataSet.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Transactor will not take the ownership of the given envelope.

          \note The envelope coordinates should be in the same coordinate system as the dataset.

          \note Not thread-safe!
        */
        virtual DataSet* getDataSet(const std::string& name,
                                    const te::dt::Property* p,
                                    const te::gm::Envelope* e,
                                    te::gm::SpatialRelation r,
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole = te::common::RAccess) = 0;

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the default geometric property.

          \param name     The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
          \param g        A geometry to be used as a spatial filter when retrieving datasets.
          \param r        The spatial relation to be used during the filter.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned DataSet.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Transactor will not take the ownership of the given geometry.

          \note The geometry coordinates should be in the same coordinate system as the dataset.

          \note This first attempt of this method will be to retrieve the default geometric column in
                the data source catalog. If it can not retrieve that information it may implicitly
                use the data source catalog loader to query the data source and retrieve the geometric
                property information.

          \note Not thread-safe!
        */
        virtual DataSet* getDataSet(const std::string& name,
                                    const te::gm::Geometry* g,
                                    te::gm::SpatialRelation r,
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole = te::common::RAccess);

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the given geometric property.

          \param name     The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
          \param p        The geometric property in order to apply the spatial filter.
          \param g        A geometry to be used as a spatial filter when retrieving datasets.
          \param r        The spatial relation to be used during the filter.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned DataSet.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Transactor will not take the ownership of the given geometry.

          \note The geometry coordinates should be in the same coordinate system as the dataset.
          
          \note Not thread-safe!
        */
        virtual DataSet* getDataSet(const std::string& name,
                                    const te::dt::Property* p,
                                    const te::gm::Geometry* g,
                                    te::gm::SpatialRelation r,
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole = te::common::RAccess) = 0;

        /*!
          \brief It executes a query that may return some data using a generic query.

          Different from the method that accepts a dataset name and
          a spatial filter, this method allows for retrieving only a
          subset of the attributes since a query can include
          a property list.

          \param q        A valid query object.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned dataset.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \exception Exception It can throws an exception if something goes wrong.

          \note Not thread-safe!
        */
        virtual DataSet* query(const Select& q, 
                               te::common::TraverseType travType = te::common::FORWARDONLY, 
                               te::common::AccessPolicy rwRole = te::common::RAccess) = 0;

        /*!
          \brief It executes a query that may returns some data using the data source native language.

          \param query    A query string in the data source native language.
          \param travType The traverse type associated to the returned dataset. 
          \param rwRole   The read and write permission associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned data set.

          \note Avoid using this method if you want portability for your application.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Not thread-safe!
        */
        virtual DataSet* query(const std::string& query, 
                               te::common::TraverseType travType = te::common::FORWARDONLY, 
                               te::common::AccessPolicy rwRole= te::common::RAccess) = 0;

        //@}

        /** @name Execution Methods
         *  Methods for executing operations against the data source.
         */
        //@{

        /*!
          \brief It executes the specifed command using a generic query representation.

          \param command A query like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE.

          \exception Exception It throws an exception if the query can not be performed.

          \note Not thread-safe!
        */
        virtual void execute(const Query& command) = 0;

        /*!
          \brief It executes the specifed command in the data source native language.

          \param command A query string in the data source native language (like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE).

          \exception Exception It throws an exception if the query can not be performed.

          \note Not thread-safe!
        */
        virtual void execute(const std::string& command) = 0;

        //@}

        /** @name Fine Grained Transactor Objects
         *  Methods for retrieving fine-grained transactor objects.
         */
        //@{

        /*!
          \brief It creates a prepared query object that may be used for query commands (select, insert, update and delete) that are used repeatedly.

          \param qName The prepared query name. 

          \return A prepared query object owned by the caller.
        */
        virtual PreparedQuery* getPrepared(const std::string& qName = std::string("")) = 0;

        /*!
          \brief It creates a batch command executor.

          \return A batch command executor owned by the caller.
        */
        virtual BatchExecutor* getBatchExecutor() = 0;

        /*!
          \brief It creates a data source catalog loader that can be used to load information about the data source catalog.

          \return A data source catalog loader owned by the caller.
        */
        virtual DataSourceCatalogLoader* getCatalogLoader() = 0;

        /*!
          \brief It creates a dataset type persistence.

          \return A dataset type persistence owned by the caller.
         */
        virtual DataSetTypePersistence* getDataSetTypePersistence() = 0;

        /*!
          \brief It creates a dataset persistence.

          \return A dataset persistence owned by the caller.
        */
        virtual DataSetPersistence* getDataSetPersistence() = 0;

        //@}

        /** @name General Methods
         *  General methods on a transactor.
         */
        //@{

        /*!
          \brief It requests that the transactor abandon processing of the current command.

          \exception Exception It may throws an exception if it is not possible to cancel the command execution.

          \note Even on success there is no guarantee that the request will have any effect.

          \note The cancel method can be safely invoked from a thread that is separate from the one manipulating the transactor.
        */
        virtual void cancel() = 0;

        /*!
          \brief It returns the last id generated by an insertion command.

          \return The last id generated by an insertion command.

          \note Useful for working with auto-increment values.
        */
        virtual boost::int64_t getLastInsertId() = 0;

        /*!
          \brief It returns the parent data source of the transactor.

          \return The caller of this method will NOT take the ownership of the returned DataSource pointer.
        */
        virtual DataSource* getDataSource() const = 0;

        /*! \brief Virtual destructor. */
        virtual ~DataSourceTransactor() {}

        //@}

      protected:

        /*! \brief Default constructor just can be called by subclasses. */
        DataSourceTransactor() {}
    };

    typedef boost::shared_ptr<DataSourceTransactor> DataSourceTransactorPtr;

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H

