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
  \file StarModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "StarModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"

te::layout::StarModel::StarModel():
	m_enumStarType(0),
	m_currentStarType(0),
	m_shapeSize(4)
{
  m_type = Enums::getInstance().getEnumObjectType()->getStarItem();

  m_borderColor = te::color::RGBAColor(0, 0, 0, 255);
  m_box = te::gm::Envelope(0., 0., 20., 20.);

  m_border = false;
  m_enumStarType = new EnumStarType();
  m_currentStarType = m_enumStarType->getStar1Type();
}

te::layout::StarModel::~StarModel()
{
	if(m_enumStarType)
	{
		delete m_enumStarType;
		m_enumStarType = 0;
	}
}

te::layout::Properties* te::layout::StarModel::getProperties() const
{
	ItemModelObservable::getProperties();

	Property pro_starName = starProperty();
	if(!pro_starName.isNull())
	{
		m_properties->addProperty(pro_starName);
	}

	return m_properties;
}

void te::layout::StarModel::updateProperties( te::layout::Properties* properties, bool notify)
{
	ItemModelObservable::updateProperties(properties);

	Properties* vectorProps = const_cast<Properties*>(properties);

	Property pro_starName = vectorProps->contains("star_type");

	if(!pro_starName.isNull())
	{
		std::string label = pro_starName.getOptionByCurrentChoice().toString();
		EnumType* enumType = m_enumStarType->searchLabel(label);
		if(enumType)
		{
			m_currentStarType = enumType;
		}
	}
	if(notify)
	{
		ContextItem context;
		notifyAll(context);
	}
}

te::layout::EnumStarType* te::layout::StarModel::getEnumStarType()
{
	return m_enumStarType;
}

te::layout::EnumType* te::layout::StarModel::getCurrentStarType()
{
	return m_currentStarType;
}

te::layout::Property te::layout::StarModel::starProperty() const
{
	Property pro_starName(m_hashCode);

	if(!m_currentStarType)
		return pro_starName;

	EnumDataType* dataType = Enums::getInstance().getEnumDataType();

	if(!dataType)
		return pro_starName;

	pro_starName.setName("star_type");
	pro_starName.setLabel("graphic type");
	pro_starName.setValue(m_currentStarType->getLabel(), dataType->getDataTypeStringList());

	Variant v;
	v.setValue(m_currentStarType->getLabel(), dataType->getDataTypeString());
	pro_starName.addOption(v);
	pro_starName.setOptionChoice(v);

	for(int i = 0 ; i < m_enumStarType->size() ; ++i)
	{
		EnumType* enumType = m_enumStarType->getEnum(i);

		if(enumType == m_enumStarType->getNoneType() || enumType == m_currentStarType)
			continue;

		Variant v;
		v.setValue(enumType->getLabel(), dataType->getDataTypeString());
		pro_starName.addOption(v);
	}

	return pro_starName;
}

double te::layout::StarModel::getShapeSize()
{
	return m_shapeSize;
}

te::color::RGBAColor te::layout::StarModel::getStarColor()
{
	return m_starColor;
}

void te::layout::StarModel::setStarColor( te::color::RGBAColor color )
{
	m_starColor = color;
}


