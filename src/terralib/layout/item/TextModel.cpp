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
  \file TextModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/property/SharedProperties.h"
#include "../core/enum/Enums.h"
#include "../core/pattern/singleton/Context.h"

// STL
#include <string>
#include <sstream> // std::stringstream

te::layout::TextModel::TextModel() :
  m_text("")
{
  m_type = Enums::getInstance().getEnumObjectType()->getTextItem();

  m_box = te::gm::Envelope(0., 0., 40., 20.);

  m_font.setPointSize(12);

  m_resizable = false;
}

te::layout::TextModel::~TextModel()
{

}

te::layout::Properties* te::layout::TextModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property pro_font(m_hashCode);
  pro_font.setName("Font");
  pro_font.setValue(m_font, dataType->getDataTypeFont());
  pro_font.setMenu(true);
  m_properties->addProperty(pro_font);

  Property pro_text(m_hashCode);
  pro_text.setName("Text");
  pro_text.setValue(m_text, dataType->getDataTypeString());
  pro_text.setVisible(false);
  m_properties->addProperty(pro_text);

  return m_properties;
}

void te::layout::TextModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);
  
  Property pro_text = vectorProps->contains("Text");

  if(!pro_text.isNull())
  {
    m_text = pro_text.getValue().toString();
  }

  Property pro_font = vectorProps->contains("Font");

  if(!pro_font.isNull())
  {
    m_font = pro_font.getValue().toFont();
  }
}

void te::layout::TextModel::setText( std::string txt )
{
  m_text = txt;
}

std::string te::layout::TextModel::getText()
{
  return m_text;
}

te::layout::Font te::layout::TextModel::getFont()
{
  return m_font;
}

void te::layout::TextModel::setFont( Font ft )
{
  m_font = ft;
}
