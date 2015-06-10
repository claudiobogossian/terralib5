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
  \file terralib/datatype/Enums.h

  \brief General enumerations for the data type module.
*/

#ifndef __TERRALIB_DATETYPE_INTERNAL_ENUMS_H
#define __TERRALIB_DATETYPE_INTERNAL_ENUMS_H

namespace te
{
  namespace dt
  {
    /*!
      \enum DateTimeType

      \brief The subtype of date and time type, based on ISO 8621.

      There are specific data types in this module to represent the types: 
      DATE, DATE_PERIOD, DATE_DURATION,  TIME_DURATION, TIME_INSTANT, TIME_PERIOD,
      TIME_INSTANT_TZ, TIME_PERIOD_TZ

      The type ORDINAL_TIME is represented by the te::dt::Int type. 
      The type STRING_TIME is represented by the te::dt::String type. 
    */
    enum DateTimeType
    {
      DATE,                 /*!< A gregorian date. Example: 10/01/2013                                                            */
      DATE_PERIOD,          /*!< A date period represents a range between two dates.                                                     */
      DATE_DURATION,        /*!< Date duration is a simple day count used for arithmetic with DATE. Example: 3                                                    */
      TIME_DURATION,        /*!< A length of time with nano-second/micro-second resolution (hh:mm:ss,ss)                                                     */
      TIME_INSTANT,         /*!< Date and Time instant data type (both date and local time).                         */
      TIME_PERIOD,          /*!< Date and time period data type (both date and local time).                          */
      TIME_INSTANT_TZ,      /*!< Time instant with time zone data type (both date and time with time zone). */
      TIME_PERIOD_TZ,       /*!< Time period with time zone data type (both date and time with time zone).  */
      ORDINAL_TIME_INSTANT, /*!< Ordinal time instant, for example: 1 -> Monday / 2 -> Tuesday /  */
      ORDINAL_TIME_PERIOD,  /*!< A period of ordinal time   */
      STRING_TIME,           /*!< A String that represents an instant of time "1999", "99".   */
      UNKNOWN
    };

    /*!
      \enum DateTimeStringFormat

      \brief The format of a string that represents a date and time. 

      It is based on the date and time representations of ISO 8621. 
      It must be used in the DateTimeProperty when the type is STRING_TIME.
    */
    enum DateTimeStringFormat
    {
      YYYY,     /*!< A year with 4 numbers. Example: 1999 */
      YY,       /*!< A year with 2 numbers. Example: 99 */
      YYYYMM,   /*!< A year with 4 numbers and the month. Example: 199903 */
      YYYYDDD,  /*!< A year with 4 numbers and the ordianl day of the year. Example: 1999360 */
      UNKNOWN_STRING_FORMAT   /*!< Unknown string format. */
    };

    /*!
      \enum DateTimeOrdinalType

      \brief The meaning of the ordial time.  

      It must be used in the DateTimeProperty when the type is ORDINAL_TIME.
    */
    enum DateTimeOrdinalType
    {
      MONTH_OF_YEAR,    /*!< from 1 (January) to 12 (December) */
      WEEK_OF_YEAR,     /*!< from 1 to 52 */
      DAY_OF_WEEK,      /*!< from 1 (Monday) to 7 (Sunday) */
      DAY_OF_MONTH,     /*!< from 1 to 31 */
      DAY_OF_YEAR,      /*!< from 001 (01/01) to 365 ou 366 (31/12) */
      HOUR_OF_DAY,      /*!< from 0 to 23 */
      MINUTE_OF_HOUR,   /*!< from 0 to 59 */
      SECOND_OF_MINUTE, /*!< from 0 to 59 */
      USER_DEFINED      /*!< A ordinal number defined by the user */
    };
    
    /*!
      \enum TemporalUnit

      \brief The unit of the temporal data 
    */
    enum TemporalUnit
    {
      YEAR,                 /*!< Year.      */
      MONTH,                /*!< Month      */
      DAY,                  /*!< Day        */
      HOUR,                 /*!< Hour       */
      MINUTE,               /*!< Minute     */
      SECOND,               /*!< Second     */
      WEEK,                 /*!< Week       */
      UNKNOWN_TEMPORAL_UNIT /*!< Unknown time unit. */
    };

    /*!
      \struct TemporalResolution

      \brief A temporal resolution is composed of a unit of time and a value.

      A temporal resolution is composed of a unit and a value. For example,
      if the temporal resolution is 16 days, the unit is DAY and the value is 16.
    */
    struct TemporalResolution
    {
        TemporalUnit  m_unit;
        unsigned int  m_value;

        TemporalResolution() 
          : m_unit(UNKNOWN_TEMPORAL_UNIT), m_value(0) { }

        TemporalResolution(TemporalUnit unit, unsigned int val) 
          : m_unit(unit), m_value(val) { }
    };

