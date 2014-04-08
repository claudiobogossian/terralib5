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
  \file LayoutOutsideModelObservable.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutOutsideModelObservable.h"
#include "LayoutObserver.h"
#include "LayoutProperty.h"
#include "LayoutProperties.h"

te::layout::LayoutOutsideModelObservable::LayoutOutsideModelObservable() :
	m_color(0),
  m_type(TPObjectUnknown)
{
  m_properties = new LayoutProperties("Unknown");
}

te::layout::LayoutOutsideModelObservable::~LayoutOutsideModelObservable()
{

}

bool te::layout::LayoutOutsideModelObservable::addObserver( LayoutObserver* o )
{
	std::pair<std::set<LayoutObserver*>::iterator,bool> p = m_observers.insert(o);

	if(p.second == true)
		return true;

	return false;
}

bool te::layout::LayoutOutsideModelObservable::removeObserver( LayoutObserver* o )
{
	int num = m_observers.erase(o);

	if(num == 1)
		return true;

	return false;
}

void te::layout::LayoutOutsideModelObservable::notifyAll( ContextLayoutItem context )
{
	std::set<LayoutObserver*>::iterator it;
	for(it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->updateObserver(context);
	}
}

te::gm::Envelope te::layout::LayoutOutsideModelObservable::getBox()
{
	return m_box;
}

void te::layout::LayoutOutsideModelObservable::setBox(te::gm::Envelope box)
{
	m_box = box;
}

int te::layout::LayoutOutsideModelObservable::getColor()
{
	return m_color;
}

void te::layout::LayoutOutsideModelObservable::setColor( int color )
{
	m_color = color;
}

void te::layout::LayoutOutsideModelObservable::setPosition( const double& x, const double& y )
{
  //Initial point to draw is : x1, y2, that corresponds 0,0 of local coordinate of a item
  double x1 = x; 
  double y1 = y - m_box.getHeight();

  double x2 = x + m_box.getWidth();
  double y2 = y;

  m_box = te::gm::Envelope(x1, y1, x2, y2);

}

te::layout::LayoutProperties* te::layout::LayoutOutsideModelObservable::getProperties() const
{
  m_properties->clear();

  LayoutProperty pro_name;
  pro_name.setName("Oi");
  LayoutProperty pro_label;
  //pro_label.setLabel("SHOW");

  m_properties->addProperty(pro_name);
  m_properties->addProperty(pro_label);

  return m_properties;
}

te::layout::LayoutAbstractObjectType te::layout::LayoutOutsideModelObservable::getType()
{
  return m_type;
}

int te::layout::LayoutOutsideModelObservable::getZValue()
{
  return m_zValue;
}

void te::layout::LayoutOutsideModelObservable::setZValue( int zValue )
{
  m_zValue = zValue;
}

void te::layout::LayoutOutsideModelObservable::updateProperties( te::layout::LayoutProperties* properties )
{
  LayoutProperties* vectorProps = const_cast<LayoutProperties*>(properties);

  std::vector<LayoutProperty> vProps = vectorProps->getProperties();
  LayoutProperty pro_name = vProps[0];
  //m_name = pro_name.getName();
}