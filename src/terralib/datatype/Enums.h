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
  \file terralib/datetype/Enums.h

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

      \brief The subtype of date and time type.
    */
    enum DateTimeType
    {
      DATE,                 /*!< Date data type.                                                            */
      DATE_PERIOD,          /*!< Date period data type.                                                     */
      DATE_DURATION,        /*!< Date duration data type.                                                   */
      TIME_DURATION,        /*!< Time period data type.                                                     */
      TIME_INSTANT,         /*!< Time instant data type (both date and local time).                         */
      TIME_PERIOD,          /*!< Time period data type (both date and local time).                          */
      TIME_INSTANT_TZ,      /*!< Time instant with time zone data type (both date and time with time zone). */
      TIME_PERIOD_TZ,       /*!< Time period with time zone data type (both date and time with time zone).  */
      ORDINAL_INSTANT,      /*!< Ordinal time instant.  */
      ORDINAL_PERIOD        /*!< Ordinal time period.   */
    };

    /*!
      \enum DateTimeResolution

      \brief The date and time resolution.
    */
    enum DateTimeResolution
    {
      YEAR,                 /*!< Year.                  */
      MONTH,                /*!< Month of the year.     */
      DAY,                  /*!< Day of the month.      */
      HOUR,                 /*!< Hour of the day.       */
      MINUTE,               /*!< Minute of the hour.    */
      SECOND,               /*!< Second of the minute.  */         
      WEEK,                 /*!< Week of the year.      */   
      DAYOFWEEK,            /*!< Day of week: from 1 (Monday) to 7 (Sunday). */      
      DAYOFYEAR,            /*!< Day of year.             */   
      UNKNOWN               /*!< Unknown time resolution. */   
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
      UNKNOWN_TYPE = 0,     /*!< Used when the data type is unknown.                         */
      VOID_TYPE = 1,        /*!< For void data types.                                        */
      BIT_TYPE = 2,         /*!< Used for 1 bit data types.                                  */
      CHAR_TYPE = 3,        /*!< Character data type (1 byte long).                          */
      UCHAR_TYPE = 4,       /*!< Unsigned character data type (1 byte long).                 */
      INT16_TYPE = 5,       /*!< Integer number data type (2 bytes long).                    */
      UINT16_TYPE = 6,      /*!< Unsigned Integer number data type (2 bytes long).           */
      INT32_TYPE = 7,       /*!< Signed Integer number data type (4 bytes long).             */
      UINT32_TYPE = 8,      /*!< Unsigned Integer number data type (4 bytes long).           */
      INT64_TYPE = 9,       /*!< Signed Integer number data type (8 bytes long).             */
      UINT64_TYPE = 10,     /*!< Unsigned Integer number data type (8 bytes long).           */
      BOOLEAN_TYPE = 11,    /*!< Boolean type (true or false).                               */
      FLOAT_TYPE = 12,      /*!< Float number (32 bits) data type.                           */
      DOUBLE_TYPE = 13,     /*!< Double number (64 bits) data type.                          */
      NUMERIC_TYPE = 14,    /*!< Arbitrary precision data type: Numeric(p, q).               */
      STRING_TYPE = 15,     /*!< String data type.                                           */
      BYTE_ARRAY_TYPE = 16, /*!< Binary data (BLOB).                                         */
      GEOMETRY_TYPE = 17,   /*!< Geometry data type.                                         */
      DATETIME_TYPE = 18,   /*!< Date and time data type.                                    */
      ARRAY_TYPE = 19,      /*!< Array of homogeneous elements.                              */
      COMPOSITE_TYPE = 20,  /*!< Composed type.                                              */
      DATASET_TYPE = 21,    /*!< When the type is a DataSet.                                 */
      RASTER_TYPE = 22,     /*!< When the type is a Raster.                                  */
      CINT16_TYPE = 23,     /*!< Complex signed integer number data type (2 + 2 bytes long). */
      CINT32_TYPE = 24,     /*!< Complex signed integer number data type (4 + 4 bytes long). */
      CFLOAT_TYPE = 25,     /*!< Complex float number (32 + 32 bits) data type.              */
      CDOUBLE_TYPE = 26,    /*!< Complex double number (64 + 64 bits) data type.             */
      XML_TYPE = 27,        /*!< XML data type                                               */
      DATASETITEM_TYPE = 28,/*!< When the type is a DataSetItem                              */
      POLYMORPHIC_TYPE = 29 /*!< When the type is polymorphic                                */
    };

    /*! \brief An anonymous union that marks the last common data type supported by TerraLib. */
    enum
    {
      LAST_COMMON_DATATYPE_CODE = POLYMORPHIC_TYPE
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATETYPE_INTERNAL_ENUMS_H

