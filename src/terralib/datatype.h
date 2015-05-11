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
  \file terralib/datatype.h

  \brief This file contains include headers for the Data Type module of TerraLib.
*/

#ifndef __TERRALIB_INTERNAL_DATATYPE_H
#define __TERRALIB_INTERNAL_DATATYPE_H

// TerraLib
#include "datatype/AbstractData.h"
#include "datatype/ArrayProperty.h"
#include "datatype/BitProperty.h"
#include "datatype/ByteArray.h"
#include "datatype/CompositeData.h"
#include "datatype/CompositeProperty.h"
#include "datatype/Config.h"
#include "datatype/DataConverterManager.h"
#include "datatype/DataType.h"
#include "datatype/DataTypeManager.h"
#include "datatype/Date.h"
#include "datatype/DateDuration.h"
#include "datatype/DatePeriod.h"
#include "datatype/DateTime.h"
#include "datatype/DateTimeInstant.h"
#include "datatype/DateTimePeriod.h"
#include "datatype/DateTimeProperty.h"
#include "datatype/DateTimeUtils.h"
#include "datatype/Enums.h"
#include "datatype/NumericProperty.h"
#include "datatype/OrdinalInstant.h"
#include "datatype/OrdinalPeriod.h"
#include "datatype/Property.h"
#include "datatype/SimpleData.h"
#include "datatype/SimpleProperty.h"
#include "datatype/StringProperty.h"
#include "datatype/TimeDuration.h"
#include "datatype/TimeInstant.h"
#include "datatype/TimeInstantTZ.h"
#include "datatype/TimePeriod.h"
#include "datatype/TimePeriodTZ.h"
#include "datatype/Utils.h"

/*!
  \defgroup datatype Data Type

  \brief The data type module implements the type system supported
         by TerraLib for dealing with data that comes from
         different data sources.

    It has an important role in TerraLib
    context since each data source has its own set of data types
    used for representing and storing data. This module works
    integrated with the dataaccess module.

    The main classes/concepts in this module are listed here. The namespace associated to the Data Type module is te::dt.
    To know more about it, see the te::dt namespace documentation.
*/

namespace te
{
  /*!
    \brief Namespace for the Data Type module of TerraLib.
  */
  namespace dt
  {
  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_DATATYPE_H

