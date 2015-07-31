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
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"
#include "../core/enum/EnumNorthArrowType.h"

te::layout::NorthModel::NorthModel():
  m_enumNorthArrowType(0),
	m_currentNorthArrowType(0),
	m_shapeSize(4)
{
  m_type = Enums::getInstance().getEnumObjectType()->getNorthItem();

  m_frameColor = te::color::RGBAColor(0, 0, 0, 255);
  m_box = te::gm::Envelope(0., 0., 20., 20.);

  m_border = true;

	m_enumNorthArrowType = new EnumNorthArrowType();
	m_currentNorthArrowType = m_enumNorthArrowType->getNorthArrowType1();
}

te::layout::NorthModel::~NorthModel()
{
	if(m_enumNorthArrowType)
	{
		delete m_enumNorthArrowType;
		m_enumNorthArrowType = 0;
	}

}


te::layout::Properties* te::layout::NorthModel::getProperties() const
{
	ItemModelObservable::getProperties();

	Property pro_northArrowName = northArrowProperty();
	if(!pro_northArrowName.isNull())
	{
		m_properties->addProperty(pro_northArrowName);
	}

	return m_properties;
}

void te::layout::NorthModel::updateProperties( te::layout::Properties* properties, bool notify)
{
	ItemModelObservable::updateProperties(properties);

	Properties* vectorProps = const_cast<Properties*>(properties);

	Property pro_northArrowName = vectorProps->getProperty("northArrow_type");

	if(!pro_northArrowName.isNull())
	{
		std::string label = pro_northArrowName.getOptionByCurrentChoice().toString();
		EnumType* enumType = m_enumNorthArrowType->searchLabel(label);
		if(enumType)
		{
			m_currentNorthArrowType = enumType;
		}
	}
	if(notify)
	{
		ContextItem context;
		notifyAll(context);
	}
}

te::layout::EnumNorthArrowType* te::layout::NorthModel::getEnumNorthArrowType()
{
	return m_enumNorthArrowType;
}

te::layout::EnumType* te::layout::NorthModel::getCurrentNorthArrowType()
{
	return m_currentNorthArrowType;
}

te::layout::Property te::layout::NorthModel::northArrowProperty() const
{
	Property pro_northArrowName(m_hashCode);

	if(!m_currentNorthArrowType)
		return pro_northArrowName;

	EnumDataType* dataType = Enums::getInstance().getEnumDataType();

	if(!dataType)
		return pro_northArrowName;

	pro_northArrowName.setName("northArrow_type");
	pro_northArrowName.setLabel("graphic type");
	pro_northArrowName.setValue(m_currentNorthArrowType->getLabel(), dataType->getDataTypeStringList());

	Variant v;
	v.setValue(m_currentNorthArrowType->getLabel(), dataType->getDataTypeString());
	pro_northArrowName.addOption(v);
	pro_northArrowName.setOptionChoice(v);

	for(int i = 0 ; i < m_enumNorthArrowType->size() ; ++i)
	{
		EnumType* enumType = m_enumNorthArrowType->getEnum(i);

		if(enumType == m_enumNorthArrowType->getNoneType() || enumType == m_currentNorthArrowType)
			continue;

		Variant v;
		v.setValue(enumType->getLabel(), dataType->getDataTypeString());
		pro_northArrowName.addOption(v);
	}

	return pro_northArrowName;
}

double te::layout::NorthModel::getShapeSize()
{
	return m_shapeSize;
}

te::color::RGBAColor te::layout::NorthModel::getNorthArrowColor()
{
	return m_northArrowColor;
}

void te::layout::NorthModel::setNorthArrowColor( te::color::RGBAColor color )
{
	m_northArrowColor = color;
}


