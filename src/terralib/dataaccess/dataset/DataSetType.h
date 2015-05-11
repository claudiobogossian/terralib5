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
  \file terralib/dataaccess/dataset/DataSetType.h

  \brief A class that models the description of a dataset.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPE_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPE_H

// TerraLib
#include "../../datatype/CompositeProperty.h"
#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
// Forward declarations 
  namespace da
  {
    class CheckConstraint;
    class Constraint;
    class DataSourceCatalog;
    class ForeignKey;
    class Index;
    class PrimaryKey;
    class Sequence;
    class UniqueKey;

    /*!
      \class DataSetType

      \brief A class that models the description of a dataset.

      A DataSetType may be used to describe the dataset schema.
      It lists the attributes of the dataset, including their names and types.

      The information about the properties (or attributes) of datasets are stored in instances of the 
      Property class. To a property it is associated a name, the data type
      and other metadata necessary
      to fully specify the type, such as the attribute size.
 
      \note This class sometimes are referred to as dataset schemas.

      \ingroup dataaccess

      \sa DataSet, te::dt::Property, te::dt::CompositeProperty, DataSourceCatalog
    */
    class TEDATAACCESSEXPORT DataSetType : public te::dt::CompositeProperty
    {
      public:

        using CompositeProperty::add;
        using CompositeProperty::getPropertyPosition;

        /** @name DataSetType Basic Methods
         *  Basic methods for a DataSetType.
         */
        //@{

        /*!
          \brief It creates a new DataSetType not associated to any DataSetType or Catalog.

          By default the DataSetType doesn't have a geometric property nor a default geometry property.

          \param name The DataSetType name.
          \param id   The DataSetType identifier.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        DataSetType(const std::string& name, unsigned int id = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy that would be used to copy from.

          \note This method is used by clone().
        */
        DataSetType(const DataSetType& rhs);

        /*! Destructor */
        ~DataSetType();

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        DataSetType& operator=(const DataSetType& rhs);


        //@}

        /** @name General Descriptive Methods
         *  Methods for obtainning information about a given dataset.
         */
        //@{

        /*!
          \brief A human descriptive title for the DataSetType.

          \return This value may be a concatenation of the DataSetType name or a more human readable text.
        */
        const std::string& getTitle() const { return m_title; }

        /*!
          \brief It sets a human descriptive title for the DataSetType.

          \param title This value can be used to display a human descriptive title for the DataSetType.
        */
        void setTitle(const std::string& title) { m_title = title; }

        /*!
          \brief It returns true if the DataSetType has at least one geometry property; otherwise, it returns false.

          \return True if the DataSetType has at least one geometry property; otherwise, it returns false.
        */
        bool hasGeom() const;

        /*!
          \brief It returns true if the DataSetType has at least one raster property; otherwise, it returns false.

          \return True if the DataSetType has at least one raster property; otherwise, it returns false.
        */
        bool hasRaster() const;

        /*!
          \brief It returns the catalog that owns the DataSetType.

          \return The catalog associated to the DataSetType.
        */
        DataSourceCatalog* getCatalog() const { return m_catalog; }

        /*!
          \brief It sets the DataSetType associated catalog.

          \param catalog The catalog associated to the DataSetType.

          \warning Take care when calling this method. If the DataSetType belongs to a DataSourceCatalog
                   remember to detach it from the catalog before calling this method.
        */
        void setCatalog(DataSourceCatalog* catalog) { m_catalog = catalog; }

        /*!
          \brief It returns the dataset category.

          Some drivers and applications can use this value to refine their functionalities.

          \return The dataset category.
        */
        int getCategory() const { return m_category; }

        /*!
          \brief It sets the dataset category.

          \param c The dataset category.
        */
        void setCategory(int c) { m_category = c; }

        //@}

        /** @name Primary Key Methods
         *  Methods for managing Primary Key
         */
        //@{

        /*!
          \brief It sets the primary key constraint.

          \param pk The primary key constraint.

          \pre The pk must not be associated to another DataSetType.

          \post The DataSetType will take the ownership of the primary key.
          \post The primary key will be automatically attached to the DataSetType.

          \note If another primary key has been already set, it will destroy it and set the new one as the valid primary key.

          \note If you inform a NULL pointer, it ill just release the internal primary key.
        */
        void setPrimaryKey(PrimaryKey* pk);

        /*!
          \brief It returns the primary key associated to the dataset type.

          \rerturn The primary key associated to the dataset type or NULL if none exists.
        */
        PrimaryKey* getPrimaryKey() const { return m_pk; }

        //@}

        /** @name Constraint Methods
         *  Methods for managing constraints
         */
        //@{

         /*!
          \brief It adds a new constraint.

          A constraint can be a: foreign key, unique key, check constraint or primary key

          \param c The constraint to be added to the DataSetType.

          \pre Don't inform a NULL pointer.
          \pre For foreign keys, if the DataSetType is associated to a DataSourceCatalog the DataSetType 
                referenced in the foreign key must already be in the catalog.

          \post The constraint will be attached to the DataSetType.
          \post The DataSetType will take the ownership of the informed constraint.
          \post For foreign keys, if the DataSetType is associated to a DataSourceCatalog, this method
                will register in the catalog an association between the foreign key and its referenced DataSetType.

          \exception Exception For foreign keys, it throws an exception if the DataSetType is associated to a DataSourceCatalog
                               and the foreign key is referencing another DataSetType that is not.
        */
        void add(Constraint* c);

         /*!
          \brief It removes the constraint.

          A constraint can be a: foreign key, unique key, check constraint or primary key

          \param c The constraint to be removed from the DataSetType.

          \pre Don't inform a NULL pointer or a unique key that doesn't belong to the DataSetType.

          \post The constraint pointer will be invalidated.
        */
        void remove(Constraint* c);

        //@}

        /** @name Unique Keys Methods
         *  Methods for managing Unique Keys
         */
        //@{

        /*!
          \brief It returns the number of unique keys defined for the dataset type.

          \return The number of unique keys defined for the dataset type.
        */
        std::size_t getNumberOfUniqueKeys() const { return m_uniqueKeys.size(); }

        /*!
          \brief It adds the unique key constraints.

          \param uks The unique key constraints to be added to the DataSetType.

          \pre Don't inform NULL pointers in the list.

          \post The unique key constraints will be attached to the DataSetType.
          \post The DataSetType will take the ownership of the informed unique key constraints.
        */
        void add(const std::vector<UniqueKey*>& uks);

         /*!
          \brief It returns the i-th unique key.

          \param i The unique key position.

          \return The i-th unique key.

          \pre i must be in the valid range [0, getNumberOfUniqueKeys()).

          \note It doesn't make range check!
        */
        UniqueKey* getUniqueKey(std::size_t i) const { return m_uniqueKeys[i]; }

        /*!
          \brief It returns a unique key with the given name or NULL if none is found.

          \param name The unique key name.

          \return A unique key with the given name or NULL.
        */
        UniqueKey* getUniqueKey(const std::string& name) const;

        /*!
          \brief It removes the unique key.

          \param uk The unique key to be removed from the DataSetType.

          \pre Don't inform a NULL pointer or a unique key that doesn't belong to the DataSetType.

          \post The unique key pointer will be invalidated.
        */
        void remove(UniqueKey* uk);

        /*! \brief It removes all unique keys from the dataset type. */
        void clearUniqueKeys();

        //@}

        /** @name Check Constraint Methods
         *  Methods for managing Check Constraints
         */
        //@{

        /*!
          \brief It returns the number of check-constraints defined over the dataset type.

          \return The number of check-constraints defined over the dataset type.
        */
        std::size_t getNumberOfCheckConstraints() const { return m_checkConstraints.size(); }

        /*!
          \brief It adds the check constraints.

          \param ccs The check constraints to be added to the DataSetType.

          \pre Don't inform NULL pointers in the list.

          \post The check constraints will be attached to the DataSetType.
          \post The DataSetType will take the ownership of the informed check constraints.
        */
        void add(const std::vector<CheckConstraint*>& ccs);

        /*!
          \brief It returns the i-th check-constraint associated to the dataset type.

          \param i The check-constraint position.

          \return The i-th check-constraint associated to the dataset type.

          \pre i must be in the valid range [0, getNumberOfCheckConstraints()).
        */
        CheckConstraint* getCheckConstraint(std::size_t i) const { return m_checkConstraints[i]; }

        /*!
          \brief It returns a check-constraint with the given name or NULL if none is found.

          \param name The check-constraint name.

          \return A check-constraint with the given name or NULL if none exist.
        */
        CheckConstraint* getCheckConstraint(const std::string& name) const;

        /*!
          \brief It removes the check constraint.

          \param cc The check constraint to be removed from the DataSetType.

          \pre Don't inform a NULL pointer or a check constraint that doesn't belong to the DataSetType.

          \post The check constraint pointer will be invalidated.
        */
        void remove(CheckConstraint* cc);

        /*! \brief It removes all check constraints. */
        void clearCheckConstraints();

        //@}

        /** @name Index Methods
         *  Methods for managing Indexes
         */
        //@{

        /*!
          \brief It returns the number of indexes defined for the dataset type.

          \return The number of indexes defined for the dataset type.
        */
        std::size_t getNumberOfIndexes() const { return m_idxs.size(); }

        /*!
          \brief It adds a new index.

          \param idx The index to be added to the DataSetType.

          \pre Don't inform a NULL pointer.

          \post The index will be attached to the DataSetType.
          \post The DataSetType will take the ownership of the informed index.
        */
        void add(Index* idx);

        /*!
          \brief It adds all the indexes.

          \param idxs A list of indexes to be added to the DataSetType.

          \pre Don't inform a NULL pointer in the list.

          \post The indexes will be attached to the DataSetType.
          \post The DataSetType will take the ownership of the informed indexes.
        */
        void add(const std::vector<Index*>& idxs);

        /*!
          \brief It returns the i-th index associated to the dataset type.

          \param i The index position.

          \return The i-th index associated to the dataset type.

          \pre i must be in the valid range [0, getNumberOfIndexes()).

          \note It doesn't make range check!
        */
        Index* getIndex(std::size_t i) const { return m_idxs[i]; }

        /*!
          \brief It returns an index with the given name or NULL if none is found.

          \param name The index name.

          \return An index with the given name or NULL.
        */
        Index* getIndex(const std::string& name) const;

        /*!
          \brief It removes the index.

          \param idx The index to be removed from the DataSetType.

          \pre Don't inform a NULL pointer or a index that doesn't belong to the DataSetType.

          \post If there is a primary key referencing the index it will be released.
          \post If there is a unique key referencing the index it will be released.
          \post The index pointer will be invalidated.
        */
        void remove(Index* idx);

        /*! \brief It removes all indexes from the dataset type. */
        void clearIndexes();

        //@}

        /** @name Foreign Key Methods
         *  Methods for managing foreign Keys
         */
        //@{

        /*!
          \brief It returns the number of foreign keys defined for the dataset type.

          \return The number of foreign keys defined for the dataset type.
        */
        std::size_t getNumberOfForeignKeys() const { return m_foreignKeys.size(); }

        /*!
          \brief It returns the i-th foreign key associated to the dataset type.

          \param i The foreign key position.

          \return The i-th foreign key associated to the dataset type.

          \pre i must be in the valid range [0, getNumberOfForeignKeys()).

          \note It doesn't make range check!
        */
        ForeignKey* getForeignKey(std::size_t i) const { return m_foreignKeys[i]; }

        /*!
          \brief It returns a foreign key with the given name or NULL if none is found.

          \param name The foreign key name.

          \return A foreign key with the given name or NULL if none exist.
        */
        ForeignKey* getForeignKey(const std::string& name) const;

        /*!
          \brief It adds a new foreign key.

          \param fk The foreign key to be added to the DataSetType.

          \pre Don't inform a NULL pointer or a fk associated to another DataSetType.
          \pre If the DataSetType is associated to a DataSourceCatalog the DataSetType referenced in the foreign key must already be in the catalog.

          \post The foreign key will be attached to the DataSetType.
          \post The DataSetType will take the ownership of the informed foreign key.
          \post If the DataSetType is associated to a DataSourceCatalog, this method
                will register in the catalog an association between the fk and its referenced DataSetType.

          \exception Exception It throws an exception if the DataSetType is associated to a DataSourceCatalog
                               and the fk is referencing another DataSetType that is not.
        */
        void add(ForeignKey* fk);

        /*!
          \brief It removes the foreign key.

          \param fk The foreign key to be removed from the DataSetType.

          \pre Don't pass a NULL pointer or a foreign key that doesn't belong to the DataSetType.

          \post The foreign key pointer will be invalidated.
          \post If the DataSetType is associated to a DataSourceCatalog, this method
                will unregister in the catalog the association between the fk and its referenced DataSetType.          

          \exception Exception It throws an exception if the fk can not be removed.
        */
        void remove(ForeignKey* fk);

        //@}        

        /** @name Re-implementation from CompositeProperty
         *  Re-implementation from CompositeProperty.
         */
        //@{

        /*!
          \brief It removes the Property.

          This method may propagate changes to:
          indexes, unique keys, primary key, foreign key,
          check-constraints, sequences and catalog.

          If the property to be removed is a geometry
          this method will take care of the default geometry property
          finding the first onde available or setting
          it to NULL if no other geometry property exists.

          \param p The Property to be removed.

          \post Any association with Primary Keys, and other stuffs will be automatically removed.
          \post The Property pointer will be invalidated.

          \todo Update CheckConstraint entries!
        */
        void remove(Property* p);

        /*! \brief It clears the DataSetType definition. */
        void clear();

        //@}

        /** @name Auxiliary Methods
         *  Auxiliary methods.
         */
        //@{

        /*!
          \brief It replaces the property p by pp and adjust all pointers inclusive in the catalog.

          \param p  The property to be replaced.
          \param pp The property that will be added in the datasettype.

          \note If the property p is the owner of a sequence, the new property pp will take the ownership.

          \note The caller of this method will take the ownership of p pointer. So, it is the caller responsability to free it!

          \note The DataSetType will take the ownership of pp pointer.
        */
        void replace(Property* p, Property* pp); 

        //@}


        /** @name Implementation From Superclass Property
         *  Methods re-implemented from the superclass Property.
         */
        //@{

        /*!
          \brief It returns a clone of the object.

          The new DataSetType will not be associated
          to any DataSourceCatalog. All associated constraints names,
          like indexes, unique keys, primary keys, foreign keys and
          check constraints will preserve the name. This is also true
          for the DataSetType name. So, be carefull
          when trying to persist the DataSetType in the same data source
          as the cloned dataset type.

          Foreign keys are copied and will reference the same objects as
          the cloned object.

          \return A clone of the object.
        */
        Property* clone() const;

        //@}

      protected:

        /** @name Protected Helper Methods
         *  Methods for dealing with some regular tasks related to DataSetType constraints.
         */
        //@{

        /*!
          \brief It removes all unique keys related to the given Property.

          \param p A Property that may take part of a unique key.
        */
        void removeUniqueKeys(Property* p);

        /*!
          \brief It removes all indexes related to the given Property.

          \param p A Property that may take part of a index.
        */
        void removeIndexes(Property* p);

        /*!
          \brief It removes all foreign keys that p takes part and all foreign keys that references p.

          This method may change this DataSetType and all other DataSetTypes in the DataSourceCatalog
          that has a foreign key referencing this property.

          \param p A Property that may take part of a foreign key.

          \note If the DataSetType is associated to a DataSourceCatalog it
                can propagate changes for other foreign keys. It is like a cascade deletion of
                related foreign keys and attributes.
        */
        void removeForeignKeys(Property* p);

        //@}

      private:

        DataSourceCatalog* m_catalog;                       //!< The associated catalog.        
        PrimaryKey* m_pk;                                   //!< The DataSetType primary key.
        std::string m_title;                                //!< A brief description of the DataSetType.
        std::vector<ForeignKey*> m_foreignKeys;             //!< A vector of foreign key constraints.
        std::vector<CheckConstraint*> m_checkConstraints;   //!< A vector of check constraints.
        std::vector<Index*> m_idxs;                         //!< A vector of indexes.
        std::vector<UniqueKey*> m_uniqueKeys;               //!< A vector of unique key constraints.
        int m_category;                                     //!< A category
    };

    typedef boost::shared_ptr<DataSetType> DataSetTypePtr;

    inline bool te::da::DataSetType::hasGeom() const
    {
      return hasPropertyOfType(te::dt::GEOMETRY_TYPE);
    }

    inline bool DataSetType::hasRaster() const
    {
      return hasPropertyOfType(te::dt::RASTER_TYPE);
    }

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPE_H


