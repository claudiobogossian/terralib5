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
  \file terralib/datatype/NumericProperty.h

  \brief The type for arbitrary precision numbers, like numeric(p, q).
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_NUMERICPROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_NUMERICPROPERTY_H

// TerraLib
#include "SimpleProperty.h"

namespace te
{
  namespace dt
  {
    /*!
      \class NumericProperty

      \brief The type for arbitrary precison numbers, like numeric(p, q).

      \ingroup datatype

      \sa SimpleProperty, Property, StringProperty, ArrayProperty, DateTimeProperty, CompositeProperty
    */
    class TEDATATYPEEXPORT NumericProperty : public SimpleProperty
    {
      public:


        /*!
          \brief It constructs a new NumericProperty.

          \param name         The attribute name.
          \param precision    The total count of significant digits in the whole number, that is, the number of digits to both sides of the decimal point.
          \param scale        The count of decimal digits in the fractional part, to the right of the decimal point.
          \param isRequired   Indicates if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.          
          
          \post The NumericProperty will take the ownership of the defaultValue.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        NumericProperty(const std::string& name,
                        unsigned int precision,
                        unsigned int scale,
                        bool isRequired = false,
                        std::string* defaultValue = 0,
                        unsigned int id = 0,
                        Property* parent = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        NumericProperty(const NumericProperty& rhs);        

        /*! \brief Destructor. */
        ~NumericProperty() {}

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        NumericProperty& operator=(const NumericProperty& rhs);

        /*!
          \brief It returns the total count of significant digits in the whole number,
                 that is, the number of digits to both sides of the decimal point.

          \return The total number of digits of the numeric type.
        */
        unsigned int getPrecision() const { return m_precision; }

       /*!
          \brief Sets the total count of significant digits in the whole number,
                 that is, the number of digits to both sides of the decimal point.
        */
        void setPrecision(unsigned int precision) { m_precision = precision; }

        /*!
          \brief It returns the count of decimal digits in the fractional part, to the right of the decimal point.

          \return The count of decimal digits in the fractional part, to the right of the decimal point.
        */
        unsigned int getScale() const { return m_scale; }

        /*!
          \brief Sets the count of decimal digits in the fractional part, to the right of the decimal point.
        */
        void setScale(unsigned int scale) { m_scale = scale; }

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other properties.

          \return A clone of the object.
        */
        Property* clone() const;

      protected:

        unsigned int m_precision;    //!< Total number of digits of a numeric value.
        unsigned int m_scale;        //!< Number of digits for the right of the decimal point of a numeric value.
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_NUMERICPROPERTY_H

