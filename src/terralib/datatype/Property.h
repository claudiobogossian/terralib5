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
  \file terralib/datatype/Property.h

  \brief It models a property definition.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_PROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_PROPERTY_H

// TerraLib
#include "Config.h"
#include "Enums.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace dt
  {
    /*!
      \class Property

      \brief It models a property definition.

      The class Property defines information about the values of a given property.
      This includes:
      <ul>
      <li>The data type associated to the property;</li>
      <li>Any restrictions on the values of the property;</li>
      </ul>

      \ingroup datatype

      \sa SimpleProperty, CompositeProperty, NumericProperty, StringProperty, DateTimeProperty

      \todo We must worry about write methods. To make classes safer we need to define pure abstract classes and implement them in the drivers.
    */
    class TEDATATYPEEXPORT Property
    {
      public:

        /*!
          \brief It initializes a new Property.

          \param name      The Property name.
          \param datatype  The property data type.
          \param id        The property identifier.
          \param parent    A reference to the parent Property of the new object.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        Property(const std::string& name,
                 int datatype,
                 unsigned int id = 0,
                 Property* parent = 0,
                 const std::string& dsName = "");

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.

          \note Copy constructor doesn't preserve parent relationship.
        */
        Property(const Property& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Property() {}

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.

          \note Assignment operator doesn't preserve parent relationship.
        */
        Property& operator=(const Property& rhs);

        /*!
          \brief It returns the property identifier.

          \return A number used to identify the property.

          \warning The identifier value may be used by data source implementations. So, don't rely on its value!
        */
        unsigned int getId() const { return m_id; }

        /*!
          \brief It sets the property identifier.

          \param id A number used to identify the property.

          \warning The identifier value may be used by data source implementations. So, don't rely on its value!
        */
        void setId(unsigned int id) { m_id = id; }

        /*!
          \brief It returns the property name.

          \return The property name.

          \note For DataSetTypes the name can have namespace prefix or schema name prefix, it will depend on the data source implementation.
        */
        const std::string& getName() const { return m_name; }

        /*!
          \brief It sets the property name.

          \param name The new property name.

          \warning Take care when calling this method for a DataSetType associated to a DataSourceCatalog.
                   Prefer using the rename method on the DataSourceCatalog API instead of using this method.
        */
        void setName(const std::string& name) { m_name = name; }

        /*!
          \brief It returns the name of the propery's dataset.

          \return The the propery's dataset name.
        */
        const std::string& getDatasetName() const { return m_datasetName; }

        /*!
          \brief It sets the property name.

          \param name The new property name.

          \warning Take care when calling this method for a DataSetType associated to a DataSourceCatalog.
                   Prefer using the rename method on the DataSourceCatalog API instead of using this method.
        */
        void setDatasetName(const std::string& dsName) { m_datasetName = dsName; }

        /*!
          \brief It returns the property data type.

          \return The property data type.
        */
        int getType() const { return m_type; }

        /*!
          \brief It returns the parent of this property, or NULL, if it doesn't have one.

          \return The parent of this property, or NULL, if it doesn't have one.
        */
        Property* getParent() const { return m_parent; }

        /*!
          \brief It associate this property to the informed parent.

          \param p Teh parent property.

          \warning Don't use this method without knowing the internal details of its use.
        */
        void setParent(Property* p) { m_parent = p; }

        /*!
          \brief It checks if the Property "p" is associated to this property or any other parent.

          This method can be used to ask if a given Property belongs
          to a more complex type (like: CompositeProperty or DataSetType).

          \param p The Property we are checking.

          \return True if the given Property "p" is associated to to this Property, otherwise, returns false.
        */
        virtual bool has(Property* p) const = 0;

        /*!
          \brief It returns a clone of the object.

          The new Property may not have associations to other elements.
          For example, a DataSetType associated to a DataSourceCatalog
          when cloned will not have the clone associate to the same DataSourceCatalog.

          \return A clone of the object.
        */
        virtual Property* clone() const = 0;

      protected:

        Property* m_parent;                             //!< The parent property type: it must be a CompositeProperty.
        unsigned int m_id;                              //!< An identification number that can be used internally.        
        int m_type;                                     //!< The property data type.
        std::string m_name;                             //!< The property name.
        std::string m_datasetName;                      //!< The property's dataset name.
    };

    /*!
      \brief For use with boost conteiners.
    */
    inline Property* new_clone(const Property& a)
    {
      return a.clone();
    }

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_PROPERTY_H

