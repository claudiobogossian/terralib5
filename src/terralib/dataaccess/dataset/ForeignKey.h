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
  \file terralib/dataaccess/dataset/ForeignKey.h

  \brief It models a foreign key constraint for a DataSetType.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FOREIGNKEY_H
#define __TERRALIB_DATAACCESS_INTERNAL_FOREIGNKEY_H

// TerraLib
#include "Constraint.h"

// STL includes
#include <vector>

namespace te
{
// Forward declarations
  namespace dt { class Property; }

  namespace da
  {

    /*!
      \class ForeignKey
      
      \brief It models a foreign key constraint for a DataSetType.

      \sa DataSetType, PrimaryKey, UniqueKey, CheckConstraint
    */
    class TEDATAACCESSEXPORT ForeignKey : public Constraint
    {
      public:

        /*!
          \brief Constructor.

          The default fk will have the actions OnDelete and OnUpdate set as NO_ACTION.

          \param id The fk identifier.
      
          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        ForeignKey(unsigned int id = 0);

        /*!
          \brief Constructor.

          \param name The foreign key constraint name.
          \param id   The fk identifier.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        ForeignKey(const std::string& name, unsigned int id = 0);

        /*!
          \brief Copy constructor not allowed.

          The new object will not have an associated DataSetType.

          \param rhs Right-hand-side instance.
        */
        ForeignKey(const ForeignKey& rhs);
  
        /*! \brief Destructor. */
        ~ForeignKey() {}

        /*!
          \brief Assignment operator.

          The new object will not have an assigned DataSetType.

          \param rhs Right-hand-side instance.

          \return A reference to this.
        */
        ForeignKey& operator=(const ForeignKey& rhs);

         /*!
          \brief It returns the properties that take part of the foreign key constraint.

          \return The properties that take part of the foreign key constraint.
        */
        const std::vector<te::dt::Property*>& getProperties() const { return m_properties; }
        
        /*!
          \brief It adds a property to the foreign key constraint.

          \param p The property to be added to the foreign key constraint.

          \pre All properties added must belong to the same DataSetType.
        */
        void add(te::dt::Property* p) { m_properties.push_back(p); }

        /*!
          \brief It sets the properties that take part of the foreign key constraint.

          \param properties The properties that take part of the foreign key constraint.

          \pre All properties added must belong to the same DataSetType.
        */
        void setProperties(const std::vector<te::dt::Property*>& properties) { m_properties = properties; }

        /*!
          \brief It returns the referenced properties (on the referenced DataSetType) of this foreign key constraint.

          \return The referenced properties (on the referenced DataSetType) of this foreign key constraint.
        */
        const std::vector<te::dt::Property*>& getReferencedProperties() const { return m_refProperties; }

        /*!
          \brief It adds a reference property (on the referenced DataSetType) of this foreign key constraint.

          \param p The referenced property (on the referenced DataSetType) of this foreign key constraint.

          \pre All properties being added must reference the same DataSetType.
        */
        void addRefProperty(te::dt::Property* p) { m_refProperties.push_back(p); }

        /*!
          \brief It sets the referenced properties (on the referenced DataSetType) of this foreign key constraint.

          \param properties The referenced properties (on the referenced DataSetType) of this foreign key constraint.

          \pre All properties must reference the same DataSetType.
        */
        void setReferencedProperties(const std::vector<te::dt::Property*>& properties) { m_refProperties = properties; }

        /*!
          \brief It returns the referenced DataSetType of this foreign key constraint.

          \return The referenced DataSetType of this foreign key constraint.
        */
        DataSetType* getReferencedDataSetType() const { return m_refDt; }

        /*!
          \brief It sets the referenced DataSetType of this foreign key constraint.

          \param refDt The referenced DataSetType of this foreign key constraint.
        */
        void setReferencedDataSetType(DataSetType* refDt) { m_refDt = refDt; }

        /*!
          \brief It returns the action performed when a referenced element value in the referenced DataSetType is being deleted.

          \return The action performed when a referenced element value in the referenced DataSetType is being deleted.
        */
        FKActionType getOnDeleteAction() const { return m_onDelete; }

        /*!
          \brief It sets the action to be performed when a referenced element value in the referenced DataSetType is being deleted.

          \param a The action to be performed when a referenced element value in the referenced DataSetType is being deleted.
        */
        void setOnDeleteAction(FKActionType a) { m_onDelete = a; }

        /*!
          \brief It returns the action performed when a referenced element value in the referenced DataSetType is being updated to a new value.

          \return The action performed when a referenced element value in the referenced DataSetType is being updated to a new value.
        */
        FKActionType getOnUpdateAction() const { return m_onUpdate; }

        /*!
          \brief It sets the action to be performed when a referenced element value in the referenced DataSetType is being updated to a new value.

          \param a The action to be performed when a referenced element value in the referenced DataSetType is being updated to a new value.
        */
        void setOnUpdateAction(FKActionType a) { m_onUpdate = a; }

        /*!
          \brief It verifies if Property takes part of the foreign key.

          \param p The Property to be verified.

          \return True if Property takes part of the foreign key, false otherwise.
        */
        bool has(te::dt::Property* p);

        /*!
          \brief It verifies if Property is referenced by the foreign key.

          \param p The Property to be verified.

          \return True if Property is referenced by the foreign key, false otherwise.
        */
        bool isReferenced(te::dt::Property* p);

        /*!
          \brief It changes a reference to property p to pp.

          \param p  A property that takes part of the foreign key or is referenced by it.
          \param pp The property that will take p place.

          \note This method will replace property checking both property list (referenced and the properties in the fk).

          \note If the property p is not in the fk attribute list this method does nothing.
        */
        void replace(te::dt::Property* p, te::dt::Property* pp);

        /*!
          \brief It returns the constraint type: FOREIGNKEY.
          
          \return The constraint type FOREIGNKEY.
        */
        ConstraintType getType() const { return FOREIGN_KEY; } 

        /*!
          \brief It returns a clone of the object.

          The new object will not have an associated DataSetType.

          \return A clone of the object.
        */
        Constraint* clone();

      private:

        FKActionType m_onDelete;                            //!< The action to be performed when a referenced element value in the referenced DataSetType is being deleted.
        FKActionType m_onUpdate;                            //!< The action to be performed when a referenced element value in the referenced DataSetType is being updated to a new value.
        DataSetType* m_refDt;                         //!< The referenced DataSetType of this foreign key constraint.
        std::vector<te::dt::Property*> m_properties;      //!< The properties that are part of the foreign key constraint.        
        std::vector<te::dt::Property*> m_refProperties;   //!< The referenced properties (on the referenced DataSetType) of this foreign key constraint.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FOREIGNKEY_H


