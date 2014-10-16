/*
 * File: DataType.i
 *
 * DataType module
 */

#define TEDATATYPEEXPORT 

%nspace te::dt::TemporalRelation;
%nspace te::dt::Property;
  
%{
#include "terralib/datatype/AbstractData.h"
#include "terralib/datatype/Array.h"
#include "terralib/datatype/ArrayProperty.h"
#include "terralib/datatype/BitProperty.h"
#include "terralib/datatype/BooleanConverters.h"
#include "terralib/datatype/ByteArray.h"
#include "terralib/datatype/ByteArrayProperty.h"
#include "terralib/datatype/CompositeData.h"
#include "terralib/datatype/CompositeProperty.h"
#include "terralib/datatype/DataConverterManager.h"
#include "terralib/datatype/DataType.h"
#include "terralib/datatype/DataTypeConverter.h"
#include "terralib/datatype/DataTypeManager.h"
#include "terralib/datatype/Date.h"
#include "terralib/datatype/DateDuration.h"
#include "terralib/datatype/DatePeriod.h"
#include "terralib/datatype/DateTime.h"
#include "terralib/datatype/DateTimeInstant.h"
#include "terralib/datatype/DateTimePeriod.h"
#include "terralib/datatype/DateTimeProperty.h"
#include "terralib/datatype/DateTimeUtils.h"
#include "terralib/datatype/Enums.h"
#include "terralib/datatype/Exception.h"
#include "terralib/datatype/IntegerConverters.h"
#include "terralib/datatype/NumericProperty.h"
#include "terralib/datatype/OrdinalInstant.h"
#include "terralib/datatype/OrdinalPeriod.h"
#include "terralib/datatype/Property.h"
#include "terralib/datatype/SimpleData.h"
#include "terralib/datatype/SimpleProperty.h"
#include "terralib/datatype/StringConverters.h"
#include "terralib/datatype/StringProperty.h"
#include "terralib/datatype/TimeDuration.h"
#include "terralib/datatype/TimeInstant.h"
#include "terralib/datatype/TimeInstantTZ.h"
#include "terralib/datatype/TimePeriod.h"
#include "terralib/datatype/TimePeriodTZ.h"
#include "terralib/datatype/Utils.h"
%}

%include "terralib/datatype/Enums.h"
%include "terralib/datatype/AbstractData.h"
%include "terralib/datatype/Array.h"
%include "terralib/datatype/Property.h"
%include "terralib/datatype/SimpleProperty.h"
%include "terralib/datatype/ArrayProperty.h"
%include "terralib/datatype/BitProperty.h"

/* %include "terralib/datatype/BooleanConverters.h" */

%include "terralib/datatype/ByteArray.h"
%include "terralib/datatype/ByteArrayProperty.h"
%include "terralib/datatype/CompositeData.h"
%include "terralib/datatype/CompositeProperty.h"
%include "terralib/datatype/DataType.h"
%include "terralib/datatype/DataTypeManager.h"
%include "terralib/datatype/DateTime.h"
%include "terralib/datatype/DateTimeInstant.h"

/* %include "terralib/datatype/Date.h" */
%import "terralib/datatype/Date.h" 

%include "terralib/datatype/DateDuration.h"
%include "terralib/datatype/DateTimePeriod.h"
%include "terralib/datatype/DatePeriod.h"
%include "terralib/datatype/DateTimeProperty.h"
%include "terralib/datatype/DateTimeUtils.h"
%include "terralib/datatype/DataTypeConverter.h" 
%include "terralib/datatype/DataConverterManager.h"

/* %include "terralib/datatype/IntegerConverters.h" */

%include "terralib/datatype/NumericProperty.h"
%include "terralib/datatype/SimpleData.h"
%include "terralib/datatype/OrdinalInstant.h"
%include "terralib/datatype/OrdinalPeriod.h"

/* %include "terralib/datatype/StringConverters.h" */

%include "terralib/datatype/StringProperty.h"
%include "terralib/datatype/TimeDuration.h"
%include "terralib/datatype/TimeInstant.h"
%include "terralib/datatype/TimeInstantTZ.h"
%include "terralib/datatype/TimePeriod.h"
%include "terralib/datatype/TimePeriodTZ.h"
%include "terralib/datatype/Utils.h"
