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
  \file terralib/datatype/DateTimeUtils.h

  \brief Utilitary function for the date and time types of the data type module.
*/

// TerraLib
#include "Date.h"
#include "DateDuration.h"
#include "DatePeriod.h"
#include "DateTimePeriod.h"
#include "DateTimeInstant.h"
#include "DateTime.h"
#include "DateTimeUtils.h"
#include "Enums.h"
#include "OrdinalInstant.h"
#include "OrdinalPeriod.h"
#include "TimeDuration.h"
#include "TimeInstant.h"
#include "TimeInstantTZ.h"
#include "TimePeriod.h"
#include "TimePeriodTZ.h"

te::dt::DateTimePeriod* te::dt::GetTemporalExtent(const te::dt::DateTime* t1, const te::dt::DateTime* t2)
{
  const te::dt::DateTime* initialTime = t1;
  const te::dt::DateTime* finalTime = t2;
  if(t1->operator>(*t2))
  {
    initialTime = t2;
    finalTime = t1;
  }

  DateTimeType timeType = initialTime->getDateTimeType();
   
  switch(timeType)
  {
    case DATE:
      return new DatePeriod(*(dynamic_cast<const Date*>(initialTime)), *(dynamic_cast<const Date*>(finalTime)));
    case DATE_PERIOD:
     return new DatePeriod(dynamic_cast<const DatePeriod*>(initialTime)->getInitialDate(), dynamic_cast<const DatePeriod*>(finalTime)->getFinalDate());
    case TIME_INSTANT:
      return new TimePeriod(*(dynamic_cast<const TimeInstant*>(initialTime)), *(dynamic_cast<const TimeInstant*>(finalTime)));
    case TIME_PERIOD:
      return new TimePeriod(dynamic_cast<const TimePeriod*>(initialTime)->getInitialTimeInstant(), 
                              dynamic_cast<const TimePeriod*>(finalTime)->getFinalTimeInstant());
    case TIME_INSTANT_TZ:
      return new TimePeriodTZ(*(dynamic_cast<const TimeInstantTZ*>(initialTime)), *(dynamic_cast<const TimeInstantTZ*>(finalTime)));
    case TIME_PERIOD_TZ:
      return new TimePeriodTZ(dynamic_cast<const TimePeriodTZ*>(initialTime)->getInitialTimeInstant(), 
                                dynamic_cast<const TimePeriodTZ*>(finalTime)->getFinalTimeInstant());
    case ORDINAL_TIME_INSTANT:
      return new OrdinalPeriod(dynamic_cast<const OrdinalInstant*>(initialTime)->getTimeInstant(), 
                                 dynamic_cast<const OrdinalInstant*>(finalTime)->getTimeInstant());
    case ORDINAL_TIME_PERIOD:
      return new OrdinalPeriod(dynamic_cast<const OrdinalPeriod*>(initialTime)->getInitialTime(), 
                                 dynamic_cast<const OrdinalPeriod*>(finalTime)->getFinalTime());
    default:
      return 0;
  }
}    

long te::dt::GetDistance(const te::dt::DateTime* t1, const te::dt::DateTime* t2)
{
  long output = 0;

  DateTimeType timeType = t1->getDateTimeType();
  switch(timeType)
  {
    case DATE:
      output = dynamic_cast<const Date*>(t1)->operator-(*(dynamic_cast<const Date*>(t2))); 
      return abs(output);
    case TIME_INSTANT:
      output = dynamic_cast<const TimeInstant*>(t1)->operator-(*(dynamic_cast<const TimeInstant*>(t2))); 
      return abs(output);  
    case TIME_INSTANT_TZ:
      output = dynamic_cast<const TimeInstantTZ*>(t1)->operator-(*(dynamic_cast<const TimeInstantTZ*>(t2)));  
      return abs(output);
    case ORDINAL_TIME_INSTANT:
      output =  (long)abs(dynamic_cast<const OrdinalInstant*>(t1)->getTimeInstant().getValue() - 
                    dynamic_cast<const OrdinalInstant*>(t2)->getTimeInstant().getValue());  
      return abs(output);

    default:
    break;
   }

   if(timeType==DATE_PERIOD)
   {
      Date date1 = dynamic_cast<const DatePeriod*>(t1)->getFinalDate();
      Date date2 = dynamic_cast<const DatePeriod*>(t2)->getInitialDate();
      if(t1->operator>(*t2))
      {
        date1 = dynamic_cast<const DatePeriod*>(t2)->getFinalDate();
        date2 = dynamic_cast<const DatePeriod*>(t1)->getInitialDate();
      }
      return abs(date2 - date1);    
   }

   if(timeType==TIME_PERIOD)
   {
      TimeInstant instant1 = dynamic_cast<const TimePeriod*>(t1)->getFinalTimeInstant();
      TimeInstant instant2 = dynamic_cast<const TimePeriod*>(t2)->getInitialTimeInstant();
      if(t1->operator>(*t2))
      {
        instant1 = dynamic_cast<const TimePeriod*>(t2)->getFinalTimeInstant();
        instant2 = dynamic_cast<const TimePeriod*>(t1)->getInitialTimeInstant();
      }
      return abs(instant2 - instant1);    
   }

   if(timeType==TIME_PERIOD_TZ)
   {
      TimeInstantTZ instant1 = dynamic_cast<const TimePeriodTZ*>(t1)->getFinalTimeInstant();
      TimeInstantTZ instant2 = dynamic_cast<const TimePeriodTZ*>(t2)->getInitialTimeInstant();
      if(t1->operator>(*t2))
      {
        instant1 = dynamic_cast<const TimePeriodTZ*>(t2)->getFinalTimeInstant();
        instant2 = dynamic_cast<const TimePeriodTZ*>(t1)->getInitialTimeInstant();
      }
      return abs(instant2 - instant1);    
   }
   
   if(timeType==ORDINAL_TIME_PERIOD)
   {
      int64_t instant1 = dynamic_cast<const OrdinalPeriod*>(t1)->getFinalTime().getValue();
      int64_t instant2 = dynamic_cast<const OrdinalPeriod*>(t2)->getInitialTime().getValue();
      if(t1->operator>(*t2))
      {
        instant1 = dynamic_cast<const OrdinalPeriod*>(t2)->getFinalTime().getValue();
        instant2 = dynamic_cast<const OrdinalPeriod*>(t1)->getInitialTime().getValue();
      }
      return (long)abs(instant2 - instant1);    
   }
  
   return output;
}    

te::dt::DateTimePeriod* te::dt::GetTimePeriod(const DateTimeInstant* t1, const DateTimeInstant* t2)
{
  DateTimeType timeType = t1->getDateTimeType();
  te::dt::DateTimePeriod* output = 0;

  switch(timeType)
  {
    case DATE:
      output = new DatePeriod(*dynamic_cast<const Date*>(t1), *dynamic_cast<const Date*>(t2)); 
      return output;
    case TIME_INSTANT:
      output = new TimePeriod(*dynamic_cast<const TimeInstant*>(t1), *dynamic_cast<const TimeInstant*>(t2)); 
      return output;  
    case TIME_INSTANT_TZ:
      output = new TimePeriodTZ(*dynamic_cast<const TimeInstantTZ*>(t1), *dynamic_cast<const TimeInstantTZ*>(t2)); 
      return output;
    case ORDINAL_TIME_INSTANT:
      output = new OrdinalPeriod( dynamic_cast<const OrdinalInstant*>(t1)->getTimeInstant().getValue(), 
                                  dynamic_cast<const OrdinalInstant*>(t2)->getTimeInstant().getValue()); 
      return output;
    default:
      return output;
   }
}

