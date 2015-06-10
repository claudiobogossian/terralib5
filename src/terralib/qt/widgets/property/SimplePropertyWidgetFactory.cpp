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
  \file terralib/qt/widget/property/SimplePropertyWidgetFactory.cpp

  \brief A factory to build widget objects.
*/

#include "../../../common/StringUtils.h"
#include "SimplePropertyWidget.h"
#include "SimplePropertyWidgetFactory.h"

te::qt::widgets::SimplePropertyWidget* te::qt::widgets::SimplePropertyWidgetFactory::make(const int& type)
{
  return te::common::AbstractFactory<te::qt::widgets::SimplePropertyWidget, int>::make(type);
}

te::qt::widgets::SimplePropertyWidget* te::qt::widgets::SimplePropertyWidgetFactory::make(const int& type, QWidget* parent)
{
  te::common::AbstractFactory<SimplePropertyWidget, int>::dictionary_type& d = te::common::AbstractFactory<SimplePropertyWidget, int>::getDictionary();

  SimplePropertyWidgetFactory* f = static_cast<SimplePropertyWidgetFactory*>(d.find(type));

  if(f == 0)
    throw ;

  SimplePropertyWidget* w = f->create(parent);

  return w;
}

te::qt::widgets::SimplePropertyWidgetFactory::~SimplePropertyWidgetFactory() 
{

}

te::qt::widgets::SimplePropertyWidgetFactory::SimplePropertyWidgetFactory(const int& factoryKey)
  : te::common::AbstractFactory<te::qt::widgets::SimplePropertyWidget, int>(factoryKey)
{
  
}