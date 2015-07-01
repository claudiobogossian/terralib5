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
  \file terralib/qt/widgets/charts/HistogramFrameFactory.cpp

  \brief A factory to build the histogram frame object.
*/

#include "HistogramFrame.h"
#include "HistogramFrameFactory.h"

te::qt::widgets::HistogramFrameFactory* te::qt::widgets::HistogramFrameFactory::sm_factory(0);

void te::qt::widgets::HistogramFrameFactory::initialize()
{
  finalize();
  sm_factory = new HistogramFrameFactory;
}

void te::qt::widgets::HistogramFrameFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::widgets::HistogramFrameFactory::~HistogramFrameFactory()
{

}

te::qt::widgets::ChartWidget* te::qt::widgets::HistogramFrameFactory::build()
{
  return new HistogramFrame;
}

te::qt::widgets::HistogramFrameFactory::HistogramFrameFactory()
  : te::qt::widgets::ChartWidgetFactory("Histogram Style")
{

}

te::qt::widgets::ChartWidget* te::qt::widgets::HistogramFrameFactory::create(QWidget* parent)
{
  return new HistogramFrame(parent);
}
