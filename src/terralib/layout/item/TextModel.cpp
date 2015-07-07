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
#include "../core/enum/Enums.h"

// STL
#include <string>
#include <sstream> // std::stringstream
#include "../core/enum/EnumDataType.h"
#include "../core/enum/EnumAlignmentType.h"

te::layout::TextModel::TextModel() :
  m_text(""),
	m_enumAlignmentType(0),
	m_currentAlignmentType(0),
	m_shapeSize(4)
{
  m_type = Enums::getInstance().getEnumObjectType()->getTextItem();

  m_box = te::gm::Envelope(0., 0., 40., 20.);

  m_font.setPointSize(12);
	m_fontColor.setColor(0,0,0);
	m_resizable = false;
  m_border = false;

	m_enumAlignmentType = new EnumAlignmentType();
	m_currentAlignmentType = m_enumAlignmentType->getAlignmentCenterType();
}

te::layout::TextModel::~TextModel()
{
	if(m_enumAlignmentType)
	{
		delete m_enumAlignmentType;
		m_enumAlignmentType = 0;
	}
}

te::layout::Properties* te::layout::TextModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property pro_font(m_hashCode);
  pro_font.setName("Font");
  pro_font.setValue(m_font, dataType->getDataTypeFont());
  m_properties->addProperty(pro_font);

  Property pro_text(m_hashCode);
  pro_text.setName("Text");
  pro_text.setValue(m_text, dataType->getDataTypeString());
  pro_font.setVisible(false);
  m_properties->addProperty(pro_text);

	Property pro_fontColor(m_hashCode);
	pro_fontColor.setName("Font Color");
	pro_fontColor.setValue(m_fontColor, dataType->getDataTypeColor());
	pro_font.setVisible(false);
	m_properties->addProperty(pro_fontColor);

	Property pro_alignmentName = alignmentProperty();
	if(!pro_alignmentName.isNull())
	{
		m_properties->addProperty(pro_alignmentName);
	}

  return m_properties;
}

void te::layout::TextModel::updateProperties( te::layout::Properties* properties, bool notify )
{
  ItemModelObservable::updateProperties(properties, false);

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

	Property pro_fontColor = vectorProps->contains("Font Color");

	if(!pro_fontColor.isNull())
	{
		m_fontColor = pro_fontColor.getValue().toColor();
	}

	Property pro_alignmentName = vectorProps->contains("Alignment");

	if(!pro_alignmentName.isNull())
	{
		std::string label = pro_alignmentName.getOptionByCurrentChoice().toString();
		EnumType* enumType = m_enumAlignmentType->searchLabel(label);
		if(enumType)
		{
			m_currentAlignmentType = enumType;
		}
	}

	if(notify)
	{
		ContextItem context;
		notifyAll(context);
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

void te::layout::TextModel::setFont( Font ft )
{
	m_font = ft;
}

te::layout::Font te::layout::TextModel::getFont()
{
  return m_font;
}

void te::layout::TextModel::setFontColor(te::color::RGBAColor clft)
{
	m_fontColor = clft;
}

te::color::RGBAColor te::layout::TextModel::getFontColor()
{
	return m_fontColor;
}

te::layout::EnumAlignmentType* te::layout::TextModel::getEnumAlignmentType()
{
	return m_enumAlignmentType;
}

te::layout::EnumType* te::layout::TextModel::getCurrentAlignmentType()
{
	return m_currentAlignmentType;
}

double te::layout::TextModel::getShapeSize()
{
	return m_shapeSize;
}

te::layout::Property te::layout::TextModel::alignmentProperty() const
{
	Property pro_alignmentName(m_hashCode);

	if(!m_currentAlignmentType)
		return pro_alignmentName;

	EnumDataType* dataType = Enums::getInstance().getEnumDataType();

	if(!dataType)
		return pro_alignmentName;

	pro_alignmentName.setName("Alignment");
	pro_alignmentName.setLabel("graphic type");
	pro_alignmentName.setValue(m_currentAlignmentType->getLabel(), dataType->getDataTypeStringList());

	Variant v;
	v.setValue(m_currentAlignmentType->getLabel(), dataType->getDataTypeString());
	pro_alignmentName.addOption(v);
	pro_alignmentName.setOptionChoice(v);

	for(int i = 0 ; i < m_enumAlignmentType->size() ; ++i)
	{
		EnumType* enumType = m_enumAlignmentType->getEnum(i);

		if(enumType == m_enumAlignmentType->getNoneType() || enumType == m_currentAlignmentType)
			continue;

		Variant v;
		v.setValue(enumType->getLabel(), dataType->getDataTypeString());
		pro_alignmentName.addOption(v);
	}

	return pro_alignmentName;
}


