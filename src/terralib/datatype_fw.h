/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file datatype_fw.h

  \brief This file contains forward declarations for the Data Type module of TerraLib.
 */

#ifndef __TERRALIB_INTERNAL_DATATYPE_FW_H
#define __TERRALIB_INTERNAL_DATATYPE_FW_H

// TerraLib
namespace te
{
  namespace dt
  {
    class AbstractData;
    class AbstractDataConverter;
    class ArrayProperty;
    class ByteArray;
    class CompositeData;
    class CompositeProperty;
    class DataConverterManager;
    class DataType;
    class DataTypeManager;
    class Date;
    class DateDuration;
    class DatePeriod;
    class DateTime;
    class DateTimeInstant;
    class DateTimePeriod;
    class DateTimeProperty;
    class NumericProperty;
    class OrdinalInstant;
    class OrdinalPeriod;
    class Property;
    class SimpleProperty;
    class StringProperty;
    class TimeDuration;
    class TimeInstant;
    class TimeInstantTZ;
    class TimePeriod;
    class TimePeriodTZ;    

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_DATATYPE_FW_H

