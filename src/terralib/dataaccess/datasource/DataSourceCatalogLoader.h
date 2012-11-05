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
  \file terralib/dataaccess/datasource/DataSourceCatalogLoader.h

  \brief It allows to retrieve information about datasets.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOGLOADER_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOGLOADER_H

// TerraLib
#include "../Config.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
// Forward declarations
  namespace dt
  {
    class Property;
  }

  namespace gm
  {
    class Envelope;
    class GeometryProperty;
  }

  namespace da
  {
// Forward declarations
    class CheckConstraint;
    class DataSetType;    
    class DataSourceTransactor;    
    class ForeignKey;    
    class Index;
    class PrimaryKey;
    class Sequence;
    class UniqueKey;

    /*!
      \class DataSourceCatalogLoader

      \brief It allows to retrieve information about datasets.

      \note All implementaions must assure thread-safety for all methods.

      \sa DataSourceTransactor, DataSetType 
    */
    class TEDATAACCESSEXPORT DataSourceCatalogLoader : public boost::noncopyable 
    {
      public:

        /*!
          \brief It searches for the list of datasets available in the data source.

          \param datasets A vector of string pointers to output the names of the datasets available in the data source.

          \exception Exception It throws an exception if something goes wrong during data source search.

          \note Each dataset in the data source must have a unique name. For example, in a DBMS the name
                may contains the schema name before the table name separated by a dot notation (".").

          \note The caller of this method will take the ownership of the strings in the output vector.
        */
        virtual void getDataSets(boost::ptr_vector<std::string>& datasets) = 0;

        /*!
          \brief It searches for information about a given dataset in the data source.

          Using this method you can get more information about a dataset:
          <ul>
          <li>the list of properties, including: name, data type, size, if the value is required or not, if it is an autoincrement</li>
          <li>primary key</li>
          <li>foreign keys</li>
          <li>unique keys</li>
          <li>check constraints</li>
          <li>indexes</li>
          </ul>

          \param datasetName The name of the dataset we are looking information for.
          \param full        If true it will try to retrieve the maximum information about the dataset in the data source.

          \return The dataset schema. The caller of this smethod will take the ownership of the returned pointer.

          \exception Exception It throws an exception if something goes wrong during data source search.

          \note No sequence information is retrieved for autoincrement properties.
          \note Even if parameter <i>full</i> is true the bounding box of the dataset will not be loaded and
                if you want that information you should call getExtent method.
        */
        virtual DataSetType* getDataSetType(const std::string& datasetName, const bool full = false) = 0;

        /*!
          \brief It retrieves the properties of the dataset.

          \param dt The dataset we are looking for the properties.

          \exception Exception It throws an exception if something goes wrong during data source search.

          \post The dataset will contain information only about properties (no pk, no uk nor check-constraints).
        */
        virtual void getProperties(DataSetType* dt) = 0;

        /*!
          \brief It retrieves a property from a dataset.

          \param datasetName  The name of a dataset.
          \param propertyName The name of the property.

          \return The property information. The caller will take the property ownership.

          \exception Exception It throws an exception if something goes wrong during data source search.
        */
        virtual te::dt::Property* getProperty(const std::string& datasetName, const std::string& propertyName) = 0;

        /*!
          \brief It retrieves the dataset's primary key.

          \param dt The dataset we are looking for the primary key information.

          \exception Exception It throws an exception if something goes wrong during data source search.

          \post The primary key may not have an associated index.
          \post The found primary key is stored in the dataset type.
        */
        virtual void getPrimaryKey(DataSetType* dt) = 0;

        /*!
          \brief It searches in the data source for unique keys associated to the given dataset.

          \param dt  The dataset we are looking unique keys information for.

          \exception Exception It throws an exception if something goes wrong during data source search.

          \pre The dataset must be retrieved by getDataSetType or loadDataSetType before calling this method.

          \post Unique keys may not have an associated index.
          \post Any found unique keys will be added to the dataset type.
        */
        virtual void getUniqueKeys(DataSetType* dt) = 0;

        /*!
          \brief It searches for foreign keys defined over the informed dataset dt.

          \param dt      The dataset we are looking foreign keys information for.
          \param fkNames The list of foreign key names associated to dt.
          \param rdts    The list of related tables to dt. 

          \exception Exception It throws an exception if something goes wrong during data source search.
        */
        virtual void getForeignKeys(DataSetType* dt, std::vector<std::string>& fkNames, std::vector<std::string>& rdts) = 0;

        /*!
          \brief It retrieves the foreign key defined over dt that is related to rdt.

          \param fkName The foreign key name relating dt to rdt.
          \param dt     The dataset that has the fk.
          \param rdt    The dataset related to dt (probably the one with a primary key).

          \return The foreign key relating the two datasets or NULL if none exists. The caller will take the ownership of the returned fk.

          \exception Exception It throws an exception if something goes wrong during data source search.
        */
        virtual ForeignKey* getForeignKey(const std::string& fkName, DataSetType* dt, DataSetType* rdt) = 0;

        /*!
          \brief It searches in the data source for indexes associated to the given dataset.

          \param dt  The dataset we are looking indexes information for.

          \exception Exception It throws an exception if something goes wrong during data source search.

          \post The indexes will have a dataset owner.
          \post Any found indexes will be added to the dataset type.
        */
        virtual void getIndexes(DataSetType* dt) = 0;

        /*!
          \brief It searches in the data source for check constraints associated to the given dataset.

          \param dt  The dataset we are looking checking-constraints information for.

          \exception Exception It throws an exception if something goes wrong during data source search.
         */
        virtual void getCheckConstraints(DataSetType* dt) = 0;

        /*!
          \brief It searches for the list of sequences available in the data source.

          \param sequences A vector of string pointers to output the names of the sequences available in the data source.

          \exception Exception It throws an exception if something goes wrong during data source search.

          \note Each sequence in the data source must have a unique name. For example, in a DBMS the name
                may contains the schema name before the sequnce name separated by a dot notation (".").

          \note The caller of this method will take the ownership of the strings in the output vector.
        */
        virtual void getSequences(std::vector<std::string*>& sequences) = 0;

        /*!
          \brief It searches information about the given sequence in the data source.

          \param seqName  The sequence we are looking information for.

          \return The sequence with the given name. The caller will take its ownership.

          \exception Exception It throws an exception if something goes wrong during data source search.
        */
        virtual Sequence* getSequence(const std::string& seqName) = 0;

        /*!
          \brief It retrieves the bounding rectangle for the given geometric property.

          \param gp The geometric property.

          \return The geometric property bounding rectangle or NULL if none is found.

          \pre The geometric property must be associated to a dataset type.

          \post The caller of this method will take the ownership of the returned box.

          \exception Exception It throws an exception if something goes wrong during MBR search.
        */
        virtual te::gm::Envelope* getExtent(const te::gm::GeometryProperty* gp) = 0;

        /*!
          \brief It loads information about the datasets available in the data source and stores their information in the data source catalog.

          Using this method each dataset type in the catalog may have information about its name,
          primary key, foreign keys, unique keys, check constraints, sequences and indexes.
          It may also take full information about their properties,
          such as name, size, type, if a value is required, and if a value is unique.

          \param full If true it will try to load the maximum information about the datasets in the data source.

          \exception Exception It throws an exception if something goes wrong during catalog load.

          \note Calling this method will refresh any previously loaded catalog information.
        */
        virtual void loadCatalog(const bool full = false) = 0;

        /*!
          \brief It return true if the data source has any dataset.

          \return True if the data source has datasets otherwise, false.
        */
        virtual bool hasDataSets() = 0;

        /*!
          \brief It checks if a dataset with the given name already exists in the data source.

          \param name The name of the dataset to check.

          \return True if the dataset exists in the data source otherwise false.

          \exception Exception It throws an exception if something goes wrong.
         */
        virtual bool datasetExists(const std::string& name) = 0;

        /*!
          \brief It checks if a primary key with the given name already exists in the data source.

          \param name The name of the primary key to check.

          \return True if the primary key exists in the data source otherwise false.

          \exception Exception It throws an exception if something goes wrong.
        */
        virtual bool primarykeyExists(const std::string& name) = 0;

        /*!
          \brief It checks if a unique key with the given name already exists in the data source.

          \param name The name of the primary key to check.

          \return True if the unique key exists in the data source otherwise false.

          \exception Exception It throws an exception if something goes wrong.
        */
        virtual bool uniquekeyExists(const std::string& name) = 0;

        /*!
          \brief It checks if a foreign key with the given name already exists in the data source.

          \param name The name of the foreign key to check.

          \return True if the foreign key exists in the data source otherwise false.

          \exception Exception It throws an exception if something goes wrong.
        */
        virtual bool foreignkeyExists(const std::string& name) = 0;

        /*!
          \brief It checks if a check-constraint with the given name already exists in the data source.

          \param name The name of the check-constraint to check.

          \return True if the check-constraint exists in the data source otherwise false.

          \exception Exception It throws an exception if something goes wrong.
        */
        virtual bool checkConstraintExists(const std::string& name) = 0;

        /*!
          \brief It checks if an index with the given name already exists in the data source.

          \param name The name of the index to check.

          \return True if the index exists in the data source otherwise false.

          \exception Exception It throws an exception if something goes wrong.
        */
        virtual bool indexExists(const std::string& name) = 0;

        /*!
          \brief It checks if a sequence with the given name already exists in the data source.

          \param name The name of the sequence to check.

          \return True if the sequence exists in the data source otherwise false.

          \exception Exception It throws an exception if something goes wrong.
        */
        virtual bool sequenceExists(const std::string& name) = 0;

        /*!
          \brief It returns a pointer to the underlying data source transactor.

          \return A pointer to the underlying data source transactor.
        */
        virtual DataSourceTransactor* getTransactor() const = 0;

        /*! \brief Virtual destructor. */
        virtual ~DataSourceCatalogLoader() {}

      protected:

        /*! \brief Default constructor just can be called by subclasses. */
        DataSourceCatalogLoader() {}
    };

    typedef boost::shared_ptr<DataSourceCatalogLoader> DataSourceCatalogLoaderPtr;

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOGLOADER_H

