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
  \file terralib/qt/widget/property/DateTimePropertyWidgetFactory.h

  \brief Defines the class DateTimePropertyWidgetFactory.
*/

#include "DateTimePropertyWidget.h"
#include "DateTimePropertyWidgetFactory.h"

te::qt::widgets::DateTimePropertyWidgetFactory* te::qt::widgets::DateTimePropertyWidgetFactory::sm_factory(0);

void te::qt::widgets::DateTimePropertyWidgetFactory::initialize()
{
  finalize();
  sm_factory = new DateTimePropertyWidgetFactory;
}

void te::qt::widgets::DateTimePropertyWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

int te::qt::widgets::DateTimePropertyWidgetFactory::getType() const
{
  return te::dt::DATETIME_TYPE;
}

te::qt::widgets::DateTimePropertyWidgetFactory::~DateTimePropertyWidgetFactory()
{

}

te::qt::widgets::SimplePropertyWidget* te::qt::widgets::DateTimePropertyWidgetFactory::build()
{
  return new DateTimePropertyWidget;
}

te::qt::widgets::DateTimePropertyWidgetFactory::DateTimePropertyWidgetFactory()
  : te::qt::widgets::SimplePropertyWidgetFactory(te::dt::DATETIME_TYPE)
{

}

te::qt::widgets::SimplePropertyWidget* te::qt::widgets::DateTimePropertyWidgetFactory::create(QWidget* parent)
{
  return new DateTimePropertyWidget(parent);
}