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
  \file terralib/qt/widgets/charts/TimeSeriesFrameFactory.cpp

  \brief A factory to build the timeSeries frame object.
*/

//Terralib
#include "TimeSeriesFrame.h"
#include "TimeSeriesFrameFactory.h"

te::qt::widgets::TimeSeriesFrameFactory* te::qt::widgets::TimeSeriesFrameFactory::sm_factory(0);

void te::qt::widgets::TimeSeriesFrameFactory::initialize()
{
  finalize();
  sm_factory = new TimeSeriesFrameFactory;
}

void te::qt::widgets::TimeSeriesFrameFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::widgets::TimeSeriesFrameFactory::~TimeSeriesFrameFactory()
{

}

te::qt::widgets::ChartWidget* te::qt::widgets::TimeSeriesFrameFactory::build()
{
  return new TimeSeriesFrame;
}

te::qt::widgets::TimeSeriesFrameFactory::TimeSeriesFrameFactory()
  : te::qt::widgets::ChartWidgetFactory("TimeSeries Style")
{

}

te::qt::widgets::ChartWidget* te::qt::widgets::TimeSeriesFrameFactory::create(QWidget* parent)
{
  return new TimeSeriesFrame(parent);
}
