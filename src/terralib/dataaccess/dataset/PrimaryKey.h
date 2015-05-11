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
  \file terralib/dataaccess/dataset/PrimaryKey.h

  \brief It describes a primary key (pk) constraint.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_PRIMARYKEY_H
#define __TERRALIB_DATAACCESS_INTERNAL_PRIMARYKEY_H

// TerraLib
#include "Constraint.h"

// STL
#include <vector>

namespace te
{
// Forward declarations
  namespace dt { class Property; }

  namespace da
  {
// Forward declarations
    class Index;    

    /*!
      \class PrimaryKey
      
      \brief It describes a primary key (pk) constraint.

      \sa DataSetType, UniqueKey, ForeignKey, CheckConstraint
    */
    class TEDATAACCESSEXPORT PrimaryKey : public Constraint
    {
      public:

        /*!
          \brief Constructor.

          \param dt The DataSetType associated to the pk.
          \param id The pk identifier.

          \post If dt is provided, the pk will belong to the given DataSetType.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        PrimaryKey(DataSetType* dt = 0, unsigned int id = 0);

        /*!
          \brief Constructor.

          \param name The primary key constraint name.
          \param dt   The DataSetType associated to the pk.
          \param id   The pk identifier.

          \post If dt is provided, the pk will belong to the given DataSetType.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        PrimaryKey(const std::string& name, DataSetType* dt = 0, unsigned int id = 0);

        /*!
          \brief Copy constructor.

          The new object will not have an assigned DataSetType.

          \param rhs Right-hand-side instance.
        */
        PrimaryKey(const PrimaryKey& rhs);

        /*! \brief Destructor. */
        ~PrimaryKey() {}

        /*!
          \brief Assignment operator.

          The new object will not have an assigned DataSetType.

          \param rhs Right-hand-side instance.

          \return A reference to this.
        */
        PrimaryKey& operator=(const PrimaryKey& rhs);

        /*!
          \brief It returns the properties that take part of the primary key.

          \return The properties that take part of the primary key.
        */
        const std::vector<te::dt::Property*>& getProperties() const { return m_properties; }

        /*!
          \brief It sets the properties that form the primary key.

          \param properties The properties that form the primary key.
        */
        void setProperties(const std::vector<te::dt::Property*>& properties) { m_properties = properties; }

        /*!
          \brief It adds a property to the list of properties of the primary key.

          \param p The property that will take part of the primary key.
        */
        void add(te::dt::Property* p) { m_properties.push_back(p); }

        /*!
          \brief It sets the associated index.

          \param idx If the primary key is associated to an index, this method will associate them.
        */
        void setAssociatedIndex(Index* idx) { m_index = idx; }

        /*!
          \brief It returns the associated index if one exists.

          \return An associated index if one exists.
        */
        Index* getAssociatedIndex() const { return m_index; }

        /*!
          \brief It verifies if Property is associated to the primary key.

          \param p The Property to be verified.

          \return True if Property is associated to the primary key, false otherwise.
        */
        bool has(const te::dt::Property* p) const;        

        /*!
          \brief It changes a reference to property p to pp.

          \param p  A property that takes part of the primary key.
          \param pp The property that will take p place.

          \note If p is not found in the primary key attribute list this method does nothing.
        */
        void replace(te::dt::Property* p, te::dt::Property* pp);

        /*!
          \brief It returns the constraint type: PRIMARYKEY.
          
          \return The constraint type PRIMARYKEY.
        */
        ConstraintType getType() const { return PRIMARY_KEY; } 

        /*!
          \brief It returns a clone of the object.

          The new object will not have an associated DataSetType.

          \return A clone of the object.
        */
        Constraint* clone();

      private:

        Index* m_index;                               //!< A pointer to an associated index.
        std::vector<te::dt::Property*> m_properties;  //!< The properties that take part of primary key constraint.
    };

  } // end namespace da
}   // end namespace terralib

#endif  // __TERRALIB_DATAACCESS_INTERNAL_PRIMARYKEY_H

