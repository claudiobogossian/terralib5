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
  \file ArrowModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ArrowModel.h"
#include "../core/ContextItem.h"
#include "../core/enum/Enums.h"

// STL
#include <cmath>
#include "../core/property/Properties.h"
#include "../core/pattern/mvc/ItemModelObservable.h"

te::layout::ArrowModel::ArrowModel():
  m_enumArrowType(0),
	m_currentArrowType(0),
	m_shapeSize(4)
{
	m_type = Enums::getInstance().getEnumObjectType()->getArrowItem();

	m_borderColor = te::color::RGBAColor(0, 0, 0, 255);
	m_box = te::gm::Envelope(0., 0., 20., 20.);

	m_border = false;
	m_enumArrowType = new EnumArrowType();
	m_currentArrowType = m_enumArrowType->getRightArrowType();
}

te::layout::ArrowModel::~ArrowModel()
{
	if(m_enumArrowType)
	{
		delete m_enumArrowType;
		m_enumArrowType = 0;
	}
}


te::layout::Properties* te::layout::ArrowModel::getProperties() const
{
	ItemModelObservable::getProperties();

	Property pro_arrowName = arrowProperty();
	if(!pro_arrowName.isNull())
	{
		m_properties->addProperty(pro_arrowName);
	}

	return m_properties;
}

void te::layout::ArrowModel::updateProperties( te::layout::Properties* properties, bool notify)
{
	ItemModelObservable::updateProperties(properties);

	Properties* vectorProps = const_cast<Properties*>(properties);

	Property pro_arrowName = vectorProps->contains("arrow_type");

	if(!pro_arrowName.isNull())
	{
		std::string label = pro_arrowName.getOptionByCurrentChoice().toString();
		EnumType* enumType = m_enumArrowType->searchLabel(label);
		if(enumType)
		{
			m_currentArrowType = enumType;
		}
	}
	if(notify)
	{
		ContextItem context;
		notifyAll(context);
	}
}

te::layout::EnumArrowType* te::layout::ArrowModel::getEnumArrowType()
{
	return m_enumArrowType;
}

te::layout::EnumType* te::layout::ArrowModel::getCurrentArrowType()
{
	return m_currentArrowType;
}

te::layout::Property te::layout::ArrowModel::arrowProperty() const
{
	Property pro_arrowName(m_hashCode);

	if(!m_currentArrowType)
		return pro_arrowName;

	EnumDataType* dataType = Enums::getInstance().getEnumDataType();

	if(!dataType)
		return pro_arrowName;

	pro_arrowName.setName("arrow_type");
	pro_arrowName.setLabel("graphic type");
	pro_arrowName.setValue(m_currentArrowType->getLabel(), dataType->getDataTypeStringList());

	Variant v;
	v.setValue(m_currentArrowType->getLabel(), dataType->getDataTypeString());
	pro_arrowName.addOption(v);
	pro_arrowName.setOptionChoice(v);

	for(int i = 0 ; i < m_enumArrowType->size() ; ++i)
	{
		EnumType* enumType = m_enumArrowType->getEnum(i);

		if(enumType == m_enumArrowType->getNoneType() || enumType == m_currentArrowType)
			continue;

		Variant v;
		v.setValue(enumType->getLabel(), dataType->getDataTypeString());
		pro_arrowName.addOption(v);
	}

	return pro_arrowName;
}

double te::layout::ArrowModel::getShapeSize()
{
	return m_shapeSize;
}

te::color::RGBAColor te::layout::ArrowModel::getArrowColor()
{
	return m_arrowColor;
}

void te::layout::ArrowModel::setArrowColor( te::color::RGBAColor color )
{
	m_arrowColor = color;
}


