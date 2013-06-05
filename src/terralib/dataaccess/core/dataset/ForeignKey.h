/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/dataset/ForeignKey.h

  \brief It models a foreign key constraint for a DataSetType.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_FOREIGNKEY_H
#define __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_FOREIGNKEY_H

// TerraLib
#include "Constraint.h"

// STL includes
#include <vector>

namespace te
{
  namespace da
  {
    namespace core
    {
      /*!
        \class ForeignKey

        \brief It models a foreign key constraint for a DataSetType.

        \sa Constraint, PrimaryKey, UniqueKey, CheckConstraint
      */
      class  ForeignKey : public Constraint
      {
        public:

          /*!
            \brief Constructor.

            \param name The foreign key constraint name.
          */
          ForeignKey(const std::string& name = "");

          /*!
            \brief Copy constructor not allowed.

            \param rhs Right-hand-side instance.
          */
          ForeignKey(const ForeignKey& rhs);
  
          /*! \brief Destructor. */
          ~ForeignKey() { }

          /*!
            \brief Assignment operator.

            \param rhs Right-hand-side instance.

            \return A reference to this.
          */
          ForeignKey& operator=(const ForeignKey& rhs);

          /*!
            \brief It returns the properties that take part of the foreign key constraint.

            \return The properties that take part of the foreign key constraint.
          */
          const std::vector<std::size_t>& getProperties() const { return m_properties; }

          /*!
            \brief It returns the properties that take part of the foreign key constraint.

            \return The properties that take part of the foreign key constraint.
          */
          std::vector<std::size_t>& getProperties() { return m_properties; }

          /*!
            \brief It adds a property to the foreign key constraint.

            \param p The property to be added to the foreign key constraint.
          */
          void add(const std::size_t propertyPos) { m_properties.push_back(propertyPos); }

          /*!
            \brief It returns the referenced properties (on the referenced DataSetType) of this foreign key constraint.

            \return The referenced properties (on the referenced DataSetType) of this foreign key constraint.
          */
          const std::vector<std::size_t>& getReferencedProperties() const { return m_refProperties; }

          /*!
            \brief It returns the referenced properties (on the referenced DataSetType) of this foreign key constraint.

            \return The referenced properties (on the referenced DataSetType) of this foreign key constraint.
          */
          std::vector<std::size_t>& getReferencedProperties() { return m_refProperties; }

          /*!
            \brief It adds a reference property (on the referenced DataSetType) of this foreign key constraint.

            \param p The referenced property (on the referenced DataSetType) of this foreign key constraint.

            \pre All properties being added must reference the same DataSetType.
          */
          void addRefProperty(const std::size_t propertyPos) { m_refProperties.push_back(propertyPos); }

          /*!
            \brief It returns the name of the referenced dataset of this foreign key constraint.

            \return The name of the referenced dataset of this foreign key constraint.
          */
          const std::string& getReferencedDataSet() const { return m_refDataset; }

          /*!
            \brief It sets the referenced DataSetType of this foreign key constraint.

            \param refDt The referenced DataSetType of this foreign key constraint.
          */
          void setReferencedDataSet(const std::string& refDataset) { m_refDataset = refDataset; }

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

          std::string m_refDataset;                   //!< The referenced dataset of this foreign key constraint.
          std::vector<std::size_t> m_properties;      //!< The properties that are part of the foreign key constraint.
          std::vector<std::size_t> m_refProperties;   //!< The referenced properties (on the referenced dataset) of this foreign key constraint.
          FKActionType m_onDelete;                    //!< The action to be performed when a referenced element value in the referenced dataset is being deleted.
          FKActionType m_onUpdate;                    //!< The action to be performed when a referenced element value in the referenced dataset is being updated to a new value.

      };

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_FOREIGNKEY_H
