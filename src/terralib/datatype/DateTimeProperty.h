/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/DateTimeProperty.h

  \brief The type for date and time types.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATETIMEPROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_DATETIMEPROPERTY_H

// TerraLib
#include "Enums.h"
#include "SimpleProperty.h"

namespace te
{
  namespace dt
  {
    /*!
      \class DateTimeProperty

      \brief The type for date and time types: date, date period, date duration, time duration, time instant, time period, time instant with time zone or time period with time zone.

      \sa SimpleProperty, Property, NumericProperty, ArrayProperty, StringProperty, CompositeProperty
    */
    class TEDATATYPEEXPORT DateTimeProperty : public SimpleProperty
    {
      public:

          /*!
          \brief It constructs a new date and time property.

          \param name         The attribute name.
          \param dateType     The date and time subtype. Must be one of: TIME_STAMP, TIME_STAMP_TZ, DATE, DATETIME, TIME or TIME_TZ.
          \param isRequired   It indicates if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.          

          \post The date time property will take the ownership of the defaultValue.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        DateTimeProperty(const std::string& name,
                         DateTimeType dateType,
                         DateTimeResolution dtResolution = UNKNOWN,
                         bool isRequired = false,
                         std::string* defaultValue = 0,
                         unsigned int id = 0,
                         Property* parent = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        DateTimeProperty(const DateTimeProperty& rhs);

        /*! \brief Destructor. */
        ~DateTimeProperty() {}

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        DateTimeProperty& operator=(const DateTimeProperty& rhs);

        /*!
          \brief It returns the date time property sub type.

          \return The string property sub type.
        */
        DateTimeType getSubType() const { return m_dSubType; }

        /*!
          \brief It sets the date time property sub type.
          
          \param t The date time property sub type.
        */
        void setSubtype(DateTimeType t) { m_dSubType = t; }

        /*!
          \brief It returns the date time resolution.

          \return The date and time resolution.
        */
        DateTimeResolution getResolution() const { return m_dtResolution; }

        /*!
          \brief It sets the date time resolution.
          
          \param r The date time resolution.
        */
        void setResolution(DateTimeResolution r) { m_dtResolution = r; }

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other properties.

          \return A clone of the object.
        */
        Property* clone() const;

      protected:

        DateTimeType        m_dSubType;       //!< The sub-type of this date time property.
        DateTimeResolution  m_dtResolution;   //!< The date and time resolution.
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_DATETIMEPROPERTY_H

