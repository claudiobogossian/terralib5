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
  \file LineModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LineModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/enum/EnumLineStyleType.h"
#include "../core/enum/EnumType.h"

te::layout::LineModel::LineModel() 
  : AbstractItemModel()
{
  te::color::RGBAColor color(0, 0, 0, 255);
  te::gm::LineString* lineString = new te::gm::LineString(0, te::gm::LineStringType);
  te::gm::GeometryShrPtr line(lineString);

  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getLineItem());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    EnumLineStyleType lineStyleType;
    EnumType* currentType = lineStyleType.getStyleSolid();

    Property property(0);
    property.setName("line_style_type");
    property.setLabel("line style type");
    property.setValue(currentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for(int i = 0; i < lineStyleType.size(); i++)
    {
      EnumType* enumType = lineStyleType.getEnum(i);

      if(enumType == lineStyleType.getStyleNone() || enumType == currentType || enumType == lineStyleType.getStyleCustomDash())
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

  {
    Property property(0);
    property.setName("geometry");
    property.setLabel("geometry");
    property.setVisible(false);
    property.setValue(line, dataType->getDataTypeGeometry());
    this->m_properties.addProperty(property);
  }
}

te::layout::LineModel::~LineModel()
{

}

void te::layout::LineModel::setProperty(const Property& property)
{
  Properties properties("");
  properties.addProperty(property);
  updateProperties(property, properties);
}

void te::layout::LineModel::setProperties(const Properties& props)
{
  Properties properties = props;
  Property property = properties.getProperty("geometry");
  updateProperties(property, properties);
}

void te::layout::LineModel::updateProperties( const Property& property, Properties& properties )
{
  if ((property.getName() == "geometry"))
  {
    const te::gm::GeometryShrPtr geometryPtr = property.getValue().toGeometry();
    te::gm::LineString const* lineString = dynamic_cast< te::gm::LineString const * > (geometryPtr.get());
    const te::gm::Envelope* env = lineString->getMBR();
    double width = env->getWidth();
    double height = env->getHeight();
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    {
      Property property(0);
      property.setName("width");
      property.setValue(width, dataType->getDataTypeDouble());
      properties.addProperty(property);
    }
    {
      Property property(0);
      property.setName("height");
      property.setValue(height, dataType->getDataTypeDouble());
      properties.addProperty(property);
    }
  }
  AbstractItemModel::setProperties(properties);
}
