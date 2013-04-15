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
  \file terralib/dataaccess/dataset/DataSetPersistence.h

  \brief An abstract class responsible for persisting a dataset in a data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETPERSISTENCE_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETPERSISTENCE_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace da
  {
// Forward declarations
    class DataSet;
    class DataSetType;
    class ObjectId;
    class ObjectIdSet;

    /*!
      \brief This class is responsible for persisting datasets in a data source.

      This is an abstract class that persists datasets in a data source.
      Each data source must implement the pure virtual methods considering its particular
      persistence support. 

      \note Some methods of this class need the full DataSetType information (Primary Keys and other stuffs).

      \sa DataSet, DataSetPersistence, DataSetType
    */
    class TEDATAACCESSEXPORT DataSetPersistence : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        DataSetPersistence() {}

        /*! \brief Virtual destructor. */
        virtual ~DataSetPersistence() {}

        /** @name Pure Virtual Methods
         *  Methods that subclasses must implement in order to support the persistence interface.
         */
        //@{

        /*!
          \brief It creates the dataset definition in a data source and then fill it with data from the input dataset.

          This method will create the dataset schema definition and will 
          save all the dataset data.

          \param dt      The source dataset definition.
          \param d       The source dataset data.
          \param limit   The number of items to be used from the input dataset. If set to 0 (default) all items are used.

          \pre All parameters must be valid pointers.

          \post It is the caller responsability to release the dataset 'd' pointer.

          \exception Exception It throws an exception if the dataset can not be created.

          \note DataSetPersistence will start reading the dataset 'd' in the
                current position. So, keep in mind that it is the caller responsability
                to inform the dataset 'd' in the right position (and a valid one) to start processing it.
        */
        virtual void create(DataSetType* dt,
                            DataSet* d,
                            std::size_t limit = 0);

        /*!
          \brief It creates the dataset definition in a data source and then fill it with data from the input dataset.

          This method will create the dataset schema definition and will 
          save all the dataset data.

          \param dt       The source dataset definition.
          \param d        The source dataset data.
          \param options  A list of optional modifiers. It is driver specific.
          \param limit    The number of items to be used from the input dataset. If set to 0 (default) all items are used.

          \pre All parameters must be valid pointers.

          \exception Exception It throws an exception if the dataset can not be created.

          \note It is the caller responsability to release the dataset 'd' pointer.

          \note DataSetPersistence will start reading the dataset 'd' in the
                current position. So, keep in mind that it is the caller responsability
                to inform the dataset 'd' in the right position (and a valid one) to start processing it.
        */
        virtual void create(DataSetType* dt,
                            DataSet* d,
                            const std::map<std::string, std::string>& options,
                            std::size_t limit = 0) = 0;

        /*!
          \brief It removes all data associated to the dataset from the data source.

          \param datasetName The dataset name.

          \post All data associated to dataset will be erased.

          \exception Exception It throws an exception if the elements of the dataset can not be removed.
        */
        virtual void remove(const std::string& datasetName) = 0;

        /*!
          \brief It removes all the informed items from the dataset.

          It removes all data collection identified by an 
          object identifier from the data source.

          \param datasetName The dataset name.
          \param oids        A list of object identifiers used to remove data from the datasource.

          \pre The parameter oids must be a valid pointer.

          \exception Exception It throws an exception if the dataset items can not be removed.
        */
        virtual void remove(const std::string& datasetName, const ObjectIdSet* oids) = 0;

        /*!
          \brief It adds more data items to the dataset in the data source.

          \param datasetName The target dataset name.
          \param d           The data items to be added to the dataset.
          \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.
          
          \pre All parameters must be valid pointers.

          \exception Exception It throws an exception if the data items can not be added.

          \note It is the caller responsability to release the dataset 'd' pointer.

          \note DataSetPersistence will start reading the dataset 'd' in the
                current position. So, keep in mind that it is the caller responsability
                to inform the dataset 'd' in the right position (and a valid one) to start processing it.
        */
        virtual void add(const std::string& datasetName, DataSet* d, std::size_t limit = 0);

        /*!
          \brief It adds more data items to the dataset in the data source.

          \param datasetName The target dataset name.
          \param d           The data items to be added to the dataset.
          \param options     A list of optional modifiers. It is driver specific.
          \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.

          \pre All parameters must be valid pointers.

          \exception Exception It throws an exception if the data items can not be added.

          \note It is the caller responsability to release the dataset 'd' pointer.

          \note DataSetPersistence will start reading the dataset 'd' in the
                current position. So, keep in mind that it is the caller responsability
                to inform the dataset 'd' in the right position (and a valid one) to start processing it.
        */
        virtual void add(const std::string& datasetName, DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit = 0) = 0;

        /*!
          \brief It updates the dataset items in the data source.

          It updates a dataset in the
          data source based on OID list.

          \param datasetName The target dataset name.
          \param dataset     The list of data items to be updated.
          \param properties  The list of properties of the dataset to be updated.
          \param oids        The list of objects to be updated.
          \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.

          \pre All parameters must be valid pointers.

          \exception Exception It throws an exception if the dataset can not be updated.

          \note It is the caller responsability to release the dataset pointer.

          \note DataSetPersistence will start reading the dataset 'd' in the
                current position. So, keep in mind that it is the caller responsability
                to inform the dataset 'd' in the right position (and a valid one) to start processing it.
        */
        virtual void update(const std::string& datasetName,
                            DataSet* dataset,
                            const std::vector<std::size_t>& properties,
                            const ObjectIdSet* oids,
                            const std::map<std::string, std::string>& options,
                            std::size_t limit = 0) = 0;

        //@}
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETPERSISTENCE_H

