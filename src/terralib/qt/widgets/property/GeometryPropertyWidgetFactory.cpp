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
  \file terralib/qt/widget/property/GeometryPropertyWidgetFactory.h

  \brief Defines the class GeometryPropertyWidgetFactory.
*/

#include "GeometryPropertyWidget.h"
#include "GeometryPropertyWidgetFactory.h"

te::qt::widgets::GeometryPropertyWidgetFactory* te::qt::widgets::GeometryPropertyWidgetFactory::sm_factory(0);

void te::qt::widgets::GeometryPropertyWidgetFactory::initialize()
{
  finalize();
  sm_factory = new GeometryPropertyWidgetFactory;
}

void te::qt::widgets::GeometryPropertyWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

int te::qt::widgets::GeometryPropertyWidgetFactory::getType() const
{
  return te::dt::GEOMETRY_TYPE;
}

te::qt::widgets::GeometryPropertyWidgetFactory::~GeometryPropertyWidgetFactory()
{

}

te::qt::widgets::SimplePropertyWidget* te::qt::widgets::GeometryPropertyWidgetFactory::build()
{
  return new GeometryPropertyWidget;
}

te::qt::widgets::GeometryPropertyWidgetFactory::GeometryPropertyWidgetFactory()
  : te::qt::widgets::SimplePropertyWidgetFactory(te::dt::GEOMETRY_TYPE)
{

}

te::qt::widgets::SimplePropertyWidget* te::qt::widgets::GeometryPropertyWidgetFactory::create(QWidget* parent)
{
  return new GeometryPropertyWidget(parent);
}