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
  \file terralib/dataaccess/datasource/DataSourceCatalog.h

  \brief It represents the system catalog of a DataSource.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOG_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOG_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>
#include <vector>

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
    */
    class TEDATAACCESSEXPORT DataSourceCatalog
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
        unsigned int getId() const { return m_id; }

        /*!
          \brief It sets the catalog identifier.

          \param id A number that can be used to identify the catalog in its DataSource.

          \warning This value will be used by data source driver implementeers. So, don't
                   rely on its value nor call it if you are not implementing a data source!
        */
        void setId(unsigned int id) { m_id = id; }

        /*!
          \brief It returns the DataSource associated to the catalog.

          \return The caller of this method will NOT take the ownership of the returned DataSource pointer.

          \note Every catalog that belongs to a DataSource will have an explicit association. If it has no parent, it doesn't belong to any store.
        */
        DataSource* getDataSource() const { return m_ds; }

        /*!
          \brief It sets the DataSource associated to the catalog.

          \param ds The DataSource to be associated to the catalog.

          \note Don't call this method in your code, it will be used by the driver developers!
        */
        void setDataSource(DataSource* ds) { m_ds = ds; }

        /*! \brief It clears the catalog, releasing all the resources used by it. */
        void clear();

        /*!
          \brief It creates a copy (clone) of the catalog.

          \return A copy of the catalog.

          \note The new catalog instance will not be associated to any DataSource.

          \note This method will preserve the identifier of the objects. Bu if you
                persist them into a DataSource their values can be changed by the persistence implementation.
        */
        DataSourceCatalog* clone() const;

        //@}

        /** @name DataSetType Management Methods
         *  Methods for managing DataSetTypes.
         */
        //@{

        /*!
          \brief It returns the number of datasets in the catalog.

          \return The number of datasets in the catalog.
        */
        std::size_t getNumberOfDataSets() const { return m_dsets.size(); }        

        /*!
          \brief It adds a new DataSetType to the catalog.

          \param dt The DataSetType to be added to the list of DataSetTypes of the catalog.

          \pre The dt must not be associated to another catalog before calling this method.
          \pre There is not a DataSetType in the catalog with the same name as the DataSetType to be added.
          \pre There is not a DataSetType in the catalog with the same id as the DataSetType to be added.
          \pre If the DataSetType has foreign keys the referenced DataSetTypes must be already in the catalog.

          \post The DataSetType will have the catalog associated to it.
          \post The catalog will take the ownership of the informed DataSetType.
          \post The catalog will keep track of foreign keys and the referenced DataSetTypes.

          \exception Exception It throws an exception if the DataSetType can not be added.
          \exception Exception It throws an exception if the DataSetType has foreign keys and the referenced DataSetTypes is not already in the catalog.
        */
        void add(DataSetType* dt);

        /*!
          \brief It adds the list of DataSetTypes to the catalog.

          \param dsets The list of DataSetTypes to be added to the catalog.

          \pre The DataSetTypes to be added must not be associated to a catalog before calling this method.
               In other words, don't inform a DataSetType belonging to a catalog.
          \pre There is not a DataSetType in the catalog with the same name as the DataSetTypes to be added.
          \pre There is not a DataSetType in the catalog with the same id as the DataSetTypes to be added.
          \pre If any DataSetType has foreign keys, the referenced DataSetTypes must be already in the catalog. So, the list order of DataSetType entries is very important.

          \post The catalog will take the ownership of the informed DataSetTypes.
          \post The DataSetTypes will have the catalog associated to it.
          \post The catalog will keep track of foreign keys and the referenced DataSetTypes.

          \exception Exception It throws an exception if a DataSetType can not be added.
          \exception Exception It throws an exception if any DataSetType has foreign keys and the referenced DataSetTypes is not already in the catalog.
        */
        void add(const std::vector<DataSetType*>& dsets);

        /*!
          \brief It removes the DataSetType from the catalog.

          \param dt      The DataSetType to be removed from the catalog.
          \param cascade If true the method will remove also all objects that depends on the DataSetType (like sequences and foreign keys).

          \pre The DataSetType must associated to the catalog.
          
          \post It will drop any association to the DataSetType or to the DataSetType attributes (like sequences and foreign keys) if casacade is true.
          \post Any association of the DataSetType with foreign keys will also be removed.
          \post The dt pointer will be invalidated.
        */
        void remove(DataSetType* dt, const bool cascade = false);

        /*!
          \brief It will detaches the DataSetType from the catalog.

          \param dt The DataSetType to be detached from the catalog.

          \pre The DataSetType must associated to the catalog.
          
          \post It will drop any association to the DataSetType or to the DataSetType attributes (like sequences and foreign keys).
          \post The caller of this method will take the ownership of dt pointer.
        */
        void detach(DataSetType* dt, const bool cascade = false);

        /*!
          \brief It renames the DataSetType in the catalog.

          \param dt      The DataSetType to be renamed.
          \param newName The new name to be assigned to the DataSetType.

          \pre The DataSetType must associated to the catalog.
          \pre There is not a DataSetType in the catalog with the same value as newName.

          \post All internal indexes pointing to dt will be fixed and the dt will have newName set.

          \exception Exception It throws an exception if there is already
                               a DataSetType with the new name in the catalog, or if the DataSetType
                               doesn't belong to the catalog.
        */
        void rename(DataSetType* dt, const std::string& newName);

        /*!
          \brief It changes the DataSetType id in the catalog.

          \param dt    The DataSetType to change its id.
          \param newId The new id for the DataSetType.

          \pre The DataSetType must associated to the catalog.
          \pre There is not a DataSetType in the catalog with the same value as newId.

          \post All internal indexes pointing to dt will be fixed and the dt will have newId set.

          \exception Exception It throws an exception if there is already
                               a DataSetType with the new id in the catalog, or if the DataSetType
                               doesn't belong to the catalog.
        */
        void setId(DataSetType* dt, unsigned int newId);

        /*!
          \brief It returns the i-th DataSetType.

          \param i The DataSetType index.

          \return The i-th DataSetType.

          \pre i must be in the range of [0, getNumberOfDataSets()).

          \note The caller will not take the ownership of the returned DataSetType.
          \note It doesn't check index range.
        */
        DataSetType* getDataSetType(std::size_t i) const { return m_dsets[i]; }

        /*!
          \brief It searches for a DataSetType with the given name in the catalog.

          \param name The name of the searched DataSetType.

          \return A DataSetType with the given name, or NULL if none is found.

          \note The caller will not take the ownership of the returned DataSetType.
        */
        DataSetType* getDataSetType(const std::string& name) const;

        /*!
          \brief It returns the DataSetType with the given identifier.          

          \param id The DataSetType identifier.

          \return A DataSetType with the given id, or NULL if none is found.

          \note The caller will not take the ownership of the returned DataSetType.
        */
        DataSetType* getDataSetTypeById(std::size_t id) const;

        /*!
          \brief It returns the DataSetType position in the internal conteiner.

          \param dt The DataSetType we are looking its position in internal conteiner.

          \return The DataSetType position.

          \note If the DataSetType position is unknown this method return npos.
         */
        std::size_t getDataSetTypePos(DataSetType* dt) const;        

        //@}        

        /** @name Sequence Management Methods
         *  Methods for managing Sequences.
         */
        //@{

        /*!
          \brief It returns the number of sequences in the catalog.

          \return The number of sequences in the catalog.
        */
        std::size_t getNumberOfSequences() const { return m_sequences.size(); }

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
          \brief It adds a list of sequences to the catalog.

          \param sqs The list of sequences to be added to the catalog.

          \pre The sequences to be added must not be associated to a catalog before calling this method.
               In other words, don't inform a sequence belonging to a catalog.
          \pre There is not a sequence in the catalog with the same name as the sequences to be added.
          \pre There is not a sequence in the catalog with the same id as the sequences to be added.

          \post The catalog will take the ownership of the informed sequences.
          \post The sequences will have the catalog associated to it.

          \exception Exception It throws an exception if a sequence can not be added.
        */
        void add(const std::vector<Sequence*>& sqs);

        /*!
          \brief It removes the sequence from the catalog.

          \param s The sequence to be removed from the catalog.

          \pre The sequence must associated to the catalog.
          
          \post The s pointer will be invalidated.
        */
        void remove(Sequence* s);

        /*!
          \brief It changes the DataSetTypeSequnce id in the catalog.

          \param s     The DataSetTypeSequnce to change its id.
          \param newId The new id for the Sequence.

          \pre The Sequence must associated to the catalog.
          \pre There is not a Sequence in the catalog with the same value as newId.

          \post All internal indexes pointing to s will be fixed and the s will have newId set.

          \exception Exception It throws an exception if there is already
                               a Sequence with the new id in the catalog, or if the Sequence
                               doesn't belong to the catalog.
        */
        void setId(Sequence* s, unsigned int newId);

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
        Sequence* getSequence(std::size_t i) const { return m_sequences[i]; }

        /*!
          \brief It searches for a Sequence with the given name in the catalog.

          \param name The name of the searched Sequence.

          \return A Sequence with the given name, or NULL if none is found.

          \note The caller will not take the ownership of the returned Sequence.
        */
        Sequence* getSequence(const std::string& name) const;
        
        /*!
          \brief It returns the Sequence with the given identifier.

          \param id The Sequence identifier.

          \return A Sequence with the given id, or NULL if none is found.

          \note The caller will not take the ownership of the returned Sequence.
         */
        Sequence* getSequenceById(std::size_t id) const;

        /*!
          \brief It returns the Sequence position in the internal conteiner.

          \param s The Sequence we are looking its position in internal conteiner.

          \return The Sequence position.

          \note If the Sequence position is unknown this method return npos.
        */
        std::size_t getSequencePos(Sequence* s) const;

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

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side instance.
        */
        DataSourceCatalog(const DataSourceCatalog& rhs);

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

        /*!
          \brief Assignment operator not allowed.

          \param rhs Right-hand-side instance.

          \return A reference to this object.
        */
        DataSourceCatalog& operator=(const DataSourceCatalog& rhs);

      private:

        unsigned int m_id;                                  //!< An identification number inside the DataSource.
        DataSource* m_ds;                                   //!< The DataSource associated to the Catalog.

// DataSetType objects
        std::vector<DataSetType*> m_dsets;                  //!< The list of DataSetTypes.
        std::map<std::size_t, std::size_t> m_dtIdIdx;       //!< A map : dt-id -> dt-pos.
        std::map<std::string, std::size_t> m_dtNameIdx;     //!< A map : dt-name -> dt-pos.

// Sequence objects
        std::vector<Sequence*> m_sequences;                     //!< The list of sequences.
        std::map<std::string, std::size_t> m_seqNameIdx;        //!< A map : sequence-name -> sequence-pos.
        std::map<std::size_t, std::size_t> m_seqIdIdx;          //!< A map : sequence-id -> sequence-pos.
        std::multimap<DataSetType*, std::size_t> m_seqFTIdx;    //!< A multimap : owner-dt -> sequence-pos.

// Foreign-key objects
        std::multimap<DataSetType*, ForeignKey*> m_dependentFkIdx;   //!< A multimap : datasettype -> fk. It tells for a DataSetType the list of fk referencing it.
   };

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOG_H


