/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Coverage.cpp

  \brief This file contains an abstract class to represent a coverage.
*/

//ST
#include "Coverage.h"

te::st::Coverage::Coverage()
{
}

/*
te::st::Coverage* te::st::Coverage::after(te::dt::DateTime* dt) const
{
  te::dt::DateTimePeriod* period = getTemporalExtent();
  std::unique_ptr<te::dt::DateTimeInstant> finalTime(period->getFinalInstant());
  
  if(dt->operator<(*finalTime.get()))
    return static_cast<te::st::Coverage*>(clone());
  return 0;
}

te::st::Coverage* te::st::Coverage::afterOrAt(te::dt::DateTime* dt) const
{
  te::dt::DateTimePeriod* period = getTemporalExtent();
  std::unique_ptr<te::dt::DateTimeInstant> finalTime(period->getFinalInstant());
  
  if(dt->operator<(*finalTime.get()) || dt->operator==(*finalTime.get()))
    return static_cast<te::st::Coverage*>(clone());
  return 0; 
}

te::st::Coverage* te::st::Coverage::during(te::dt::DateTimePeriod* dt, bool includeInitTime, bool includeFinalTime) const
{
  te::dt::DateTimePeriod* period = getTemporalExtent();
  std::unique_ptr<te::dt::DateTimeInstant> initialTime(period->getInitialInstant());
  std::unique_ptr<te::dt::DateTimeInstant> finalTime(period->getFinalInstant());
  
  if( (dt->operator>(*initialTime.get()) || dt->operator==(*initialTime.get()))
      && (dt->operator<(*finalTime.get()) || dt->operator==(*finalTime.get())))
  {
    return static_cast<te::st::Coverage*>(clone());
  }
  
  return 0;
}

te::st::Coverage* te::st::Coverage::before(te::dt::DateTime* dt) const
{
  te::dt::DateTimePeriod* period = getTemporalExtent();
  std::unique_ptr<te::dt::DateTimeInstant> initialTime(period->getInitialInstant());
  
  if(dt->operator>(*initialTime.get()))
    return static_cast<te::st::Coverage*>(clone());
  return 0;
}

te::st::Coverage* te::st::Coverage::beforeOrAt(te::dt::DateTime* dt) const
{
  te::dt::DateTimePeriod* period = getTemporalExtent();
  std::unique_ptr<te::dt::DateTimeInstant> initialTime(period->getInitialInstant());
  
  if(dt->operator>(*initialTime.get()) || dt->operator==(*initialTime.get()))
    return static_cast<te::st::Coverage*>(clone());
  return 0;
}
*/


