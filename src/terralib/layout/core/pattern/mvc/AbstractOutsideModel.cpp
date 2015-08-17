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
  \file AbstractOutsideModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractOutsideModel.h"
#include "Observer.h"
#include "../../property/Property.h"
#include "../../property/Properties.h"
#include "../singleton/Context.h"
#include "../../enum/Enums.h"

// STL
#include <ctime>
#include <iostream>

te::layout::AbstractOutsideModel::AbstractOutsideModel() :
  m_color(0),
  m_publicProperties(0),
  m_type(0),
  m_id(0),
  m_resizable(true),
  m_hashCode(0),
  m_name("unknown")
{
  EnumObjectType* type = Enums::getInstance().getEnumObjectType();
  m_type = type->getObjectUnknown();
  
  m_properties = new Properties(m_name);

  m_hashCode = calculateHashCode();
  m_properties->setHashCode(m_hashCode);

  m_publicProperties = new Properties(m_name, 0, m_hashCode);
}

te::layout::AbstractOutsideModel::~AbstractOutsideModel()
{
  if(m_properties)
  {
    delete m_properties;
    m_properties = 0;
  }
  
  if(m_publicProperties)
  {
    delete m_publicProperties;
    m_publicProperties = 0;
  }
}

te::gm::Envelope te::layout::AbstractOutsideModel::getBox()
{
	return m_box;
}

void te::layout::AbstractOutsideModel::setBox(te::gm::Envelope box)
{
	m_box = box;
}

int te::layout::AbstractOutsideModel::getColor()
{
	return m_color;
}

void te::layout::AbstractOutsideModel::setColor(int color)
{
	m_color = color;
}

void te::layout::AbstractOutsideModel::setPosition(const double& x, const double& y)
{
  //Initial point to draw is : x1, y1, that corresponds 0,0 of local coordinate of a item  
  double x1 = x; 
  double x2 = x + m_box.getWidth();

  double y1 = y;
  double y2 = y + m_box.getHeight();

  m_box = te::gm::Envelope(x1, y1, x2, y2);
}

te::layout::Properties* te::layout::AbstractOutsideModel::getProperties() const
{
  m_properties->clear();

  Property pro_name(m_hashCode);
  pro_name.setName(m_name);

  m_properties->addProperty(pro_name);

  m_properties->setTypeObj(m_type);
  return m_properties;
}

te::layout::EnumType* te::layout::AbstractOutsideModel::getType()
{
  return m_type;
}

void te::layout::AbstractOutsideModel::setType(EnumType* type)
{
  m_type = type;
}

int te::layout::AbstractOutsideModel::getZValue()
{
  return m_zValue;
}

void te::layout::AbstractOutsideModel::setZValue(int zValue)
{
  m_zValue = zValue;
}

void te::layout::AbstractOutsideModel::updateProperties(te::layout::Properties* properties, bool notify)
{
  Properties* vectorProps = const_cast<Properties*>(properties);

  std::vector<Property> vProps = vectorProps->getProperties();
  Property pro_name = vProps[0];
  
  if(notify)
  {
		Subject::notify();
  }
}

std::string te::layout::AbstractOutsideModel::getName()
{
  return m_name;
}

int te::layout::AbstractOutsideModel::getId()
{
  return m_id;
}

void te::layout::AbstractOutsideModel::setId(int id)
{
  m_id = id;
}

void te::layout::AbstractOutsideModel::setResizable(bool resize)
{
  m_resizable = resize;
}

bool te::layout::AbstractOutsideModel::isResizable()
{
  return m_resizable;
}

int te::layout::AbstractOutsideModel::getHashCode()
{
  return m_hashCode;
}

int te::layout::AbstractOutsideModel::calculateHashCode()
{
  int nameLength = m_name.length();
  int id = m_id;
  int type = 0;
  if(m_type)
  {
    type = m_type->getId();
  }

  int propertiesLength = 0;

  if(m_properties)
  {
    Properties* props = getProperties();

    if(props)
    {
      propertiesLength = getProperties()->getProperties().size();
    }
  }

  int hashcode = (nameLength + id + type + propertiesLength) * 5;

  // current date/time based on current system
  std::time_t now = std::time(0);

  std::tm *ltm = localtime(&now);

  if(!ltm)
    return hashcode;

  // print various components of tm structure.
  int year = 1900 + ltm->tm_year;
  int month = 1 + ltm->tm_mon;
  int day = ltm->tm_mday;
  int hour = 1 + ltm->tm_hour;
  int numberSecond = 1 + ltm->tm_min;
  int numberMilliseconds =  1 + ltm->tm_sec;

  hashcode *= year + month + day;
  hashcode *= hour + numberSecond + numberMilliseconds;

  return hashcode;
}

te::layout::Properties* te::layout::AbstractOutsideModel::getPublicProperties() const
{
  if(!m_properties || m_publicProperties)
  {
    return 0;
  }

  m_publicProperties->clear();

  std::vector<Property>::const_iterator it = m_properties->getProperties().begin();

  for( ; it != m_properties->getProperties().end() ; ++it )
  {
    if(it->isPublic())
    {
      m_publicProperties->addProperty(*it);
    }
  }

  m_publicProperties->setTypeObj(m_type);

  return m_publicProperties;
}






