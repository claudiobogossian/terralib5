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
  \file NorthModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NorthModel.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumNorthArrowType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"

te::layout::NorthModel::NorthModel()
  : AbstractItemModel()
{
  te::color::RGBAColor color(0, 0, 0, 255);

  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getNorthItem());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    EnumNorthArrowType northArrowType;
    EnumType* currentType = northArrowType.getNorthArrowType1();

    Property property(0);
    property.setName("northArrow_type");
    property.setLabel("graphic type");
    property.setValue(currentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for(int i = 0 ; i < northArrowType.size() ; ++i)
    {
      EnumType* enumType = northArrowType.getEnum(i);

      if(enumType == northArrowType.getNoneType() || enumType == currentType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("color");
    property.setLabel("color");
    property.setValue(color, dataType->getDataTypeColor());
    this->m_properties.addProperty(property);
  }
}

te::layout::NorthModel::~NorthModel()
{

}

