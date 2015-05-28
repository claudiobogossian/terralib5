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
  \file RectangleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "RectangleModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../core/property/Property.h"
#include "../core/pattern/mvc/ItemModelObservable.h"

te::layout::RectangleModel::RectangleModel():
  m_enumRectangleType(0),
	m_currentRectangleType(0),
	m_shapeSize(4)
{
  m_type = Enums::getInstance().getEnumObjectType()->getRectangleItem();

  m_borderColor = te::color::RGBAColor(0, 0, 0, 255);
  m_box = te::gm::Envelope(0., 0., 20., 20.);

  m_border = false;
	m_enumRectangleType = new EnumRectangleType();
	m_currentRectangleType = m_enumRectangleType->getSimpleRectangleType();
}

te::layout::RectangleModel::~RectangleModel()
{
	if(m_enumRectangleType)
	{
		delete m_enumRectangleType;
		m_enumRectangleType = 0;
	}
}

te::layout::Properties* te::layout::RectangleModel::getProperties() const
{
	ItemModelObservable::getProperties();

	Property pro_rectangleName = rectangleProperty();
	if(!pro_rectangleName.isNull())
	{
		m_properties->addProperty(pro_rectangleName);
	}

	return m_properties;
}

void te::layout::RectangleModel::updateProperties( te::layout::Properties* properties )
{
	ItemModelObservable::updateProperties(properties);

	Properties* vectorProps = const_cast<Properties*>(properties);

	Property pro_rectangleName = vectorProps->contains("rectangle_type");

	if(!pro_rectangleName.isNull())
	{
		std::string label = pro_rectangleName.getOptionByCurrentChoice().toString();
		EnumType* enumType = m_enumRectangleType->searchLabel(label);
		if(enumType)
		{
			m_currentRectangleType = enumType;
		}
	}
}

te::layout::EnumRectangleType* te::layout::RectangleModel::getEnumRectangleType()
{
	return m_enumRectangleType;
}

te::layout::EnumType* te::layout::RectangleModel::getCurrentRectangleType()
{
	return m_currentRectangleType;
}

te::layout::Property te::layout::RectangleModel::rectangleProperty() const
{
	Property pro_rectangleName(m_hashCode);

	if(!m_currentRectangleType)
		return pro_rectangleName;

	EnumDataType* dataType = Enums::getInstance().getEnumDataType();

	if(!dataType)
		return pro_rectangleName;

	pro_rectangleName.setName("rectangle_type");
	pro_rectangleName.setLabel("graphic type");
	pro_rectangleName.setValue(m_currentRectangleType->getLabel(), dataType->getDataTypeStringList());

	Variant v;
	v.setValue(m_currentRectangleType->getLabel(), dataType->getDataTypeString());
	pro_rectangleName.addOption(v);
	pro_rectangleName.setOptionChoice(v);

	for(int i = 0 ; i < m_enumRectangleType->size() ; ++i)
	{
		EnumType* enumType = m_enumRectangleType->getEnum(i);

		if(enumType == m_enumRectangleType->getNoneType() || enumType == m_currentRectangleType)
			continue;

		Variant v;
		v.setValue(enumType->getLabel(), dataType->getDataTypeString());
		pro_rectangleName.addOption(v);
	}

	return pro_rectangleName;
}

double te::layout::RectangleModel::getShapeSize()
{
	return m_shapeSize;
}

te::color::RGBAColor te::layout::RectangleModel::getRectangleColor()
{
	return m_rectangleColor;
}

void te::layout::RectangleModel::setRectangleColor( te::color::RGBAColor color )
{
	m_rectangleColor = color;
}
