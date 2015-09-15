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
  \file EnumDataType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumDataType.h"

te::layout::EnumDataType::EnumDataType() :
  m_dataTypeNone(0),
  m_dataTypeInt(0),
  m_dataTypeDouble(0),
  m_dataTypeLong(0),
  m_dataTypeFloat(0),
  m_dataTypeString(0),
  m_dataTypeBool(0),
  m_dataTypeColor(0),
  m_dataTypeGridSettings(0),
  m_dataTypeStringList(0),
  m_dataTypeFont(0),
  m_dataTypeEnvelope(0),
  m_dataTypeImage(0),
  m_dataTypeTextGridSettings(0),
  m_dataTypeGroup(0),
  m_dataTypeMapChoice(0),
  m_dataTypeLegendChoice(0),
  m_dataTypeGenericVariant(0),
  m_dataTypeLayerList(0),
  m_dataTypeStringVector(0),
  m_dataTypeSVGView(0),
  m_dataTypeGeometry(0)
{
  init();
}

te::layout::EnumDataType::~EnumDataType()
{
 if(m_dataTypeNone)
  {
    delete m_dataTypeNone;
    m_dataTypeNone = 0;
  }
  if(m_dataTypeInt)
  {
    delete m_dataTypeInt;
    m_dataTypeInt = 0;
  }
  if(m_dataTypeDouble)
  {
    delete m_dataTypeDouble;
    m_dataTypeDouble = 0;
  }
  if(m_dataTypeLong)
  {
    delete m_dataTypeLong;
    m_dataTypeLong = 0;
  }
  if(m_dataTypeFloat)
  {
    delete m_dataTypeFloat;
    m_dataTypeFloat = 0;
  }
  if(m_dataTypeString)
  {
    delete m_dataTypeString;
    m_dataTypeString = 0;
  }
  if(m_dataTypeBool)
  {
    delete m_dataTypeBool;
    m_dataTypeBool = 0;
  }
  if(m_dataTypeGridSettings)
  {
    delete m_dataTypeGridSettings;
    m_dataTypeGridSettings = 0;
  }
  if(m_dataTypeStringList)
  {
    delete m_dataTypeStringList;
    m_dataTypeStringList = 0;
  }
  if(m_dataTypeFont)
  {
    delete m_dataTypeFont;
    m_dataTypeFont = 0;
  }
  if(m_dataTypeEnvelope)
  {
    delete m_dataTypeEnvelope;
    m_dataTypeEnvelope = 0;
  }
  if(m_dataTypeImage)
  {
    delete m_dataTypeImage;
    m_dataTypeImage = 0;
  }
  if(m_dataTypeTextGridSettings)
  {
    delete m_dataTypeTextGridSettings;
    m_dataTypeTextGridSettings = 0;
  }
  if(m_dataTypeGroup)
  {
    delete m_dataTypeGroup;
    m_dataTypeGroup = 0;
  }
  if(m_dataTypeMapChoice)
  {
    delete m_dataTypeMapChoice;
    m_dataTypeMapChoice = 0;
  }
  if(m_dataTypeLegendChoice)
  {
    delete m_dataTypeLegendChoice;
    m_dataTypeLegendChoice = 0;
  }
  if(m_dataTypeGenericVariant)
  {
    delete m_dataTypeGenericVariant;
    m_dataTypeGenericVariant = 0;
  }
  if(m_dataTypeLayerList)
  {
    delete m_dataTypeLayerList;
    m_dataTypeLayerList = 0;
  }
  if(m_dataTypeStringVector)
  {
    delete m_dataTypeStringVector;
    m_dataTypeStringVector = 0;
  }
  if(m_dataTypeSVGView)
  {
    delete m_dataTypeSVGView;
    m_dataTypeSVGView = 0;
  }
  if(m_dataTypeGeometry)
  {
    delete m_dataTypeGeometry;
    m_dataTypeGeometry = 0;
  }
}

void te::layout::EnumDataType::init()
{
  m_dataTypeNone = createEnum("None", this);

  m_dataTypeInt = createEnum("Int", this);

  m_dataTypeDouble = createEnum("Double", this);

  m_dataTypeLong = createEnum("Long", this);

  m_dataTypeFloat = createEnum("Float", this);

  m_dataTypeString = createEnum("String", this);

  m_dataTypeBool = createEnum("Bool", this);

  m_dataTypeColor = createEnum("Color", this);

  m_dataTypeGridSettings = createEnum("GridSettings", this, "Grid Settings");

  m_dataTypeStringList = createEnum("StringList", this);

  m_dataTypeFont = createEnum("Font", this);

  m_dataTypeEnvelope = createEnum("Envelope", this);

  m_dataTypeImage = createEnum("Image", this);

  m_dataTypeTextGridSettings = createEnum("TextGridSettings", this);

  m_dataTypeGroup = createEnum("Group", this);

  m_dataTypeMapChoice = createEnum("MapChoice", this, "Legend Choice");

  m_dataTypeLegendChoice = createEnum("LegendChoice", this, "Legend Choice");

  m_dataTypeGenericVariant = createEnum("GenericVariant", this, "Generic Variant");

  m_dataTypeLayerList = createEnum("Layer", this, "Layer");

  m_dataTypeStringVector = createEnum("StringVector", this, "String Vector");

  m_dataTypeSVGView = createEnum("SVGView", this, "SVG View");

  m_dataTypeGeometry = createEnum("Geometry", this, "Geometry");
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeNone() const
{
  return m_dataTypeNone;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeInt() const
{
  return m_dataTypeInt;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeDouble() const
{
  return m_dataTypeDouble;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeLong() const
{
  return m_dataTypeLong;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeFloat() const
{
  return m_dataTypeFloat;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeString() const
{
  return m_dataTypeString;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeBool() const
{
  return m_dataTypeBool;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeColor() const
{
  return m_dataTypeColor;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeGridSettings() const
{
  return m_dataTypeGridSettings;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeStringList() const
{
  return m_dataTypeStringList;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeFont() const
{
  return m_dataTypeFont;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeEnvelope() const
{
  return m_dataTypeEnvelope;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeImage() const
{
  return m_dataTypeImage;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeTextGridSettings() const
{
  return m_dataTypeTextGridSettings;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeGroup() const
{
  return m_dataTypeGroup;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeMapChoice() const
{
  return m_dataTypeMapChoice;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeLegendChoice() const
{
  return m_dataTypeLegendChoice;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeGenericVariant() const
{
  return m_dataTypeGenericVariant;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeLayerList() const
{
  return m_dataTypeLayerList;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeStringVector() const
{
  return m_dataTypeStringVector;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeSVGView() const
{
  return m_dataTypeSVGView;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeGeometry() const
{
  return m_dataTypeGeometry;
}

