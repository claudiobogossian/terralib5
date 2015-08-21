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
  \file NewItemFactory.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NewItemFactory.h"
#include "terralib/geometry/Coord2D.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../../core/property/Properties.h"
#include "../../../../../core/pattern/mvc/AbstractItemModel.h"

// STL
#include <string>

te::layout::NewItemFactory::~NewItemFactory()
{

}

te::layout::NewItemFactory::NewItemFactory(const std::string& factoryKey)
: te::common::ParameterizedAbstractFactory<AbstractItemView, std::string, ItemFactoryParamsCreate>(factoryKey)
{

}

void te::layout::NewItemFactory::setProperties(AbstractItemModel* model, ItemFactoryParamsCreate params)
{
  if (!model)
    return;

  std::string     name = params.getName();
  int             id = params.getId();
  double          width = params.getWidth();
  double          height = params.getHeight();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Properties props;

  Property prop_name(0);
  prop_name.setName("name");
  prop_name.setValue(name, dataType->getDataTypeString());
  props.addProperty(prop_name);

  Property prop_id(0);
  prop_id.setName("id");
  prop_id.setValue(id, dataType->getDataTypeInt());
  props.addProperty(prop_id);

  if (width > 0)
  {
    Property prop_width(0);
    prop_width.setName("width");
    prop_width.setLabel("Width");
    prop_width.setValue(width, dataType->getDataTypeDouble());
    props.addProperty(prop_width);
  }

  if (height > 0)
  {
    Property prop_height(0);
    prop_height.setName("height");
    prop_height.setLabel("Height");
    prop_height.setValue(height, dataType->getDataTypeDouble());
    props.addProperty(prop_height);
  }

  //update properties
  model->setProperties(props);
}

