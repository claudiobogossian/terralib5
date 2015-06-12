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
  \file terralib/dataaccess/dataset/UniqueKey.h

  \brief It describes a unique key (uk) constraint.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_UNIQUEKEY_H
#define __TERRALIB_DATAACCESS_INTERNAL_UNIQUEKEY_H

// TerraLib
#include "Constraint.h"

// STL
#include <string>
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
      \class UniqueKey
      
      \brief It describes a unique key (uk) constraint.

      \sa DataSetType, Index, PrimaryKey, ForeignKey, DataSetTypeCheckRestriction
    */
    class TEDATAACCESSEXPORT UniqueKey : public Constraint
    {
      public:

        /*!
          \brief Constructor.

          \param dt The DataSetType associated to this uk.
          \param id The uk identifier.

          \post If dt is provided, the uk will belong to the given DataSetType.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        UniqueKey(DataSetType* dt = 0, unsigned int id = 0);

        /*!
          \brief Constructor.

          \param name The unique key (uk) constraint name.
          \param dt   The DataSetType associated to this uk.
          \param id   The uk identifier.

          \post If dt is provided, the uk will belong to the given DataSetType.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        UniqueKey(const std::string& name, DataSetType* dt = 0, unsigned int id = 0);

        /*!
          \brief Copy constructor.

          The new object will not have an associated DataSetType.

          \param rhs Right-hand-side instance.
        */
        UniqueKey(const UniqueKey& rhs);        

        /*! \brief Destructor. */
        ~UniqueKey() {}

        /*!
          \brief Assignment operator.

          The new object will not have an assigned DataSetType.

          \param rhs Right-hand-side instance.

          \return A reference to this.
        */
        UniqueKey& operator=(const UniqueKey& rhs);

        /*!
          \brief It returns the properties that form the unique key.

          \return The properties that form the unique key.
        */
        const std::vector<te::dt::Property*>& getProperties() const { return m_properties; }

        /*!
          \brief It sets the properties that form the unique key.

          \param properties The properties that form the unique key.
        */
        void setProperties(const std::vector<te::dt::Property*>& properties) { m_properties = properties; }

        /*!
          \brief It adds the property to the list of properties that participates in the unique key.

          \param p The Property that will take part of the unique key.
         */
        void add(te::dt::Property* p) { m_properties.push_back(p); }        

        /*!
          \brief It returns the associated index if one exists.

          \return An associated index if one exists.
        */
        Index* getAssociatedIndex() const { return m_index; }

        /*!
          \brief It sets the associated index.

          \param idx A index associated to the unique key.
        */
        void setAssociatedIndex(Index* idx) { m_index = idx; }

        /*!
          \brief It verifies if Property is associated to the unique key.

          \param p The Property to be verified.

          \return True if Property is associated to the unique key, false otherwise.
        */
        bool has(const te::dt::Property* p) const;

        /*
          \brief It changes a reference to property p to pp.

          \param p  A property that takes part of the unique key.
          \param pp The property that will take p place.

          \note If the property p is not in the uk attribute list this method does nothing.
        */
        void replace(te::dt::Property* p, te::dt::Property* pp);

        /*!
          \brief It returns the constraint type: UNIQUEKEY.
          
          \return The constraint type UNIQUEKEY.
        */
        ConstraintType getType() const { return UNIQUE_KEY; }

        /*!
          \brief It returns a clone of the object.

          The new object will not have an associated DataSetType.

          \return A clone of the object.
        */
        Constraint* clone();

      private:
        
        Index* m_index;                               //!< A pointer to an associated index.
        std::vector<te::dt::Property*> m_properties;  //!< The properties that are part of the unique key constraint.
    };

  } // end namespace da 
}   // end namespace terralib

#endif  // __TERRALIB_DATAACCESS_INTERNAL_UNIQUEKEY_H



