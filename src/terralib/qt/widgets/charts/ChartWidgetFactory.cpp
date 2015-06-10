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
  \file terralib/qt/widgets/settings/ChartWidgetFactory.cpp

  \brief A factory to build widget objects.
*/

#include "../../../common/StringUtils.h"
#include "ChartWidgetFactory.h"

te::qt::widgets::ChartWidget* te::qt::widgets::ChartWidgetFactory::make(const std::string& cwType)
{
  return te::common::AbstractFactory<te::qt::widgets::ChartWidget, std::string>::make(cwType);
}

te::qt::widgets::ChartWidget* te::qt::widgets::ChartWidgetFactory::make(const std::string& cwType, QWidget* parent)
{
  te::common::AbstractFactory<ChartWidget, std::string>::dictionary_type& d = te::common::AbstractFactory<ChartWidget, std::string>::getDictionary();

  ChartWidgetFactory* f = static_cast<ChartWidgetFactory*>(d.find(cwType));

  if(f == 0)
    throw ;

  ChartWidget* cw = f->create(parent);

  return cw;
}

te::qt::widgets::ChartWidgetFactory::~ChartWidgetFactory() 
{

}

te::qt::widgets::ChartWidgetFactory::ChartWidgetFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::qt::widgets::ChartWidget, std::string>(factoryKey)
{
  
}