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
  \file terralib/datatype/DateTimeProperty.h

  \brief The type for date and time types.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATETIMEPROPERTY_H
#define __TERRALIB_DATATYPE_INTERNAL_DATETIMEPROPERTY_H

// TerraLib
#include "Enums.h"
#include "SimpleProperty.h"
#include "TimeInstant.h"

namespace te
{
  namespace dt
  {
     /*!
      \class DateTimeProperty

      \brief The type for date and time types: date, date period, date duration, time duration, time instant, time period, time instant with time zone or time period with time zone.

      \ingroup datatype

      \sa SimpleProperty, Property, NumericProperty, ArrayProperty, StringProperty, CompositeProperty
    */
    class TEDATATYPEEXPORT DateTimeProperty : public SimpleProperty
    {
      public:

         /*!
          \brief It constructs a new date and time property.

          \param name         The attribute name.
          \param dateType     The date and time subtype. 
          \param isRequired   It indicates if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.          

          \post The date time property will take the ownership of the defaultValue.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        DateTimeProperty(const std::string& name,
                         DateTimeType dateType = DATE,
                         bool isRequired = false,
                         std::string* defaultValue = 0,
                         unsigned int id = 0,
                         Property* parent = 0);

        /*!
          \brief It constructs a new date and time property.

          \param name         The attribute name.
          \param dateType     The date and time subtype. 
          \param tmpRes       The temporal resolution.
          \param isRequired   It indicates if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.          

          \post The date time property will take the ownership of the defaultValue.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        DateTimeProperty(const std::string& name,
                         DateTimeType dateType,
                         TemporalResolution tmpRes,
                         bool isRequired = false,
                         std::string* defaultValue = 0,
                         unsigned int id = 0,
                         Property* parent = 0);

        /*!
          \brief It constructs a new date and time property that is represented by string

          \param name         The attribute name.
          \param tmpRes       The temporal resolution.
          \param strFormat    The format of the string that represents a time information
          \param isRequired   It indicates if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.          

          \post The date time property will take the ownership of the defaultValue.

          \note This constructor must be used when the data type is STRING_TIME

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        DateTimeProperty(const std::string& name,
                         TemporalResolution tmpRes,
                         DateTimeStringFormat strFormat,
                         bool isRequired = false,
                         std::string* defaultValue = 0,
                         unsigned int id = 0,
                         Property* parent = 0);

        /*!
          \brief It constructs a new date and time property that is represented by ordinal numbers.

          \param name         The attribute name.
          \param tmpRes       The temporal resolution.
          \param ordType      The type of the ordinal time
          \param startTime    The start time when the ordinal type is USER_DEFINED 
          \param isRequired   It indicates if the the property is required (mandatory) or not.
          \param defaultValue The default value to be used, if none is informed.          
          \param id           The property identifier.
          \param parent       A reference to the parent Property of the new object if it has one.          

          \post The date time property will take the ownership of the defaultValue.

          \note This constructor must be used when the data type is ORDINAL_TIME

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        DateTimeProperty(const std::string& name,
                         TemporalResolution tmpRes,
                         DateTimeOrdinalType ordType,
                         TimeInstant startTime,
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
          \brief It returns the temporal resolution.

          \return The temporal resolution.
        */
        TemporalResolution getResolution() const { return m_tempResolution; }

        /*!
          \brief It sets the temporal resolution.
          
          \param r The temporal resolution.
        */
        void setResolution(TemporalResolution t) { m_tempResolution = t; }

        /*!
          \brief It returns the string format

          \return The format of the string that represents the date and time
        */
        DateTimeStringFormat getStrFormat() const { return m_strFormat; }

        /*!
          \brief It sets the string format
          
          \param f The format of the string that represents the date and time
        */
        void setStrFormat(DateTimeStringFormat f) { m_strFormat = f; }

        /*!
          \brief It returns the ordinal type, when the date and time is represented by a ordinal number

          \return The ordinal type, when the date and time is represented by a ordinal number
        */
        DateTimeOrdinalType getOrdinalType() const { return m_ordinalType; }

        /*!
          \brief It sets the ordinal type
          
          \param t The ordinal type, when the date and time is represented by a ordinal number
        */
        void setOrdinalType(DateTimeOrdinalType t) { m_ordinalType = t; }

        /*!
          \brief It returns the start time associated to the user-defined ordinal date and time. 

          \return The start time associated to the user-defined ordinal date and time.
        */
        TimeInstant getOrdinalStartTime() const { return m_ordinalStartTime; }

        /*!
          \brief It sets the start time associated to the user-defined ordinal date and time.
          
          \param t The start time associated to the user-defined ordinal date and time.
        */
        void setOrdinalStartTime(TimeInstant t) { m_ordinalStartTime = t; }

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other properties.

          \return A clone of the object.
        */
        Property* clone() const;

      protected:

        DateTimeType          m_dSubType;         //!< The sub-type of this date time property.
        TemporalResolution    m_tempResolution;   //!< The temporal resolution.
        DateTimeStringFormat  m_strFormat;        //!< The string format when the data type is STRING_TIME
        DateTimeOrdinalType   m_ordinalType;      //!< The ordinal type when the data type is ORDINAL_TIME.
        TimeInstant           m_ordinalStartTime; //!< The start time when the ordinal type is USER_DEFINED 
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_DATETIMEPROPERTY_H

