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
  \file terralib/datatype/ArrayProperty.h

  \brief The type for variable-length multidimensional arrays.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_ARRAYPROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_ARRAYPROPERTY_H

// TerraLib
#include "SimpleProperty.h"

namespace te
{
  namespace dt
  {
    /*!
      \class ArrayProperty

      \brief The type for variable-length multidimensional arrays.

      \ingroup datatype

      \sa Property, SimpleProperty, NumericProperty, StringProperty, DateTimeProperty, CompositeProperty
    */
    class TEDATATYPEEXPORT ArrayProperty : public SimpleProperty
    {
      public:

        /*!
          \brief It constructs a new property for array data type.

          \param name         The array property name.
          \param elementType  The data type of array elements.
          \param isRequired   Tells if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used if none is informed.
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.
          
          \post The array property will take the ownership of the defaultValue.
          \post The array property will take the ownership of the elementType.
          \post The elementType will have array property as its parent.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        ArrayProperty(const std::string& name,
                      Property* elementType,
                      bool isRequired = false,
                      std::string* defaultValue = 0,
                      unsigned int id = 0,
                      Property* parent = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.

          \note This method is used by clone().
        */
        ArrayProperty(const ArrayProperty& rhs);

        /*! \brief Destructor. */
        ~ArrayProperty();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        ArrayProperty& operator=(const ArrayProperty& rhs);

        /*!
          \brief It returns the type of array elements.

          \return The type of array elements.
        */
        Property* getElementType() const { return m_elementType; }

        /*!
          \brief It sets the type of array elements.

          \param t The type of array elements.

          \post The ArrayProperty will take the ownership of the given element type.
          \post The elementType will have ArrayProperty as its parent.
        */
        void setElementType(Property* t);

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other elements.

          \return A clone of the object.
        */
        Property* clone() const;

      protected:
        
        Property* m_elementType;   //!< The data type of array elements.
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_ARRAYPROPERTY_H

