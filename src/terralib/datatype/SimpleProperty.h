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
  \file terralib/datatype/SimpleProperty.h

  \brief An atomic property like an integer or double.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_SIMPLEPROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_SIMPLEPROPERTY_H

// TerraLib
#include "Property.h"

namespace te
{
  namespace dt
  {
    /*!
      \class SimpleProperty

      \brief An atomic property like an integer or double.

      \sa Property, CompositeProperty, NumericProperty, StringProperty, ArrayProperty, DateTimeProperty

      \ingroup datatype

      \todo The default value must be changed to an AbstractData.
    */
    class TEDATATYPEEXPORT SimpleProperty : public Property
    {
      public:

        /*!
          \brief It constructs a new simple property.

          \param name         The simple property name.
          \param datatype     The property data type.
          \param isRequired   Tells if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used or NULL if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.
          
          \post The simple property will take the ownership of the default value.
          
          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
          \warning By default, a simple property is not an autonumber (or serial).
        */
        SimpleProperty(const std::string& name,
                       int datatype,
                       bool isRequired = false,
                       std::string* defaultValue = 0,
                       unsigned int id = 0,
                       Property* parent = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        SimpleProperty(const SimpleProperty& rhs);

        /*! \brief Destructor. */
        virtual ~SimpleProperty();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        SimpleProperty& operator=(const SimpleProperty& rhs);

        /*!
          \brief It returns true if the attribute is required, otherwise it returns false.

          \return True if the attribute is required, otherwise it returns false.
        */
        bool isRequired() const { return m_isRequired; }

        /*!
          \brief It tells if the property is required or not.

          \param r If true the property is marked as required, otherwise it is not required.
        */
        void setRequired(bool r) { m_isRequired = r; }

        /*!
          \brief It returns true if the attribute is an autonumber, otherwise it returns false.

          \return True if the attribute is an autonumber, otherwise it returns false.
        */
        bool isAutoNumber() const { return m_isAutoNumber; }

        /*!
          \brief It tells if the property is an autonumber or not.

          \param a If true the property is marked as an autonumber (serial), otherwise it is not.
        */
        void setAutoNumber(bool a) { m_isAutoNumber = a; }

        /*!
          \brief It returns the default value associated to the property, or NULL if none is associated.

          \return The default value associated to the property, or NULL if none is associated.

          \note The caller of this method will not take the ownership of the given string.
        */
        std::string* getDefaultValue() const { return m_defaultValue; }

        /*!
          \brief It sets the default value associated to the property, or NULL if none is associated.

          \param d The default value associated to the property, or NULL if none is associated.

          \note The SimpleProperty qill take the ownership of the given string.
        */
        void setDefaultValue(std::string* d);

        /*!
          \brief It checks if the Property "p" is associated to this property or any other parent.

          This method can be used to ask if a given Property belongs
          to a more complex type (like: CompositeProperty or DataSetType).

          \param p The Property we are checking.

          \return True if the given Property "p" is associated to to this Property, otherwise, returns false.
        */
        bool has(Property* p) const;
        
        /*!
          \brief It returns a clone of the object.

          For a simple type, this method will not preserve
          the parent Property pointer.
          
          \return A clone of the object.
        */
        virtual Property* clone() const;

      protected:

        bool m_isRequired;                  //!< This flag indicates if the attribute is required or not.
        bool m_isAutoNumber;                //!< A flag that indicates if this is an autonumber or serial type.
        std::string* m_defaultValue;        //!< Default value.
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_SIMPLEPROPERTY_H

