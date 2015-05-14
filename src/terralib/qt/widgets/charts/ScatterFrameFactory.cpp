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
  \file terralib/qt/widgets/charts/ScatterFrameFactory.cpp

  \brief A factory to build the scatter frame object.
*/

//Terralib
#include "ScatterFrame.h"
#include "ScatterFrameFactory.h"

te::qt::widgets::ScatterFrameFactory* te::qt::widgets::ScatterFrameFactory::sm_factory(0);

void te::qt::widgets::ScatterFrameFactory::initialize()
{
  finalize();
  sm_factory = new ScatterFrameFactory;
}

void te::qt::widgets::ScatterFrameFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::widgets::ScatterFrameFactory::~ScatterFrameFactory()
{

}

te::qt::widgets::ChartWidget* te::qt::widgets::ScatterFrameFactory::build()
{
  return new ScatterFrame;
}

te::qt::widgets::ScatterFrameFactory::ScatterFrameFactory()
  : te::qt::widgets::ChartWidgetFactory("Scatter Style")
{

}

te::qt::widgets::ChartWidget* te::qt::widgets::ScatterFrameFactory::create(QWidget* parent)
{
  return new ScatterFrame(parent);
}
