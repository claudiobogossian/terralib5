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
  \file terralib/dataaccess/dataset/DataSetTypePersistence.h

  \brief An abstract class responsible for persisting a dataset type definition in a data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCE_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCE_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declarations
  namespace dt { class Property; }

  namespace da
  {
// Forward declarations 
    class DataSetType;
    class CheckConstraint;
    class ForeignKey;
    class Index;
    class DataSetTypePersistence;
    class PrimaryKey;
    class Sequence;
    class UniqueKey;

    /*!
      \class DataSetTypePersistence

      \brief An abstract class responsible for persisting a dataset type definition in a data source.

      This is an abstract class that persists a DataSetType in a data source.
      Each data source must implement this abstract class, considering its particular
      persistence support.

      The methods of this class realize DataSetType changes into a data source. 
      During this process, the DataSetType in memory is updated from the data source DataSetType.
      This class does not ensure the synchronism between the data source catalog in memory and
      the data source. A data source catalog will be updated only if 
      the DataSetType pointer is associated to a data source catalog. Otherwise, 
      this class does not update the data source catalog. 

      \sa DataSetType, te::dt::Property
    */
    class TEDATAACCESSEXPORT DataSetTypePersistence : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        DataSetTypePersistence() {}

        /*! \brief Virtual destructor. */
        virtual ~DataSetTypePersistence() {}

        /** @name Pure Virtual Methods
         *  Methods that subclasses must implement in order to support the persistence interface.
         */
        //@{

        /*!
          \brief It creates the DataSetType definition in the target data source.

          The informed DataSetType will be created in the target data source.

          If a dataset type with the same name already exists in the target data source this may
          throws an exception.

          After calling this method, the dataset type will receive a new identification. If
          it is associated to a catalog, the method will automatically adjust its entry in the catalog.

          If the dataset type is not associated to the catalog, the method does NOT add it to the target data source catalog.

          If you want to create a DataSetType in a data source with the same data
          of an existing DataSetType from another data source, first of all,
          you must create a fresh copy of the DataSetType (see the method clone()
          in the DataSetType class).

          \param dt      The DataSetType to be created. It may be changed if the underlying data source
                         creates new dataset type items or if it doesn't have support for some of them.

          \pre All parameters must be valid pointers.
          \pre The DataSetTypes referenced by foreign keys must be already in the data source catalog.

          \post In some data sources this method may outputs implicit indexes, sequences or constraints.
                The method, if necessary, will create and adjust the dataset type.

          \exception Exception It throws an exception if the DataSetType cannot be created.
        */
        virtual void create(DataSetType* dt);

        /*!
          \brief It creates the DataSetType definition in the target data source.

          The informed DataSetType will be created in the target data source.

          If a dataset type with the same name already exists in the target data source this may
          throws an exception.

          After calling this method, the dataset type will receive a new identification. If
          it is associated to a catalog, the method will automatically adjust its entry in the catalog.

          If the dataset type is not associated to the catalog, the method does NOT add it to the target data source catalog.

          If you want to create a DataSetType in a data source with the same data
          of an existing DataSetType from another data source, first of all,
          you must create a fresh copy of the DataSetType (see the method clone()
          in the DataSetType class).

          \param dt      The DataSetType to be created. It may be changed if the underlying data source
                         creates new dataset type items or if it doesn't have support for some of them.
          \param options A list of optional modifiers. It is driver specific.

          \pre All parameters must be valid pointers.
          \pre The DataSetTypes referenced by foreign keys must be already in the data source catalog.

          \post In some data sources this method may outputs implicit indexes, sequences or constraints.
                The method, if necessary, will create and adjust the dataset type.

          \exception Exception It throws an exception if the DataSetType cannot be created.
        */
        virtual void create(DataSetType* dt, const std::map<std::string, std::string>& options) = 0;

        /*!
          \brief It clones the dataset in the data source.

          \param datasetName    The dataset to be cloned.
          \param newDataSetName The name of the cloned dataset.
          \param options        A list of optional modifiers. It is driver specific.
        */
        virtual void clone(const std::string& datasetName,
                           const std::string& newDatasetName,
                           const std::map<std::string, std::string>& options) = 0;

        /*!
          \brief It drops the DataSetType from the data source.

          The DataSetType will be removed from the data source associated to it.

          If the DataSetType is associated to a DataSourceCatalog,
          this method will automatically remove it from the catalog. In this case,
          this method can propagate changes to the catalog, for example, removing associated 
          sequences and foreign keys. Otherwise, the data source catalog is not updated.

          \param dt The DataSetType to be removed from data source.

          \post If the DataSetType is associated to a data source catalog, it will automatically remove 
                the given DataSetType from the data source catalog. Otherwise, the data source catalog 
                is not updated.
          \post The given DataSetType pointer will be invalidated as soon as the DataSetType is
                removed from its data source. 
          \post If the DataSetType is associated to a data source catalog, this may cause changes 
                propagation through the data source catalog objects: changing in sequences, foreign
                keys and other stuffs. 

          \exception Exception It throws an exception if the DataSetType can not be removed.
        */
        virtual void drop(const std::string& datasetName) = 0;

        /*!
          \brief It renames the DataSetType.

          The DataSetType will be renamed in the data source associated to it.

          If the DataSetType is associated to a data source catalog, this method can cause changes 
          in the catalog.

          \param dt      The DataSetType to be renamed.
          \param newName The new DataSetType name.

          \pre The DataSetType must be a valid pointer.

          \post If the DataSetType is associated to a data source catalog, 
                it will be renamed in the associate data source catalog. In this case, 
                this method can cause a cascade propagation in the data source catalog.
                Otherwise, the data source catalog will be not updated.
          \post The given DataSetType pointer will receive the new name.
          
          \exception Exception It throws an exception if the DataSetType can not be renamed.
        */
        virtual void rename(DataSetType* dt, const std::string& newName) = 0;
   
        /*!
          \brief It adds a new property to the DataSetType.

          It will create the given property in the DataSetType data source and associate
          it to the DataSetType.          

          If you want to add a property in a DataSetType with the same data
          of an existing one from another DataSetType, first of all,
          you must create a fresh copy of the property (see the method clone()
          in the Property class). After that, the property will be added to the
          DataSetType in memory.

          \param dt The DataSetType where the Property will be added.
          \param p  The new Property to be added.

          \pre The parameters must be valid pointers.

          \post This method can cause a cascade propagation in the data source catalog. 

          \exception Exception It throws an exception if the Property can not add to the DataSetType.
        */
        virtual void add(DataSetType* dt, te::dt::Property* p) = 0;

        /*!
          \brief It removes a property.

          This method will also remove every object
          associated to the property, such as, unique key, primary
          key and foreign key.

          The property must be associated to a DataSetType before calling this method.

          \param p The property to be removed from the DataSetType.

          \pre The parameters must be valid pointers.

          \post After being removed, the property pointer will be invalidaded.
          \post This method can cause a cascade propagation in the data source catalog and in the DataSetType: changing in constraints and other stuffs.

          \exception Exception It throws an exception if the property can not be removed.
          \exception Exception It throws an exception if the property is not associated to a DataSetType.
        */
        virtual void drop(te::dt::Property* p) = 0;

        /*!
          \brief It renames the property.

          In order to rename a property, it must belong to a DataSetType.
          If the operation is successful, 
          the property in memory will have its name automatically changed in the dataset type.

          \param p       The property to be renamed.
          \param newName The new property name.

          \pre The property must be a valid pointer. 

          \post This method can cause a cascade propagation in the DataSetType and associate objects: changing in constraints and other stuffs.

          \exception Exception It throws an exception if the property can not be renamed.
          \exception Exception It throws an exception if the property is not associated to a DataSetType.
        */
        virtual void rename(te::dt::Property* p, const std::string& newName) = 0; 

        /*!
          \brief It updates a property.

          It will update an old property based on a new property information.
          Therefore, the old property must belong to a valid DataSetType and 
          its DataSetType must belong to a data source. The old property will become
          invalid and the new one will take its place in the associated DataSetType.

          \param oldP The property to be changed.
          \param newP The property with the new information.

          \exception Exception It throws an exception if the DataSetType cannot be updated.
          \exception Exception It throws an exception if the property is not associated to a DataSetType.

          \note The parameters must be valid pointers. 

          \note The newP must be not associated to another object before calling this method.

          \note If the operation was successful, the new property will be associated to the same DataSetType
                of the oldP, and the oldP pointer will be invalidated.

          \warning This method can cause a cascade propagation in the data source catalog: changing in constraints and other stuffs.
        */
        virtual void update(te::dt::Property* oldP, te::dt::Property* newP) = 0;
        
        /*!
          \brief It adds a primary key constraint in the DataSetType.

          The DataSetType must be associated to a data source.
          After adding the given primary key in the data source, 
          it will be associated to the DataSetType in memory.

          \param dt  The DataSetType where the primary key will be added.
          \param pk  The primary key constraint.

          \pre The parameters must be valid pointers.           
          \pre The property that will be updated to primary key must already exist in data source. 
          
          \post This method can cause a cascade propagation in the DataSetType indexes.

          \exception Exception It throws an exception if the primary key cannot be added to the DataSetType.
        */
        virtual void add(DataSetType* dt, PrimaryKey* pk) = 0; 
        
        /*!
          \brief It removes the primary key constraint from the DataSetType which it is associated.

          The given primary key constraint must belong to a DataSetType and its DataSetType must be associated
          to a data souce. After removing the given primary key from the data source, 
          it will be removed from its DataSetType in memory.

          \param pk The primary key constraint that will be removed.

          \pre The primary key must be a valid pointer. 

          \post After being removed, the primary key pointer will be invalidaded.
          \post This method can cause a cascade propagation in the DataSetType indexes.

          \exception Exception It throws an exception if the primary key cannot be removed from its DataSetType.
        */
        virtual void drop(PrimaryKey* pk) = 0;
        
        /*!
          \brief It adds a unique key constraint to the DataSetType.

          The DataSetType must be associated to a data source.
          After adding the given unique key in the data source, 
          it will be associated to the DataSetType in memory.

          \param dt The DataSetType where the unique key will be added.
          \param uk The unique key constraint.

          \pre The parameters must be valid pointers.

          \post This method can cause a cascade propagation in the DataSetType indexes.

          \exception Exception It throws an exception if the unique key cannot be added to the DataSetType.
        */
        virtual void add(DataSetType* dt, UniqueKey* uk) = 0; 
        
        /*!
          \brief It removes the unique key constraint from the DataSetType which it is associated.

          The given unique key constraint must belong to a DataSetType and its DataSetType must be associated
          to a data souce. After removing the given unique key from the data source, 
          it will be removed from its DataSetType in memory.

          \param uk The unique key constraint.

          \post After being removed, the unique key pointer will be invalidaded.
          \post This method can cause a cascade propagation in the DataSetType indexes.

          \pre The parameter must be a valid pointer. 

          \exception Exception It throws an exception if the unique key cannot be removed from its DataSetType.
        */
        virtual void drop(UniqueKey* uk) = 0;

        /*!
          \brief It adds an index in a DataSetType.

          The DataSetType must be associated to a data source, and the given index
          must not belong to any DataSetType. After adding the given index in the data source, 
          it will be associated to the DataSetType in memory.

          \param dt       The DataSetType where the index will be added.
          \param index    The index to be added.

          \pre The parameters must be valid pointers. 

          \post It will automatically attach the index to the DataSetType.

          \exception Exception It throws an exception if the index cannot be added to the DataSetType.
        */
        virtual void add(DataSetType* dt, Index* index); 
        
        /*!
          \brief It adds an index in a DataSetType.

          The DataSetType must be associated to a data source, and the given index
          must not belong to any DataSetType. After adding the given index in the data source, 
          it will be associated to the DataSetType in memory.

          \param dt       The DataSetType where the index will be added.
          \param index    The index to be added.
          \param options  A list of optional modifiers. It is driver specific.

          \pre The parameters must be valid pointers. 

          \post It will automatically attach the index to the DataSetType.

          \exception Exception It throws an exception if the index cannot be added to the DataSetType.
        */
        virtual void add(DataSetType* dt, Index* index, const std::map<std::string, std::string>& options) = 0; 
        
        /*!
          \brief It removes the index from the DataSetType which it is associated.

          The given index must belong to a DataSetType and its DataSetType must be associated
          to a data souce. After removing the index from the data source, 
          it will be removed from its DataSetType in memory.

          \param index The index associated to a DataSetType.

          \pre The index must be a valid pointer. 

          \post After being removed, the index pointer will be invalidaded.
          \post The changes can propagate to associated primary key or unique key.

          \exception Exception It throws an exception if the index cannot be removed from its DataSetType.
        */
        virtual void drop(Index* index) = 0;

        /*!
          \brief It adds a foreign key constraint to a DataSetType.

          After adding the given foreign key in the data source, 
          it will be associated to the DataSetType in memory. 

          \param dt  The DataSetType where the foreign key will be added.
          \param fk      The foreign key constraint.

          \pre The parameters must be valid pointers. 

          \post It will automatically attach the fk to the DataSetType.
          \post This may propagate changes to the DataSourceCatalog associated to the persistence.

          \exception Exception It throws an exception if the foreign key cannot be added to the DataSetType.
        */
        virtual void add(DataSetType* dt, ForeignKey* fk) = 0;

        /*!
          \brief It removes the foreign key constraint from the DataSetType which it is associated.

          The given foreign key constraint must belong to a DataSetType and its DataSetType must be associated
          to a data souce. After removing the foreign key constraint from the data source, 
          it will be removed from its DataSetType in memory.

          \param fk The foreign key constraint that will be removed.

          \pre The parameter must be a valid pointer. 

          \post It will automatically remove the fk from the DataSetType.
          \post After being removed, the foreign key pointer will be invalidaded.
          \post This may propagate changes to the DataSourceCatalog associated to the persistence.

          \exception Exception It throws an exception if the foreign key cannot be removed from its DataSetType.          
        */
        virtual void drop(ForeignKey* fk) = 0;

        /*!
          \brief It adds a check constraint to the DataSetType.

          The DataSetType must be associated to a data source.
          After adding the given check constraint in the data source, 
          it will be associated to the DataSetType in memory.

          \param dt The DataSetType where the check constraint will be added.
          \param cc The check constraint.

          \pre The parameters must be valid pointers. 

          \post It will automatically attach the cc to the DataSetType.

          \exception Exception It throws an exception if the check constraint cannot be added to the DataSetType.
        */
        virtual void add(DataSetType* dt, CheckConstraint* cc) = 0;
   
        /*!
          \brief It removes the check constraint from the DataSetType which it is associated..

          The given check constraint must belong to a DataSetType and its DataSetType must be associated
          to a data souce. After removing the check constraint from the data source, 
          it will be removed from its DataSetType in memory.
          
          The given check constraint may belong to a DataSetType. 

          \param cc The check constraint that will be removed.

          \exception Exception It throws an exception if the check constraint cannot be removed from its DataSetType.

          \note The check constraint must be a valid pointer. 

          \note After being removed, the check constraint pointer will be invalidaded.
        */
        virtual void drop(CheckConstraint* cc) = 0;
   
        /*!
          \brief It creates a new sequence in the data source.

          After calling this method, the given sequence will receive a new identification. If
          it is associated to a data source catalog, the method will automatically 
          adjust its entry in the catalog.
          
          \param sequence The sequence that will be created in the data source.

          \pre The parameters must be valid pointers.

          \exception Exception It throws an exception if the sequence cannot be created to the data source.
        */
        virtual void create(Sequence* sequence) = 0;
   
        /*!
          \brief It removes the sequence from the data source.
          
          If the sequence is associated to a data source catalog, it will
          be removed from the catalog. 

          \param sequence The sequence that will be removed.

          \pre The parameter must be a valid pointer. 

          \post After being removed, the sequence pointer will be invalidaded.
          \post If it is associated to a data source catalog, this may propagate changes to it.

          \exception Exception It throws an exception if the sequence cannot be removed from the data source.
        */
        virtual void drop(Sequence* sequence) = 0;

        //@}
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCE_H

