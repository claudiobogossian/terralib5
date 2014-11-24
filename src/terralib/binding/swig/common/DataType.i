/*
 * File: DataType.i
 *
 * DataType module
 */

#define TEDATATYPEEXPORT
#define BOOST_DATE_TIME_DECL 
  
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

%nspace te::dt::DateTimeType;
%nspace te::dt::DateTimeResolution;
%nspace te::dt::TemporalRelation;
%nspace te::dt::BasicRelation;
%nspace te::dt::StringType;
%nspace te::dt::AbstractData;
%nspace te::dt::Array;
%nspace te::dt::Property;
%nspace te::dt::SimpleProperty;
%nspace te::dt::ArrayProperty;
%nspace te::dt::BitProperty;
%nspace te::dt::ByteArray;
%nspace te::dt::ByteArrayProperty;
%nspace te::dt::CompositeData;
%nspace te::dt::CompositeProperty;
%nspace te::dt::Date;
%nspace te::dt::DataType;
%nspace te::dt::DataTypeManager;
%nspace te::dt::DateTime;
%nspace te::dt::DateTimeInstant;
%nspace te::dt::DateDuration;
%nspace te::dt::DateTimePeriod;
%nspace te::dt::DatePeriod;
%nspace te::dt::DateTimeProperty;
%nspace te::dt::DateTimeUtils;
%nspace te::dt::DataTypeConverter; 
%nspace te::dt::DataConverterManager;
%nspace te::dt::NumericProperty;
%nspace te::dt::SimpleData;
%nspace te::dt::OrdinalInstant;
%nspace te::dt::OrdinalPeriod;
%nspace te::dt::StringProperty;
%nspace te::dt::TimeDuration;
%nspace te::dt::TimeInstant;
%nspace te::dt::TimeInstantTZ;
%nspace te::dt::TimePeriod;
%nspace te::dt::TimePeriodTZ;

%nspace boost::gregorian::greg_year;
%nspace boost::gregorian::greg_month;
%nspace boost::gregorian::greg_day;

%include "boost/date_time/gregorian/greg_day.hpp"
%include "boost/date_time/gregorian/greg_month.hpp"
%include "boost/date_time/gregorian/greg_year.hpp"

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
%include "terralib/datatype/Date.h"
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
