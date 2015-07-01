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
  \file terralib/qt/widgets/se/BasicFillWidgetFactory.cpp

  \brief A concrete factory that builds a fill widget used to configure a basic fill element.
*/

// TerraLib
#include "BasicFillWidget.h"
#include "BasicFillWidgetFactory.h"

// Factory key
std::string te::qt::widgets::BasicFillWidgetFactory::sm_factoryKey("BasicFill");

// Global factory
te::qt::widgets::BasicFillWidgetFactory* te::qt::widgets::BasicFillWidgetFactory::sm_factory(0);

void te::qt::widgets::BasicFillWidgetFactory::initialize()
{
  finalize();
  sm_factory = new BasicFillWidgetFactory;
}

void te::qt::widgets::BasicFillWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::widgets::BasicFillWidgetFactory::~BasicFillWidgetFactory()
{
}

te::qt::widgets::AbstractFillWidget* te::qt::widgets::BasicFillWidgetFactory::build()
{
  return new BasicFillWidget;
}

te::qt::widgets::BasicFillWidgetFactory::BasicFillWidgetFactory()
  : te::qt::widgets::AbstractFillWidgetFactory(sm_factoryKey)
{
}
