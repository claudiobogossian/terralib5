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
  \file terralib/qt/widgets/charts/ChartFrameFactory.cpp

  \brief A factory to build the Chart frame object.
*/

#include "ChartStyleFrame.h"
#include "ChartStyleFrameFactory.h"

te::qt::widgets::ChartStyleFrameFactory* te::qt::widgets::ChartStyleFrameFactory::sm_factory(0);

void te::qt::widgets::ChartStyleFrameFactory::initialize()
{
  finalize();
  sm_factory = new ChartStyleFrameFactory;
}

void te::qt::widgets::ChartStyleFrameFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::widgets::ChartStyleFrameFactory::~ChartStyleFrameFactory()
{

}

te::qt::widgets::ChartWidget* te::qt::widgets::ChartStyleFrameFactory::build()
{
  return new ChartStyleFrame;
}

te::qt::widgets::ChartStyleFrameFactory::ChartStyleFrameFactory()
  : te::qt::widgets::ChartWidgetFactory("Chart Style")
{

}

te::qt::widgets::ChartWidget* te::qt::widgets::ChartStyleFrameFactory::create(QWidget* parent)
{
  return new ChartStyleFrame(parent);
}