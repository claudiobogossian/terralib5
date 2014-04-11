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
  \file OutsideModelObservable.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "OutsideModelObservable.h"
#include "Observer.h"
#include "Property.h"
#include "Properties.h"

te::layout::OutsideModelObservable::OutsideModelObservable() :
	m_color(0),
  m_type(TPObjectUnknown)
{
  m_properties = new Properties("Unknown");
}

te::layout::OutsideModelObservable::~OutsideModelObservable()
{

}

bool te::layout::OutsideModelObservable::addObserver( Observer* o )
{
	std::pair<std::set<Observer*>::iterator,bool> p = m_observers.insert(o);

	if(p.second == true)
		return true;

	return false;
}

bool te::layout::OutsideModelObservable::removeObserver( Observer* o )
{
	int num = m_observers.erase(o);

	if(num == 1)
		return true;

	return false;
}

void te::layout::OutsideModelObservable::notifyAll( ContextItem context )
{
	std::set<Observer*>::iterator it;
	for(it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->updateObserver(context);
	}
}

te::gm::Envelope te::layout::OutsideModelObservable::getBox()
{
	return m_box;
}

void te::layout::OutsideModelObservable::setBox(te::gm::Envelope box)
{
	m_box = box;
}

int te::layout::OutsideModelObservable::getColor()
{
	return m_color;
}

void te::layout::OutsideModelObservable::setColor( int color )
{
	m_color = color;
}

void te::layout::OutsideModelObservable::setPosition( const double& x, const double& y )
{
  //Initial point to draw is : x1, y2, that corresponds 0,0 of local coordinate of a item
  double x1 = x; 
  double y1 = y - m_box.getHeight();

  double x2 = x + m_box.getWidth();
  double y2 = y;

  m_box = te::gm::Envelope(x1, y1, x2, y2);

}

te::layout::Properties* te::layout::OutsideModelObservable::getProperties() const
{
  m_properties->clear();

  Property pro_name;
  pro_name.setName("Oi");
  Property pro_label;
  //pro_label.setLabel("SHOW");

  m_properties->addProperty(pro_name);
  m_properties->addProperty(pro_label);

  return m_properties;
}

te::layout::LayoutAbstractObjectType te::layout::OutsideModelObservable::getType()
{
  return m_type;
}

int te::layout::OutsideModelObservable::getZValue()
{
  return m_zValue;
}

void te::layout::OutsideModelObservable::setZValue( int zValue )
{
  m_zValue = zValue;
}

void te::layout::OutsideModelObservable::updateProperties( te::layout::Properties* properties )
{
  Properties* vectorProps = const_cast<Properties*>(properties);

  std::vector<Property> vProps = vectorProps->getProperties();
  Property pro_name = vProps[0];
  //m_name = pro_name.getName();
}