    /*!
      \enum TemporalRelation

      \brief Temporal relations between date and time (Source: Allen, 1991).
    */
    enum TemporalRelation
    {  
      UNKNOWN_TEMPORAL_RELATION = 0,  /*!< */
      BEFORE = 1,                     /*!< */
      AFTER = 2,                      /*!< */
      DURING = 4,                     /*!< */
      EQUALS = 8,                     /*!< */
      MEETS = 16,                     /*!< */
      OVERLAPS = 32,                  /*!< */
      STARTS = 64,                    /*!< */
      FINISHES = 128
    };

    /*!
      \enum BasicRelation

      \brief Relations between simple attribute values. 
    */
    enum BasicRelation
    {  
      UNKNOWN_BASIC_RELATION = 0, /*!< */
      LESS = 1,                   /*!< */
      MORE = 2,                   /*!< */
      EQUAL = 4                   /*!< */
    };

    /*!
      \enum StringType

      \brief The subtype of string property.
    */
    enum StringType
    {
      FIXED_STRING, /*!< For fixed-length strings (blank padded if needed).  */
      VAR_STRING,   /*!< For variable length string with a limited size.     */
      STRING        /*!< For variable unlimited length string.               */
    };

    /*!
      \brief An anonymous union for all the basic data types supported by TerraLib.

      \warning If you change this list, please, update all code dependencies (for example: LAST_DATATYPE_CODE).
    */
    enum
    {
      UNKNOWN_TYPE = 0,      /*!< Used when the data type is unknown.                         */
      VOID_TYPE = 1,         /*!< For void data types.                                        */
      BIT_TYPE = 2,          /*!< Used for 1 bit data types.                                  */
      CHAR_TYPE = 3,         /*!< Character data type (1 byte long).                          */
      UCHAR_TYPE = 4,        /*!< Unsigned character data type (1 byte long).                 */
      INT16_TYPE = 5,        /*!< Integer number data type (2 bytes long).                    */
      UINT16_TYPE = 6,       /*!< Unsigned Integer number data type (2 bytes long).           */
      INT32_TYPE = 7,        /*!< Signed Integer number data type (4 bytes long).             */
      UINT32_TYPE = 8,       /*!< Unsigned Integer number data type (4 bytes long).           */
      INT64_TYPE = 9,        /*!< Signed Integer number data type (8 bytes long).             */
      UINT64_TYPE = 10,      /*!< Unsigned Integer number data type (8 bytes long).           */
      BOOLEAN_TYPE = 11,     /*!< Boolean type (true or false).                               */
      FLOAT_TYPE = 12,       /*!< Float number (32 bits) data type.                           */
      DOUBLE_TYPE = 13,      /*!< Double number (64 bits) data type.                          */
      NUMERIC_TYPE = 14,     /*!< Arbitrary precision data type: Numeric(p, q).               */
      STRING_TYPE = 15,      /*!< String data type.                                           */
      BYTE_ARRAY_TYPE = 16,  /*!< Binary data (BLOB).                                         */
      GEOMETRY_TYPE = 17,    /*!< Geometry data type.                                         */
      DATETIME_TYPE = 18,    /*!< Date and time data type.                                    */
      ARRAY_TYPE = 19,       /*!< Array of homogeneous elements.                              */
      COMPOSITE_TYPE = 20,   /*!< Composed type.                                              */
      DATASET_TYPE = 21,     /*!< When the type is a DataSet.                                 */
      RASTER_TYPE = 22,      /*!< When the type is a Raster.                                  */
      CINT16_TYPE = 23,      /*!< Complex signed integer number data type (2 + 2 bytes long). */
      CINT32_TYPE = 24,      /*!< Complex signed integer number data type (4 + 4 bytes long). */
      CFLOAT_TYPE = 25,      /*!< Complex float number (32 + 32 bits) data type.              */
      CDOUBLE_TYPE = 26,     /*!< Complex double number (64 + 64 bits) data type.             */
      XML_TYPE = 27,         /*!< XML data type.                                              */
      DATASETITEM_TYPE = 28, /*!< When the type is a DataSetItem.                             */
      POLYMORPHIC_TYPE = 29, /*!< When the type is polymorphic.                               */
      R4BITS_TYPE = 30,      /*!< When the type is a raster with 4 bits.                      */
      R2BITS_TYPE = 31,      /*!< When the type is a raster with 2 bits.                      */
      R1BIT_TYPE = 32        /*!< When the type is a raster with 1 bit.                       */
    };

    /*! \brief An anonymous union that marks the last common data type supported by TerraLib. */
    enum
    {
      LAST_COMMON_DATATYPE_CODE = POLYMORPHIC_TYPE
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATETYPE_INTERNAL_ENUMS_H

