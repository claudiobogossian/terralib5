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
  \file terralib/dataaccess/datasource/DataSourceCatalog.h

  \brief It represents the system catalog of a DataSource.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOG_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOG_H

// TerraLib
#include "../Config.h"
#include "../dataset/DataSetType.h"

// STL
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declarations
  namespace dt { class Property; }

  namespace da
  {
// Forward declarations

    class DataSetType;
    class ForeignKey;
    class Sequence;
    class DataSource;

    /*!
      \class DataSourceCatalog

      \brief It represents the system catalog of a DataSource.

      \sa DataSource, Property, DataSetType, Sequence

      \ingroup dataaccess
    */
    class TEDATAACCESSEXPORT DataSourceCatalog : public boost::noncopyable
    {
      public:

        /** @name Basic Methods
         *  Basic methods for a DataSourceCatalog. 
         */
        //@{

        /*! \brief It creates a new DataSourceCatalog. */
        DataSourceCatalog();

        /*! \brief Destructor. */
        ~DataSourceCatalog();

        /*!
          \brief It returns the catalog identifier.

          \return A number that may be used to identify the catalog.

          \warning This value will be used by data source driver implementeers. So, don't rely on its value!
        */
        unsigned int getId() const;

        /*!
          \brief It sets the catalog identifier.

          \param id A number that can be used to identify the catalog in its DataSource.

          \warning This value will be used by data source driver implementeers. So, don't
                   rely on its value nor call it if you are not implementing a data source!
        */
        void setId(unsigned int id);

        /*!
          \brief It returns the DataSource associated to the catalog.

          \return The caller of this method will NOT take the ownership of the returned DataSource pointer.

          \note Every catalog that belongs to a DataSource will have an explicit association. If it has no parent, it doesn't belong to any store.
        */
        DataSource* getDataSource() const;

        /*!
          \brief It sets the DataSource associated to the catalog.

          \param ds The DataSource to be associated to the catalog.

          \note Don't call this method in your code, it will be used by the driver developers!
        */
        void setDataSource(DataSource* ds);

        /*! \brief It clears the catalog, releasing all the resources used by it. */
        void clear();

        //@}

        /** @name DataSetType Management Methods
         *  Methods for managing DataSetTypes.
         */
        //@{

        /*!
          \brief It returns the number of datasets in the catalog.

          \return The number of datasets in the catalog.
        */
        std::size_t getNumberOfDataSets() const;

        /*!
          \brief It checks if a dataset schema with the given name is in the catalog.

          \param name The dataset schema name to be cheked in the catalog.

          \return True if a dataset with the given name exists in the catalog.
        */
        bool datasetExists(const std::string& name) const;

        /*!
          \brief It adds a new dataset schema to the catalog.

          \param dt The dataset schema to be added to the catalog.

          \pre The dt must not be associated to another catalog before calling this method.
          \pre There is not a dataset schema in the catalog with the same name as dt.
          \pre If the dataset schema has foreign keys the referenced schema must be already in the catalog.

          \post The dataset schema will be associated to the catalog.
          \post The catalog will keep track of foreign keys and the referenced dataset schema.

          \exception Exception It throws an exception if the dataset schema can not be added.
        */
        void add(const DataSetTypePtr& dt);

        /*!
          \brief It removes the dataset schema from the catalog.

          \param dt      The dataset schema to be removed from the catalog.
          \param cascade If true the method will remove also all objects that depends on the dataset schema (like sequences and foreign keys).

          \pre The dataset schema must be associated to the catalog.
          
          \post It will drop any association to the dataset schema or its attributes (like sequences and foreign keys) if casacade is true.
        */
        void remove(DataSetType* dt, const bool cascade = false);

        /*!
          \brief It renames the dataset schema in the catalog.

          \param dt      The dataset schema to be renamed.
          \param newName The new name to be assigned to dt.

          \pre The dataset schema must be associated to the catalog.
          \pre There is not a dataset schema in the catalog with the same value as newName.

          \post All internal indexes pointing to dt will be fixed and the dt will have newName set.

          \exception Exception It throws an exception if there is already
                               a dataset schema with the new name in the catalog, or if it
                               doesn't belong to the catalog.
        */
        void rename(DataSetType* dt, const std::string& newName);

        /*!
          \brief It returns the i-th dataset schema.

          \param i The dataset schema index.

          \return The i-th dataset schema.

          \pre i must be in the range: [0, getNumberOfDataSets()).

          \note It doesn't check index range.
        */
        const DataSetTypePtr& getDataSetType(std::size_t i) const;

        /*!
          \brief It searches for a dataset schema with the given name in the catalog.

          \param name The name of the searched dataset schema.

          \return A dataset schema with the given name or NULL if none is found.
        */
        const DataSetTypePtr& getDataSetType(const std::string& name) const;

        //@}

        /** @name Sequence Management Methods
         *  Methods for managing Sequences.
         */
        //@{

        /*!
          \brief It returns the number of sequences in the catalog.

          \return The number of sequences in the catalog.
        */
        std::size_t getNumberOfSequences() const;

        /*!
          \brief It adds a new sequence to the catalog.

          \param s The sequence to be added to the catalog.

          
          \pre The sequence must not be associated to a catalog before calling this method.
          \pre There is not a sequence in the catalog with the same name as the sequence to be added.
          \pre There is not a sequence in the catalog with the same id as the sequence to be added.
          \pre If the sequence is owned by a given DataSetType property, the DataSetType must already be in the catalog.

          \post The sequence will have the catalog associated to it.
          \post The catalog will take the ownership of the informed sequence.

          \exception Exception It throws an exception if the sequence can not be added.
          \exception Exception It throws an exception if the sequence is owned by a given DataSetType property and this DataSetType is not in the catalog.
        */
        void add(Sequence* s);

        /*!
          \brief It removes the sequence from the catalog.

          \param s The sequence to be removed from the catalog.

          \pre The sequence must associated to the catalog.
          
          \post The s pointer will be invalidated.
        */
        void remove(Sequence* s);

        /*!
          \brief It will detaches the Sequence from the catalog.

          \param s The Sequence to be detached from the catalog.

          \pre The Sequence must associated to the catalog.
          
          \post It will drop any association to the Sequence.
          \post The caller of this method will take the ownership of s pointer.
        */
        void detach(Sequence* s);

        /*!
          \brief It returns the i-th Sequence.

          \param i The Sequence index.

          \return The i-th Sequence.

          \pre i must be in the range of [0, getNumberOfSequences()).

          \note The caller will not take the ownership of the returned Sequence.
          \note It doesn't check the index range.
        */
        Sequence* getSequence(std::size_t i) const;

        /*!
          \brief It searches for a Sequence with the given name in the catalog.

          \param name The name of the searched Sequence.

          \return A Sequence with the given name, or NULL if none is found.

          \note The caller will not take the ownership of the returned Sequence.
        */
        Sequence* getSequence(const std::string& name) const;

        //@}

        /** @name Helper Methods
         *  Methods used to keep the associations among catalog's objects.
         */
        //@{

        /*!
          \brief It checks if the referenced DataSetType is in the catalog and associate the fk to it.

          \param fk The foreign key to keep the association.

          \pre The DataSetType referenced by the fk must be in the catalog.
          \pre The foreign key must not have been associated before.

          \post If the DataSetType referenced by the foreign key is in the catalog, it will have an association in the catalog.

          \exception It throws an exception if the referenced DataSetType is not in the DataSourceCatalog.
        */
        void addRef(ForeignKey* fk);

        /*!
          \brief It drops the reference between the foreign key and its referenced DataSetType.

          \param fk The foreign key to remove the association from the catalog.

          \pre The DataSetType referenced by the fk must be in the catalog.
          \pre The foreign key must have been associated before.

          \post The association between the DataSetType and the foreign key will be removed.

          \exception It throws an exception if the referenced DataSetType is not in the DataSourceCatalog.
        */
        void removeRef(ForeignKey* fk);

        /*!
          \brief It returns the list of fk referencing the given DataSetType in the catalog.

          \param dt The DataSetType we are searching for foreign keys referencing it.

          \return The list of fk referencing the given DataSetType as a range defined over a pair of iterators.

          \note In the returned iterator, the second component is the end special mark.
        */
        std::pair<std::multimap<DataSetType*, ForeignKey*>::const_iterator,
                  std::multimap<DataSetType*, ForeignKey*>::const_iterator>
        getRefFK(DataSetType* dt) const { return m_dependentFkIdx.equal_range(dt); }

        /*!
          \brief It drops Sequences that depends on the property.

          \param p A Property that may owns a sequence in the catalog.

          \post All Sequences referencing p will be dropped (released).
        */
        void dropDependentSequences(te::dt::Property* p);

        //@}

      protected:

        /** @name Helper Methods for Sequences
         *  Methods used in the internals of DataSourceCatalog to manage Sequences.
         */
        //@{

        /*!
          \brief It checks if the Sequence is owned by a DataSetType property and if that DataSetType is in the catalog.

          \param s The Sequence we are checking the dependency of a FetaureType.

          \exception Exception It throws an exception if the sequence is owned by a
                               FetaureType that is not in the catalog nor could be determined.
        */
        void checkSequenceDependency(Sequence* s) const;

        /*!
          \brief It looks if the sequence is owned by a given DataSetType property and insert this information into the sequence depency index.

          \param s The sequence we want to index owner information.

          \pre If the sequence is owned, the associated DataSetType is already in the catalog.

          \post If the sequence s is owned it will have an index entry to the referred DataSetType.

          \exception Exception It throws an exception if the sequence is owned by a
                               FetaureType that is not in the catalog nor could be determined.
        */
        void indexSequenceDependency(Sequence* s);

        /*!
          \brief It drops Sequences that depends on the DataSetType dt.

          \param dt A DataSetType that belongs to the catalog.

          \post All Sequences referencing dt will be dropped (released).
        */
        void dropDependentSequences(DataSetType* dt);

        /*!
          \brief It drops the Sequence entry in the DataSetType sequence dependency list.

          \param s A Sequence to be removed from the list of dependency.

          \post The entry pointing to the Sequence will be dropped (released).

          \exception Exception It throws an exception if the sequence is owned by a dt and it is not in the catalog.
        */
        void dropDependentSequenceEntry(Sequence* s);

        //@}

        /** @name Helper Methods for Foreign Keys
         *  Methods used in the internals of DataSourceCatalog to manage Foreign Keys.
         */
        //@{

        /*!
          \brief It checks if the DataSetTypes referenced by dt's foreign keys are in the catalog.

          \param dt The DataSetType we are checking the foreign key dependency is ok.

          \exception Exception It throws an exception if one of the foreign key
                               has a referenced FetaureType that is not in the catalog.
        */
        void checkFKsDependency(DataSetType* dt) const;

        /*!
          \brief It looks for foreign keys in the DataSetType dt and try to index then with respect to referenced DataSetTypes.

          \param dt The DataSetType whose foreign keys will be indexed.

          \pre All DataSetTypes referenced by dt foreign keys must be in the catalog.
          \pre The foreign keys of dt must not have be indexed before.

          \post If the DataSetType dt has foreign keys they will have an index entry to the referred DataSetType.
        */
        void indexFKs(DataSetType* dt);

        /*!
          \brief It drops foreign keys from other DataSetTypes that depends on the attributes in the given DataSetType dt.

          \param dt A DataSetType that belongs to the catalog.

          \post All DataSetTypes foreign keys referencing dt will be dropped (released).
        */
        void dropDependentFKs(DataSetType* dt);

        //@}

      private:

        struct dataset_name_cmp
        {
          typedef std::string result_type;

          result_type operator()(const DataSetTypePtr& dt) const;
        };

        typedef boost::multi_index::multi_index_container<
                  DataSetTypePtr,
                  boost::multi_index::indexed_by<
                    boost::multi_index::ordered_unique<dataset_name_cmp>,
                    boost::multi_index::random_access<>
                  >
                > dataset_idx_type;

        struct sequence_name_cmp
        {
          typedef std::string result_type;

          result_type operator()(const Sequence* const s) const;
        };

        typedef boost::multi_index::multi_index_container<
                  Sequence*,
                  boost::multi_index::indexed_by<
                    boost::multi_index::ordered_unique<sequence_name_cmp>,
                    boost::multi_index::random_access<>
                  >
                > sequence_idx_type;

// DataSetType objects
        dataset_idx_type m_datasets;

// Sequence objects
        sequence_idx_type m_sequences;

        std::multimap<DataSetType*, sequence_idx_type::iterator> m_seqFTIdx;    //!< A multimap : owner-dt -> sequence-it.

// Foreign-key objects
        std::multimap<DataSetType*, ForeignKey*> m_dependentFkIdx;   //!< A multimap : datasettype -> fk. It tells for a DataSetType the list of fk referencing it.

        unsigned int m_id;     //!< An identification number inside the DataSource.
        DataSource* m_ds;      //!< The DataSource associated to the Catalog.

        static DataSetTypePtr sm_nullds;
    };

    typedef boost::shared_ptr<DataSourceCatalog> DataSourceCatalogPtr;

    inline unsigned int DataSourceCatalog::getId() const
    {
      return m_id;
    }

    inline void DataSourceCatalog::setId(unsigned int id)
    {
      m_id = id;
    }

    inline DataSource* DataSourceCatalog::getDataSource() const
    {
      return m_ds;
    }

    inline void DataSourceCatalog::setDataSource(DataSource* ds)
    {
      m_ds = ds;
    }

    inline std::size_t DataSourceCatalog::getNumberOfDataSets() const
    {
      return m_datasets.size();
    } 

    inline std::size_t DataSourceCatalog::getNumberOfSequences() const
    {
      return m_sequences.size();
    }

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOG_H


