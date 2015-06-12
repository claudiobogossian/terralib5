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
  \file terralib/datatype/StringProperty.h

  \brief The type for string types: FIXED_STRING, VAR_STRING or STRING.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_STRINGPROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_STRINGPROPERTY_H

// TerraLib
#include "../common/CharEncodingConv.h"
#include "Enums.h"
#include "SimpleProperty.h"

namespace te
{
  namespace dt
  {
    /*!
      \class StringProperty

      \brief The type for string types: FIXED_STRING, VAR_STRING or STRING.

      \ingroup datatype

      \sa SimpleProperty, Property, NumericProperty, ArrayProperty, DateTimeProperty, CompositeProperty
    */
    class TEDATATYPEEXPORT StringProperty : public SimpleProperty
    {
      public:

        /*!
          \brief It constructs a new string property.

          \param name         The attribute name.
          \param strType      The string subtype. Must be one of: FIXED_STRING, VAR_STRING, or STRING.
          \param size         The maximum size (or 0 for unlimited strings).
          \param isRequired   It indicates if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.          

          \post The string property will take the ownership of the defaultValue.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        StringProperty(const std::string& name,
                       StringType strType = STRING,
                       std::size_t size = 0,
                       bool isRequired = false,
                       std::string* defaultValue = 0,
                       unsigned int id = 0,
                       Property* parent = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.

          \note This method is used by clone().
        */
        StringProperty(const StringProperty& rhs);

        /*! \brief Destructor. */
        ~StringProperty() {}

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        StringProperty& operator=(const StringProperty& rhs);

        /*!
          \brief It returns the string property sub type.

          \return The string property sub type.
        */
        StringType getSubType() const { return m_strSubType; }

        /*!
          \brief It sets the string property sub type.
          
          \param t The string property sub type.
        */
        void setSubtype(StringType t) { m_strSubType = t; }

        /*!
          \brief It returns the maximum number of characters for a varying string, or the number of characters for a fixed length string. No meaning for STRING.

          \return The maximum number of characters for a varying string, or the number of characters for a fixed length string. No meaning for STRING.
        */
        std::size_t size() const { return m_size; }

        /*!
          \brief It sets the maximum number of characters for a varying string, or the number of characters for a fixed length string.
          
          This value has no meaning for a STRING.

          \param size The maximum size.
        */
        void setSize(std::size_t s) { m_size = s; }

        /*!
          \brief It returns the string property character encoding.

          \return The string property character encoding.
        */
        te::common::CharEncoding getCharEncoding() const { return m_ce; }

        /*!
          \brief It sets the string property character encoding.

          \param ce The character encoding type.
        */
        void setCharEncoding(const te::common::CharEncoding& ce) { m_ce = ce; }

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other properties.

          \return A clone of the object.
         */
        Property* clone() const;

      protected:

        std::size_t m_size;            //!< Maximum number of characters for a varying string or the number of characters for a fixed length string. No meaning for STRING.
        StringType m_strSubType;       //!< The sub-type of this string property.
        te::common::CharEncoding m_ce; //!< The string property character encoding.
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_STRINGPROPERTY_H

