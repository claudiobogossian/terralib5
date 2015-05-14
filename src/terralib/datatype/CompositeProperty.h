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
  \file terralib/datatype/CompositeProperty.h

  \brief A base class for a compound properties (non-atomic properties).
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_COMPOSITEPROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_COMPOSITEPROPERTY_H

// TerraLib
#include "Property.h"

// STL
#include <vector>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace dt
  {
    /*!
      \class CompositeProperty

      \brief A base class for a compound property (non-atomic properties).

      \ingroup datatype

      \sa  Property, SimpleProperty, NumericProperty, StringProperty, ArrayProperty, DateTimeProperty
    */
    class TEDATATYPEEXPORT CompositeProperty : public Property
    {
      public:

        /*!
          \brief It creates a new CompositeProperty.

          \param cname    The composite type name.
          \param name     The property name.
          \param id       The property identifier.
          \param parent   A reference to the parent Property of the new object if it has one.
          
          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        CompositeProperty(const std::string& cname,
                          const std::string& name,
                          unsigned int id = 0,
                          Property* parent = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        CompositeProperty(const CompositeProperty& rhs);

        /*! \brief Virtual destructor. */
        virtual ~CompositeProperty();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        CompositeProperty& operator=(const CompositeProperty& rhs);

        /*!
          \brief It returns the name of the composite type.

          \return The name of the composite type.

          \note This name may be different from the property name (returned by getName method)!
         */
        const std::string& getCompositeName() const { return m_cname; }

        /*!
          \brief It sets the composite type name.

          \param name The composite type name.
        */
        void setCompositeName(const std::string& cname) { m_cname = cname; }

        /*!
          \brief It adds a new property to the CompositeProperty.

          \param p The property to be inserted.

          \pre The Property p must be valid.
          \pre The Property p must not be associated to another property.
          
          \post The property p will be associated to CompositeProperty.
          \post The CompositeProperty takes the ownership of the property.
        */
        void add(Property* p);

        /*!
          \brief It adds a list of property types to the CompositeProperty.

          \param ps The list of properties to be added.

          \pre The properties must be valid.
          \pre The properties must not be associated to another property.

          \note The CompositeProperty takes the ownership of the properties.
        */
        void add(const std::vector<Property*>& ps);

        void add(const boost::ptr_vector<te::dt::Property>& ps);

        /*!
          \brief It removes the property from the composite.

          \param p The property to be removed from the composite.
          
          \post The property pointer will be invalidated.
        */
        virtual void remove(Property* p);

        /*!
          \brief It returns the number of properties of the CompositeProperty.

          \return The number of properties of the CompositeProperty.
        */
        std::size_t size() const { return m_properties.size(); }

        /*!
          \brief It returns the list of properties describing the CompositeProperty.

          \return The list of Propertys describing the CompositeProperty.
        */
        const std::vector<Property*>& getProperties() const { return m_properties; }

        /*!
          \brief It returns the list of properties describing the CompositeProperty.

          \return The list of Propertys describing the CompositeProperty.
        */
        std::vector<Property*>& getProperties() { return m_properties; }

        /*!
          \brief It returns the i-th property.

          \param i The property position in the property array.

          \return A pointer to the i-th property. Don't delete it, as it belongs to the CompositeProperty.

          \pre i must be in the valid range [0, size()).
        */
        Property* getProperty(std::size_t i) const { return m_properties[i]; }

        /*!
          \brief It returns the property with the given name or NULL if none is found.

          \param name The name of the property we are looking for.

          \return A pointer to a property or NULL if none is found. Don't delete it, as it belongs to the CompositeProperty.
        */
        Property* getProperty(const std::string& name) const;

        /*!
          \brief It returns the property position based on its name.

          \param name The property name.

          \return A property position in the property array.
        */
        std::size_t getPropertyPosition(const std::string& name) const;

        /*!
          \brief It returns the property position .

          \param p The property.

          \return A property position in the property array.
        */
        std::size_t getPropertyPosition(const Property* p) const;

        /*!
          \brief It searches for a property with the given ID.

          \param id The property ID.

          \return A pointer to the first property with the given ID if found or NULL otherwise. Don't delete it, as it belongs to the CompositeProperty.
        */
        Property* getPropertyById(unsigned int id) const;

        /*!
          \brief Tells if there is a property of the given data type.
        */
        bool hasPropertyOfType(const int t) const;

        /*!
          \brief returns the first property of the given data type. Caller doesn't take ownership of the returned pointer.
        */
        Property* findFirstPropertyOfType(const int t) const;

        /*!
          \brief It copies the properties from the vector.

          \param ps The list of properties to be copied.

          \note The CompositeProperty will not take the ownership of the properties, it will clone each one.
                So, it is the caller responsability to release them.
        */
        void copy(const std::vector<Property*>& ps);

        /*! \brief It clears the CompositeProperty definition. */
        virtual void clear();

        /*!
          \brief It checks if the Property "p" is associated to this property or any other parent.

          \param p The Property we are checking.

          \return True if the given Property "p" is associated to to this Property, otherwise, returns false.
        */
        bool has(Property* p) const;

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other elements.

          \return A clone of the object.
        */
        virtual Property* clone() const;

      protected:
        
        /*!
          \brief It creates a new Property.

          \param cname    The composite type name.
          \param name     The Property name.
          \param t        The data type id.
          \param id       The property identifier.
          \param parent   A reference to the parent Property of the new object if it has one.
        */
        CompositeProperty(const std::string& cname,
                          const std::string& name,
                          int t,
                          unsigned int id,
                          Property* parent);

      protected:

        std::string m_cname;                  //!< The composite type name.
        std::vector<Property*> m_properties;  //!< The list of property types that make the CompositeProperty.
    };

    inline bool CompositeProperty::hasPropertyOfType(const int t) const
    {
      return findFirstPropertyOfType(t) != 0;
    }

    inline Property* CompositeProperty::findFirstPropertyOfType(const int t) const
    {
      const std::size_t size = m_properties.size();

      for(std::size_t i = 0; i != size; ++i)
        if(m_properties[i]->getType() == t)
          return m_properties[i];

      return 0;
    }

  } // end namespace dt
}   // end namespace te


#endif  // __TERRALIB_DATATYPE_INTERNAL_COMPOSITEPROPERTY_H


