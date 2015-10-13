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
  \file GridMapModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridMapModel.h"

#include "../core/property/GridSettingsConfigProperties.h"
#include "../core/property/SharedProperties.h"

te::layout::GridMapModel::GridMapModel() 
  : AbstractItemModel()
{
  std::string name;
  std::string mapName;

  //Reference Settings
  double scale = 0.;

  // Grid 
  bool visible = true;
  double lneHrzGap = 1000.; // 0.
  double lneVrtGap = 1000.; // 0.
  double m_initialGridPointX = 0.; 
  double m_initialGridPointY = 0.;

  // Line
  te::color::RGBAColor lineColor(0, 0, 0, 255);
  double lineWidth = 0.;

  // Text: Basic Configuration
  int pointTextSize = 6;
  std::string fontFamily("Arial");
  te::color::RGBAColor textColor(0, 0, 0, 255);

  // Text: Advanced configuration
  bool visibleAllTexts = true;
  bool superscriptText = false;
  double lneVrtDisplacement = 1.;
  double lneHrzDisplacement = 1.;
  bool bottomText = true;
  bool leftText = true;
  bool rightText = true;
  bool topText = true;
  bool bottomRotateText = false;
  bool leftRotateText = false;
  bool rightRotateText = false;
  bool topRotateText = false;

  double crossOffSet = 2.;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  GridSettingsConfigProperties settingsConfig;
  SharedProperties sharedProps;

  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getGridMapItem());

  // Grid settings
  {
    std::string name;
    Property property(0);
    property.setName("GridSettings");
    property.setMenu(true);
    property.setPublic(true);
    property.setValue(name, dataType->getDataTypeGridSettings());  
    m_properties.addProperty(property);
  }
  
  // Grid 
  {
    Property property(0);
    property.setName(settingsConfig.getVisible());
    property.setComposeWidget(true);
    property.setValue(visible, dataType->getDataTypeBool());  
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLneHrzGap());
    property.setComposeWidget(true);
    property.setValue(lneHrzGap, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLneVrtGap());
    property.setComposeWidget(true);
    property.setValue(lneVrtGap, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getInitialGridPointX());
    property.setComposeWidget(true);
    property.setValue(m_initialGridPointX, dataType->getDataTypeDouble());  
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getInitialGridPointY());
    property.setComposeWidget(true);
    property.setValue(m_initialGridPointY, dataType->getDataTypeDouble());  
    m_properties.addProperty(property);
  }

  //Just one is visible
  {
    EnumGridStyleType gridStyleType;
    EnumType* currentGridStyle = gridStyleType.getStyleContinuous();

    Property property(0);
    property.setName(settingsConfig.getStyle());
    property.setComposeWidget(true);
    property.setValue(currentGridStyle->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentGridStyle->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < gridStyleType.size(); ++i)
    {
      EnumType* enumType = gridStyleType.getEnum(i);

      if (enumType == gridStyleType.getStyleNone() || enumType == currentGridStyle)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    m_properties.addProperty(property);
  }

  // Line
  {
    EnumLineStyleType lineStyleType;
    EnumType* currentLineStyle = lineStyleType.getStyleSolid();

    Property property(0);
    property.setName(settingsConfig.getLineStyle());
    property.setComposeWidget(true);
    property.setValue(currentLineStyle->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentLineStyle->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < lineStyleType.size(); ++i)
    {
      EnumType* enumType = lineStyleType.getEnum(i);

      if (enumType == lineStyleType.getStyleNone() || enumType == currentLineStyle)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getLineColor());
    property.setComposeWidget(true);
    property.setValue(lineColor, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getLineWidth());
    property.setComposeWidget(true);
    property.setValue(lineWidth, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

  // Text: Basic Configuration
  {
    Property property(0);
    property.setName(settingsConfig.getPointTextSize());
    property.setComposeWidget(true);
    property.setValue(pointTextSize, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }
  
  {
    Property property(0);
    property.setName(settingsConfig.getFontText());
    property.setComposeWidget(true);
    property.setValue(fontFamily, dataType->getDataTypeString());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getTextColor());
    property.setComposeWidget(true);
    property.setValue(textColor, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }

  // Text: Advanced configuration
  {
    Property property(0);
    property.setName(settingsConfig.getVisibleAllTexts());
    property.setComposeWidget(true);
    property.setValue(visibleAllTexts, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getSuperscriptText());
    property.setComposeWidget(true);
    property.setVisible(false); //need review
    property.setValue(superscriptText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getLneVrtDisplacement());
    property.setComposeWidget(true);
    property.setValue(lneVrtDisplacement, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getLneHrzDisplacement());
    property.setComposeWidget(true);
    property.setValue(lneHrzDisplacement, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getBottomText());
    property.setComposeWidget(true);
    property.setValue(bottomText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLeftText());
    property.setComposeWidget(true);
    property.setValue(leftText, dataType->getDataTypeBool());  
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getRightText());
    property.setComposeWidget(true);
    property.setValue(rightText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getTopText());
    property.setComposeWidget(true);
    property.setValue(topText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getBottomRotateText());
    property.setComposeWidget(true);
    property.setValue(bottomRotateText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLeftRotateText());
    property.setComposeWidget(true);
    property.setValue(leftRotateText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getRightRotateText());
    property.setComposeWidget(true);
    property.setValue(rightRotateText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getTopRotateText());
    property.setComposeWidget(true);
    property.setValue(topRotateText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
    {
    Property property(0);
    property.setName(settingsConfig.getCrossOffset());
    property.setComposeWidget(true);
    property.setValue(crossOffSet, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

//updating properties
  {
    Property property(0);
    property.setName("connect_item_position");
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }
}

te::layout::GridMapModel::~GridMapModel()
{

}
