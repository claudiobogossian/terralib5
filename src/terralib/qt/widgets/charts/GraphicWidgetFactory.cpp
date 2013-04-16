/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/settings/GraphicWidgetFactory.cpp

  \brief A factory to build widget objects.
*/

#include "../../../common/StringUtils.h"
#include "GraphicWidget.h"
#include "GraphicWidgetFactory.h"

te::qt::widgets::GraphicWidget* te::qt::widgets::GraphicWidgetFactory::make(const std::string& cwType)
{
  return te::common::AbstractFactory<te::qt::widgets::GraphicWidget, std::string>::make(cwType);
}

te::qt::widgets::GraphicWidget* te::qt::widgets::GraphicWidgetFactory::make(const std::string& cwType, QWidget* parent)
{
  te::common::AbstractFactory<GraphicWidget, std::string>::dictionary_type& d = te::common::AbstractFactory<GraphicWidget, std::string>::getDictionary();

  GraphicWidgetFactory* f = static_cast<GraphicWidgetFactory*>(d.find(cwType));

  if(f == 0)
    throw ;

  GraphicWidget* cw = f->create(parent);

  return cw;
}

te::qt::widgets::GraphicWidgetFactory::~GraphicWidgetFactory() 
{

}

te::qt::widgets::GraphicWidgetFactory::GraphicWidgetFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::qt::widgets::GraphicWidget, std::string>(factoryKey)
{
  
}