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
  \file  terralib/qt/widgets/charts/TimeSeriesChart.cpp

  \brief A class to represent a timeSeries' chart.
*/

// TerraLib
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../datatype/Date.h"
#include "../../../datatype/DateTime.h"
#include "../../../datatype/TimeInstant.h"
#include "../../../qt/widgets/se/Utils.h"
#include "../../../st/core/timeseries/TimeSeries.h"
#include "../../../st/core/timeseries/TimeSeriesIterator.h"
#include "Enums.h"
#include "TimeSeriesChart.h"
#include "StringScaleDraw.h"
#include "TimeSeriesStyle.h"
#include "Utils.h"

//QWT
#include <qwt_symbol.h>
#include <qwt_plot.h>

//Boost
#include "boost/date_time/gregorian/gregorian.hpp"

//STL
#include <limits>


te::qt::widgets::TimeSeriesChart::TimeSeriesChart(te::st::TimeSeries* data) :
  QwtPlotCurve(),
  m_timeSeries(data)
{
  //Set style
  setStyle(QwtPlotCurve::Lines);
  setPaintAttribute(QwtPlotCurve::ClipPolygons);

  //Set Values
  setData();

  //Adjusting the symbol
  m_timeSeriesStyle =  new te::qt::widgets::TimeSeriesStyle();
  QPen linePen;
  te::qt::widgets::Config(linePen, m_timeSeriesStyle->getStroke());
}

te::qt::widgets::TimeSeriesChart::TimeSeriesChart(te::st::TimeSeries* data, TimeSeriesStyle* style, size_t size) :
  QwtPlotCurve(),
  m_timeSeries(data),
  m_timeSeriesStyle(style)
{
  //Set style
  setStyle(QwtPlotCurve::Lines);
  setPaintAttribute(QwtPlotCurve::ClipPolygons);

  //Set Values
  setData();
}

void te::qt::widgets::TimeSeriesChart::setData()
{
  QPolygonF samples;
  std::set<std::string> dates;
  boost::gregorian::date basedate(1400, 01, 01);

  te::st::TimeSeriesIterator it = m_timeSeries->begin();
  while(it != m_timeSeries->end())
  {
    te::dt::DateTime* dateTime = it.getTime();
    double date=0;

    if(dateTime->getDateTimeType() == te::dt::TIME_INSTANT)
    {
      std::auto_ptr<te::dt::TimeInstant> ti ((te::dt::TimeInstant*)dateTime);
      boost::gregorian::date_duration days = ti->getDate().getDate() - basedate;
      long long int boostDays = days.days();
      date = (double) boostDays;
      dates.insert(boost::gregorian::to_iso_extended_string(ti->getDate().getDate()));
    }
    else if(dateTime->getDateTimeType() == te::dt::DATE)
    {
      std::auto_ptr<te::dt::Date> d ((te::dt::Date*)dateTime);
      boost::gregorian::date_duration days = d->getDate() - basedate;
      date = days.days();
      dates.insert(boost::gregorian::to_iso_extended_string(d->getDate()));
    }
    samples += QPointF(date, it.getDouble());
    ++it;
  }

  m_scaleDraw = new te::qt::widgets::StringScaleDraw(dates);
  setSamples( samples );
}

te::qt::widgets::TimeSeriesChart::~TimeSeriesChart()
{
  delete m_timeSeriesStyle;
  delete m_scaleDraw;
}

int  te::qt::widgets::TimeSeriesChart::rtti() const
{
  return te::qt::widgets::TIMESERIES_CHART;
}

te::qt::widgets::StringScaleDraw* te::qt::widgets::TimeSeriesChart::getScaleDraw()
{
  return m_scaleDraw;
}

void te::qt::widgets::TimeSeriesChart::setScaleDraw( StringScaleDraw* newScaleDraw)
{
  delete m_scaleDraw;
  m_scaleDraw = newScaleDraw;
}

void te::qt::widgets::TimeSeriesChart::attach(QwtPlot* plot)
{
  //plot->setAxisScaleDraw(QwtPlot::xBottom, m_scaleDraw);
  plot->axisScaleDraw(QwtPlot::xBottom)->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  plot->axisScaleDraw(QwtPlot::xBottom)->setLabelRotation(-60);
  QwtPlotCurve::attach(plot);
}

te::st::TimeSeries* te::qt::widgets::TimeSeriesChart::getTimeSeries()
{
 return m_timeSeries;
}

void te::qt::widgets::TimeSeriesChart::setTimeSeries(te::st::TimeSeries* newTimeSeries)
{
  delete m_timeSeries;
  m_timeSeries = newTimeSeries;
}

te::qt::widgets::TimeSeriesStyle* te::qt::widgets::TimeSeriesChart::getTimeSeriesStyle()
{
 return m_timeSeriesStyle->clone();
}

void te::qt::widgets::TimeSeriesChart::setTimeSeriesStyle(te::qt::widgets::TimeSeriesStyle* newStyle)
{
  delete m_timeSeriesStyle;
  m_timeSeriesStyle = newStyle;

  //The pen that will be used
  QPen linePen;

  //Configuring the pen and brush based on the current style
  te::qt::widgets::Config(linePen, m_timeSeriesStyle->getStroke());

  //Updating the chart's pen.
  setPen(linePen);
}

