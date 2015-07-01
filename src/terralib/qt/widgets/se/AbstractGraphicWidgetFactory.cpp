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
  \file terralib/qt/widgets/se/AbstractGraphicWidgetFactory.cpp

  \brief The abstract factory of widgets used to configure Symbology Enconding Graphic element.
*/

// TerraLib
#include "AbstractGraphicWidgetFactory.h"

te::qt::widgets::AbstractGraphicWidgetFactory::~AbstractGraphicWidgetFactory()
{
}

void te::qt::widgets::AbstractGraphicWidgetFactory::RegisteredWidgets(std::vector<std::string>& keys)
{
  te::common::AbstractFactory<AbstractGraphicWidget, std::string>::dictionary_type& dic =  te::common::AbstractFactory<AbstractGraphicWidget, std::string>::getDictionary();
  std::map<std::string, te::common::AbstractFactory<AbstractGraphicWidget, std::string>*>::const_iterator it;
  for(it = dic.begin(); it != dic.end(); ++it) // for each factory
    keys.push_back(it->second->getKey());
}

te::qt::widgets::AbstractGraphicWidgetFactory::AbstractGraphicWidgetFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::qt::widgets::AbstractGraphicWidget, std::string>(factoryKey)
{
}
