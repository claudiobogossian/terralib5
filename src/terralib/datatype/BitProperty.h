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
  \file terralib/datatype/BitProperty.h

  \brief Property for bit types.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_BITPROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_BITPROPERTY_H

// TerraLib
#include "SimpleProperty.h"

namespace te
{
  namespace dt
  {
    /*!
      \class BitProperty

      \brief Property for bit types.

      \sa SimpleProperty, Property, NumericProperty, ArrayProperty, DateTimeProperty, CompositeProperty, StringProperty
    */
    class TEDATATYPEEXPORT BitProperty : public SimpleProperty
    {
      public:

        /*!
          \brief It constructs a new bit property.

          \param name         The attribute name.
          \param nbits        The number of bits.
          \param isRequired   It indicates if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.          

          \post The bit property will take the ownership of the defaultValue.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        BitProperty(const std::string& name,
                    std::size_t nbits = 1,
                    bool isRequired = false,
                    std::string* defaultValue = 0,
                    unsigned int id = 0,
                    Property* parent = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.

          \note This method is used by clone().
        */
        BitProperty(const BitProperty& rhs);

        /*! \brief Destructor. */
        ~BitProperty() {}

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        BitProperty& operator=(const BitProperty& rhs);

        /*!
          \brief It returns the number of bits represented in the property.

          \return The number of bits represented in the property.
        */
        std::size_t numBits() const { return m_nbits; }

        /*!
          \brief It sets the number of bits represented in the property.

          \param size The number of bits represented in the property.
        */
        void setNumBits(std::size_t s) { m_nbits = s; }

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other properties.

          \return A clone of the object.
         */
        Property* clone() const;

      protected:

        std::size_t m_nbits;       //!< The number of bits represented by the property.
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_BITPROPERTY_H

