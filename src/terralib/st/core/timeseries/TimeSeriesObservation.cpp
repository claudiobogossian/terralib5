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
  \file TimeSeriesObservation.cpp

  \brief This file contains a class to represent a time series observation.
*/

//ST
#include "TimeSeriesObservation.h"


te::st::TimeSeriesObservation::TimeSeriesObservation(te::dt::DateTime* t, te::dt::AbstractData* v) 
  : m_time(t), m_value(v)
{
}

te::dt::DateTime* te::st::TimeSeriesObservation::getTime() const
{
  return m_time.get();
}

void te::st::TimeSeriesObservation::setTime(te::dt::DateTime* t)
{
  m_time.reset(t);
}

te::dt::AbstractData* te::st::TimeSeriesObservation::getValue() const
{
  return m_value.get();
}

double te::st::TimeSeriesObservation::getDouble() const
{
  return atof(m_value->toString().c_str());
}

int te::st::TimeSeriesObservation::getInt() const
{
  return atoi(m_value->toString().c_str());
}

std::string te::st::TimeSeriesObservation::getString() const
{
  return m_value->toString();
}

bool te::st::TimeSeriesObservation::operator<(const TimeSeriesObservation& other) const
{
  return m_time->operator<(*other.getTime());
}

te::st::TimeSeriesObservation::~TimeSeriesObservation()
{
}

