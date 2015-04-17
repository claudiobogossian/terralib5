/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  m_dataTypeImage(0),
  m_dataTypeTextGridSettings(0)
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
}

void te::layout::EnumDataType::init()
{
  m_dataTypeNone = new EnumType(0, "None", this);
  m_enums.push_back(m_dataTypeNone);

  m_dataTypeInt = new EnumType(1, "Int", this);
  m_enums.push_back(m_dataTypeInt);

  m_dataTypeDouble = new EnumType(2, "Double", this);
  m_enums.push_back(m_dataTypeDouble);

  m_dataTypeLong = new EnumType(3, "Long", this);
  m_enums.push_back(m_dataTypeLong);

  m_dataTypeFloat = new EnumType(4, "Float", this);
  m_enums.push_back(m_dataTypeFloat);

  m_dataTypeString = new EnumType(5, "String", this);
  m_enums.push_back(m_dataTypeString);

  m_dataTypeBool = new EnumType(6, "Bool", this);
  m_enums.push_back(m_dataTypeBool);

  m_dataTypeColor = new EnumType(7, "Color", this);
  m_enums.push_back(m_dataTypeColor);

  m_dataTypeGridSettings = new EnumType(8, "GridSettings", this);
  m_enums.push_back(m_dataTypeGridSettings);

  m_dataTypeStringList = new EnumType(9, "StringList", this);
  m_enums.push_back(m_dataTypeStringList);

  m_dataTypeFont = new EnumType(10, "Font", this);
  m_enums.push_back(m_dataTypeFont);

  m_dataTypeImage = new EnumType(11, "Image", this);
  m_enums.push_back(m_dataTypeImage);

  m_dataTypeTextGridSettings = new EnumType(12, "TextGridSettings", this);
  m_enums.push_back(m_dataTypeTextGridSettings);
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

te::layout::EnumType* te::layout::EnumDataType::getDataTypeImage() const
{
  return m_dataTypeImage;
}

te::layout::EnumType* te::layout::EnumDataType::getDataTypeTextGridSettings() const
{
  return m_dataTypeTextGridSettings;
}
