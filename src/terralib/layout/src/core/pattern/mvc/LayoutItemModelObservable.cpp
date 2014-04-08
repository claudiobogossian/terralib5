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
  \file LayoutItemModelObservable.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutItemModelObservable.h"
#include "LayoutObserver.h"
#include "LayoutProperty.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "LayoutProperties.h"

te::layout::LayoutItemModelObservable::LayoutItemModelObservable() :
  m_id(0),
  m_name("unknown"),
  m_type(TPObjectUnknown)
{
  m_box = te::gm::Envelope(0,0,0,0);

  m_properties = new LayoutProperties(m_name);
}

te::layout::LayoutItemModelObservable::~LayoutItemModelObservable()
{

}

bool te::layout::LayoutItemModelObservable::addObserver( LayoutObserver* o )
{
  std::pair<std::set<LayoutObserver*>::iterator,bool> p = m_observers.insert(o);

  if(p.second == true)
    return true;

  return false;
}

bool te::layout::LayoutItemModelObservable::removeObserver( LayoutObserver* o )
{
  int num = m_observers.erase(o);

  if(num == 1)
    return true;

  return false;
}

void te::layout::LayoutItemModelObservable::notifyAll( ContextLayoutItem context )
{
  std::set<LayoutObserver*>::iterator it;
  for(it = m_observers.begin(); it != m_observers.end(); ++it)
  {
    (*it)->updateObserver(context);
  }
}

te::layout::LayoutProperties* te::layout::LayoutItemModelObservable::getProperties() const
{  
  m_properties->clear();

  LayoutProperty pro_name;
  pro_name.setName("name");
  pro_name.setId("unknown");
  pro_name.setValue(m_name, DataTypeString);

  LayoutProperty pro_id;
  pro_id.setName("id");
  pro_id.setId("unknown");
  pro_id.setValue(m_id, DataTypeInt);

  m_properties->addProperty(pro_name);
  m_properties->addProperty(pro_id);

  return m_properties;
}

te::gm::Envelope te::layout::LayoutItemModelObservable::getBox()
{
  return m_box;
}

void te::layout::LayoutItemModelObservable::setBox(te::gm::Envelope box)
{
  m_box = box;
}

te::color::RGBAColor te::layout::LayoutItemModelObservable::getBackgroundColor()
{
  return m_backgroundColor;
}

void te::layout::LayoutItemModelObservable::setBackgroundColor( te::color::RGBAColor color )
{
  m_backgroundColor = color;
}

void te::layout::LayoutItemModelObservable::setBorderColor( te::color::RGBAColor color )
{
  m_borderColor = color;
}

te::color::RGBAColor te::layout::LayoutItemModelObservable::getBorderColor()
{
  return m_borderColor;
}

void te::layout::LayoutItemModelObservable::setName( std::string name )
{
  m_name = name;
  if(m_properties)
    m_properties->setObjectName(m_name);
}

std::string te::layout::LayoutItemModelObservable::getName()
{
  return m_name;
}

void te::layout::LayoutItemModelObservable::setPosition( const double& x, const double& y )
{
  //Initial point to draw is : x1, y2, that corresponds 0,0 of local coordinate of a item
  double x1 = x; 
  double y1 = y - m_box.getHeight();

  double x2 = x + m_box.getWidth();
  double y2 = y;

  m_box = te::gm::Envelope(x1, y1, x2, y2);
}

bool te::layout::LayoutItemModelObservable::contains( const te::gm::Coord2D &coord ) const
{
  te::gm::Envelope env(coord.x, coord.y, coord.x, coord.y);

  if(env.isValid())
    return m_box.contains(env);

  return false;
}

void te::layout::LayoutItemModelObservable::updateProperties( te::layout::LayoutProperties* properties )
{
  LayoutProperties* vectorProps = const_cast<LayoutProperties*>(properties);

  std::vector<LayoutProperty> vProps = vectorProps->getProperties();
  LayoutProperty pro_name = vProps[0];
  m_name = pro_name.getName();
}

te::layout::LayoutAbstractObjectType te::layout::LayoutItemModelObservable::getType()
{
  return m_type;
}

int te::layout::LayoutItemModelObservable::getZValue()
{
  return m_zValue;
}

void te::layout::LayoutItemModelObservable::setZValue( int zValue )
{
  m_zValue = zValue;
}


