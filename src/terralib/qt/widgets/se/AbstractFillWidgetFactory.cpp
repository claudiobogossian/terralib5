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
  \file terralib/qt/widgets/se/AbstractFillWidgetFactory.cpp

  \brief The abstract factory of widgets used to configure Symbology Enconding Fill element.
*/

// TerraLib
#include "AbstractFillWidgetFactory.h"

te::qt::widgets::AbstractFillWidgetFactory::~AbstractFillWidgetFactory()
{
}

void te::qt::widgets::AbstractFillWidgetFactory::RegisteredWidgets(std::vector<std::string>& keys)
{
  te::common::AbstractFactory<AbstractFillWidget, std::string>::dictionary_type& dic =  te::common::AbstractFactory<AbstractFillWidget, std::string>::getDictionary();
  std::map<std::string, te::common::AbstractFactory<AbstractFillWidget, std::string>*>::const_iterator it;
  for(it = dic.begin(); it != dic.end(); ++it) // for each factory
    keys.push_back(it->second->getKey());
}

te::qt::widgets::AbstractFillWidgetFactory::AbstractFillWidgetFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::qt::widgets::AbstractFillWidget, std::string>(factoryKey)
{
}
