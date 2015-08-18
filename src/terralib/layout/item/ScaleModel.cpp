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
  \file ScaleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleModel.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumScaleType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"

te::layout::ScaleModel::ScaleModel()
  : AbstractItemModel()
{
  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getScaleItem());

  double width = 70.;
  double height = 30.;
  double scale = 250000.;
  double scaleGapX = 20.;
  double scaleGapY = 5.;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

//adding properties
  {
    Property property(0);
    property.setName("scale");
    property.setLabel("Scale");
    property.setValue(scale, dataType->getDataTypeDouble());
    property.setMenu(true);
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("scale_width_rect_gap");
    property.setLabel("Scale Gap X");
    property.setValue(scaleGapX, dataType->getDataTypeDouble());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("scale_height_rect_gap");
    property.setLabel("Scale Gap Y");
    property.setValue(scaleGapY, dataType->getDataTypeDouble());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    std::string emptyString;

    Property property(0);
    property.setName("map_name");
    property.setLabel("Map Name");
    property.setValue(emptyString, dataType->getDataTypeStringList());

    Variant v;
    v.setValue(emptyString, dataType->getDataTypeString());
    property.addOption(v);
    m_properties.addProperty(property);
  }

  {
    EnumScaleType enumScaleType;
    EnumType* currentScaleType = enumScaleType.getDoubleAlternatingScaleBarType();

    Property property(0);
    property.setName("scale_type");
    property.setLabel("graphic type");
    property.setValue(currentScaleType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentScaleType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for(int i = 0 ; i < enumScaleType.size(); ++i)
    {
      EnumType* enumType = enumScaleType.getEnum(i);

      if(enumType == enumScaleType.getNoneType() || enumType == currentScaleType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);

      m_properties.addProperty(property);
    }
  }

//updating properties
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }
}

te::layout::ScaleModel::~ScaleModel()
{
}
